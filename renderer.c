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
#include <stdlib.h>
#include <locale.h>
#include <libintl.h>
#include "const.h"
#include "renderer.h"

void render_permanent_background(const char *name)
{
	FILE *source;
	char c;
	int i, j;
	source = fopen(name, "r");
	if(!source) {
		perror(name);
		endwin();
		exit(1);
	}	
	for(i = 0; i < maxrow; ++i) {
		for(j = 0; j < maxcol; ++j) {
			c = fgetc(source);
			if(c == '\n')
				c = fgetc(source);
			mvaddch(i, j, c);
		}
	}
	fclose(source);
}

char environment_type(const int next_y, const int next_x, const char (*level))
{
	FILE *source;
	char c;
	char n = ' ';
	int i, j;
	source = fopen(level, "r");
	if(!source) {
		perror(level);
		endwin();
		exit(1);
	}	
	for(i = 0; i < maxrow; ++i) {
		for(j = 0; j < maxcol; ++j) {
			c = fgetc(source);
			if(c == '\n')
				c = fgetc(source);
			if(i == next_y && j == next_x) {
				fclose(source);
				return c;
			}
		}
	}
	return n;
}

void show_environment(const char (*level), const struct mpointer *mp)
{
	FILE *source;
	char c;
	int i, j;
	int y = mp->body->cur_y;
	int x = mp->body->cur_x;
	source = fopen(level, "r");
	if(!source) {
		perror(level);
		endwin();
		exit(1);
	}	
	for(i = 0; i <maxrow; ++i) {
		for(j = 0; j <= maxcol; ++j) {
			c = fgetc(source);
			if( c == '\n')
				continue;
			else if((i >= y - visheight && i <= y + visheight) &&
				(j >= x - viswidth && j <= x + viswidth)) {	
				if(c == char_ladder) {
					attrset(COLOR_PAIR(col_ladder));
					mvaddch(i, j, c);
					attroff(COLOR_PAIR(col_ladder));
				}
				else
					mvaddch(i, j, c);
			}
		}
	}
	fclose(source);
}

void hide_environment(const struct mpointer *mp)
{
	int i, j;
	short vsblt;
	attrset(COLOR_PAIR(col_bw));
	for(i = 0; i < maxrow; ++i) {
		for(j = 0; j < maxcol; ++j) {
			vsblt = visibility(mp, i, j);
			if(!vsblt)
				mvaddch(i, j, char_empty); 
		}
	}
	attroff(COLOR_PAIR(col_bw));
}

void render_borders(const char *name)
{
	FILE *source;
	char c;
	int i, j;
	source = fopen(name, "r");
	if(!source) {
		perror(name);
		endwin();
		exit(1);
	}	
	for(i = 0; i < maxrow; ++i) {
		for(j = 0; j < maxcol; ++j) {
			c = fgetc(source);
			if(c == '\n')
				c = fgetc(source);
			if(c != char_empty)
				mvaddch(i, j, c);
		}
	}
	fclose(source);
}

void map_preparation(const char *source, char *temp, char (**level))
{
	FILE *from;
	FILE *to;
	int i, j;
	int c;
	from = fopen(source, "r");
	if(!from) {
		perror(source);
		endwin();
		exit(1);
	}
	to = fopen(temp, "w");
	if(!to) {
		perror(temp);
		endwin();
		exit(1);
	}
	for(i = 0; i < maxrow; ++i) {
		for(j = 0; j <= maxcol; ++j) { /*<= because '\n' is taken into account*/
			c = fgetc(from);
		if(c == char_solid || c == char_rockfall ||
		   c == char_ladder  || c == '\n')
			fputc(c, to);
		else
			fputc(' ', to);
		}
	}
	fclose(from);
	fclose(to);	
	(*level) = temp;
}

void map_renderer(const char (*level)) /*render map of current level*/
{
	FILE *source;
	char c;
	int i, j;
	source = fopen(level, "r");
	if(!source) {
		perror(level);
		endwin();
		exit(1);
	}	
	for(i = 0; i < maxrow; ++i) {
		for(j = 0; j < maxcol; ++j) {
			c = fgetc(source);
			if(c == '\n')
				c = fgetc(source);
			mvaddch(i, j, c);
		}
	}
	fclose(source);
}

void add_point(struct mpointer *mp, const int cur_y, const int cur_x, const char type)
{
	struct points *tmp = NULL;
	struct points *aux = NULL;
	tmp = malloc(sizeof(struct points));
	tmp->type = type;
	tmp->cur_y = cur_y;
	tmp->cur_x = cur_x;
	if(type == char_spoint)
		tmp->cost = spoint_cost;
	else if(type == char_bpoint)
		tmp->cost = bpoint_cost;
	tmp->prev = NULL;
	tmp->next = NULL;
	if(mp->pnts == NULL) {
		mp->pnts = tmp;
	}	
	else {
		aux = mp->pnts;
		while(aux->next != NULL)
			aux = aux->next;
		aux->next = tmp;
		tmp->prev = aux;
	}
}

void rm_point(struct mpointer *mp) /*removing a used point*/
{
	struct points *tmp = mp->pnts;
	struct q_body *tmpb = mp->body;
	while(tmp != NULL) {
		if(tmp->cur_y == tmpb->cur_y && tmp->cur_x == tmpb->cur_x) {
			if(tmp == mp->pnts) {
				update_points_counter(mp, tmp->type);
				mp->pnts = tmp->next;
				if(mp->pnts != NULL)
					mp->pnts->prev = NULL;
				free(tmp);
			}
			else if(tmp->next == NULL) {
				update_points_counter(mp, tmp->type);
				if(tmp->prev != NULL)
					tmp->prev->next = NULL;
				free(tmp);
			}
			else {
				update_points_counter(mp, tmp->type);
				tmp->next->prev = tmp->prev;
				tmp->prev->next = tmp->next;
				free(tmp);
			}
		}
		else
			tmp = tmp->next;
	}
}

void show_points(const struct mpointer *mp) /*showing unused points*/
{
	short vsblt;
	struct points *tmp = mp->pnts;
	attrset(COLOR_PAIR(col_points));
	while(tmp != NULL) {
		vsblt = visibility(mp, tmp->cur_y, tmp->cur_x);
		if(vsblt)	
			mvaddch(tmp->cur_y, tmp->cur_x, tmp->type);
		tmp = tmp->next;
	}
	attroff(COLOR_PAIR(col_points));
}

void update_points_counter(struct mpointer *mp, const char ptype)
{
	if(ptype == char_spoint)
		mp->counter->points += spoint_cost;
	else if(ptype == char_bpoint)
		mp->counter->points += bpoint_cost;
	show_counter(mp);
}

void update_health_counter(struct mpointer *mp, const short shift)
{
	mp->counter->health += shift;
	show_counter(mp);
}

short visibility(const struct mpointer *mp, const int cur_y, const int cur_x)
{
	int y = mp->body->cur_y;
	int x = mp->body->cur_x;
	if((cur_y >= y - visheight && cur_y <= y + visheight) &&
	   (cur_x >= x - viswidth && cur_x <= x + viswidth))	
		return TRUE;
	else
		return FALSE;

}

void show_counter(const struct mpointer *mp)
{
    attrset(COLOR_PAIR(col_pcounter));
    mvprintw(1, maxcol, _("Points - %d"), mp->counter->points);
    attroff(COLOR_PAIR(col_pcounter));
    attrset(COLOR_PAIR(col_hcounter));
    mvprintw(2, maxcol, _("Health - %d "), mp->counter->health);

}

void show_inventory(const struct mpointer *mp)
{
	short firsty = 4;
	short i = 1;
	short col;
	short cp; /*color pair*/
	char *key;
	struct inventory *tmp = mp->inv;
	attrset(COLOR_PAIR(col_blue));
	mvprintw(firsty, maxcol, _("Inventory:"));
	attroff(COLOR_PAIR(col_blue));
	while(tmp != NULL) {
		col = tmp->type;
		switch(col) {
		case red_key:
			cp = col_red;
			key = _(REDKEY);
			break;
		case green_key:
			cp = col_green;
			key = _(GREENKEY);
			break;
		case blue_key:
			cp = col_blue;
			key = _(BLUEKEY);
			break;
		}
		attrset(COLOR_PAIR(cp));
		mvprintw(firsty+i, maxcol, "%s", key);
		attroff(COLOR_PAIR(cp));
		tmp = tmp->next;
		++i;
	}
}

void render_win()
{
	FILE *source;
	char c;
	int i, j;
	source = fopen(WIN, "r");
	if(!source) {
		perror(WIN);
		endwin();
		exit(1);
	}	
	for(i = 0; i < maxrow; ++i) {
		for(j = 0; j <= maxcol; ++j) {
			c = fgetc(source);
			if(c == char_solid)
				attrset(COLOR_PAIR(col_yellow));
			else if(c == 'G' || c == '<' || c == '>')
				attrset(COLOR_PAIR(col_q));
			else if(c == '@' || c == 'N')
				attrset(COLOR_PAIR(col_bw));
			else if(c == '*' || c == '|' || c == '_' || c == '\\' || c == '/')
				attrset(COLOR_PAIR(col_green));
			mvaddch(i, j, c);
		}
	}
	attrset(COLOR_PAIR(col_bw));
	fclose(source);
}

void show_level_num(const short lev_num)
{
	mvprintw(maxrow+1, 1, _("Level %d"), lev_num);
}
