/*\
 *  Role playing, management and strategy game
 *  Copyright (C) 2013-2014 Quentin SANTOS
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
\*/

#ifndef W_EVENT_H
#define W_EVENT_H

typedef struct event   event_t;
typedef struct evtList evtList_t;

#include "../universe/event.h"

struct event
{
	kindOf_event_t* t;
	float x;
	float y;
	float p;

	sfSound* sound; // TODO
};

struct evtList
{
	size_t n;
	event_t* d;
};

void evtList_init(evtList_t* l);
void evtList_exit(evtList_t* l);

void evtList_push(evtList_t* l, kindOf_event_t* t, float x, float y);

void evtList_doRound(evtList_t* l, float duration);

#endif
