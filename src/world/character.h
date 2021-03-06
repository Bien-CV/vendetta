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

#ifndef W_CHARACTER_H
#define W_CHARACTER_H

typedef struct character character_t;

#include "inventory.h"
#include "skill.h"
#include "status.h"
#include "object.h"
#include "world.h"
#include "../universe/character.h"

struct character
{
	object_t o;
	world_t* w;
	kindOf_character_t* t;

	char alive;

	float  go_x;
	float  go_y;
	uuid_t go_o;
	char attack;

	direction_t dir;
	float       step;
	float       attackDelay;
	char        inWater;

	ai_t*     ai;
	ai_data_t ai_data;

	inventory_t inventory;
	uuid_t hasBuilding;
	uuid_t inBuilding;

	skill_t* skills;
	status_t statuses[N_STATUSES];

	int* equipment;
};

#include "../universe/material.h"
#include "../universe/mine.h"
#include "../universe/building.h"
#include "building.h"

void character_init(character_t* c, world_t* w, kindOf_character_t* t);
void character_exit(character_t* c);

float character_vitality     (character_t* c);
float character_getSkill     (character_t* c, int skill);
float character_maxOfStatus  (character_t* c, int s);
float character_maxOfMaterial(character_t* c, kindOf_material_t* m);
float character_armor        (character_t* c);

void character_addStatus(character_t* c, int s, float q);
void character_weary    (character_t* c, float f);
void character_train    (character_t* c, int skill, float work);

void character_stop  (character_t* c);
void character_move  (character_t* c, float dx, float dy);
void character_goAt  (character_t* c, float  x, float  y);
void character_goto  (character_t* c, uuid_t uuid);
void character_attack(character_t* c, uuid_t uuid);
char character_eat   (character_t* c, int material);
void character_eatFor(character_t* c, int status);

void  character_setPosition(character_t* c, float x, float y);
float character_attacked   (character_t* c, float work);

void character_goMine   (character_t* c, kindOf_mine_t* t);
char character_buildAuto(character_t* c, kindOf_building_t* t);
char character_buildAt  (character_t* c, kindOf_building_t* t, float x, float y);
char character_delHome  (character_t* c);

building_t* character_get_inBuilding (character_t* c);
building_t* character_get_hasBuilding(character_t* c);

size_t character_currentAction(character_t* c, char* buffer, size_t n);

// character_round.c
void character_doWork  (character_t* c, object_t* o, float duration);
char character_doAttack(character_t* c, object_t* o);
void character_doMove  (character_t* c, float duration, float dx, float dy);
void character_doRound (character_t* c, float duration);

#endif
