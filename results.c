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
#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include "const.h"
#include "results.h"

void update_results(const char *usrname, const int points)
{
    struct result *res = NULL;
    struct result *newres = NULL;
    int i, j;
	char srcdir[MAXPATHARRAY];
	char *homedir = getenv("HOME"); /*take home directories path*/
	if(!homedir) {
		perror("The home directory is not defined.\n");
		endwin();
		exit(2);
	}
	for(i = 0; homedir[i]; ++i)
		srcdir[i] = homedir[i];
	for(j = 0; RESSRC[j]; ++j) {
		srcdir[i] = RESSRC[j];
		++i;
	}
	srcdir[i] = '\0';
    preparate_res_temp(srcdir);
    prev_results(&res);
    newres = malloc(sizeof(struct result));
    for(i = 0; i < NAMEARR; ++i) {
        newres->name[i] = usrname[i];
    }
    newres->points = points;
    newres->prev = NULL;
    newres->next = NULL;
    place_result(&res, newres);
    update_result_file(res, srcdir);
}

short isbckg(const int cur_y, const int cur_x)
{
    if(cur_y < resstrty || cur_y >= resstrty+resquantity ||
       cur_x < resstrtx || cur_x > resstrtx+reslenght-1)
        return TRUE;
    else
        return FALSE;
}

void prev_results(struct result **res)
{
	FILE *temp;
	struct result *tmp = NULL;
	struct result *aux = NULL;
	int i, j;
	int c;
	int k = 0;
	int p = 0;
	int d = 1000;
	temp = fopen(RESTEMP, "r");
	if(!temp)
		temp = fopen(RESEMPTY, "r");
	if(!temp) {
		perror(RESEMPTY);
		endwin();
		exit(1);	
	}
	for(i = 0; i < resquantity; ++i) {
		tmp = malloc(sizeof(struct result));
		tmp->points = 0;
		tmp->next = NULL;
		tmp->prev = NULL;
		for(j = 0; j <= reslenght; ++j) {
			c = fgetc(temp);
			if(j > 1 && j < 18) {
				tmp->name[k] = c;
				++k;
			}		
			else if(j > 18 && j < reslenght) {
				p = c - '0';
				p = p * d;
				tmp->points += p;
				d = d / 10;
			}
			else if(j == reslenght) {
				tmp->name[k] = '\0';	
				k = 0;
				d = 1000;
			}
		}
		if(*res == NULL) {
			*res = tmp;
			aux = tmp;
		}
		else {
			tmp->prev = aux;
			aux->next = tmp;
			aux = tmp;
		}
	}
	fclose(temp);
}

void preparate_res_temp(char *srcdir)
{
    FILE *source;
    FILE *temp;
    int i, j, k = 0;
    int c;
    short bckg;
    source = fopen(srcdir, "r");
    temp = fopen(RESTEMP, "w");
	if(!source) {
		rewrite_res_file(RESEMPTY, srcdir);
		source = fopen(srcdir, "r");
	}
    if(!source) {
        perror(srcdir);
        endwin();
        exit(1);
    }
	if(!temp) {
		perror(RESTEMP);
		endwin();
		exit(1);
	}
    for(i = 0; i < maxrow; ++i) {
        for(j = 0; j <= maxcol; ++j) {
            c = fgetc(source);
            bckg = isbckg(i, j);
            if(c == '\n')
                continue;
            if(!bckg) {
                fputc(c, temp);
				++k;
                if(k == reslenght)
                    fputc('\n', temp);
            }
        }
		k = 0;
    }
    fclose(source);
    fclose(temp);
}

void place_result(struct result **res, struct result *newres)
{
	struct result *tmp = *res;
	while(tmp->next != NULL) { /*go to end of list*/
		tmp = tmp->next;
	}
	while(tmp != NULL) {
		if(newres->points >= tmp->points)
			tmp = tmp->prev;
		else if(tmp->next == NULL) {
			newres->prev = tmp;
			tmp->next = newres;
			break;
		}
		else {
			tmp->next->prev = newres;
			newres->next = tmp->next;
			tmp->next = newres;
			newres->prev = tmp;
			break;
		}
	}
	if(newres->prev == NULL) {
		tmp = *res;
		newres->next = tmp;
		tmp->prev = newres;
		*res = newres;	
	}
	while(tmp->next != NULL) {
		tmp = tmp->next;
	}
	tmp->prev->next = NULL;
	free(tmp);
}

void update_result_file(const struct result *res, char *srcdir)
{
    FILE *source;
    FILE *temp;
    short bckg;
    int place = 1;
    int i, j, k;
    int c;
    source = fopen(srcdir, "r");
	if(!source) {
		perror(srcdir);
		endwin();
		exit(1);
	}
    temp = fopen(RESTEMP, "w");
	if(!temp) {
		perror(RESTEMP);
		endwin();
		exit(1);
	}
    for(i = 0; i < maxrow; ++i) {
        for(j = 0; j <= maxcol; ++j) {
            c = fgetc(source);
			k = reslenght-1;
            bckg = isbckg(i, j);
            if(bckg) {
                fputc(c, temp);
            }
            else {
                fprintf(temp, "%d %s %04d", place, res->name, res->points);
				res = res->next;
				while(k != 0) {
                	c = fgetc(source);
					--k;	
					++j;
				}
                ++place;
            }
        }
    }
    fclose(source);
    fclose(temp);
	rewrite_res_file(RESTEMP, srcdir);
}

void rewrite_res_file(const char *srcdir, char *dstdir)
{
	FILE *source;
	FILE *destination;
	int i, j;
	int c;
	source = fopen(srcdir, "r");
	if(!source) {
		perror(srcdir);
		endwin();
		exit(1);	
	}
	destination = fopen(dstdir, "w");
	if(!destination) {
		perror(dstdir);
		endwin();
		exit(1);	
	}
	for(i = 0; i < maxrow; ++i)
		for(j = 0; j <= maxcol; ++j) {
			c = fgetc(source);
			fputc(c, destination);
		}
	fclose(source);
	fclose(destination);
}

void ask_user_name(char *usrname)
{
    int c;
    int i = 0;
    int j;
    int k = 0;
	int notletter = 0;
	endwin();
    printf(_("Enter your name(a-z, max %d): "), MAXNAMESIZE);
    while((c = getchar()) != key_enter) {
        if(((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))&& i < MAXNAMESIZE) {
            usrname[i] = c;
            ++i;
        }
		else if(c == '\b') {
			if(notletter > 0)
				--notletter;
			else if(notletter == 0)
				--i;
			if(i < 0)
				i = 0;
		}
		else {
			++notletter;
		}
    }
    k += i;
    for(j = 0; j < MAXNAMESIZE-i+4; ++j) {
        usrname[k] = '.';
        ++k;
    }
    usrname[k] = '\0';
}
