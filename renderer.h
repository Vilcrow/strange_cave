/*
Strange Cave game

Copyright (C) 2022-2023-2023 S.V.I 'Vilcrow', <svi@vilcrow.net>
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

#ifndef RENDERER_H_SENTRY
#define RENDERER_H_SENTRY

void render_permanent_background(const char *name);
char environment_type(const int next_y, const int next_x, const char (*level));
void show_environment(const char (*level), const struct mpointer *mp);
void hide_environment(const struct mpointer *mp);
void render_borders(const char *name);
void map_preparation(const char *source, char *temp, char (**level));
void map_renderer(const char *(level));
void add_point(struct mpointer *mp, const int cur_y, const int cur_x, const char type);
void rm_point(struct mpointer *mp);
void show_points(const struct mpointer *mp);
void update_points_counter(struct mpointer *mp, const char ptype);
void update_health_counter(struct mpointer *mp, const short shift);
short visibility(const struct mpointer *mp, const int cur_y, const int cur_x);
void show_counter(const struct mpointer *mp);
void show_inventory(const struct mpointer *mp);
void render_win();
void show_level_num(const short lev_num);

#endif
