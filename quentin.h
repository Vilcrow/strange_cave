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

#ifndef QUENTIN_H_SENTRY
#define QUENTIN_H_SENTY

void create_struct(struct mpointer **mp);
void change_body(struct mpointer *mp, const int q_condition);
void show_quentin(const struct mpointer *mp, const char (*level));
void move_quentin(struct mpointer *mp, const char (*level), const int q_condition,
				  const short dy, const short dx);
void jump(struct mpointer *mp, const char (*level), const int q_condition);
void fall(struct mpointer *mp, const char (*level), const int q_condition);
void quentin_control(struct mpointer *mp, const char (*level));
void case_key_up(struct mpointer *mp, const char (*level));
void case_key_down(struct mpointer *mp, const char (*level));
void case_key_left(struct mpointer *mp, const char (*level));
void case_key_right(struct mpointer *mp, const char (*level));
void case_key_escape(const struct mpointer *mp);
void case_key_space();
void case_err(struct mpointer *mp, const char (*level));

#endif
