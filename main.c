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

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include "const.h"
#include "colors.h"
#include "game.h"
#include "renderer.h"
#include "menupage.h"

int main()
{
	int choice = 0;
	initscr();
	if(!has_colors()) {
		endwin();
		fprintf(stderr, _("Your terminal don't support colors\n"));
		return 1;	
	}
	cbreak();
	keypad(stdscr, 1);
	noecho();
	curs_set(0);
	start_color();
	set_colors();
	attrset(COLOR_PAIR(col_bw));
	menu_selection(&choice);
	if(choice == c_start)	
		game();
	endwin();
	return 0;
}
