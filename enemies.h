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
#ifndef ENEMIES_H_SENTRY
#define ENEMIES_H_SENTRY

void enemy_preparation(const char *level, struct mpointer *mp);
void mv_rockfall(struct mpointer *mp, const char (*level));
void mv_beetles(struct mpointer *mp, const char (*level));
void add_rockfall(struct mpointer *mp, const int cur_y, const int cur_x);
void add_beetle(struct mpointer *mp, const int cur_y, const int cur_x);
void clear_enemies(struct mpointer *mp);

#endif
