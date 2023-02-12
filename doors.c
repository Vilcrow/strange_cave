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

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <locale.h>
#include <libintl.h>
#include "const.h"
#include "renderer.h"
#include "doors.h"

void doors_preparation(const char *level, struct mpointer *mp)
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
		for(j = 0; j <= maxcol; ++j) {
			c = fgetc(source);
			if(c == char_door)
				add_door(mp, i, j);
			else if(c == red_key || c == green_key || c == blue_key)
				add_key(mp, i, j, c); 
		}
	}
	fclose(source);
}

void add_door(struct mpointer *mp, const int cur_y, const int cur_x)
{
	struct doors *tmp = NULL;
	tmp = malloc(sizeof(struct doors));
	tmp->cur_y = cur_y;
	tmp->cur_x = cur_x;
	tmp->prev = NULL;
	tmp->next = NULL;
	if(mp->drs == NULL) {
		tmp->type = red_key;
		mp->drs = tmp;	
	}
	else if(mp->drs->next == NULL) {
		tmp->prev = mp->drs;
		mp->drs->next = tmp;
		tmp->type = green_key;
	}
	else {
		tmp->prev = mp->drs->next;
		mp->drs->next->next = tmp;
		tmp->type = blue_key;
	}
}

void rm_door(struct mpointer *mp, const int cur_y, const int cur_x)
{
	struct doors *tmp = mp->drs;	
	while(tmp != NULL) {
		if(tmp->cur_y == cur_y && tmp->cur_x == cur_x) {
			if(tmp == mp->drs) {
				mp->drs = tmp->next;
				free(tmp);
				break;
			}
			else {
				tmp->prev->next = tmp->next;
				if(tmp->next != NULL)
					tmp->next->prev = tmp->prev;
				free(tmp);
				break;
			}
		}
		tmp = tmp->next;
	}
}

void doors_check(struct mpointer *mp)
{
	struct doors *dtmp = mp->drs;
	while(dtmp != NULL) {
		if((dtmp->cur_y == mp->body->cur_y) &&
		   ((dtmp->cur_x == mp->body->cur_x-1)
		 || (dtmp->cur_x == mp->body->cur_x+1))) {
			open_door(mp, dtmp);	
			break;
		}
		dtmp = dtmp->next;
	}
}

void open_door(struct mpointer *mp, struct doors *door)
{
	struct inventory *itmp = mp->inv;
	while(itmp != NULL) {
		if(itmp->type == door->type) {
			rm_door(mp, door->cur_y, door->cur_x);
			break;
		}
		itmp = itmp->next;
	}
}

void add_key(struct mpointer *mp, const int cur_y, const int cur_x,
			 const char c)
{
	struct keys *tmp = NULL;
	struct keys *aux = mp->ks;
	tmp = malloc(sizeof(struct keys));
	tmp->type = c;
	tmp->cur_y = cur_y;
	tmp->cur_x = cur_x;
	tmp->prev = NULL;
	tmp->next = NULL;
	if(mp->ks == NULL) {
		mp->ks = tmp;	
	}
	else {
		while(aux->next != NULL)
			aux = aux->next;
		tmp->prev = aux;
		aux->next = tmp;
	}
}

void rm_key(struct mpointer *mp, const int cur_y, const int cur_x)
{
	struct keys *tmp = mp->ks;	
	while(tmp != NULL) {
		if(tmp->cur_y == cur_y && tmp->cur_x == cur_x) {
			if(tmp == mp->ks) {
				mp->ks = tmp->next;
				free(tmp);
				break;
			}
			else {
				tmp->prev->next = tmp->next;
				if(tmp->next != NULL)
					tmp->next->prev = tmp->prev;
				free(tmp);
				break;
			}
		}
		tmp = tmp->next;
	}
}

void keys_check(struct mpointer *mp)
{
	struct keys *tmp = mp->ks;
	while(tmp != NULL) {
		if(tmp->cur_y == mp->body->cur_y && tmp->cur_x == mp->body->cur_x) {
			add_key_to_inventory(mp, tmp->type);
			rm_key(mp, tmp->cur_y, tmp->cur_x);
			break;
		}
		tmp = tmp->next;
	}
}

void add_key_to_inventory(struct mpointer *mp, const char type)
{
	struct inventory *tmp = NULL;
	struct inventory *aux = mp->inv;
	tmp = malloc(sizeof(struct inventory));
	tmp->prev = NULL;
	tmp->next = NULL;
	tmp->type = type;
	if(mp->inv == NULL)
		mp->inv = tmp;
	else {
		while(aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = tmp;
		tmp->prev = aux;
	}
}

void show_doors(const struct mpointer *mp)
{
	struct doors *tmp = mp->drs;
	int color;
	short vsblt;
	while(tmp != NULL) {
		vsblt = visibility(mp, tmp->cur_y, tmp->cur_x);
		if(tmp->type == red_key)
			color = col_red;
		else if(tmp->type == green_key)
			color = col_green;
		else if(tmp->type == blue_key)
			color = col_blue;
		attrset(COLOR_PAIR(color));
		if(vsblt)
			mvaddch(tmp->cur_y, tmp->cur_x, char_door);
		tmp = tmp->next;	
	}
	attroff(COLOR_PAIR(color));
}

void show_keys(const struct mpointer *mp)
{
	struct keys *tmp = mp->ks;
	int color;
	short vsblt;
	while(tmp != NULL) {
		vsblt = visibility(mp, tmp->cur_y, tmp->cur_x);
		if(tmp->type == red_key)
			color = col_red;
		else if(tmp->type == green_key)
			color = col_green;
		else if(tmp->type == blue_key)
			color = col_blue;
		attrset(COLOR_PAIR(color));
		if(vsblt)
			mvaddch(tmp->cur_y, tmp->cur_x, tmp->type);
		tmp = tmp->next;
	}
	attroff(COLOR_PAIR(color));
}

void clear_doors(struct mpointer *mp)
{
	struct doors *dtmp = mp->drs;
	struct keys *ktmp = mp->ks;
	while(dtmp != NULL) {
		mp->drs = dtmp->next;
		free(dtmp);
		dtmp = mp->drs;	
	}	
	while(ktmp != NULL) {
		mp->ks = ktmp->next;
		free(ktmp);
		ktmp = mp->ks;
	}
	mp->drs = NULL;
	mp->ks = NULL;
}

short is_door(const struct mpointer *mp, const int cur_y, const int cur_x)
{
	struct doors *dtmp = mp->drs;
	while(dtmp != NULL) {
		if(dtmp->cur_y == cur_y && dtmp->cur_x == cur_x) {
			return TRUE;
		}
		dtmp = dtmp->next;
	}	
	return FALSE;
}

void clear_inventory(struct mpointer *mp)
{
	struct inventory *tmp = mp->inv;
	while(tmp != NULL) {
		mp->inv = tmp->next;
		free(tmp);
		tmp = mp->inv;
	}
	mp->inv = NULL;
}
