#include <cassert>
#include <cstring>
#include <curses.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <dirent.h>

#include "menu.h"
#include "palette.h"
#include "data_read.h"
#include "DungeonGenerator/dungeon_generator.h"

#define DEF_FCLR COLOR_WHITE
#define DEF_BCLR COLOR_BLACK

#define C_N 'w'
#define C_S 'x'
#define C_W 'a'
#define C_E 'd'
#define C_NW 'q'
#define C_NE 'e'
#define C_SW 'z'
#define C_SE 'c'
#define C_OPEN_INV 'i'

#define MC_UP 'w'
#define MC_DOWN 'x'
#define MC_SELECT ' '

using namespace std;

///typedefs
typedef string ESSENCE;
typedef string TILE;
typedef string TILESPWNFLAG;

///macro functions
#define ERR(fstr,f) \
    {clear(); \
    mvprintw(0,0,fstr,f); \
    getch();}

///ENUMERATIONS
enum ATT_TYPE{
    ATT_QUICK,
    ATT_NORMAL,
    ATT_HEAVY,
};
enum MOVEACTION{
    MOVEACTION_DAM,
};

///CLASSES
///point class (group of coordinates)
struct Point{
    int x,y;
    Point():x(0),y(0){}
    Point(int x,int y):x(x),y(y){}
};
///status class
struct Status{
    //variables
    float att;
    float def;
    float luck;
    float agi;
    float reg;
    bool nomana;
    //constructors
    Status(float att,float def,float luck,float agi,float reg,bool nomana):
        att(att),
        def(def),
        luck(luck),
        agi(agi),
        reg(reg),
        nomana(nomana)
    {}
    Status():
        att(1),
        def(1),
        luck(1),
        agi(1),
        reg(1),
        nomana(0)
    {}
    //functions
    void multiply(Status status){
        att *=status.att;
        def *=status.def;
        luck*=status.luck;
        agi *=status.agi;
        reg *=status.reg;
        nomana=status.nomana;
    }
    void divide(Status status){
        att /=status.att;
        def /=status.def;
        luck/=status.luck;
        agi /=status.agi;
        reg /=status.reg;
        nomana=status.nomana;
    }
    void add(Status status){
        att +=status.att;
        def +=status.def;
        luck+=status.luck;
        agi +=status.agi;
        reg +=status.reg;
        nomana |= status.nomana;
    }
    int print(int x,int y){
        if(att!=1) mvprintw(y++,x,"ATT: %f",att);
        if(def!=1) mvprintw(y++,x,"DEF: %f",def);
        if(luck!=1) mvprintw(y++,x,"LCK: %f",luck);
        if(agi!=1) mvprintw(y++,x,"AGI: %f",agi);
        if(reg!=1) mvprintw(y++,x,"REG: %f",reg);
        if(nomana) mvprintw(y++,x,"NOMANA");
        return y;
    }
};
///essence bonus id class
struct EssenceVal{
    ESSENCE name;
    int val;
};
///essence class
struct Essence{
    //variables
    string name;
    vector<EssenceVal> bonuses;
};
///essence global variables
vector<Essence*> gEssences;
///essence global functions
Essence *search_essence(string name){
    for(int j=0;j<gEssences.size();j++){
        if(gEssences[j]->name==name) return gEssences[j];
    }
    ERR("ERROR: no %s essence found",name.c_str());
    return NULL;
}
///soul class
class Soul{
    private:
        //variables
        vector<EssenceVal> essenceConcentrations;
        int maxEssenceConcentration;
        //initialization function
        void start(vector<EssenceVal> essConcs){
            maxEssenceConcentration=0;
            for(int j=0;j<essConcs.size();j++){
                essenceConcentrations.push_back(essConcs[j]);
                maxEssenceConcentration+=essConcs[j].val;
            }
        }
    public:
        //friend classes
        friend class SoulTemplate;
        //constructor
        Soul(){
            maxEssenceConcentration=0;
        }
        Soul(vector<EssenceVal> essConcs){
            start(essConcs);
        }
        //functions
        int getEssenceVal(ESSENCE essenceName){
            for(vector<EssenceVal>::iterator q=essenceConcentrations.begin();q!=essenceConcentrations.end();q++){
                if(q->name==essenceName) return q->val;
            }
            return -1;
        }
        double getEssencePercVal(ESSENCE essenceName){
            for(vector<EssenceVal>::iterator q=essenceConcentrations.begin();q!=essenceConcentrations.end();q++){
                if(q->name==essenceName) return (double)(q->val) / (double)maxEssenceConcentration;
            }
            return -1.0;
        }
        //returns true if essence was already present and false if new essence was added
        bool setEssenceVal(ESSENCE essenceName,int val){
            for(vector<EssenceVal>::iterator q=essenceConcentrations.begin();q!=essenceConcentrations.end();q++){
                if(q->name==essenceName){
                    maxEssenceConcentration += val - q->val;
                    if(val==0) q=essenceConcentrations.erase(q);
                    else q->val=val;
                    return true;
                }
            }
            essenceConcentrations.push_back({essenceName,val});
            return false;
        }
};
///soul template class
class SoulTemplate{
    private:
        //variables
        string id;
        Soul soul;
    public:
        //constructor
        SoulTemplate(string id,string startString){
            start(id,startString);
        }
        //constructor like function
        void start(string id,string startString){
            vector<EssenceVal> essConcs;
            stringstream ss(startString);
            while(!ss.eof()){
                string s;
                int n;
                ss >> s;
                if(!search_essence(s)){
                    stringstream ess;
                    ess << "ERR: no \"" << s << "\" essence found in \"" << id << "\" template";
                    ERR("%s",ess.str().c_str());
                }
                ss >> n;
                essConcs.push_back({s,n});
            }
            soul.start(essConcs);
            this->id=id;
        }
        //make soul from soul template
        Soul makeSoul(){return soul;}
        //friend functions
        friend SoulTemplate *search_soul_template(string id);
};
///soul template global variables
vector<SoulTemplate*> gSoulTemplates;
///soul template global functions
//search soul template by id
SoulTemplate *search_soul_template(string id){
    for(vector<SoulTemplate*>::iterator i=gSoulTemplates.begin();i<gSoulTemplates.end();i++){
        if((*i)->id==id) return *i;
    }
    ERR("ERR: nonexistent soul template id searched (%s)",id.c_str());
    return NULL;
}
///effect class
struct Effect{
    //variables
    string name;
    string description;
    int compressionLv;
    Status multStatus;
    enum DURATION{
        NONE,
        ATTACK,
        BATTLE,
        TIME,
        SICKNESS,
        WEAR
    }durationType;
    /*!union DURATION_INFO{*/
    int durationTime;
    string wearItemId;
    /*!};*/
    //static functions
    static bool compareDurations(Effect eff1,Effect eff2){
        if(eff1.durationType!=eff2.durationType) return false;
        else{
            if(eff1.durationType==ATTACK || eff1.durationType==BATTLE || eff1.durationType==WEAR) return true;
            else if(eff1.durationType==TIME){
                if(eff1.durationTime==eff2.durationTime) return true;
                else return false;
            }
            else if(eff1.durationType==SICKNESS){
                //!WIP
                return true;
            }
        }
    }
    //initialization functions
    void start(string effString){
        stringstream ss;
        ss << effString;
        //default values in case nothing is modified
        durationType=NONE;
        name="NONE";
        description="NONE";
        multStatus=Status();
        //return if null string
        if(ss.peek()=='.') return;
        //start reading string
        do{
            //get com
            string com;
            while(ss.peek()!='(') com+=ss.get();
            //add name
            if(com=="NAM"){
                assert(ss.get()=='(');
                name.clear();
                while(ss.peek()!=')') name+=ss.get();
                ss.get();
            }
            //add description
            else if(com=="DES"){
                assert(ss.get()=='(');
                description.clear();
                while(ss.peek()!=')') description+=ss.get();
                ss.get();
            }
            //add duration type and info
            else if(com=="DUR"){
                string type;
                assert(ss.get()=='(');
                while(ss.peek()!=',' && ss.peek()!=')') type+=ss.get();
                //decide type
                if(type=="A") durationType=ATTACK;
                else if(type=="B") durationType=BATTLE;
                else if(type=="T") durationType=TIME;
                else if(type=="S") durationType=SICKNESS;
                else if(type=="W") durationType=WEAR;
                //decide infos
                if(type=="A" || type=="B"){
                    //no info
                    assert(ss.get()==')');
                }
                else if(type=="T"){
                    //amount of turns
                    string s;
                    assert(ss.get()==',');
                    while(ss.peek()!=')') s+=ss.get();
                    durationTime=atoi(s.c_str());
                    ss.get();
                }
                else if(type=="S" || type=="W"){
                    //WIP (no info for now)
                    assert(ss.get()==')');
                }
                else{
                    //invalid decide type
                    cout << "ERR: " << type << "is not a valid decide type" << endl;
                    assert(0);
                }
            }
            //add nomana "multiply"
            else if(com=="NMN"){
                string s;
                assert(ss.get()=='(');
                while(ss.peek()!=')') s+=ss.get();
                multStatus.nomana=atoi(s.c_str());
                ss.get();
            }
            //add attack multiply
            else if(com=="ATT"){
                string s;
                assert(ss.get()=='(');
                while(ss.peek()!=')') s+=ss.get();
                multStatus.att=atof(s.c_str());
                ss.get();
            }
            //add attack multiply
            else if(com=="DEF"){
                string s;
                assert(ss.get()=='(');
                while(ss.peek()!=')') s+=ss.get();
                multStatus.def=atof(s.c_str());
                ss.get();
            }
            //add attack multiply
            else if(com=="LCK"){
                string s;
                assert(ss.get()=='(');
                while(ss.peek()!=')') s+=ss.get();
                multStatus.luck=atof(s.c_str());
                ss.get();
            }
            //add attack multiply
            else if(com=="AGI"){
                string s;
                assert(ss.get()=='(');
                while(ss.peek()!=')') s+=ss.get();
                multStatus.agi=atof(s.c_str());
                ss.get();
            }
            //add attack multiply
            else if(com=="REG"){
                string s;
                assert(ss.get()=='(');
                while(ss.peek()!=')') s+=ss.get();
                multStatus.reg=atof(s.c_str());
                ss.get();
            }
            //invalid command
            else{
                assert(0);
            }
            assert(ss.peek()==';' || ss.peek()=='.');
        }while(ss.get()!='.');
    }
    //constructors
    Effect(string name,string description,Status multStatus,DURATION durationType):
        name(name),
        description(description),
        compressionLv(1),
        multStatus(multStatus),
        durationType(durationType)
    {}
    Effect():
        name("NONE"),
        description("NONE"),
        compressionLv(1),
        multStatus(Status()),
        durationType(ATTACK)
    {}
    Effect(string effString):
        compressionLv(1)
    {
        start(effString);
    }
    //getters
    bool isEmpty(){return durationType==NONE;}
    string getDurationTypeString(){
        switch(durationType){
            case NONE:
                return "none";
                break;
            case ATTACK:
                return "attack";
                break;
            case BATTLE:
                return "battle";
                break;
            case TIME:
                return "time";
                break;
            case SICKNESS:
                return "sickness";
                break;
            case WEAR:
                return "wear";
                break;
        }
    }
    string getDurationInfoString(){
        stringstream ss;
        switch(durationType){
            case ATTACK:
                return "lasts until next attack";
            case BATTLE:
                return "lasts for the remainder of the battle";
            case TIME:
                ss << "lasts " << durationTime << " game turns";
                return ss.str();
            case SICKNESS:
                return "WIP (works like attack type for now";
            case WEAR:
                return "permanent. Given by an equipped item";
        }
    }
    short getFclr(){
        switch(durationType){
            case ATTACK:
                return COLOR_RED;
            case BATTLE:
                return COLOR_MAGENTA;
            case TIME:
                return COLOR_YELLOW;
            case SICKNESS:
                return COLOR_GREEN;
            case WEAR:
                return COLOR_CYAN;
        }
    }
    short getBclr(){
        //WIP (always return black
        return COLOR_BLACK;
    }
    //functions
    void compress(Status compressStatus){
        multStatus.multiply(compressStatus);
        compressionLv++;
        if(compressionLv==2){
            name+=" x2";
        }
        else{
            if(compressionLv<10) name[name.size()-1]=(char)('0'+compressionLv);
            else if(compressionLv==10){
                name[name.size()-1]='1';
                name.push_back('0');
            }
            else if(compressionLv<100){
                name[name.size()-2]=(char)('0'+compressionLv/10);
                name[name.size()-1]=(char)('0'+compressionLv%(((int)compressionLv/10)*10));
            }
            else if(compressionLv==99){
                name[name.size()-2]='M';
                name[name.size()-1]='A';
                name.push_back('X');
            }
        }
    }
    //return true if status is at lv 0
    bool decompress(Status decompressStatus){
        compressionLv--;
        if(compressionLv==0) return true;
        multStatus.divide(decompressStatus);
        if(compressionLv==1){
            name.erase(name.size()-3,name.size());
        }
        else if(compressionLv>1 && compressionLv<9){
            name[name.size()-1]=(char)('0'+compressionLv);
        }
        else if(compressionLv==9){
            name.erase(name.size()-1,name.size());
            name[name.size()-1]='9';
        }
        else if(compressionLv>9 && compressionLv<98){
            name[name.size()-2]=(char)('0'+compressionLv/10);
            name[name.size()-1]=(char)('0'+compressionLv%(((int)compressionLv/10)*10));
        }
        else if(compressionLv==98){
            name.erase(name.size()-1,name.size());
            name[name.size()-1]='8';
            name[name.size()-2]='9';
        }
        return false;
    }
};

///tile class
struct Tile{
    string id;
    string name;
    string description;
    std::vector<TILESPWNFLAG> spawnFlags;
    char gra;
    short fclr,bclr;
    ESSENCE essence;
    int danger;
    bool passable;
    int stair; //! 0 for nothing; 1 for up; -1 for down
    Tile(){;}
    Tile(string id,string name,string description,ESSENCE essence,char gra,short fclr,short bclr,int danger,bool passable,int stair):id(id),name(name),description(description),essence(essence),gra(gra),fclr(fclr),bclr(bclr),danger(danger),passable(passable),stair(stair){}
};
///Tile global variables
//global tile models
vector<Tile*> gTiles;
///Tile global functions
//search tiles
Tile* search_tile(string id){
    for(int j=0;j<gTiles.size();j++){
        if(gTiles[j]->id==id) return gTiles[j];
    }
    ERR("could not find [%s] tile",id.c_str());
    assert(false);
    return NULL;
}

///entity class
struct Entity{
    //variables
    string name,description;
    //constructors
    Entity();
    //functions
    ;
};
///entity functions
Entity::Entity(){
    name="NO NAME";
    description="NO DES";
}

///question defines
#define QESS_OPT "Q_OPT"
#define QESS_ALL "Q_ALL"
///question class
struct Question{
    //variables
    string question;
    string answers[4];
    ESSENCE essence;
    int dif;
    int rightAnswer;
    //constructors
    Question();
    Question(string q,string a1,string a2,string a3,string a4);
    //functions
    string ask();
    bool isRight(string choice);
};
///question functions
//constructors
Question::Question(){
    question.clear();
    for(int j=0;j<4;j++){
        answers[j].clear();
    }
}
//print the question menu on the screen and make it interactive
string Question::ask(){
    Menu qMenu;
    clear();
    glstart_menu(&qMenu,0,0,(char*)question.c_str(),(char*)answers[0].c_str(),(char*)answers[1].c_str(),(char*)answers[2].c_str(),(char*)answers[3].c_str(),"stop");
    string s(inter_menu(&qMenu));
    return s;
}
//check if specified choice is right
bool Question::isRight(string choice){
    if(choice==answers[rightAnswer]) return true;
    return false;
}
///question global variables
vector<Question> gQuestions;

///attack move class
struct AttMove{
    //variables
    string id,name,des;
    string sverb,pverb;
    int mana;
    int agility;
    int chance;
    int qstDif;
    vector<ESSENCE> qEssence;
    vector<MOVEACTION> action;
    Effect qstEff;
    Effect selfEff;
    Effect eneEff;
    //functions
    void giveNameAndDesToEff(Effect& eff){
        if(eff.name=="NONE") eff.name=name+" effect";
        if(eff.description=="NONE") eff.description="effect given by the move \""+name+"\".";
    }
};

///speak node class
struct SpeakNode{
    //enumerations
    enum TXTOWN{
        TXTOWN_USER,
        TXTOWN_CPU,
    };
    //variables
    string title;
    string ret;
    vector<TXTOWN> textOwners;
    vector<string> text;
    vector<SpeakNode*> links;
    SpeakNode *back;
    //constructors
    SpeakNode(SpeakNode *b=NULL):ret("NONE"),back(b){}
    SpeakNode(string title,SpeakNode *b=NULL):title(title),ret("NONE"),back(b){}
};

///body part class
struct BodyPart{
    //variables
    string type;
    bool used;
    //constructors
    BodyPart():type("NONE"),used(false){}
    BodyPart(string type):type(type),used(false){}
};

///body class
struct Body{
    //variables
    vector<BodyPart> bodyParts;
    //functions
    void addBodyPart(BodyPart bp){
        bodyParts.push_back(bp);
    }
    //static functions
    static string makeBodyPartsString(vector<string> bps){
        stringstream ss;
        ss<<"(";
        for(int j=0;j<bps.size();j++){
            ss<<bps[j];
            ss<<"x1";
            ss<<"/";
        }
        return ss.str();
    }
};

///body template class
struct BodyTemplate{
    string id;
    vector<string> bodyParts;
    void start(string startString){
        stringstream ss;
        ss << startString;
        //return if null string
        assert(ss.peek()!='.');
        //start reading string
        do{
            //get body part type
            string type,number;
            while(ss.peek()!='(') type+=ss.get();
            assert(ss.get()=='(');
            //get number
            while(ss.peek()!=')') number+=ss.get();
            //assign to template
            for(int j=atoi(number.c_str());j>0;j--) bodyParts.push_back(type);
            //
            ss.get();
            assert(ss.peek()==';' || ss.peek()=='.');
        }while(ss.get()!='.');
    }
    Body makeBody(){
        Body body;
        for(int j=0;j<bodyParts.size();j++){
            body.addBodyPart(BodyPart(bodyParts[j]));
        }
        return body;
    }
};
///body template global variables
vector<BodyTemplate*> bodyTemplates;
///body templates global functions
//search a body template by id
BodyTemplate *search_body_template(string id){
    for(vector<BodyTemplate*>::iterator i=bodyTemplates.begin();i<bodyTemplates.end();i++){
        if((*i)->id==id) return *i;
    }
    ERR("ERR: nonexistent body template id searched (%s)",id.c_str());
    return NULL;
}

///item component classes and class
//item class
struct Item{
    //spawn component
    struct SpawnComponent{
        vector<ESSENCE> spawnEssences;
        int spawnLv;
        int rarity;
    };
    //wear component
    struct WearComponent{
        vector<string> neededBodyParts;
        Effect wearEffect;
        bool equipped;
        WearComponent():equipped(false){}
    };
    //use component
    struct UseComponent{
        Effect useEffect;
    };
    //quest component
    struct QuestComponent{
        string key;
    };
    //variables
    string id;
    string name,description;
    int size;
    vector<ESSENCE> essences;
    SpawnComponent* spawnComp;
    WearComponent* wearComp;
    UseComponent* useComp;
    QuestComponent* questComp;
    Item():spawnComp(NULL),wearComp(NULL),useComp(NULL),questComp(NULL){}
    /*Item(Item& itm){
        (*this)=itm;
        if(itm.spawnComp!=NULL) spawnComp=new spawnComp(itm.spawnComp);
        else spawnComp=NULL;
        if(itm.wearComp!=NULL) wearComp=new wearComp(itm.wearComp);
        else wearComp=NULL;
        if(itm.useComp!=NULL) useComp=new useComp(itm.useComp);
        else useComp=NULL;
        if(itm.questComp!=NULL) questComp=new questComp(itm.spawnComp);
        else questComp=NULL;
    }*/
};
///item global variables
//all items templates
vector<Item*> items;
///item global functions
//search a item by id
Item *search_item(string id){
    for(vector<Item*>::iterator i=items.begin();i<items.end();i++){
        if((*i)->id==id) return *i;
    }
    ERR("ERR: searched nonexistent item id (%s)",id.c_str());
    return NULL;
}

///inventory class
struct Inventory{
    //variables
    int maxSize;
    int curSize;
    vector<Item> items;
    //constructors
    Inventory():maxSize(0),curSize(0){}
    //functions
    bool addItem(Item item){
        if(curSize+item.size>maxSize) return false;
        curSize+=item.size;
        items.push_back(item);
        return true;
    }
    int removeItem(Item item){
        int n=0;
        for(int j=0;j<items.size();j++){
            if(items[j].id==item.id){
                curSize-=items[j].size;
                items.erase(items.begin()+j);
                j--;
                n++;
            }
        }
        return n;
    }
    bool removeItem(int index){
        if(index>=items.size()){
            assert(0);
            return false;
        }
        curSize-=items[index].size;
        items.erase(items.begin()+index);
        return true;
    }
};

///life class
struct Life:public Entity{
    //enumerations
    enum MOVE{
        USER,
        DUMMY,
        IMMOBILE,
    }moveType;
    //variables
    string id,name,des;
    char gra;
    short fclr,bclr;
    int x,y;
    int vx,vy;
    int rar,swnLv;
    int hp;
    int mana;
    Status status;
    Inventory inventory;
    Body body;
    ESSENCE spawnEssence;
    Soul soul;
    vector<Effect> effects;
    vector<AttMove*> moves;
    SpeakNode *speakHead;
    vector<string> dialogMods;
    //constructors
    ;
    //get status modified by effects
    Status effSt(){
        Status retSt=status;
        for(int j=0;j<effects.size();j++){
            retSt.multiply(effects[j].multStatus);
        }
        return retSt;
    }
    //virtual functions
    void speak();
    //functions
    string showDialog();
    void setCoos(int x,int y){this->x=x;this->y=y;}
    int getAttMoveIndexByName(string name){
        for(int j=0;j<moves.size();j++){
            if(moves[j]->name==name) return j;
        }
        return -1;
    }
    //add an effect to effect vector of life and return a string announcing them
    string applyEffect(Effect eff){
        ///check if effect is empty
        if(eff.isEmpty()) return "NONE";
        ///check if effect is already present
        bool doubleEffect=false;
        for(int j=0;j<effects.size();j++){
            //check name
            if((effects[j].compressionLv==1 && effects[j].name==eff.name) || (effects[j].compressionLv>1 && effects[j].name.find(eff.name)!=string::npos)){
                //check duration type and info
                if(Effect::compareDurations(effects[j],eff)){
                    //compress effect
                    effects[j].compress(eff.multStatus);
                    doubleEffect=true;
                }
            }
        }
        ///add effect to effects
        if(!doubleEffect) effects.push_back(eff);
        ///build effect announcement string
        stringstream ss;
        string effName=(eff.name=="NONE" ? "effect" : eff.name);
        ss << effName << " applied to " << name << ": ";
        Status st=eff.multStatus;
        //nomana
        if(st.nomana)  ss << "nomana mode activated! /";
        //multipliers
        if(st.att!=1)  ss << "attack boosted by " << st.att << " / ";
        if(st.def!=1)  ss << "defense boosted by " << st.def << " / ";
        if(st.luck!=1) ss << "luck boosted by " << st.luck << " / ";
        if(st.agi!=1)  ss << "agility boosted by " << st.agi << " / ";
        //null string case
        if(ss.str()==effName+" applied to "+name+": ") return "NONE";
        //return
        return ss.str();
    }
    //remove all attack duration type effects
    void resetAttackEffects(){
        for(int j=0;j<effects.size();j++){
            if(effects[j].durationType==Effect::ATTACK){
                effects.erase(effects.begin()+j);
                j--;
            }
        }
    }
    //remove all attack duration type effects
    void resetBattleEffects(){
        for(int j=0;j<effects.size();j++){
            if(effects[j].durationType==Effect::BATTLE){
                effects.erase(effects.begin()+j);
                j--;
            }
        }
    }
    //remove a turn from every time based effect and apply regeneration
    void advanceTime(int turns){
        for(vector<Effect>::iterator q=effects.begin();q!=effects.end();q++){
            if(q->durationType==Effect::TIME){
                if(q->durationTime-turns<=0){
                    q = effects.erase(q);
                    q--;
                }
                else
                    q->durationTime-=turns;
            }
        }
        hp += effSt().reg;
    }
    //show all effects applied to life
    void showEffects(){
        clear();
        //print name box
        set_pair(bclr,fclr);
        for(int y=0;y<3;y++) mvaddch(y,0,'*');
        //print "graphics"
        set_pair(fclr,bclr);
        mvaddch(1,2,gra);
        //print name
        set_pair(COLOR_WHITE,COLOR_BLACK);
        mvaddstr(1,4,name.c_str());
        //print effect description divider
        for(int y=3;y<TERMINAL_MAX_Y+1;y++){
            set_pair(COLOR_YELLOW,COLOR_WHITE);
            mvaddch(y,TERMINAL_MAX_X/2,'|');
        }
        //create menu for effects
        Menu effm;
        start_menu(&effm,1,4,"effects",COLOR_WHITE,COLOR_BLACK,MC_SELECT,MC_UP,MC_DOWN,'e',COLOR_WHITE,COLOR_RED,"stop");
        for(int j=0;j<effects.size();j++) add_menuMember(&effm,effects[j].name.c_str(),effects[j].getFclr(),effects[j].getBclr());
        add_menuMember(&effm,"exit",COLOR_WHITE,COLOR_BLACK);
        //make menu interactive
        while(true){
            if(effects.size()==0){
                mvaddstr(4,1,"there are no effects...");
                getch();
                break;
            }
            //get choice as id
            int choice=idinter_menu(&effm);
            //exit if necessary
            if(choice==effects.size()) break;
            //clear
            clear_rectangle(TERMINAL_MAX_X/2+1,3,TERMINAL_MAX_X,TERMINAL_MAX_Y);
            //print description of effect with given id
            Effect eff=effects[choice];
            int X=TERMINAL_MAX_X/2+2;
            //name
            set_pair(eff.getBclr(),eff.getFclr());
            mvaddstr(3,X,eff.name.c_str());
            //description
            int y=4;
            set_pair(COLOR_WHITE,COLOR_BLACK);
            for(int j=0;j<eff.description.size();){
                for(int x=X;x<TERMINAL_MAX_X;x++,j++){
                    if(j>=eff.description.size()) break;
                    mvaddch(y,x,eff.description.c_str()[j]);
                }
                y++;
            }
            //duration
            set_pair(eff.getFclr(),eff.getBclr());
            mvprintw(y+1,X,"duration type: %s",eff.getDurationTypeString().c_str());
            //duration info
            set_pair(COLOR_WHITE,COLOR_BLACK);
            mvprintw(y+2,X,"%s",eff.getDurationInfoString().c_str());
            //print status
            set_pair(COLOR_WHITE,COLOR_BLACK);
            eff.multStatus.print(X,y+4);
        }
    }
    //add an item
    bool addItem(Item item){
        //update inventory max size
        inventory.maxSize=status.att*5;
        //add item to inventory
        return inventory.addItem(item);
    }
    //remove an item
    bool removeItem(Item item){
        return inventory.removeItem(item);
    }
    //equip
    bool equip(Item& itm){
        assert(itm.wearComp!=NULL);
        assert(!itm.wearComp->equipped);
        //check if enough body parts
        bool toBeUsed[body.bodyParts.size()];
        for(int j=0;j<body.bodyParts.size();j++){
            toBeUsed[j]=false;
        }
        bool check;
        for(int j=0;j<itm.wearComp->neededBodyParts.size();j++){
            check=false;
            for(int k=0;k<body.bodyParts.size();k++){
                if(itm.wearComp->neededBodyParts[j]==body.bodyParts[j].type && !body.bodyParts[j].used && !toBeUsed[j]){
                    check=true;
                    toBeUsed[j]=true;
                    break;
                }
            }
            if(!check) break;
        }
        if(check){
            for(int j=0;j<body.bodyParts.size();j++){
                if(toBeUsed[j]=true) body.bodyParts[j].used=true;
            }
            applyEffect(itm.wearComp->wearEffect);
            itm.wearComp->equipped=true;
        }
        return check;
    }
    //unequip
    bool unequip(Item& itm){
        assert(itm.wearComp!=NULL);
        assert(itm.wearComp->equipped);
        bool check;
        //reset body parts
        for(int j=0;j<itm.wearComp->neededBodyParts.size();j++){
            check=false;
            for(int k=0;k<body.bodyParts.size();k++){
                if(itm.wearComp->neededBodyParts[j]==body.bodyParts[j].type && body.bodyParts[j].used){
                    body.bodyParts[j].used=false;
                    check=true;
                    break;
                }
                assert(check);
            }
        }
        //reset equipped flag
        itm.wearComp->equipped=false;
        //reset equip effect (WIP)
        for(int j=0;j<effects.size();j++){
            if(effects[j].durationType==Effect::WEAR){
                if(effects[j].wearItemId==itm.id){
                    if(effects[j].decompress(itm.wearComp->wearEffect.multStatus)){
                        effects.erase(effects.begin()+j);
                    }
                }
            }
        }
    }
    //show inventory
    void showInventory(){
        clear();
        //print name box
        set_pair(bclr,fclr);
        for(int y=0;y<3;y++) mvaddch(y,0,'+');
        //print "graphics"
        set_pair(fclr,bclr);
        mvaddch(1,2,gra);
        //print name
        set_pair(COLOR_WHITE,COLOR_BLACK);
        mvaddstr(1,4,name.c_str());
        //print effect description divider
        for(int y=3;y<TERMINAL_MAX_Y+1;y++){
            set_pair(COLOR_BLUE,COLOR_GREEN);
            mvaddch(y,TERMINAL_MAX_X/2,'{');
        }
        //create menu for items
        Menu itmm;
        start_menu(&itmm,1,4,"items",COLOR_WHITE,COLOR_BLACK,MC_SELECT,MC_UP,MC_DOWN,'e',COLOR_WHITE,COLOR_RED,"stop");
        for(int j=0;j<inventory.items.size();j++) add_menuMember(&itmm,inventory.items[j].name.c_str(),COLOR_WHITE,COLOR_BLACK);
        add_menuMember(&itmm,"exit",COLOR_WHITE,COLOR_BLACK);
        //make menu interactive
        while(true){
            if(inventory.items.size()==0){
                mvaddstr(4,1,"there are no items...");
                getch();
                break;
            }
            //get choice as id
            int choice=idinter_menu(&itmm);
            //exit if necessary
            if(choice==inventory.items.size()) break;
            //clear
            clear_rectangle(TERMINAL_MAX_X/2+1,3,TERMINAL_MAX_X,TERMINAL_MAX_Y);
            //print description of effect with given id
            Item itm=inventory.items[choice];
            int X=TERMINAL_MAX_X/2+2;
            //name
            set_pair(COLOR_BLACK,COLOR_WHITE);
            mvaddstr(3,X,itm.name.c_str());
            //description
            int y=4;
            set_pair(COLOR_WHITE,COLOR_BLACK);
            for(int j=0;j<itm.description.size();){
                for(int x=X;x<TERMINAL_MAX_X;x++,j++){
                    if(j>=itm.description.size()) break;
                    mvaddch(y,x,itm.description.c_str()[j]);
                }
                y++;
            }
            //quest key
            if(itm.questComp!=NULL){
                set_pair(COLOR_BLUE,COLOR_BLACK);
                mvprintw(y+=2,X,"quest key: %s",itm.questComp->key.c_str());
            }
            //print rarity
            if(itm.spawnComp!=NULL){
                set_pair(COLOR_YELLOW,COLOR_BLACK);
                mvprintw(y+=1,X,"spawn level: %d",itm.spawnComp->spawnLv);
                mvprintw(y+=1,X,"rarity: %d",itm.spawnComp->rarity);
            }
            //wear requirements
            if(itm.wearComp!=NULL){
                set_pair(COLOR_MAGENTA,COLOR_BLACK);
                mvprintw(y+=2,X,"needed body parts:");
                set_pair(COLOR_WHITE,COLOR_BLACK);
                mvprintw(y+=1,X,"%s",Body::makeBodyPartsString(itm.wearComp->neededBodyParts).c_str());
            }
            //wear effect
            if(itm.wearComp!=NULL){
                Effect eff=itm.wearComp->wearEffect;
                set_pair(eff.getFclr(),eff.getBclr());
                mvprintw(y+=2,X,"wear effect: %s (%s)",eff.name.c_str(),eff.getDurationTypeString().c_str());
                set_pair(COLOR_WHITE,COLOR_BLACK);
                y=eff.multStatus.print(X,y+=1);
            }
            //use effect
            if(itm.useComp!=NULL){
                Effect eff=itm.useComp->useEffect;
                set_pair(eff.getFclr(),eff.getBclr());
                mvprintw(y+=2,X,"use effect: %s (%s)",eff.name.c_str(),eff.getDurationTypeString().c_str());
                set_pair(COLOR_WHITE,COLOR_BLACK);
                y=eff.multStatus.print(X,y+=1);
            }
            //start use menu
            Menu usem;
            start_menu(&usem,1,4,"what you can do",COLOR_WHITE,COLOR_BLACK,MC_SELECT,MC_DOWN,MC_UP,'e',COLOR_WHITE,COLOR_RED,"stop");
            if(itm.wearComp!=NULL){
                if(!itm.wearComp->equipped) add_menuMember(&usem,"equip",COLOR_CYAN,COLOR_BLACK);
                else add_menuMember(&usem,"unequip",COLOR_CYAN,COLOR_BLACK);
            }
            if(itm.useComp!=NULL) add_menuMember(&usem,"use",COLOR_GREEN,COLOR_BLACK);
            erase_menu(itmm);
            string useCom(inter_menu(&usem));
            if(useCom=="equip"){
                if(!equip(itm)){
                    clear_rectangle(TERMINAL_MAX_X/2+1,4,TERMINAL_MAX_X,TERMINAL_MAX_Y);
                    set_pair(COLOR_RED,COLOR_BLACK);
                    mvprintw(5,TERMINAL_MAX_X/2+2,"you don't have the right body parts");
                    refresh();
                    getch();
                }
            }
            else if(useCom=="unequip"){
                unequip(itm);
            }
            else if(useCom=="use"){
                ///use(itm);
            }
            clear_rectangle(0,4,TERMINAL_MAX_X/2-1,TERMINAL_MAX_Y);
            clear_rectangle(TERMINAL_MAX_X/2+1,3,TERMINAL_MAX_X,TERMINAL_MAX_Y);
        }
    }
};
///life global variables
vector<Life*> life;
int mesn=0;
vector<string> allMes;
vector<short> mesColors;
bool mesSub=false;
string speakBox;
Life *adv=NULL;
///life global functions
//search a life by id
Life *search_life(string id){
    for(vector<Life*>::iterator i=life.begin();i<life.end();i++){
        if((*i)->id==id) return *i;
    }
    ERR("%s",id.c_str());
    assert(0);
    return NULL;
}
//battle
#define BSMX 51 //!MUST BE ODD
#define BSMY 13
#define BSMM 10
//display battle message
void setMesSub(bool toggle){
    mesSub=toggle;
    if(!mesSub){
        for(int j=0;j<mesColors.size();j++){
            mesColors[j]=DEF_FCLR;
        }
    }
}
void printBatmes(){
    for(int j=0;j<allMes.size();j++){
        set_pair(mesColors[j],DEF_BCLR);
        mvprintw(BSMY+j+1,0,"%s",allMes[j].c_str());
    }
    set_pair(DEF_FCLR,DEF_BCLR);
    refresh();
}
void batmes(string mes){
    if(mes!="NONE"){
        mesn++;
        if(mesn>BSMM){
            allMes.erase(allMes.begin());
            mesColors.erase(mesColors.begin());
            clear();
        }
        allMes.push_back(mes);
        if(mesSub) mesColors.push_back(COLOR_YELLOW);
        else mesColors.push_back(DEF_FCLR);
        printBatmes();
    }
}
//normal attack
void attack(int pl,ATT_TYPE type,Life &latt,Life &ldef){
    //calculate hit chance
    stringstream attVerb,missVerb;
    int hitChance,bHitChance=latt.effSt().agi*100/(latt.effSt().agi+ldef.effSt().agi);
    if(type==ATT_NORMAL){
        hitChance=bHitChance;
    }
    else if(type==ATT_QUICK){
        hitChance=bHitChance*(rand()%3+1);
        latt.applyEffect(Effect("Quickness","makes your attacks weaker, but more likely to hit",Status(0.5,1,1,1,1,1),Effect::ATTACK));
        attVerb << "quickly ";
    }
    else if(type==ATT_HEAVY){
        hitChance=bHitChance*0.25*(rand()%4+1);
        latt.applyEffect(Effect("Heaviness","makes your attacks stronger, but less likely to hit",Status(0.5,1,1,1,1,1),Effect::ATTACK));
        attVerb << "heavily ";
    }
    if(pl==0 || pl==2){
        attVerb << "attacks";
        missVerb << "misses";
    }
    else if(pl==1){
        attVerb << "attack";
        missVerb << "miss";
    }
    if(hitChance<40 && type!=ATT_HEAVY) hitChance=40;
    if(hitChance<5) hitChance=5;
    //check if attack is successful
    string defName;
    string attName;
    if(pl==1){
        attName="you";
        defName="the " + ldef.name;
    }
    if(pl==2){
        defName="you";
        attName="the " + latt.name;
    }
    if(rand()%100<hitChance){
        //extract needed values
        int att=(int)latt.effSt().att;
        int attMaxLuck=(int)latt.effSt().luck;
        int def=(int)ldef.effSt().def;
        int defMaxLuck=(int)ldef.effSt().luck;
        //randomly calculate luck boosts
        int attLuckBoost=rand()%attMaxLuck;
        int defLuckBoost=rand()%defMaxLuck;
        //apply luck boosts
        int luckAtt=att+(att*attLuckBoost)/100;
        int luckDef=def+(def*defLuckBoost)/100;
        //calculate damage dealt
        int dam=luckAtt-(luckAtt*luckDef)/100;
        //prevent damage from being negative
        if(dam<0) dam=0;
        //inflict damage
        else ldef.hp-=dam;
        /*
        ERR("%d ",att);
        ERR("%d ",attMaxLuck);
        ERR("%d ",def);
        ERR("%d ",defMaxLuck);
        ERR("%d ",attLuckBoost);
        ERR("%d ",defLuckBoost);
        ERR("%d ",luckAtt);
        ERR("%d ",luckDef);
        ERR("%d ",dam);
        */
        //print attack
        stringstream ss;
        ss << attName << " " << attVerb.str() << " " << defName << " inflicting " << dam << "hp.";
        setMesSub(true);
        batmes(ss.str());
    }
    else{
        //print miss
        stringstream ss;
        ss << attName << " " << missVerb.str() << " " << defName;
        setMesSub(true);
        batmes(ss.str());
    }
    //reset all attack duration multipliers
    latt.resetAttackEffects();
}
//move special attack
bool moveAttack(int pl,Life &latt,Life &ldef,int mvIndex){
    AttMove am=*latt.moves[mvIndex];
    //announce question
    if(pl==1){
        setMesSub(true);
        std::stringstream ss;
        ss << "you use " << latt.moves[mvIndex]->name << "!";
        batmes(ss.str());
    }
    else if(pl==2){
        setMesSub(true);
        std::stringstream ss;
        ss << "the " << latt.name << " uses " << latt.moves[mvIndex]->name << "!";
        batmes(ss.str());
    }
    //player - ask question
    if(pl==1){
        //check if question is needed or optional
        bool question=true;
        if(am.qEssence.empty()) question=false;
        else if(find(am.qEssence.begin(),am.qEssence.end(),QESS_OPT)!=am.qEssence.end()){
            Menu m;
            glstart_menu(&m,BSMX+2,0,"ANSWER A QUESTION?","yes","no","stop");
            string choice(inter_menu(&m));
            if(choice=="no") question=false;
            erase_menu(m);
        }
        //ask question (maybe)
        if(question){
            //check if qessence contains all
            bool all=false;
            if(find(am.qEssence.begin(),am.qEssence.end(),QESS_ALL)!=am.qEssence.end()){
                all=true;
            }
            //find question of the attmove qessence
            int qi;
            do{
                qi=rand()%gQuestions.size();
            }while((gQuestions[qi].essence!=am.qEssence[rand()%am.qEssence.size()] && !all) || gQuestions[qi].dif!=am.qstDif);
            string choice=gQuestions[qi].ask();
            //apply question effect
            if(gQuestions[qi].isRight(choice)){
                mvprintw(7,0,"that is correct!");
                setMesSub(true);
                batmes(latt.applyEffect(am.qstEff));
            }
            else mvprintw(7,0,"wrong answer...");
            refresh();
            getch();
            clear();
        }
    }
    //cpu - 50/50 chance of getting question effects
    else if(pl==2){
        if(rand()%100<50){
            setMesSub(true);
            batmes(latt.applyEffect(am.qstEff));
        }
    }
    //calculate if move happens
    if(rand()%100>=am.chance){
        setMesSub(true);
        if(pl==1) batmes("you fail at applying the move!");
        else{
            std::stringstream ss;
            ss << "the " << latt.name << " fails at applying the move!";
            batmes(ss.str());
        }
        return true;
    }
    //check if nomana mode is available
    bool nomana=false;
    for(int j=0;j<latt.effects.size();j++){
        if(latt.effects[j].multStatus.nomana){
            nomana=true;
            setMesSub(true);
            if(pl==1) batmes("the move requires you no mana.");
            else{
                std::stringstream ss;
                ss << "the " << latt.name << " does not waste any mana at applying the move.";
                batmes(ss.str());
            }
            break;
        }
    }
    //if nomana is not available check if enough mana
    if(!nomana){
        //check if enough mana and subtract mana cost
        if(latt.mana<am.mana){
            setMesSub(true);
            batmes("not enough mana");
            return false;
        }
        else latt.mana-=am.mana;
    }
    //find out agility
    int agility;
    if(am.agility==-1) agility=latt.effSt().agi;
    else agility=(latt.effSt().agi+am.agility)/2;
    //apply self effects
    setMesSub(true);
    batmes(latt.applyEffect(am.selfEff));
    //apply enemy effect
    batmes(ldef.applyEffect(am.eneEff));
    //execute move
    for(vector<MOVEACTION>::iterator q=am.action.begin();q!=am.action.end();q++){
        //attack enemy
        if(*q==MOVEACTION_DAM){
            attack(pl,ATT_NORMAL,latt,ldef);
        }
    }
    //return success
    return true;
}
int printBattleArena(Life &you,Life ene){
    //print frame
    set_pair(COLOR_WHITE,COLOR_RED);
    for(int y=0;y<BSMY;y+=BSMY-1){
        for(int x=0;x<BSMX;x++){
            mvaddch(y,x,' ');
        }
    }
    for(int x=0;x<BSMX;x+=(BSMX-1)/2){
        for(int y=1;y<BSMY-1;y++){
            mvaddch(y,x,' ');
        }
    }
    //print you statistics
    set_pair(DEF_FCLR,DEF_BCLR);
    mvprintw(1,1,"%s   ",you.name.c_str());
    mvprintw(2,1,"hp: %d   ",you.hp);
    mvprintw(3,1,"mana: %d   ",you.mana);
    //print ene statistics
    mvprintw(1,(BSMX-1)/2+1,"%s   ",ene.name.c_str());
    mvprintw(2,(BSMX-1)/2+1,"hp: %d   ",ene.hp);
    mvprintw(3,(BSMX-1)/2+1,"mana: %d   ",ene.mana);
    //print graphics
    set_pair(DEF_FCLR,DEF_BCLR);
    mvaddch(BSMY/2,BSMX/4,you.name[0]);
    mvaddch(BSMY/2,BSMX/4*3,ene.name[0]);
}
int battle(Life &you,Life ene){
    clear();
    while(you.hp>0 && ene.hp>0){
        //print battle menu and proceed with the player's turn
        while(true){
            //print "arena"
            printBattleArena(you,ene);
            //print menu
            Menu bm;
            glstart_menu(&bm,BSMX+2,0,"CHOICES:","attack","quick attack","heavy attack","use an attack move","show status effects","show enemy's status effects","escape","stop");
            string choice(inter_menu(&bm));
            //normal attack
            if(choice=="attack"){
                attack(1,ATT_NORMAL,you,ene);
                break;
            }
            //quick attack
            if(choice=="quick attack"){
                attack(1,ATT_QUICK,you,ene);
                break;
            }
            //heavy attack
            if(choice=="heavy attack"){
                attack(1,ATT_HEAVY,you,ene);
                break;
            }
            //attack move
            else if(choice=="use an attack move"){
                erase_menu(bm);
                Menu amm;
                glstart_menu(&amm,BSMX+2,0,"ATT MOVES:","stop");
                for(int j=0;j<you.moves.size();j++){
                    gladd_menuMember(&amm,you.moves[j]->name.c_str());
                }
                gladd_menuMember(&amm,"back <-");
                choice=inter_menu(&amm);
                int attMoveIndex=you.getAttMoveIndexByName(choice);
                if(attMoveIndex!=-1){
                    erase_menu(amm);
                    if(moveAttack(1,you,ene,attMoveIndex)) break;
                    else continue;
                }
                else if(choice=="back <-"){
                    erase_menu(amm);
                    continue;
                }
                else{
                    batmes("ERR: no such move...");
                };
            }
            //show status effects
            else if(choice=="show status effects"){
                you.showEffects();
                clear();
                printBattleArena(you,ene);
                printBatmes();
                continue;
            }
            //show enemy's status effects
            else if(choice=="show enemy's status effects"){
                ene.showEffects();
                clear();
                printBattleArena(you,ene);
                printBatmes();
                continue;
            }
            //escape from battle
            else if(choice=="escape"){
                ///very WIP way of exiting
                ene.hp=-1;
                break;
            }
        }
        //add a turn (for time based effects)
        you.advanceTime(1);
        //proceed with the CPU's turn
        if(you.hp<0 || ene.hp<0) break;
        //try an attack move for a random amount of time
        int j;
        for(j=rand()%10;j>=0;j--){
            int k;
            if(ene.moves[k=rand()%ene.moves.size()]->mana<=ene.mana && rand()%100<50){
                moveAttack(2,ene,you,k);
                break;
            }
        }
        //choose a random attack based on agility
        if(j==-1){
            int r=rand()%100;
            if(r<15) attack(2,ATT_QUICK,ene,you);
            else if(r+=15<70) attack(2,ATT_NORMAL,ene,you);
            else if(r+=70<15) attack(2,ATT_HEAVY,ene,you);
        }
        //add a turn (for time based effects)
        ene.advanceTime(1);
        //
        setMesSub(false);
    }
    int ret;
    if(you.hp<=0){
        stringstream ss;
        ss << "you die...";
        batmes(ss.str());
        getch();
        ret=1;
    }
    else if(ene.hp<=0){
        stringstream ss;
        ss << ene.name << " dies!";
        batmes(ss.str());
        //reset battle duration types effects
        you.resetBattleEffects();
        getch();
        ret=2;
    }
    else ret=0;
    mesn=0;
    allMes.clear();
    mesColors.clear();
    return ret;
}
//speak box
#define SB_GRA_X 0
#define SB_GRA_Y 15
#define SB_GRA_W 6
#define SB_GRA_H 4
#define SB_BOX_X (SB_GRA_X+SB_GRA_W+1) + 0
#define SB_BOX_Y (SB_GRA_Y)
#define SB_BOX_W 72
#define SB_BOX_H (SB_GRA_H)
#define SM_X 0
#define SM_Y SB_BOX_Y
void interSpeakBox(string s,char gra){
    //print gra borders
    const char BGRA='+';
    set_pair(COLOR_YELLOW,COLOR_BLUE);
    for(int x=0;x<=SB_GRA_W;x++){
        mvaddch(SB_GRA_Y,SB_GRA_X+x,BGRA);
        mvaddch(SB_GRA_Y+SB_GRA_H,SB_GRA_X+x,BGRA);
    }
    for(int y=0;y<=SB_GRA_H;y++){
        mvaddch(SB_GRA_Y+y,SB_GRA_X,BGRA);
        mvaddch(SB_GRA_Y+y,SB_GRA_X+SB_GRA_W,BGRA);
    }
    //print gra
    mvaddch(SB_GRA_Y+SB_GRA_H/2,SB_GRA_X+SB_GRA_W/2,gra);
    //print box borders
    const char BBOX='\\';
    set_pair(COLOR_GREEN,COLOR_YELLOW);
    for(int x=0;x<=SB_BOX_W;x++){
        mvaddch(SB_BOX_Y,SB_BOX_X+x,BBOX);
        mvaddch(SB_BOX_Y+SB_BOX_H,SB_BOX_X+x,BBOX);
    }
    for(int y=0;y<=SB_BOX_H;y++){
        mvaddch(SB_BOX_Y+y,SB_BOX_X,BBOX);
        mvaddch(SB_BOX_Y+y,SB_BOX_X+SB_BOX_W,BBOX);
    }
    //print speak string
    set_pair(COLOR_WHITE,COLOR_BLACK);
    mvaddstr(SB_BOX_Y+SB_BOX_H/2,SB_BOX_X+2,s.c_str());
    //show everything
    refresh();
    getch();
    //erase everything
    draw_rectangle(SB_GRA_X,SB_GRA_Y,SB_GRA_X+SB_GRA_W,SB_GRA_Y+SB_GRA_H,COLOR_BLACK,COLOR_BLACK,' ');
    draw_rectangle(SB_BOX_X,SB_BOX_Y,SB_BOX_X+SB_BOX_W,SB_BOX_Y+SB_BOX_H,COLOR_BLACK,COLOR_BLACK,' ');
}
///life member functions
string Life::showDialog(){
    SpeakNode *sn=speakHead;
    while(true){
        int lastText;
        for(lastText=0;lastText<sn->text.size();lastText++){
            if(sn->text[lastText]!="RET"){
                if(sn->textOwners[lastText]==SpeakNode::TXTOWN_USER) interSpeakBox(sn->text[lastText],adv->gra);
                else interSpeakBox(sn->text[lastText],gra);
            }
        }
        if(sn->ret!="NONE") return sn->ret;
        if(!sn->links.empty()){
            Menu m;
            glstart_menu(&m,SM_X,SM_Y,sn->text[lastText-1].c_str(),"stop");
            for(int j=0;j<sn->links.size();j++){
                gladd_menuMember(&m,sn->links[j]->title.c_str());
            }
            sn=sn->links[idinter_menu(&m)];
            erase_menu(m);
        }
        else return "NONE";
    }
}
void Life::speak(){
    stringstream ss(showDialog());
    string s;
    ss >> s;
    bool err=false;
    //universal dialog returns
    //none (ignore)
    if(s=="NONE"){
        ;
    }
    //battle
    else if(s=="BATTLE"){
        battle(*adv,*this);
    }
    else if(!dialogMods.empty()){
        //specific dialog returns
        for(vector<string>::iterator q=dialogMods.begin();q!=dialogMods.end();q++){
            string dm=*q;
            /*
            //quest maker
            if(dm=="QUESTMAKER"){
                //give a random quest of a specified level
                if(s=="GIVE_RAND_QUEST"){
                    int lv;
                    ss >> lv;
                    adv->addItem(new QuestMap(lv));
                }
                //error
                else err=true;
            }
            */
        }
    }
    else err=true;
    if(err) ERR("%s not recognized as a REC value",s.c_str());
}

/*
///item class
struct Item:public Entity{
    //variables
    string name;
    //pure virtual functions
    virtual void equip()=0;
    virtual void unequip()=0;
};
*/

///board class
struct Board{
    //variables
    vector<Life*> blife;
    vector< vector<TILE> > tiles;
    short bgclr;
    int level;
    //constructors
    Board(int maxx,int maxy,short bgclr,int level);
    Board(short bgclr);
    Board();
    //functions
    TILE at(int x,int y);
    Tile getTile(int x,int y);
    Life *getLife(int x,int y);
    void autoGenerate(int maxx,int maxy,int seed);
    void generateFromFile(string fileName);
    bool isOcc(int x,int y);
    bool checkSpawn(int x,int y);
    bool spawn(int x,int y,Life* life);
    bool spawn(int x,int y,string id);
    int move(); //!return stair value of stair tile
    void print(int tx,int ty,int bx,int by,int w,int h);
    void print(int sx,int sy);
    void print(int sx,int sy,Life *life);
    void print(Life *life);
    void print();
    //static functions
    static bool isInField(int x,int y,int fx,int fy,int w,int h);
};

///board static functions
//returns true if the given x/y coordinates are within range of the given sx-sy;ex-ey field
bool Board::isInField(int x,int y,int fx,int fy,int w,int h){
    if(x<fx || x>fx+w || y<fy || y>fy+h) return false;
    return true;
}

///board functions
//constructor
Board::Board(int maxx,int maxy,short bgclr,int level):
    bgclr(bgclr),
    level(level)
{
    tiles.resize(maxx);
    for(auto& col : tiles)
        col.resize(maxy);
    for(auto& col : tiles){
        for(auto& tile : col){
            tile = "empty";
        }
    }
}
Board::Board(short bgclr):Board(0, 0, bgclr, 0){}
Board::Board():Board(COLOR_BLACK){}
//getters
TILE Board::at(int x,int y){
    return tiles[x][y];
}
Tile Board::getTile(int x,int y){
    if(x > tiles.size() || y > tiles[0].size()){
        std::stringstream ss;
        ss << "coordinates out of bounds: (" << x << "; " << y << ")" << " max: (" << tiles.size() << "; " << tiles[0].size() << ")";
        ERR("%s", ss.str().c_str());
    }
    return *search_tile(tiles[x][y]);
}
Life *Board::getLife(int x,int y){
    for(vector<Life*>::iterator q=blife.begin();q!=blife.end();q++){
        Life *l=*q;
        if(l->x==x && l->y==y) return l;
    }
    return NULL;
}
//other functions
void Board::generateFromFile(string fileName){
    ifstream f(fileName.c_str());
    //check if id is ok
    while(f.get()!=':');
    string id;
    f >> id;
    assert(id==fileName);
    //get name
    while(f.get()!=':');
    //f >> name;
    //get symbol array
    struct Symbol{
        string type;
        string id;
        char symbol;
    };
    //while()
}
bool Board::isOcc(int x,int y){
    for(int j=0;j<blife.size();j++){
        if(blife[j]->x==x && blife[j]->y==y) return true;
    }
    return false;
}
bool Board::checkSpawn(int x,int y){
    if(isOcc(x,y) || y>=tiles[0].size() || x>=tiles.size() || !getTile(x,y).passable) return false;
    return true;
}
bool Board::spawn(int x,int y,Life* life){
    if(!checkSpawn(x,y)) return false;
    life->setCoos(x,y);
    blife.push_back(life);
    return true;
}
bool Board::spawn(int x,int y,string id){
    return spawn(x,y,new Life(*(search_life(id))));
}
int Board::move(){
    for(int j=0;j<blife.size();j++){
        Life::MOVE mt=blife[j]->moveType;
        if(mt==Life::USER){
            bool goOn=true;
            int x=0,y=0;
            while(goOn){
                goOn=false;
                char com=getch();
                switch(com){
                    case C_N:
                        y--;
                        break;
                    case C_E:
                        x++;
                        break;
                    case C_S:
                        y++;
                        break;
                    case C_W:
                        x--;
                        break;
                    case C_NW:
                        x--;
                        y--;
                        break;
                    case C_NE:
                        x++;
                        y--;
                        break;
                    case C_SE:
                        x++;
                        y++;
                        break;
                    case C_SW:
                        x--;
                        y++;
                        break;
                    case C_OPEN_INV:
                        blife[j]->showInventory();
                        goOn=true;

                        break;
                    default:
                        break;
                }
            }
            int fx=blife[j]->x+x;
            int fy=blife[j]->y+y;
            if(checkSpawn(fx,fy)){
                blife[j]->x=fx;
                blife[j]->y=fy;
                //check for grass battles
                if(getTile(fx,fy).danger>0){
                    //check if spawn happens
                    if(rand()%100<50){
                        int loopCheck;
                        int lcEssence=0,lcHighSpawnLv=0,lcRarity=0;
                        for(loopCheck=0;loopCheck<1000;loopCheck++){
                            int ene=rand()%life.size();
                            //enemy's essence is different from the tile's
                            if(life[ene]->spawnEssence!=getTile(fx,fy).essence){
                                lcEssence++;
                                continue;
                            }
                            //spawn level too high
                            if(life[ene]->swnLv>level*getTile(fx,fy).danger){
                                lcHighSpawnLv++;
                                continue;
                            }
                            //spawn level too low
                            //!if(life[ene]->swnLv<level/5) continue; WIP
                            //rarity test
                            if(rand()%100>(100-life[ene]->rar)){
                                lcRarity++;
                                continue;
                            }
                            //!WIP upgrade life
                            ;
                            //start battle
                            battle(*(blife[j]),*life[ene]);
                            break;
                        }
                        if(loopCheck == 1000){
                            stringstream ss;
                            ss << "ERR: loopCheck failed while finding correct enemy for spawning in this tile:\n";
                            ss << "essence: " << getTile(fx,fy).essence << "\n";
                            ss << "danger: " << getTile(fx,fy).danger << "\n";
                            ss << "loop checks: essence(" << lcEssence << "); highSpawnLv(" << lcHighSpawnLv << "); rarity(" << lcRarity << ")\n";
                            ERR("%s",ss.str().c_str());
                        }
                    }
                }
                //check if tile is stairs
                else if(getTile(fx,fy).stair!=0){
                    //assert
                    assert(getTile(fx,fy).stair==1 || getTile(fx,fy).stair==-1);
                    //return stair value
                    return getTile(fx,fy).stair;
                }
            }
            else if(isOcc(fx,fy)){
                //check if unit can speak
                Life *l=getLife(fx,fy);
                SpeakNode *DB_l_speakHead=l->speakHead;
                if(l->speakHead!=NULL){
                    l->speak();
                }
            }
        }
        else if(mt==Life::DUMMY){
            int x,y;
            do{
                x=rand()%3-1;
                y=rand()%3-1;
            }while(!checkSpawn(blife[j]->x+x,blife[j]->y+y));
            blife[j]->x+=x;
            blife[j]->y+=y;
        }
        else if(mt==Life::IMMOBILE){
            ;
        }
    }
    ///no stairs touched - return 0
    return 0;
}
void Board::print(int tx,int ty,int bx,int by,int w,int h){
    for(int y=0;y<h;y++){
        for(int x=0;x<w;x++){
            set_pair(getTile(bx+x,by+y).fclr,bgclr);
            mvaddch(ty+y,tx+x,search_tile(tiles[bx+x][by+y])->gra);
        }
    }
    for(int j=0;j<blife.size();j++){
        Life *l=blife[j];
        if(isInField(l->x,l->y,bx,by,w,h)){
            short bclr=l->bclr;
            if(bclr==COLOR_NULL) bclr=bgclr;
            set_pair(l->fclr,bclr);
            mvaddch(ty+(l->y-by),tx+(l->x-bx),l->gra);
        }
    }
    set_pair(DEF_FCLR,DEF_BCLR);
}
void Board::print(int sx,int sy){
    print(sx,sy,0,0,tiles.size(),tiles[0].size());
}
void Board::print(){
    print(0,0);
}
void Board::print(int sx,int sy,Life* life){
    if(life==NULL) return;
    int vsx=life->x-life->vx;
    int vsy=life->y-life->vy;
    int vw=2*life->vx+1;
    int vh=2*life->vy+1;
    if(vsx<0) vsx=0;
    if(vsx+vw>tiles.size()) vsx=tiles.size()-vw;
    if(vsy<0) vsy=0;
    if(vsy+vh>tiles[0].size()) vsy=tiles[0].size()-vh;
    print(sx,sy,vsx,vsy,vw,vh);
}
void Board::print(Life *life){
    print(0,0,life);
}


///dungeon class (TEMP)
class Dungeon{
    public:
        //structures
        struct Floor{
            //variables
            vector<ESSENCE> essences;
            int seed;
            int difficulty;
            //functions
            Board* generateBoard(Life* adv);
        };
        //constructors
        Dungeon(int seed,int depth,int difficulty,Life* adv);
        //getters
        Board* getBoard(){return currentBoard;}
        int getDepth(){return floors.size();}
        int getDifficulty(){return floors[1].difficulty-floors[0].difficulty;}
        //setters
        void setAdventurer(Life* adv){currentAdv=adv;}
        //functions
        void move();
    private:
        //current board
        Board* currentBoard;
        //current adventurer (main controllable life)
        Life* currentAdv;
        //floors
        vector<Floor> floors;
        //int current room number
        int currentRoomId;
};

///floor functions
//generate board from floor parameters
Board* Dungeon::Floor::generateBoard(Life* adv){
    //random width and height of the floor
    size_t width = dgen::rrange(50, 100);
    size_t height = dgen::rrange(50, 100);
    //return value
    Board* retBoard = new Board(width, height, COLOR_BLACK, difficulty);
    //floor tile and wall tile
    TILE floorTile("med_floor");
    TILE wallTile("med_wall");
    //generation debug file
    ofstream dbf("dungeon_generation_log.txt", ofstream::app);
    //set seed
    srand(seed);
    //generate randomly sized dungeon floor (board) with random parameters
    dgen::Board board(width, height);
    dgen::GenInfo ginf;
    //...
    board.generate(ginf, dbf);
    //pick tiles to build dungeon with
    auto findSuitableTileType = [this](const TILESPWNFLAG& spawnFlag) {
        std::vector<TILE> suitableTiles;
        for(const auto tile : gTiles) {
            if(std::find(tile->spawnFlags.begin(), tile->spawnFlags.end(), spawnFlag) != tile->spawnFlags.end()) {
                bool tileOk = false;
                if(std::find(essences.begin(), essences.end(), tile->essence) != essences.end())
                    suitableTiles.push_back(tile->id);
            }
        }
        return suitableTiles;
    };
    auto suitableFloors = findSuitableTileType("FLOOR");
    if(suitableFloors.empty())
        suitableFloors.push_back("def_floor");
    auto suitableGrasses = findSuitableTileType("GRASS");
    if(suitableGrasses.empty())
        suitableGrasses.push_back("def_grass");
    auto suitableWalls = findSuitableTileType("WALL");
    if(suitableWalls.empty())
        suitableWalls.push_back("def_wall");
    auto suitableDownStairs = findSuitableTileType("DSTAIRS");
    if(suitableDownStairs.empty())
        suitableDownStairs.push_back("def_dstairs");
    auto suitableUpStairs = findSuitableTileType("USTAIRS");
    if(suitableUpStairs.empty())
        suitableUpStairs.push_back("def_ustairs");
    //convert board into floor
    for(size_t x = 0; x < retBoard->tiles.size(); x++){
        for(size_t y = 0; y < retBoard->tiles[0].size(); y++){
            auto tile = board.getTile({x,y});
            if(tile == dgen::Tile::Floor){
                if(!dgen::rrange(2))
                    retBoard->tiles[x][y] = suitableGrasses[dgen::rrange(suitableGrasses.size())];
                else
                    retBoard->tiles[x][y] = suitableFloors[dgen::rrange(suitableFloors.size())];
            }
            else retBoard->tiles[x][y] = suitableWalls[dgen::rrange(suitableWalls.size())];
        }
    }
    //spawn adventurer on board
    while(!retBoard->spawn(dgen::rrange(0, width), dgen::rrange(0, height), adv));
    //spawn some stairs
    auto stairsNum = dgen::rrange(1,3);
    for(size_t j = 0; j < stairsNum; ++j){
        int x, y;
         do{
            x = dgen::rrange(width);
            y = dgen::rrange(height);
         }while(!retBoard->checkSpawn(x, y));
         retBoard->tiles[x][y] = suitableDownStairs[dgen::rrange(suitableDownStairs.size())];
    }
    //return
    return retBoard;
}

///dungeon class functions
//constructors
Dungeon::Dungeon(int seed,int depth,int difficulty,Life* adv){
    //check parameters
    assert(difficulty>0 && adv!=NULL);
    //set adv
    currentAdv=adv;
    //set seed
    srand(seed);
    //generate dungeon randomly
    floors.push_back({{},rand(),difficulty});
    for(int j=0;j<depth-1;j++){
        //set room parameters (for future room generations)
        floors.push_back({{},rand(),j*difficulty});
    }
    //generate current board from starting room
    currentRoomId=0;
    currentBoard=floors[0].generateBoard(currentAdv);
}
//move function (expanded version of board's move, with stairs check)
void Dungeon::move(){
    //use board move
    int ret=currentBoard->move();
    //check stairs
    if(ret!=0){
        delete currentBoard;
        currentRoomId-=ret;
        assert(currentRoomId>=0 && currentRoomId<floors.size());
        currentBoard=floors[currentRoomId].generateBoard(currentAdv);
    }
}

///OTHER GLOBAL VARIABLES///
vector<AttMove*> attMoves;
Board board;


///GLOBAL FUNCTIONS///
//start game
short atocolor(string s){
    if(s=="WHITE") return COLOR_WHITE;
    if(s=="BLACK") return COLOR_BLACK;
    if(s=="YELLOW") return COLOR_YELLOW;
    if(s=="CYAN") return COLOR_CYAN;
    return COLOR_RED;
}
Life::MOVE atomovetype(string s){
    if(s=="USER") return Life::USER;
    if(s=="DUMMY") return Life::DUMMY;
    if(s=="IMMOBILE") return Life::IMMOBILE;
    return Life::IMMOBILE;
}
MOVEACTION atomoveaction(string s){
    if(s=="DAM") return MOVEACTION_DAM;
    return MOVEACTION_DAM;
}
void start_game(){
    menuEclr=0;
    menuSelKey='s';
    menuUpKey='w';
    menuDownKey='x';
    menuBackKey='e';
    menuTitleFclr=COLOR_RED;
    menuTitleBclr=COLOR_BLACK;
    menuMemberFclr=COLOR_WHITE;
    menuMemberBclr=COLOR_BLACK;
    menuSelFclr=COLOR_BLACK;
    menuSelBclr=COLOR_WHITE;
}
//search a move by id
AttMove *search_move(string id){
    for(vector<AttMove*>::iterator i=attMoves.begin();i<attMoves.end();i++){
        if((*i)->id==id) return *i;
    }
    ERR("ERR: attempt to call a nonexistent move id (%s)",id.c_str());
    return NULL;
}
//load all questions
void load_questions(const char *fileName){
    ifstream f(fileName);
    string s;
    char c;
    Question q;
    while(f.get()!=':');
    while(true){
        while(f.get()!=':');
        f >> s;
        if(s=="END") break;
        q.essence=s;;
        f >> q.dif;
        do{
            q.question.push_back(f.get());
        }while(f.peek()!='[');
        for(int j=0;j<4;j++){
            if(j==0) f.get();
            if(f.get()=='1') q.rightAnswer=j;
            f.get();
            do{
                q.answers[j].push_back(f.get());
            }while(f.peek()!=']');
            if(j!=3) while(f.get()!='[');
        }
        gQuestions.push_back(q);
        q.question.clear();
        for(int j=0;j<4;j++) q.answers[j].clear();
    }
    f.close();
}
//load all essences
void load_essence(const char *dirName){
    DIR* dir;
    dirent* dirnt;
    string s;
    char c;
    int n;
    assert(dir=opendir(dirName));
    while(dirnt=readdir(dir)){
        if(strcmp(dirnt->d_name,".")==0 || strcmp(dirnt->d_name,"..")==0) continue;
        stringstream ss;
        ss << dirName << "/" << dirnt->d_name;
        ifstream f(ss.str().c_str());
        while(f.get()!=':');
        while(true){
            Essence e;
            f >> e.name;
            if(e.name=="END") break;
            f >> c;
            if(c!='['){
                stringstream ess;
                ess << "FATAL ERROR: " << c << " is supposed to be a \"[\" in file \"" << dirnt->d_name << "\" in the " << dirName << " directory.";
                ERR("%s",ess.str().c_str());
            }
            assert(c=='[');
            while(true){
                f >> s;
                if(s=="]") break;
                f >> n;
                e.bonuses.push_back({s,n});
            }
            gEssences.push_back(new Essence(e));
            e.bonuses.clear();
        }
        f.close();
    }
    //check if there's a double essence somewhere...
    for(int j=0;j<gEssences.size();j++){
        for(int k=0;k<gEssences.size();k++){
            if(gEssences[j]->name==gEssences[k]->name && k!=j){
                ERR("FATAL ERROR: found double %s essence name.",gEssences[j]->name.c_str());
                assert(false);
            }
        }
    }
}
//load soul templates
void load_soul_templates(const char *fileName){
    ifstream f(fileName);
    while(f.get()!=':');
    while(true){
        string name,startString;
        f >> name;
        if(name=="END") break;
        while(f.get()!='[');
        while(f.peek()!=']') startString.push_back(f.get());
        f.get();
        startString.push_back('/0');
        gSoulTemplates.push_back(new SoulTemplate(name,startString));
    }
}
//load all tiles
//load all moves
void load_moves(const char *fileName){
    ifstream f(fileName);
    string s;
    char c;
    while(f.get()!=':');
    while(true){
        AttMove m;
        //id
        f >> m.id;
        if(m.id=="END") break;
        //name
        f >> m.name;
        //verbs
        f >> m.sverb;
        f >> m.pverb;
        //question types
        while(f.get()!='[');
        while(true){
            f >> s;
            if(s=="]") break;
            m.qEssence.push_back(s);
        }
        //questions difficulty
        f >> m.qstDif;
        //mana required
        f >> m.mana;
        //move actions
        while(f.get()!='[');
        while(true){
            f >> s;
            if(s=="]") break;
            m.action.push_back(atomoveaction(s));
        }
        //agility
        f >> m.agility;
        //chance
        f >> m.chance;
        //questions effects
        while(f.get()!='[');
        s.clear();
        while(f.peek()!=']') s+=f.get();
        m.qstEff=Effect(s);
        m.giveNameAndDesToEff(m.qstEff);
        //move base effect on player
        while(f.get()!='[');
        s.clear();
        while(f.peek()!=']') s+=f.get();
        m.selfEff=Effect(s);
        m.giveNameAndDesToEff(m.selfEff);
        //move base effect on enemy
        while(f.get()!='[');
        s.clear();
        while(f.peek()!=']') s+=f.get();
        m.eneEff=Effect(s);
        m.giveNameAndDesToEff(m.eneEff);
        //description
        while(f.get()!='[');
        do{
            m.des.push_back(f.get());
        }while(f.peek()!=']');
        f.get();
        attMoves.push_back(new AttMove(m));
    }
    f.close();
}
//load all tiles
void load_tiles(const char* fName){
    //open directory
    DIR *dir = opendir("tiles");
    if(dir == nullptr){
        ERR("%s", "could not open tiles directory");
        assert(false);
    }
    //go through directory file
    dirent *dirR;
    readdir(dir);
    readdir(dir);
    while(dirR = readdir(dir)){
        //open file
        std::string fNam = string("tiles/") + dirR->d_name;
        ifstream f(fNam.c_str());
        //set dread defaults
        dread::setDefaults(fNam,&f,'[',']');
        //go on until : sign
        dread::skipUntil(':');
        //first get
        string id;
        f >> id;
        int j;
        for(j=0;id != "END" && j < 1000;j++){
            //tile tmp
            Tile tile;
            //id
            tile.id = id;
            id.clear();
            //name
            tile.name = dread::readSingle();
            //get essences
            tile.essence = dread::readSingle();
            //get spawn flags
            string spawnFlag;
            string spawnFlags = dread::readSingle();
            stringstream sfss;
            sfss << spawnFlags;
            while(sfss >> spawnFlag)
                tile.spawnFlags.push_back(spawnFlag);
            //graphics
            f >> tile.gra;
            //color
            string color;
            f >> color;
            tile.fclr = atocolor(color);
            color.clear();
            f >> color;
            tile.bclr = atocolor(color);
            //danger
            f >> tile.danger;
            //passable
            f >> tile.passable;
            //stairs
            f >> tile.stair;
            //ad tile to gTiles
            gTiles.push_back(new Tile(tile));
            //get id
            f >> id;
            //check loop safety
            if(j == 1000) ERR("%s","loop limit reached (1000) when loading tiles");
        }
    }
}
//load body templates
void load_body_templates(const char *fileName){
    ifstream f(fileName);
    while(f.get()!=':');
    while(true){
        string s;
        BodyTemplate bt;
        f >> bt.id;
        if(bt.id=="END") break;
        while(f.get()!='[');
        while(f.peek()!=']')s+=f.get();
        bt.start(s);
        bodyTemplates.push_back(new BodyTemplate(bt));
        f.get();
    }
    f.close();
}
//load all existing items
void load_items(const char *fileName){
    ifstream f(fileName);
    string s;
    Item i;
    while(f.get()!=':');
    while(true){
        //id
        f >> i.id;
        if(i.id=="END") break;
        //name
        s.clear();
        while(f.get()!='[');
        while(f.peek()!=']') s+=f.get();
        f.get();
        i.name=s;
        s.clear();
        //size
        f >> i.size;
        //wear body template
        f >> s;
        if(s!="NONE"){
            i.wearComp=new Item::WearComponent();
            i.wearComp->neededBodyParts=search_body_template(s)->bodyParts;
        }
        //quest key
        f >> s;
        if(s!="NONE"){
            i.questComp=new Item::QuestComponent();
            i.questComp->key=s;
        }
        //essences
        while(f.get()!='[');
        while(true){
            f >> s;
            if(s=="]") break;
            i.essences.push_back(s);
        }
        //spawn level and rarity
        int spawnLv,rarity;
        f >> spawnLv;
        f >> rarity;
        if(spawnLv==-1) assert(rarity==-1);
        else{
            i.spawnComp=new Item::SpawnComponent();
            i.spawnComp->spawnLv=spawnLv;
            i.spawnComp->rarity=rarity;
        }
        //wear effect
        s.clear();
        while(f.get()!='[');
        while(f.peek()!=']') s+=f.get();
        if(i.wearComp==NULL) assert(s==".");
        else i.wearComp->wearEffect=Effect(s);
        i.wearComp->wearEffect.wearItemId=i.id;
        f.get();
        //use effect
        s.clear();
        while(f.get()!='[');
        while(f.peek()!=']') s+=f.get();
        if(s!="."){
            i.useComp=new Item::UseComponent();
            i.useComp->useEffect=Effect(s);
        }
        f.get();
        //description
        s.clear();
        while(f.get()!='[');
        while(f.peek()!=']') s+=f.get();
        i.description=s;
        f.get();
        //put item in items
        items.push_back(new Item(i));
    }
    f.close();
}
//load all living entities
void load_life(const char *fileName){
    ifstream f(fileName);
    string s;
    char c;
    int n;
    while(f.get()!=':');
    while(true){
        Life l;
        f >> l.id;
        if(l.id=="END") break;
        l.speakHead=NULL;
        l.status=Status();
        f >> c;
        s.clear();
        l.name.clear();
        while(true){
            f >> s;
            if(s=="]") break;
            l.name+=s;
        }
        f >> l.gra;
        f >> s;
        l.fclr=atocolor(s);
        f >> s;
        l.bclr=atocolor(s);
        f >> s;
        l.moveType=atomovetype(s);
        f >> s;
        l.spawnEssence=s;
        f >> l.rar;
        f >> l.swnLv;
        f >> l.hp;
        f >> l.mana;
        f >> l.status.att;
        f >> l.status.def;
        f >> l.status.luck;
        f >> l.status.agi;
        f >> l.vx;
        f >> l.vy;
        for(int j=0;j<4;j++){
            f >> s;
            if(s!="NO") l.moves.push_back(search_move(s));
        }
        //make body
        f >> s;
        assert(s=="[");
        while(true){
            f >> s;
            if(s=="]") break;
            else if(s=="TEMPLATE"){
                f >> s;
                l.body=search_body_template(s)->makeBody();
            }

        }
        //make soul
        f >> s;
        assert(s=="[");
        while(true){
            f >> s;
            if(s=="]") break;
            else if(s=="TEMPLATE"){
                f >> s;
                l.soul=search_soul_template(s)->makeSoul();
            }
            else{
                if(search_essence(s)==NULL){
                    stringstream ess;
                    ess << "ERR: essence " << s << ", associated with life " << l.name << " does not exist.";
                    ERR("%s",ess.str().c_str());
                }
                f >> n;
                l.soul.setEssenceVal(s,n);
            }
        }
        //make inventory
        f >> s;
        assert(s=="[");
        while(true){
            f >> s;
            if (s=="]") break;
            assert(l.addItem(*(search_item(s))));
        }
        while(f.get()!='[');
        do{
            l.des.push_back(f.get());
        }while(f.peek()!=']');
        //get description
        f.get();
        life.push_back(new Life(l));
    }
    f.close();
}
//load life dialog
void load_dialog(const char *fileName){
    ifstream f(fileName);
    //constants
    const char* TEXT="TEXT";
    const char* LINKS="LINKS";
    const char* TITLE="TITLE";
    const char* IF="IF";
    const char* RET="RET";
    const char* START="[";
    const char* END="]";
    //variables
    string s;
    Life *l;
    //read mode enum
    enum RM{
        RM_NODE,
        RM_LINKS,
        RM_RET,
    }rm=RM_NODE;
    while(true){
        f >> s;
        if(s=="END") break;
        if((l=search_life(s))==NULL){ERR("%s is not a life id.",s.c_str());}
        else{
            f >> s;
            while(s==":"){
                f >> s;
                l->dialogMods.push_back(s);
                f >> s;
            }
            l->speakHead=new SpeakNode();
            SpeakNode *curSN=l->speakHead;
            s.clear();
            while(true){
                f >> s;
                if(s=="}END") break;
                if(rm==RM_NODE){
                    if(s=="TEXT" || s=="YOUTEXT"){
                        if(s=="YOUTEXT") curSN->textOwners.push_back(SpeakNode::TXTOWN_USER);
                        else curSN->textOwners.push_back(SpeakNode::TXTOWN_CPU);
                        s.clear();
                        f.get();f.get();
                        while(f.peek()!=']') s+=f.get();
                        f.get();
                        curSN->text.push_back(s);
                    }
                    else if(s=="RET"){
                        s.clear();
                        f.get();f.get();
                        while(f.peek()!=']') s+=f.get();
                        f.get();
                        curSN->text.push_back("RET");
                        curSN->ret=s;
                        rm=RM_RET;
                    }
                    else if(s=="LINKS"){
                        f.get();f.get();
                        rm=RM_LINKS;
                        continue;
                    }
                    else if(s=="}"){
                        if(curSN->back==NULL) break;
                        curSN=curSN->back;
                        rm=RM_LINKS;
                        continue;
                    }
                    else{
                        ERR("%s not a valid dialog instruction in NODE read mode",s.c_str());
                    }
                }
                else if(rm==RM_LINKS){
                    if(s=="TITLE"){
                        s.clear();
                        f.get();f.get();
                        while(f.peek()!=']') s+=f.get();
                        f.get();f.get();
                        SpeakNode *tmp=new SpeakNode(s,curSN);
                        curSN->links.push_back(tmp);
                        curSN=curSN->links.at(curSN->links.size()-1);
                        rm=RM_NODE;
                        continue;
                    }
                    else if(s=="}"){
                        rm=RM_NODE;
                        continue;
                    }
                    else{
                        ERR("%s not a valid dialog instruction in LINKS read mode",s.c_str());
                    }
                }
                else if(rm==RM_RET){
                    if(s=="}"){
                        if(curSN->back==NULL) break;
                        curSN=curSN->back;
                        rm=RM_LINKS;
                        continue;
                    }
                    else{
                        ERR("%s not a valid dialog instruction in RET read mode",s.c_str());
                    }
                }
            }
        }
    }
}

///MAIN///
int main()
{
    initscr();
    noecho();
    curs_set(0);
    start_color();
    /*Menu menu;
    start_menu(&menu,0,0,"main menu",COLOR_RED,COLOR_WHITE,'s','u','m',KEY_BACKSPACE,COLOR_BLACK,COLOR_WHITE,"play the game",COLOR_WHITE,COLOR_BLACK,"exit",COLOR_WHITE,COLOR_BLACK,"stop");
    if(!strcmp("play the gamen",inter_menu(menu))){
        printw("ok then :)");
    }*/
    //set menu's global setting
    menuSelKey=MC_SELECT;
    menuUpKey=MC_UP;
    menuDownKey=MC_DOWN;
    //load stuff
    load_essence("essences");
    load_tiles("tiles.txt");
    load_soul_templates("soul templates.txt");
    load_questions("questions.txt");
    load_moves("moves.txt");
    load_body_templates("body templates.txt");
    load_items("items.txt");
    load_life("life.txt");
    load_dialog("dialog.txt");
    //initialize and set adventurer
    adv=new Life(*(search_life("adventurer")));
    //initialize dungeon
    Dungeon dungeon(time(NULL),10,1,adv);
    //start main loop
    int gameTime=0; //!WIP
    dungeon.getBoard()->print(1,1,adv);
    do{
        gameTime++;
        dungeon.move();
        clear();
        dungeon.getBoard()->print(1,1,adv);
        if(gameTime%10==0) adv->advanceTime(1);
    }while(adv->hp>0);
    //exit program
    return 0;
}
