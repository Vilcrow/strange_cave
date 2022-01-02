/*
Strange Cave game

Copyright (C) 2022 S.V.I 'Vilcrow', <svi@vilcrow.net>
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
#include "const.h"
#include "colors.h"

void set_colors()
{
	init_pair(col_bw, COLOR_WHITE, COLOR_BLACK);
	init_pair(col_q, COLOR_GREEN, COLOR_BLACK);
	init_pair(col_borders, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(col_points, COLOR_YELLOW, COLOR_BLACK);
	init_pair(col_beetle, COLOR_RED, COLOR_BLACK);
	init_pair(col_rockfall, COLOR_BLUE, COLOR_BLACK);
	init_pair(col_hcounter, COLOR_RED, COLOR_BLACK);
	init_pair(col_pcounter, COLOR_BLUE, COLOR_BLACK);
	init_pair(col_red, COLOR_RED, COLOR_BLACK);
	init_pair(col_green, COLOR_GREEN, COLOR_BLACK);
	init_pair(col_blue, COLOR_BLUE, COLOR_BLACK);
	init_pair(col_yellow, COLOR_YELLOW, COLOR_BLACK);
	init_pair(col_ladder, COLOR_MAGENTA, COLOR_BLACK);
}
