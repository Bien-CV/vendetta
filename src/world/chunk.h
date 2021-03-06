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

#ifndef W_CHUNK_H
#define W_CHUNK_H

typedef struct chunk chunk_t;

#include <SFML/Graphics.h>

#include "object.h"
#include "world.h"
#include "mine.h"
#include "building.h"

#define TILE_SIZE 16
#define LAND(C,I,J) ((C)->lands[(I)*(C)->cols + (J)])

struct chunk
{
	object_t o;
	world_t* w;

	int rows;
	int cols;
	short* lands;

	int water_step;
	sfVertexArray* array; // TODO :(

	size_t n_mines;
	mine_t** mines;

	size_t n_buildings;
	building_t** buildings;
};

void chunk_init(chunk_t* c, world_t* w, float x, float y, int rows, int cols);
void chunk_exit(chunk_t* c);

void chunk_update(chunk_t* c);
void chunk_updwtr(chunk_t* c);

void chunk_pushMine(chunk_t* c, mine_t* m);

void chunk_pushBuilding(chunk_t* c, building_t* b);
void chunk_delBuilding (chunk_t* c, uuid_t uuid);

#endif
