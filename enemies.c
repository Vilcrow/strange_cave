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
#include "quentin.h"
#include "enemies.h"
#include "renderer.h"

void enemy_preparation(const char *level, struct mpointer *mp) /*create enemies structures*/
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
            if(c == char_rockfall)
                add_rockfall(mp, i, j);
            else if(c == char_beetle)
                add_beetle(mp, i, j);
            else if(c == char_spoint)
                add_point(mp, i, j, char_spoint);
            else if(c == char_bpoint)
                add_point(mp, i, j, char_bpoint);
        }
    }
    fclose(source);
}

void mv_rockfall(struct mpointer *mp, const char (*level))
{
	struct rockfall *tmp = mp->rfall;
	char env_type;
	short vsblt;
	int qy = mp->body->cur_y;
	int qx = mp->body->cur_x;
	attrset(COLOR_PAIR(col_rockfall));
	while(tmp != NULL) {
		if(tmp->cur_y == tmp->start_y) {
			++tmp->cur_y;
		}
		else {
			env_type = environment_type(tmp->cur_y, tmp->start_x, level);	
			if(env_type == char_empty || env_type == char_ladder) {
				vsblt = visibility(mp, tmp->cur_y, tmp->start_x);
				if(vsblt) {
					mvaddch(tmp->cur_y-1, tmp->start_x, char_empty);
					mvaddch(tmp->cur_y, tmp->start_x, char_rockfall);
				}
				if(tmp->cur_y == qy && tmp->start_x == qx) {
					update_health_counter(mp, -1);
					tmp->cur_y = tmp->start_y;
				}	/*the rockfall hit Quentin*/
				++tmp->cur_y;
			}
			else {
				mvaddch(tmp->cur_y-1, tmp->start_x, char_empty);
				tmp->cur_y = tmp->start_y;	
			}
		}
		tmp = tmp->next;
	}
	attroff(COLOR_PAIR(col_rockfall));
}

void mv_beetles(struct mpointer *mp, const char (*level))
{
	struct beetle *tmp = mp->btl;
	char setype; /*side environment type*/
	char betype; /*botoom environment type*/
	int qy = mp->body->cur_y;
	int qx = mp->body->cur_x;
	short vsblt;
	while(tmp != NULL) {
		if(tmp->cur_y == qy && tmp->cur_x == qx) {	/*the beetle hit Quentin*/
			update_health_counter(mp, -1);
			tmp->dir *= -1;
			tmp->cur_x += tmp->dir;
			move_quentin(mp, level, stay, 0, 0);
		}
		setype = environment_type(tmp->cur_y, tmp->cur_x+tmp->dir, level);	
		if(setype == char_empty || setype == char_ladder) {
			betype = environment_type(tmp->cur_y+1, tmp->cur_x+tmp->dir, level);
			if(betype == char_solid || betype == char_ladder)
				tmp->cur_x += tmp->dir;
			else
				tmp->dir *= -1;
		}
		else
			tmp->dir *= -1;
		vsblt = visibility(mp, tmp->cur_y, tmp->cur_x);
		setype = environment_type(tmp->cur_y, tmp->cur_x-tmp->dir, level);	
		attrset(COLOR_PAIR(col_beetle));
		if(vsblt) {
			if(setype == char_empty || setype == char_ladder)
				mvaddch(tmp->cur_y, tmp->cur_x-tmp->dir, char_empty);
			mvaddch(tmp->cur_y, tmp->cur_x, char_beetle);
		}
		else {
			if(setype == char_empty)
				mvaddch(tmp->cur_y, tmp->cur_x-tmp->dir, char_empty);
		}
		tmp = tmp->next;
	}
	attroff(COLOR_PAIR(col_beetle));
}

void add_rockfall(struct mpointer *mp, const int cur_y, const int cur_x)
{
    struct rockfall *tmp = NULL;
    struct rockfall *aux = NULL;
    tmp = malloc(sizeof(struct rockfall));
    tmp->start_y = cur_y;
    tmp->start_x = cur_x;
    tmp->cur_y = tmp->start_y;
    tmp->prev = NULL;
    tmp->next = NULL;
    if(mp->rfall == NULL) {
        mp->rfall = tmp;
    }
    else {
        aux = mp->rfall;
        while(aux->next)
            aux = aux->next;
        aux->next = tmp;
        tmp->prev = aux;
    }
}

void add_beetle(struct mpointer *mp, const int cur_y, const int cur_x)
{
    struct beetle *tmp = NULL;
    struct beetle *aux = NULL;
    tmp = malloc(sizeof(struct beetle));
    tmp->cur_y = cur_y;
    tmp->cur_x = cur_x;
    tmp->dir = 1; /*moving to right side*/
    tmp->prev = NULL;
    tmp->next = NULL;
    if(mp->btl == NULL) {
        mp->btl = tmp;
    }
    else {
        aux = mp->btl;
        while(aux->next != NULL)
            aux = aux->next;
        aux->next = tmp;
        tmp->prev = aux;
    }
}

void clear_enemies(struct mpointer *mp)
{
	struct rockfall *rtmp = mp->rfall;
	struct beetle *btmp = mp->btl;
	struct points *ptmp = mp->pnts;
	while(rtmp != NULL) {
		mp->rfall = rtmp->next;
		free(rtmp);
		rtmp = mp->rfall;
	}
	while(btmp != NULL) {
		mp->btl = btmp->next;
		free(btmp);
		btmp = mp->btl;
	}
	while(ptmp != NULL) {
		mp->pnts = ptmp->next;
		free(ptmp);
		ptmp = mp->pnts;
	}
	mp->rfall = NULL;
	mp->btl = NULL;
	mp->pnts = NULL;
}
