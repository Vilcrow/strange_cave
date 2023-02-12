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
#include <unistd.h>
#include <locale.h>
#include <libintl.h>
#include "const.h"
#include "game.h"
#include "doors.h"
#include "results.h"
#include "enemies.h"
#include "renderer.h"
#include "quentin.h"

void game()
{
	struct mpointer *mp;
	enum level_num ln = one;
	create_struct(&mp);
	timeout(delay_duration);
	while(mp->counter->health > 0 && ln <= win) {
		switch(ln) {
		case one:
			level(mp, LEVELONESRC, LEVONE, ln);
			++ln;
			break;
		case two:
			level(mp, LEVELTWOSRC, LEVTWO, ln);
			++ln;
			break;
		case three:
			level(mp, LEVELTHREESRC, LEVTHREE, ln);
			++ln;
			break;
		case four:
			level(mp, LEVELFOURSRC, LEVFOUR, ln);
			++ln;
			break;
		case five:
			level(mp, LEVELFIVESRC, LEVFIVE, ln);
			++ln;
			break;
		case six:
			level(mp, LEVELSIXSRC, LEVSIX, ln);
			++ln;
			break;
		case seven:
			level(mp, LEVELSEVENSRC, LEVSEVEN, ln);
			++ln;
			break;
		case eight:
			level(mp, LEVELEIGHTSRC, LEVEIGHT, ln);
			++ln;
			break;
		case nine:
			level(mp, LEVELNINESRC, LEVNINE, ln);
			++ln;
			break;
		case win:
			finish_game(mp);
			++ln;
			break;
		}
	}
}

void game_flow(struct mpointer *mp, const char (*level))
{
	keys_check(mp);
	doors_check(mp);
	show_points(mp);
	show_doors(mp);
	show_keys(mp);
	mv_beetles(mp, level);
	mv_rockfall(mp, level);
	quentin_control(mp, level);
	rm_point(mp);
	show_inventory(mp);
}

void clear_items(struct mpointer *mp)
{
	clear_enemies(mp);
	clear_doors(mp);
	clear_inventory(mp);
}

void preparate_new_level(struct mpointer *mp, const int cur_y, const int cur_x)
{
	clear_items(mp);
	mp->body->cur_y = cur_y;
	mp->body->cur_x = cur_x;
	mp->body->prev->cur_y = cur_y;
	mp->body->next->cur_y = cur_y;
	mp->body->prev->cur_x = mp->body->cur_x-1;
	mp->body->next->cur_x = mp->body->cur_x+1;
}
void level(struct mpointer *mp, const char *source, char *temp,
           const short lev_num)
{
	char (*level);
	clear();
	show_counter(mp);
	show_level_num(lev_num);
	map_preparation(source, temp, &level);
	enemy_preparation(source, mp);
	doors_preparation(source, mp);
	while(mp->body->cur_y >= 0 && mp->counter->health > 0) {
		game_flow(mp, level);
	}
	if(mp->counter->health > 0) {
		preparate_new_level(mp, starty, mp->body->cur_x);
	}
	else {
		game_over(mp);
	}
}

void finish_game(const struct mpointer *mp)
{
	int c;
    char usrname[NAMEARR];
	timeout(-1);
	render_win();
	mvaddstr(maxrow, 1, _("You win."));
	mvaddstr(maxrow+1, 1, _("Press ENTER to quit the game."));
	while((c = getch()) != key_enter) {
		;
	}
    ask_user_name(usrname);
    update_results(usrname, mp->counter->points);
    endwin();
}

void game_over(const struct mpointer *mp)
{
    char usrname[NAMEARR];
    sleep(sleep_duration);
	clear();
    attrset(COLOR_PAIR(col_red));
    render_permanent_background(GAMEOVER);
	refresh();
    sleep(sleep_duration);
    attroff(COLOR_PAIR(col_red));
    if(mp->counter->points > 0) {
        ask_user_name(usrname);
        update_results(usrname, mp->counter->points);
    }
    endwin();
}
