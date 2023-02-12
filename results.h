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

#ifndef RESULTS_H_SENTRY
#define RESULTS_H_SENTRY

void update_results(const char *usrname, const int points);
short isbckg(const int cur_y, const int cur_x);
void prev_results(struct result **res);
void preparate_res_temp(char *srcdir);
void place_result(struct result **res, struct result *newres);
void update_result_file(const struct result *res, char *srcdir);
void rewrite_res_file(const char *srcdir, char *dstdir);
void ask_user_name(char *usrname);

#endif
