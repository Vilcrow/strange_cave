/*
Strange Cave game

Copyright (C) 2022-2023 S.V.I 'Vilcrow', <svi@vilcrow.net>
--------------------------------------------------------------------------------
LICENCE:
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
--------------------------------------------------------------------------------
*/

#ifndef CONST_H_SENTRY
#define CONST_H_SENTRY

#define _(STR) (STR)
#define TRUE 1
#define FALSE 0
#define MAXNAMESIZE 12
#define NAMEARR 17
#define MAXPATHARRAY 300
#define MENUBCKG "files/source/menu.txt"
#define KEYBINDINGSBCKG "files/source/key_bindings.txt"
#define BORDERSBCKG "files/source/borders.txt"
#define LEVELONESRC "files/source/level_1.txt"
#define LEVELTWOSRC "files/source/level_2.txt"
#define LEVELTHREESRC "files/source/level_3.txt"
#define LEVELFOURSRC "files/source/level_4.txt"
#define LEVELFIVESRC "files/source/level_5.txt"
#define LEVELSIXSRC "files/source/level_6.txt"
#define LEVELSEVENSRC "files/source/level_7.txt"
#define LEVELEIGHTSRC "files/source/level_8.txt"
#define LEVELNINESRC "files/source/level_9.txt"
#define GAMEOVER "files/source/gameover.txt"
#define WIN "files/source/win.txt"
#define RESEMPTY "files/source/resempty.txt" /*results files*/
#define RESSRC "/ressrc.txt"
#define RESTEMP "/tmp/restemp.txt"
#define LEVONE "/tmp/strcave_lev1.txt"  /*temporary files*/
#define LEVTWO "/tmp/strcave_lev2.txt"
#define LEVTHREE "/tmp/strcave_lev3.txt"
#define LEVFOUR "/tmp/strcave_lev4.txt"
#define LEVFIVE "/tmp/strcave_lev5.txt"
#define LEVSIX "/tmp/strcave_lev6.txt"
#define LEVSEVEN "/tmp/strcave_lev7.txt"
#define LEVEIGHT "/tmpp/strcave_lev8.txt"
#define LEVNINE "/tmp/strcave_lev9.txt"
#define REDKEY "Red key"
#define GREENKEY "Green key"
#define BLUEKEY "Blue key"
#define BACKSTRING ">BACK<      clear"
#define CLEARSTRING " back      >CLEAR<"

enum {sleep_duration =   2};
enum {delay_duration = 250};
enum {maxrow         =  30};
enum {maxcol         =  80};
enum {c_start        =   1};  /*menu*/
enum {c_exit         =   2};
enum {mnstrtx        =   5};
enum {mnstrty        =  11};
enum {start_slct     =   1};
enum {results_slct   =   2};
enum {control_slct   =   3};
enum {language_slct  =   4};
enum {exit_slct      =   5};
enum {resstrty       =  17}; /*results page*/
enum {resstrtx       =   2};
enum {resquantity    =   9};
enum {reslenght      =  23};
enum {namesx         =   2};
enum {pntssx         =  19};
enum {pntslenght     =   4};
enum {char_empty     = ' '}; /*map*/
enum {char_solid     = '#'};
enum {char_ladder    = 'H'};
enum {char_spoint    = 'o'};
enum {char_bpoint    = 'O'};
enum {char_rockfall  = 'Y'};
enum {char_beetle    = 'w'};
enum {char_door      = '|'};
enum {red_key        = 'r'};
enum {green_key      = 'g'};
enum {blue_key       = 'b'};
enum {spoint_cost    =   1}; /*small point cost*/
enum {bpoint_cost    =   5}; /*big point cost*/
enum {startx         =   5}; /*Quentin*/
enum {starty         =  28};
enum {strthealth     =  10};
enum {jumph           =   2}; /*jump height*/
enum {cell_count     =  27}; /*count of visible cells without Quentin*/
enum {visheight      =   2};
enum {viswidth       =   5};
enum {key_enter      =  10};
enum {key_escape     =  27};
enum {key_space      =  32};
enum {col_bw         =   1}; /*color pairs*/
enum {col_q          =   2};
enum {col_borders    =   3};
enum {col_points     =   4};
enum {col_beetle     =   5};
enum {col_rockfall   =   6};
enum {col_pcounter   =   7};
enum {col_hcounter   =   8};
enum {col_red        =   9};
enum {col_green      =  10};
enum {col_blue       =  11};
enum {col_yellow     =  12};
enum {col_ladder     =  13};
enum q_condition {stay, left, right, fall_down, jump_up};
enum level_num {one = 1, two, three, four, five, six, seven, eight, nine, win};

struct q_body {			/*quentin body*/
	char item;
	int cur_y, cur_x;
	struct q_body *next;
	struct q_body *prev;
};

struct inventory {
	char type;
	struct inventory *next;
	struct inventory *prev;
};

struct counter { /*counter of health, points and other*/
	int health;
	int points;
};
					
struct rockfall { /*enemies*/
	int start_y;
	int start_x;
	int cur_y;
	struct rockfall *prev;
	struct rockfall *next;
};

struct beetle {
	int cur_y;
	int cur_x;
	short dir; /*moving directon*/
	struct beetle *prev;
	struct beetle *next;
};

struct points {
	char type;
	int cur_y;
	int cur_x;
	int cost;
	struct points *prev;
	struct points *next;

};

struct doors {
	char type;
	int cur_y;
	int cur_x;
	struct doors *prev;
	struct doors *next;
};

struct keys {
	char type;
	int cur_y;
	int cur_x;
	struct keys *prev;
	struct keys *next;
};


struct mpointer { /*pointer to other pointers*/
	struct q_body *body;
	struct inventory *inv;
	struct counter *counter;
	struct rockfall *rfall;
	struct beetle *btl;
	struct points *pnts;
	struct doors *drs;
	struct keys *ks;
};

struct result {
	char name[NAMEARR];
	int points;
	struct result *prev;
	struct result *next;
};

#endif
