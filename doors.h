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
#ifndef DOORS_H_SENTRY
#define DOORS_H_SENTRY

void doors_preparation(const char *level, struct mpointer *mp);
void add_door(struct mpointer *mp, const int cur_y, const int cur_x);
void rm_door(struct mpointer *mp, const int cur_y, const int cur_x);
void doors_check(struct mpointer *mp);
void open_door(struct mpointer *mp, struct doors *door);
void add_key(struct mpointer *mp, const int cur_y, const int cur_x,
			 const char c);
void rm_key(struct mpointer *mp, const int cur_y, const int cur_x);
void keys_check(struct mpointer *mp);
void add_key_to_inventory(struct mpointer *mp, const char type);
void show_doors(const struct mpointer *mp);
void show_keys(const struct mpointer *mp);
void clear_doors(struct mpointer *mp);
short is_door(const struct mpointer *mp, const int cur_y, const int cur_x);
void clear_inventory(struct mpointer *mp);

#endif
