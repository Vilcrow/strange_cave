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
#include "const.h"
#include "menupage.h"
#include "renderer.h"

void show_menu()
{
	attrset(COLOR_PAIR(1));
	render_permanent_background(MENUBCKG);
	move(mnstrty, mnstrtx);
	addstr(_(">START<"));
	move(mnstrty+2, mnstrtx);
	addstr(_("results"));
	move(mnstrty+4, mnstrtx);
	addstr(_("control"));
	move(mnstrty+6, mnstrtx);
	addstr(_("language"));
	move(mnstrty+8, mnstrtx);
	addstr(_("exit"));
}

void menu_selection(int *choice)
{
	int key;
	int ch = 0;
	int pos = start_slct;
	show_menu();
	while(ch == 0) {
		key = getch();
		switch(key) {
		case KEY_UP:
			menu_key_up(&pos);
			break;
		case KEY_DOWN:
			menu_key_down(&pos);
			break;
		case key_enter:
			menu_key_enter(&pos, &ch);
			break;
		}
		curs_set(0);
	}
	*choice = ch;
	clear();
}

void show_control_page()
{
	render_permanent_background(KEYBINDINGSBCKG);
}

void create_border()
{
	render_permanent_background(BORDERSBCKG);
}

void show_results_page(const char *srcdir)
{
	FILE *check;
	check = fopen(srcdir, "r");
	if(!check)
		clear_results(srcdir);
	else
		fclose(check);
	render_permanent_background(srcdir);
}

void clear_results(const char *srcdir)
{
    FILE *empty;
    FILE *current;
    int i, j;
    int c;
    empty = fopen(RESEMPTY, "r");
	if(!empty) {
		perror(RESEMPTY);
		endwin();
		exit(1);	
	}
    current = fopen(srcdir, "w");
	if(!current) {
		perror(srcdir);
		endwin();
		exit(1);
	}
    for(i = 0; i < maxrow; ++i) {
        for(j = 0; j <= maxcol; ++j) {
            c = fgetc(empty);
            fputc(c, current);
        }
    }
    fclose(empty);
    fclose(current);
}

void menu_key_up(int *pos)
{
	switch(*pos) {
	case start_slct: 
		move(mnstrty, mnstrtx);
		addstr(_(" start "));
		move(mnstrty+8, mnstrtx);
		addstr(_(">EXIT<"));
		*pos = exit_slct;
		break;
	case results_slct: 
		move(mnstrty+2, mnstrtx);
		addstr(_(" results "));
		move(mnstrty, mnstrtx);
		addstr(_(">START<"));
		*pos = start_slct;	
		break;
	case control_slct:
		move(mnstrty+4, mnstrtx);
		addstr(_(" control "));
		move(mnstrty+2, mnstrtx);
		addstr(_(">RESULTS<"));
		*pos = results_slct;	
		break;
	case language_slct:
		move(mnstrty+6, mnstrtx);
		addstr(_(" language "));
		move(mnstrty+4, mnstrtx);
		addstr(_(">CONTROL<"));
		*pos = control_slct;	
		break;
	case exit_slct:
		move(mnstrty+8, mnstrtx);
		addstr(_(" exit "));
		move(mnstrty+6, mnstrtx);
		addstr(_(">LANGUAGE<"));
		*pos = language_slct;	
		break;
	}
}

void menu_key_down(int *pos)
{
	switch(*pos) {
	case start_slct:
		move(mnstrty, mnstrtx);
		addstr(_(" start "));
		move(mnstrty+2, mnstrtx);
		addstr(_(">RESULTS<"));
		*pos = results_slct;
		break;
	case results_slct:
		move(mnstrty+2, mnstrtx);
		addstr(_(" results "));
		move(mnstrty+4, mnstrtx);
		addstr(_(">CONTROL<"));
		*pos = control_slct;	
		break;
	case control_slct:
		move(mnstrty+4, mnstrtx);
		addstr(_(" control "));
		move(mnstrty+6, mnstrtx);
		addstr(_(">LANGUAGE<"));
		*pos = language_slct;	
		break;
	case language_slct:
		move(mnstrty+6, mnstrtx);
		addstr(_(" language "));
		move(mnstrty+8, mnstrtx);
		addstr(_(">EXIT<"));
		*pos = exit_slct;	
		break;
	case exit_slct:
		move(mnstrty+8, mnstrtx);
		addstr(_(" exit "));
		move(mnstrty, mnstrtx);
		addstr(_(">START<"));
		*pos = start_slct;	
		break;
	}
}

void menu_key_enter(int *pos, int *ch)
{
	int ex = 0;
	switch(*pos) {
	case start_slct:
		*ch = c_start; /*the game has started*/
		break;
	case results_slct:
		clear();
		results_page();
		*pos = start_slct;
		show_menu();
		break;
	case control_slct:
		clear();
		show_control_page();	
		do {
			;
		} while((ex = getch()) != key_enter); /*waiting for closing*/
		clear();
		*pos = start_slct;
		show_menu();
		break;
	case language_slct:
		break;
	case exit_slct:
		*ch = c_exit;
		break;
	}
}

void results_page()
{
	char srcdir[MAXPATHARRAY];
	char *homedir = getenv("HOME"); /*take home directories path*/
	int key;
	int y = 27;
	int x = 4;
	short pos = 0; /* can be 0 or 1*/
	short backslct = 0; /*also*/
	int i, j;
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
	show_results_page(srcdir);
	while(backslct != 1) {
		key = getch();
		switch(key) {
		case KEY_LEFT:
			if(pos == 0) {
				mvaddstr(y, x, CLEARSTRING);
				pos = 1;
			}
			else {
				mvaddstr(y, x, BACKSTRING);
				pos = 0;
			}
			refresh();
			break;
		case KEY_RIGHT:
			if(pos == 0) {
				mvaddstr(y, x, CLEARSTRING);
				pos = 1;
			}
			else {
				mvaddstr(y, x, BACKSTRING);
				pos = 0;
			}
			refresh();
			break;
		case key_enter:
			if(pos == 0) {
				backslct = 1;
				break;
			}
			else {
				clear_results(srcdir);
				pos = 0;
				show_results_page(srcdir);
			}
			refresh();
			break;
		}
	}
}
