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

#include <stdlib.h>
#include <ncurses.h>
#include "const.h"
#include "renderer.h"
#include "doors.h"
#include "quentin.h"
#include "game.h"

void create_struct(struct mpointer **mp)
{
    struct q_body    *tmp1 = NULL;
    struct q_body    *tmp2 = NULL;
    struct mpointer  *tmp3 = NULL;
	struct counter   *tmp5 = NULL;
    tmp3 = malloc(sizeof(struct mpointer));
    tmp1 = malloc(sizeof(struct q_body));
    tmp1->item = '<'; /*Qientin body*/
    tmp1->cur_y = 0;
    tmp1->cur_x = 0;
    tmp1->next = NULL;
    tmp1->prev = NULL;
    tmp2 = malloc(sizeof(struct q_body));
    tmp2->item = 'G';
    tmp2->cur_y = tmp1->cur_y;
    tmp2->cur_x = tmp1->cur_x + 1;
    tmp2->next = NULL;
    tmp2->prev = tmp1;
    tmp1->next = tmp2;
    tmp1 = malloc(sizeof(struct q_body));
    tmp1->item = '>';
    tmp1->cur_y = tmp2->cur_y;
    tmp1->cur_x = tmp2->cur_x + 1;
    tmp1->next = NULL;
    tmp1->prev = tmp2;
    tmp2->next = tmp1;
	tmp3->body = tmp2;
	tmp3->inv = NULL;
    tmp5 = malloc(sizeof(struct counter));
	tmp5->health = strthealth;	
	tmp5->points = 0;
	tmp3->counter = tmp5;
	tmp3->body->cur_y = starty; /*Quentin start position*/
    tmp3->body->cur_x = startx;
    tmp3->body->prev->cur_y = starty;
    tmp3->body->prev->cur_x = startx - 1;
    tmp3->body->next->cur_y = starty;
    tmp3->body->next->cur_x = startx + 1;
	tmp3->rfall = NULL; /*enemies*/
	tmp3->btl = NULL;
	tmp3->pnts = NULL; /*points*/
	tmp3->drs = NULL; /*doors and keys*/
	tmp3->ks = NULL;
	*mp = tmp3; /*main pointer*/
}

void change_body (struct mpointer *mp, const int q_condition)
{
	struct q_body *tmp1 = mp->body;
	struct q_body *tmp2 = NULL;
	char l, c, r;
	c = 'G';
	switch(q_condition) {
	case stay:				/*<G>*/
		l = '<';
		r = '>';
		break;
	case left:				/*G>*/
		l = ' ';
		r = '>';
		break;
	case right:				/*<G*/
		l = '<';
		r = ' ';
		break;
	case fall_down:			/*\G/*/
		l = '\\';
		r = '/';
		break;
	case jump_up:			/*/G\*/
		l = '/';
		r = '\\';
		break;
	}
	tmp1->item = c;
	tmp2 = tmp1->prev;
	tmp2->item = l;
	tmp2 = tmp1->next;
	tmp2->item = r;
}

void show_quentin(const struct mpointer *mp, const char (*level))
{
	char env_type;
	struct q_body *tmp = mp->body->prev;
	attrset(COLOR_PAIR(col_q));
	while(tmp != NULL) {
		env_type = environment_type(tmp->cur_y, tmp->cur_x, level);
		if(tmp->item != char_empty &&
		  (env_type == char_empty || env_type == char_ladder))
			mvaddch(tmp->cur_y, tmp->cur_x, tmp->item);
		tmp = tmp->next;
	}
	attroff(COLOR_PAIR(col_q));
}

void move_quentin(struct mpointer *mp, const char (*level), const int q_condition,
				  const short dy, const short dx)
{
	struct q_body *tmp = mp->body->prev;
	change_body(mp, q_condition);
	while(tmp != NULL) {
		tmp->cur_y += dy;	
		tmp->cur_x += dx;	
		tmp = tmp->next;
	}
	hide_environment(mp);
	attrset(COLOR_PAIR(col_borders));
	render_borders(BORDERSBCKG);
	attroff(COLOR_PAIR(col_borders));
	attrset(COLOR_PAIR(col_bw));
	show_environment(level, mp);
	show_quentin(mp, level);
	attroff(COLOR_PAIR(col_bw));
}

void jump(struct mpointer *mp, const char (*level), const int q_condition)
{
    int env_type = environment_type(mp->body->cur_y-1, mp->body->cur_x, level);
    int height = jumph;
	change_body(mp, jump_up);
    while(height != 0 && env_type == char_empty) {
		move_quentin(mp, level, jump_up, -1, 0);
        height += -1;
        env_type = environment_type(mp->body->cur_y-1, mp->body->cur_x, level);
    }
}

void fall(struct mpointer *mp, const char (*level), const int q_condition)
{
	change_body(mp, fall_down);
	move_quentin(mp, level, fall_down, 1, 0);
    show_quentin(mp, level);
}

void quentin_control(struct mpointer *mp, const char (*level))
{
	int key;
	key = getch();
	switch(key) {
	case KEY_UP:
		case_key_up(mp, level);
		break;
	case KEY_DOWN:
		case_key_down(mp, level);
		break;
	case KEY_LEFT:
		case_key_left(mp, level);
		break;
	case KEY_RIGHT:
		case_key_right(mp, level);
		break;
	case key_escape:
		case_key_escape(mp);
		break;
	case key_space:
		case_key_space();
		break;
	case ERR:
		case_err(mp, level);
		break;
	default:
	    ;
    }
}

void case_key_up(struct mpointer *mp, const char (*level))
{
	char env_type;
	env_type = environment_type(mp->body->cur_y+1, mp->body->cur_x, level);
	if(env_type != char_empty || env_type == char_ladder) {
		env_type = environment_type(mp->body->cur_y-1, mp->body->cur_x, level);
		if(env_type == char_empty)
			jump(mp, level, jump_up);
		else if(env_type == char_ladder)
			move_quentin(mp, level, jump_up, -1, 0);
		}
	else {
	env_type = environment_type(mp->body->cur_y, mp->body->cur_x, level);
	if(env_type == char_ladder)
		move_quentin(mp, level, jump_up, -1, 0);
	}
}

void case_key_down(struct mpointer *mp, const char (*level))
{
	char env_type;
	env_type = environment_type(mp->body->cur_y+1, mp->body->cur_x, level);
	if(env_type == char_ladder)
		move_quentin(mp, level, fall_down, 1, 0);
	else {
		env_type = environment_type(mp->body->cur_y, mp->body->cur_x, level);
		if(env_type == char_ladder) {
			env_type = environment_type(mp->body->cur_y+1, mp->body->cur_x, level);
			if(env_type == char_empty)
				move_quentin(mp, level, fall_down, 1, 0);
		}
	}
}

void case_key_left(struct mpointer *mp, const char (*level))
{
	char env_type;
	short door;
	env_type = environment_type(mp->body->cur_y, mp->body->cur_x-1, level);
	door = is_door(mp, mp->body->cur_y, mp->body->cur_x-1);
	if((env_type == char_empty || env_type == char_ladder) && !door)
		move_quentin(mp, level, left, 0, -1);
	env_type = environment_type(mp->body->cur_y+1, mp->body->cur_x, level);
	if(env_type == char_empty)
		fall(mp, level, fall_down);

}

void case_key_right(struct mpointer *mp, const char (*level))
{
	char env_type;
	short door;
	env_type = environment_type(mp->body->cur_y, mp->body->cur_x+1, level);
	door = is_door(mp, mp->body->cur_y, mp->body->cur_x+1);
	if((env_type == char_empty || env_type == char_ladder) && !door)
		move_quentin(mp, level, right, 0, 1);
	env_type = environment_type(mp->body->cur_y+1, mp->body->cur_x, level);
	if(env_type == char_empty)
		fall(mp, level, fall_down);
}

void case_key_escape(const struct mpointer *mp)
{
	int c;
	int i;
	int x = 1;
	timeout(-1);
	mvaddstr(maxrow, x, _("Quit game?(y/n)"));
	c = getch();
	for(i = 0; i < maxcol; ++i)
		mvaddch(maxrow, i, char_empty);
	if(c == 'y') {
		game_over(mp);
		endwin();
		exit(0);		
	}
	else
		timeout(delay_duration);
}

void case_key_space()
{
	int c;
	int i;
	timeout(-1);
	mvaddstr(maxrow, 1, _("Pause"));
	while((c = getch()) != key_space) {
		;
	}
	for(i = 0; i < maxcol; ++i)
		mvaddch(maxrow, i, char_empty);
	timeout(delay_duration);
}

void case_err(struct mpointer *mp, const char (*level))
{
	char env_type;
	env_type = environment_type(mp->body->cur_y, mp->body->cur_x, level);
	if(env_type == char_ladder)
		move_quentin(mp, level, stay, 0, 0);
	else {
		env_type = environment_type(mp->body->cur_y+1, mp->body->cur_x, level);
		if(env_type == char_empty)
			fall(mp, level, fall_down);
		else
			move_quentin(mp, level, stay, 0, 0);
	}
}
