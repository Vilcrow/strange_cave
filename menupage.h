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

#ifndef MENUPAGE_H_SENTRY
#define MENUPAGE_H_SENTRY

void show_menu();
void menu_selection(int *choice);
void show_control_page();
void create_border();
void show_results_page(const char *srcdir);
void clear_results(const char *srcdir);
void menu_key_up(int *pos);
void menu_key_down(int *pos);
void menu_key_enter(int *pos, int *ch);
void results_page();

#endif
