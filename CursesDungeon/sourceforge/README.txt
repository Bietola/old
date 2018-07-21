COPYRIGHT:
this C++ project uses the "pdcurses.dll" file which was downloaded from "https://sourceforge.net/projects/pdcurses/?source=directory" and WAS NOT made by me, I just put it there because it's needed to run the executable file (curses dungeon.exe).  If anyone thinks or KNOWS that this should not be done, please tell me :)

THINGS TO DO:
The game has three major flaws (if we ignore the fact that it can barely be called an actual game :D)
-All of its code is written on a single file (main.cpp), and this will be the first thing that I will change if I decide to continue working on this.
-Enemies are stupid... they follow the player regardless of the obstacles separating them from him/her.
-The ground is too randomly generated, in the future I will change the code to allow the creation of actual rooms and corridors.

NOTES:
Firstly, I'll specify that english is NOT my native language (as you might have already found out by yourself XD), so excuse me if reading this gave you an headache.
Finally, I would like to thank anyone who tries out this "game" and to thank even more the people who wrote an helpful review about it.

----------------------------CHANGELOG----------------------------



//**v0.4.1 - The Status Update**//
GENERAL STUFF
	A lot of attributes were added to the status array, including:
-Maximum HP, which were needed as an attribute to make       regeneration work right.
-Regeneration, which made it possible for entities to 	regenerate health in different quantities (slimes could regenerate up to 0.5 health in one turn, while creepers only 0.3)
-Colors...detailed explanation in the "tech stuff"
	In addition, mastichini were finaly made faster with a velocity ranging from 1.2 to 1.5.

TECHNICAL STUFF
	Two attributes were added to the status array to make colors work: ps_clr and ps_cpp.  ps_cpp was the number of the COLOR_PAIR that was assigned to that location, while ps_clr was the character color assigned to the same location.  The background color was always COLOR_BLACK and everything that didn't have a color (and so had the ps_clr and ps_cpp filled with a t_null value) was displayed in COLOR_WHITE (which was indicated in the "clr_def" variable; for "color default").


//**v0.4.0 - The Vision Spawn Bug Update**//

--to be written--

//**v0.3.3 - The Init_Color() Update**//
WARNING : ALTHOUGH THIS V. IS PLAYABLE AND EXECUTABLE, THERE ARE ERRORS IN ITS GRAPHICAL RAPPRESENTATIONS

GENERAL STUFF
	Colors were added.. but not in the way they were supposed to :).
	Velocity was also added, and worked as mentioned in v0.3.2, and mastichini were finaly made faster with a velocity ranging from 1.2 to 1.5.

TECHNICAL STUFF
	Since I found out that the PCcurses library only offered 8 standard colors, I tryed to add to the third dimension of the status array four more empty spaces which would contain the color pair number and the RGB quantities of a particular color to the place rappresented by the status array.  The attempt failed... so after trying to DEBUG the program in a lot of ways, even creating a three dimensional stand-by array for colors (called color), I finally surrendered and was stuck with using only the default colors.
	The status array was converted from an int array to a float array, to support the new velocity concept.
	New functions were added to calculate a random %.1f number or a random int number inside a given range.



//**v0.3.2 - The No Velocity Update**//
GENERAL STUFF
	This update was an attempt to rewrite the code of the previsious update, which was lost.  "No Velocity" comes from the fact that I orignally planned to add a "VEL" attribute to non-playable entities that would make them either faster or slower than the player (for example: moving two places at a time, or taking two turns to move one place).  My attemps to do this failed, thought, and the version was released without this feature.
	In addition, the "mastichino" was added to the "spawn_m()" function, and therefore became an official entity.  The mastichino still moved at the same speed as other monsters, but was supposed to be the "chosen one" to move faster than others.

TECHNICAL STUFF
	The identifiers "adv" and "adventurer" were added to the spawn_m() function.  This could be used to spawn the player over and over, and not only once like the other identifiers, because it was based on the global variables that rappresented the players actual attributes.
	In addition other identifiers were added as abbreviations of already present identifiers (ex.: "creep" for "creeper" or "masti" for "mastichino").



//**v0.3.1 - The Lost Code**//
WARNING : THIS VERSION'S CODE IS GONE (but not the executable file)

GENERAL STUFF
	thanks to this update I found out the ugly truth about executable files XD.  In fact, since executable files cannot be changed back into the code that created them AND, as a coincidence, I lost the actuall code while passing the version to a friend... I had to rewrite the whole mns_move() function (which made entities other than the player move by themselves) from scratch XD.
	Anyways, in this update, as was already mentioned, entities other than the player were made able to actually move and attack with the mns_move() function.



//**v0.3.0 - Move Function Update**//
WARNING : THIS VERSION WAS UNPLAYABLE (but not unexecutable)

TECHNICAL STUFF
	To make moving both the player and other entities (which still didn't move or attack) easier, I tried to make a "move()" function that would then have been used both in the "adv_move()" and in the "mns_move()" functions.  The attempt resulted only in a really buggy game, full of printed "DEBUGGER" functions...the move() function was never created.



//**v0.2.1 - Dynamic Functions Update**//
WARNING : THIS VERSION WAS UNPLAYABLE (but not unexecutable)

TECHNICAL STUFF
	To add "special status effects" (like sleep, confusion, etc...) to entities and to improve the random ground generation function (generate_g()) to make possible the generarion of different biomes, I tryed experimenting with the stdarg.h library.  This was useless, thought, becouse I could never make it work right...
	Some changes I made in this function remained permanent throughout the later versions, thought:  This changes include the spawn_m() function, which made spawning entities much easier by only requiring the coordinates and a const* char variable type as an identifier for which entity you might want to spawn (for now, only "creeper" and "slime" where present in the list of identifiers).



//**v0.2.0 - Curses Update**//
GENERAL STUFF
	althought the game's name remained "dungeon", the whole game's code (or at least its graphical functions) were rewritten using the PDcurses library:  this put an end to the flashes.
	g_wall changed from 117 to '#' and all of the commands changed from the numpad without the num lock to the numpad WITH the num lock.



//**v0.1.0 - Dungeon**//
GENERAL STUFF
	The game's name was still "dungeon" and all you could really do was move around with your character and attack monsters (only creepers and slimes) who always had the same att and HP (which were the only two attributes at the moment), the PDcurses library was not yet used and the game still "flashed" at every movement.

TECHNICAL STUFF
	The game's board was divided into four different arrays that all had the dimension of the board:
1.the ground array, which could contain only the graphics of "ground tiles" (g_light) or "walls" (g_wall), the player could only move in places where the ground array was filled with g_light, and monsters could only spawn on g_light, too. 
2.the item array, which was an unused array as big as the board.
3.the life array, which contained the graphics of all of the entities on board (currently only l_adv, l_slime, and l_crepper).
4.the status array, which contained the att and the HP of the place it rappresented (if no entity was present on some coos, then this array would be filled with the t_null variable (=0)).
NB: all of this arrays were bidimensinal, apart from the status array wich was tridimensional.
