#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_LEN 100

#define AN_MAX 272
#define ON_MAX 10

#define SYM_INFO ':'
#define SYM_END ':'

///element structure
typedef struct{
    char symbol[BUF_LEN];
    char name[BUF_LEN];
    char root[BUF_LEN];
    int atomicNum;
    int group;
    int ons[ON_MAX];
    int maxOns;
}Element;

///global periodic table data
int numEle=0;
Element ptable[AN_MAX];

///get a string in a file until a specified character is found
int fgetsuc(FILE* f,char* s,char c){
    int j=0;
    s[j]=fgetc(f);
    while(s[j]!=c && s[j]!=EOF){
        j++;
        s[j]=fgetc(f);
    }
    if(s[j]==EOF){
        s[j]='\0';
        return EOF;
    }
    s[j]='\0';
    return 0;
}

///check if a specified character is a letter
int is_letter(char c){
    if((c>='a' && c<='z') || (c>='A' && c<='Z')) return 1;
    else return 0;
}
///check if a specified character is a capital letter
int is_cap(char c){
    if(c>='A' && c<='Z') return 1;
    else return 0;
}
///check if a specified character is a non capital letter
int is_nocap(char c){
    if(c>='a' && c<='z') return 1;
    else return 0;
}

///show the elements that are currently stored into the periodic table
void show_ptable(){
    int j;
    printf("LA TAVOLA PERIODICA\n\n");
    for(j=0;j<numEle;j++){
        int k=0;
        printf("%d(%d) %s %s ",ptable[j].atomicNum,ptable[j].group,ptable[j].symbol,ptable[j].name);
        while(ptable[j].ons[k]!=0){
            if(ptable[j].ons[k]>0) printf("+");
            printf("%d ",ptable[j].ons[k]);
            k++;
        }
        if(strcmp(ptable[j].root,"none")) printf("(%s)",ptable[j].root);
        printf("\n\n");
    }
}

///load the periodic table file and store its data somewhere
void load_ptable(char* file){
    //temp vars
    Element* e;
    char c;
    char s[BUF_LEN];
    int i;
    int j;
    int n;
    //load file
    FILE* f=fopen(file,"r");
    if(f==NULL) return;
    //arrive at info symbol
    c=0;
    while(c!=SYM_INFO) c=fgetc(f);
    //read elements info
    while(fscanf(f,"%d",&i)==1){
        //increment element counter
        numEle++;
        //atomic number
        e=&ptable[i-1];
        e->atomicNum=i;
        //+group (calculation)
        //period 1
        if(i==1) e->group=1;
        else if(i==2) e->group=18;
        //periods 1-3
        else if(i<=18){
            if(i<=10) n=2;
            else if(i<=18) n=10;    //could be else
            if(i-2<3) e->group=i-n;
            else e->group=i-n+10;
        }
        //periods 4-7
        else if(i<=111){    //could be else
            if(i<=36) n=18;
            else if(i<=54) n=36;
            else if(i<=86) n=54;
            else if(i<=111) n=86;   //could be else
            if((i>57 && i<87) || i>89) e->group=i-n-14;
            else e->group=i-n;
        }
        //symbol
        fscanf(f,"%s",e->symbol);
        //name
        fscanf(f,"%s",e->name);
        //space
        c=fgetc(f);
        //reset arrays
        for(j=0;j<ON_MAX;j++) e->ons[j]=0;
        e->maxOns=0;
        //oxidation numbers
        for(j=0;j<ON_MAX;j++){
            c=fgetc(f);
            e->maxOns++;
            if(c=='+'){
                c=fgetc(f);
                if(c!='-'){
                    i=(int)(c-'0');
                    e->ons[j]=i;
                }
                else{
                    fscanf(f,"%d",&i);
                    e->ons[j]=i;
                    j++;
                    e->ons[j]=-i;
                }
            }
            else if(c=='-'){
                fscanf(f,"%d",&i);
                e->ons[j]=-i;
            }
            else if(c=='0') e->ons[j]=0;
            else{
                ///IT IS SUPPOSED TO BE AN ERROR.... FIX REQUEIRED printf("ERROR: %c is not + or -\n",c);
                break;
            }
        }
        //nomenclature names
        //if there's another one, the element has a root
        c=fgetc(f);
        if(c!='\n' && c!=EOF){
            e->root[0]=c;
            e->root[1]='\0';
            fscanf(f,"%s",s);
            strcat(e->root,s);
        }
        else strcpy(e->root,"none");
    }
    //close file
    fclose(f);
}

///figure out if specified element any kind of metal
int is_metal(Element e){
    int an=e.atomicNum;
    if(e.group<=12) return 1;
    else if(an==13 || an==31 || an==49 || an==81 || an==50 || an==82 || an==83) return 1;
    else return 0;
}

///find the element associated to the specified symbol
Element fele_symbol(char* symbol){
    int j;
    for(j=0;j<AN_MAX;j++){
        if(strcmp(ptable[j].symbol,symbol)==0){
            return ptable[j];
        }
    }
    printf("ERROR: %s is not an element symbol\n",symbol);
}

///find an element inside a given formula
Element fele_formula(char* formula,int index){
    int j=0,k=0;
    char symbol[3];
    while(formula[j]!='\0'){
        if(is_cap(formula[j])){
            symbol[0]=formula[j];
            j++;
            if(is_nocap(formula[j])){
                symbol[1]=formula[j];
                symbol[2]='\0';
                j++;
            }
            else symbol[1]='\0';
            k++;
            if(k==index){
                return fele_symbol(symbol);
            }
        }
        else j++;
    }
}

///return the number of elements involved in the formula
int num_ele(char* formula){
    int j=0,k=0;
    while(formula[j]!='\0'){
        if(is_cap(formula[j])) k++;
        j++;
    }
    return k;
}

///find the index of an element in a specified formula
int feleindex(Element e,char* formula){
    int j;
    Element te;
    for(j=1;j<=num_ele(formula);j++){
        te=fele_formula(formula,j);
        if(strcmp(te.symbol,e.symbol)==0) return j;
    }
    printf("ERROR: element %s (%s) not found while searching its index in the formula %s\n",e.name,e.symbol,formula);
    return 0;
}

///find the number of atoms of a specified element in a specified formula
int felenum(Element e,char* formula){
    //!DEBUG printf("initiate felenum: %s %s\n",e.name,formula);
    int j=0;
    char symbol[3];
    while(formula[j]!='\0'){
        //get rid of numbers (WIP)
        while(!is_letter(formula[j])) j++;
        //get the element first letter (capital)
        symbol[0]=formula[j];
        j++;
        //if the element has a second letter, add it to the formula
        if(is_nocap(formula[j])){
            symbol[1]=formula[j];
            symbol[2]='\0';
            j++;
        }
        else symbol[1]='\0';
        //!DEBUG printf("found symbol: %s\n",symbol);
        //!DEBUG getch();
        //check if the specified symbol has been found
        if(strcmp(symbol,e.symbol)==0){
            //if the next character is a letter, or if the formula is done, return 1
            if(is_letter(formula[j]) || formula[j]=='\0') return 1;
            //else return number after the element symbol
            else{
                //!DEBUG printf("%d\n",(int)(formula[j]-'0'));
                return (int)(formula[j]-'0');
            }
        }
    }
    printf("ERROR: could not determine the number of atoms of %s (%s) contained in the formula %s\n",e.name,e.symbol,formula);
    return 0;
}

///find the max POSITIVE oxidation number of a given element
int fmaxon(Element e){
    int j;
    int max=-1000;
    for(j=0;j<e.maxOns;j++){
        if(e.ons[j]>max && e.ons[j]>0) max=e.ons[j];
    }
    return max;
}

///find the min POSITIVE oxidation number of a given element
int fminon(Element e){
    int j;
    int min=1000;
    for(j=0;j<e.maxOns;j++){
        if(e.ons[j]<min && e.ons[j]>0) min=e.ons[j];
    }
    return min;
}

///find the middle POSITIVE oxidation number of a given element
float fmedon(Element e){
    int j,k;
    int deltaOn[e.maxOns];
    for(j=0;j<e.maxOns;j++){
        if(e.ons[j]>0){
            deltaOn[j]=0;
            for(k=0;k<e.maxOns;k++){
                if(e.ons[k]>0){
                    if(e.ons[j]>e.ons[k]) deltaOn[j]++;
                    else if(e.ons[j]<e.ons[k]) deltaOn[j]--;
                }
            }
        }
        else deltaOn[j]=1000;
    }
    for(j=0;j<e.maxOns;j++){
        if(deltaOn[j]==0) return (float)e.ons[j];
        else if(deltaOn[j]==1 || deltaOn[j]==-1){
            int mj=j;
            j++;
            while(deltaOn[j]!=1 && deltaOn[j]!=-1) j++;
            return ((float)e.ons[j]+(float)e.ons[mj])/2;
        }
    }
    printf("ERROR: can't find out the middle oxidation number of %s (%s)\n",e.name,e.symbol);
    return 0;
}

///
int num_poson(Element e){
    int j,num=0;
    for(j=0;j<e.maxOns;j++){
        if(e.ons[j]>0) num++;
    }
    return num;
}

///calculate the oxidation number of an element in a specified formula
//!WIP: only works with formulas composed of two elements
int calcon(Element e,char* formula){
    int j1,j2,n1,n2;
    Element e1,e2;
    int ei=feleindex(e,formula);
    e1=fele_formula(formula,1);
    e2=fele_formula(formula,2);
    n1=felenum(e1,formula);
    n2=felenum(e2,formula);
    for(j1=0;j1<e1.maxOns;j1++){
        for(j2=0;j2<e2.maxOns;j2++){
            if(n1*e1.ons[j1]==-n2*e2.ons[j2] || n2*e2.ons[j2]==-n1*e1.ons[j1]){
                if(ei==1) return e1.ons[j1];
                else if(ei==2) return e2.ons[j2];
                else{
                    printf("ERROR: 'calcon' function does not yet provide support for formulas with more than two elements\n");
                    return 0;
                }
            }
        }
    }
    printf("ERROR: cannot calculate the oxidation number of %s (%s) in the formula %s\n",e.name,e.symbol,formula);
    return 0;
}

///adds the suffixes/prefixes bound to the "osoico" rule to the name of an element according to a formula
void osoico(char* name,Element e,char* formula,int gender){
    int on=calcon(e,formula);
    int numon=num_poson(e);
    int maxon=fmaxon(e);
    int minon=fminon(e);
    float medon=fmedon(e);
    printf("n.o. %s: %d\n",e.name,on);
    printf("MXmnmd  %s: %d %d %.1f\n",e.name,maxon,minon,medon);
    //perico
    if(numon>=4 && on==maxon){
        strcpy(name,"per");
        if(strcmp(e.root,"none")) strcat(name,e.root);
        else strcat(name,e.name);
        if(gender==1) strcat(name,"ico");
        else strcat(name,"ica");
    }
    //ico
    else if((numon>1 && numon<4 && on==maxon) || (numon>=4 && on<maxon && on>medon)){
        if(strcmp(e.root,"none")) strcpy(name,e.root);
        else strcpy(name,e.name);
        if(gender==1) strcat(name,"ico");
        else strcat(name,"ica");
    }
    //di
    else if(numon==1 || on==medon){
        strcpy(name,"di ");
        strcat(name,e.name);
    }
    //oso
    else if((numon>1 && numon<4 && on==minon) || (numon>=4 && on>minon && on<medon)){
        if(strcmp(e.root,"none")) strcpy(name,e.root);
        else strcpy(name,e.name);
        if(gender==1) strcat(name,"oso");
        else strcat(name,"osa");
    }
    //ipooso
    else if(numon>=4 && on==minon){
        strcpy(name,"ipo");
        if(strcmp(e.root,"none")) strcat(name,e.root);
        else strcat(name,e.name);
        if(gender==1) strcat(name,"oso");
        else strcat(name,"osa");
    }
    //ERROR
    else{
        printf("ERROR: can't find any compatible prefix/suffix arrangement\n");
        return;
    }
}

///figure out the type of a specified formula
void formula_type(char* type,char* formula){
    //temp vars
    Element e[2];
    char c='\0';
    char s[BUF_LEN]=" ";
    //eccezioni
    FILE* f=fopen("eccezioni.txt","r");
    //go on until info symbol
    while(c!=SYM_INFO) c=fgetc(f);
    fgetc(f);
    //find exception
    while(fgetsuc(f,s,',')!=EOF){
        if(strcmp(s,formula)==0){
            fgetsuc(f,s,',');
            fgetsuc(f,s,'\n');
            strcpy(type,s);
            fclose(f);
            return;
        }
        else{
            fgetsuc(f,s,',');
            fgetsuc(f,s,'\n');
        }
    }
    fclose(f);
    //composti binari
    if(num_ele(formula)==2){
        e[0]=fele_formula(formula,1);
        e[1]=fele_formula(formula,2);
        //ossidi (basici e anidridi)
        if(e[1].atomicNum==8){
            if(is_metal(e[0])) strcpy(type,"ossido basico");
            else strcpy(type,"ossido acido");
        }
        //idracidi
        else if(e[0].atomicNum==1 && (e[1].group==16 || e[1].group==17)) strcpy(type,"idracido");
        //idracidi invertiti (errore di scrittura)
        else if(e[1].atomicNum==1 && (e[0].group==16 || e[0].group==17)) strcpy(type,"idracido invertito");
        //idruri (ionici e covalenti)
        else if(e[1].atomicNum==1){
            if(is_metal(e[0])) strcpy(type,"idruro ionico");
            else strcpy(type,"idrudo covalente");
        }
        //composti di non metalli
        else if(!is_metal(e[0]) && !is_metal(e[1])) strcpy(type,"composto di non-metalli");
        //sali binari
        else if(is_metal(e[0]) && (e[1].group==16 || e[1].group==17)) strcpy(type,"sale binario");
        //tipo non trovato
        else printf("ERROR: cannot name the type of the formula %s\n",formula);
    }
    else printf("ERROR: the function formula_type does not yet work with formulas with more than three elements (%s)",formula);
}

///figure out the name of a specified formula
void  name_formula(char* name,char* formula){
    //temp vars
    Element e[2];
    char s[BUF_LEN],type[BUF_LEN];
    char c='\0';
    //eccezioni
    FILE* f=fopen("eccezioni.txt","r");
    //go on until info symbol
    while(c!=SYM_INFO) c=fgetc(f);
    c=fgetc(f); //get the \n after the info symbol
    //find exception
    while(fgetsuc(f,s,',')!=EOF){
        //!DEBUG printf("%s(%d) %s(%d) %d\n",s,strlen(s),formula,strlen(formula),strcmp(s,formula));
        if(strcmp(s,formula)==0){
            printf("eccezione trovata\n");
            fgetsuc(f,s,',');
            strcpy(name,s);
            fclose(f);
            return;
        }
        else{
            fgetsuc(f,s,',');
            fgetsuc(f,s,'\n');
        }
    }
    fclose(f);
    //composti binari
    if(num_ele(formula)==2){
        e[0]=fele_formula(formula,1);
        e[1]=fele_formula(formula,2);
        formula_type(type,formula);
        //ossidi basici
        if(strcmp(type,"ossido basico")==0){
            //first element
            strcpy(name,"ossido ");
            //second element (osoico)
            osoico(s,e[0],formula,1);
            strcat(name,s);
        }
        //ossido acido/anidride
        else if(strcmp(type,"ossido acido")==0){
            //first element
            strcpy(name,"anidride ");
            //second element (osoico)
            osoico(s,e[0],formula,2);
            strcat(name,s);
        }
        //idracidi e idracidi invertiti
        else if(strcmp(type,"idracido")==0 || strcmp(type,"idracido invertito")==0){
            //first element
            strcpy(name,"acido ");
            //second element (idrico)
            if(strcmp(type,"idracido")==0){
                if(strcmp(e[1].root,"none")) strcat(name,e[1].root);
                else strcat(name,e[1].name);
            }
            else{
                if(strcmp(e[0].root,"none")) strcat(name,e[0].root);
                else strcat(name,e[0].name);
            }
            strcat(name,"idrico");
            //inversion warning
            if(strcmp(type,"idracido invertito")==0) printf("ATTENZIONE: %s, essendo un'idracido, dovrebbe essere scritto con H prima di %s\n",formula,e[0].symbol);
        }
        //idruri ionici
        else if(strcmp(type,"idruro ionico")==0){
            //first element
            strcpy(name,"idrudo ");
            //second element (osoico)
            osoico(s,e[0],formula,1);
            strcat(name,s);
        }
        //idruri covalenti
        else if(strcmp(type,"idrudo covalente")==0){
            //first element
            strcpy(name,"idrudo ");
            //second element (di)
            strcat(name,"di ");
            strcat(name,e[0].name);
        }
        //sali binari e composti di non-metalli
        else if(strcmp(type,"sale binario")==0 || strcmp(type,"composto di non-metalli")==0){
            //first element (uro)
            if(strcmp(e[1].root,"none")) strcpy(name,e[1].root);
            else strcpy(name,e[1].name);
            strcat(name,"uro ");
            //second element (osoico)
            osoico(s,e[0],formula,1);
            strcat(name,s);
        }
    }
}

int main(){
    char formula[BUF_LEN],name[BUF_LEN],type[BUF_LEN];
    //load custom periodic table
    load_ptable("tavola periodica.txt");
    show_ptable();
    while(1){
        //get formula
        printf("inserire formula chimica:\n");
        scanf("%s",formula);
        //exit if necessary
        if(strcmp(formula,"fine")==0) break;
        //output formula name
        formula_type(type,formula);
        name_formula(name,formula);
        printf("%s (%s)\n\n",name,type);
    }
    return 0;
}
