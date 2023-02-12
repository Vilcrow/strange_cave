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

#ifndef GAME_H_SENTRY
#define GAME_H_SENTRY

void game();
void game_flow(struct mpointer *mp, const char (*level));
void clear_items(struct mpointer *mp);
void preparate_new_level(struct mpointer *mp, const int cur_y, const int cur_x);
void level(struct mpointer *mp, const char *source, char *temp,
           const short lev_num);
void finish_game(const struct mpointer *mp);
void game_over(const struct mpointer *mp);

#endif
