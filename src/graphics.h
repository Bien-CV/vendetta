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

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SFML/Graphics.h>

typedef struct graphics graphics_t;

struct graphics
{
	sfRenderWindow* render;
	sfBool          hasFocus;
	sfFont*         font;

	sfView* world_view;
	sfView* overlay_view;

	char**      filenames;
	sfTexture** textures;

	size_t     n_sprites;
	size_t     a_sprites;
	sfSprite** sprites;

	float step;
	float fps;
};

void graphics_init(graphics_t* g);
void graphics_exit(graphics_t* g);

sfTexture* graphics_loadImage(graphics_t* g, const char* filename);
int        graphics_spriteId (graphics_t* g, const char* filename);
sfSprite*  graphics_sprite   (graphics_t* g, const char* filename);

void graphics_drawRoundRect  (graphics_t* g, float x, float y, float w, float h);
void graphics_drawCursor     (graphics_t* g, int t);
void graphics_drawTooltip    (graphics_t* g, const char* txt);
void graphics_drawProgressBar(graphics_t* g, float x, float y, float w, float h, float p, char c);
void graphics_drawScrollBar  (graphics_t* g, float x, float y, float w, float h, float r, float p);

void sfText_setUTF8(sfText* text, const char* string);

char sfText_contains  (sfText*   text,   sfVector2f point);
char sfSprite_contains(sfSprite* sprite, sfVector2f point);

#endif
