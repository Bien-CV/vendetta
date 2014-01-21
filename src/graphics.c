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

#include "graphics.h"

#include <stdlib.h>
#include <math.h>

#include "version.h"
#include "mem.h"
#include "string.h"

#define HT_MAX_TEXTURES (16*1024)

static unsigned int find(graphics_t* g, const char* str);

graphics_t* graphics_init(void)
{
	graphics_t* g = CALLOC(graphics_t, 1);

	sfVideoMode mode = {1024, 768, 32};
	g->render = sfRenderWindow_create(mode, "Vendetta " VERSION, sfResize | sfClose, NULL);
	if (g->render == NULL)
		exit(1);
	sfRenderWindow_setMouseCursorVisible(g->render, sfFalse);

	g->hasFocus = sfTrue;

	g->font = sfFont_createFromFile("DejaVuSans.ttf");
	if (g->font == NULL)
		exit(1);

	g->filenames = CALLOC(char*, HT_MAX_TEXTURES);
	memset(g->filenames, 0, sizeof(char*) * HT_MAX_TEXTURES);

	g->textures = CALLOC(sfTexture*, HT_MAX_TEXTURES);
	memset(g->textures, 0, sizeof(sfTexture*) * HT_MAX_TEXTURES);

	g->n_sprites = 0;
	g->a_sprites = 0,
	g->sprites = NULL;

	g->step = 0;
	g->fps  = 0;

	return g;
}

void graphics_exit(graphics_t* g)
{
	for (size_t i = 0; i < g->n_sprites; i++)
	{
		sfSprite_destroy(g->sprites[i]);
	}
	for (size_t i = 0; i < HT_MAX_TEXTURES; i++)
	{
		sfTexture_destroy(g->textures[i]);
		free(g->filenames[i]);
	}

	sfRenderWindow_destroy(g->render);
	free(g);
}

int graphics_spriteForImg(graphics_t* g, const char* filename)
{
	int id = find(g, filename);
	if (g->filenames[id] == NULL)
	{
		g->filenames[id] = strdup(filename);

		sfTexture* texture = sfTexture_createFromFile(filename, NULL);
		if (texture == NULL)
		{
			fprintf(stderr, "Failed to load texture '%s'\n", filename);
			exit(1);
		}

		g->textures[id] = texture;
	}

	sfSprite* sprite = sfSprite_create();
	sfSprite_setTexture(sprite, g->textures[id], sfTrue);

	if (g->n_sprites == g->a_sprites)
	{
		g->a_sprites = g->a_sprites ? 2*g->a_sprites : 1;
		g->sprites = CREALLOC(g->sprites, sfSprite*, g->a_sprites);
	}

	g->sprites[g->n_sprites] = sprite;
	return g->n_sprites++;
}

static unsigned int hash(const char* str)
{
	unsigned int h = 0x4e67c6a7;
	for (; *str; str++)
		h ^= (h << 5) + (*str) + (h >> 2);
	return h % HT_MAX_TEXTURES;
}
static unsigned int find(graphics_t* g, const char* str)
{
	unsigned int start = hash(str);
	unsigned int id = start;
	while (g->filenames[id] != NULL && strcmp(g->filenames[id], str) != 0)
	{
		id = (id+1) % HT_MAX_TEXTURES;
		if (id == start)
		{
			fprintf(stderr, "Too many textures\n");
			exit(1);
		}
	}
	return id;
}

void graphics_drawCursor(graphics_t* g, int t)
{
	sfVector2i imouse = sfMouse_getPositionRenderWindow(g->render);
	sfVector2f mouse = {imouse.x, imouse.y};

	static sfSprite* sprite = NULL;
	if (sprite == NULL)
	{
		int id = graphics_spriteForImg(g, "cursors.png");
		sprite = g->sprites[id];
	}

	sfIntRect rect = {24*t, 0, 24, 24};
	sfSprite_setTextureRect(sprite, rect);

	sfSprite_setPosition(sprite, mouse);
	sfRenderWindow_drawSprite(g->render, sprite, NULL);
}

void graphics_drawTooltip(graphics_t* g, const char* txt)
{
	sfVector2i imouse = sfMouse_getPositionRenderWindow(g->render);
	sfVector2f mouse = {imouse.x, imouse.y};

	static sfText* text = NULL;
	static sfRectangleShape* frame = NULL;
	if (text == NULL)
	{
		text = sfText_create();
		sfText_setFont         (text, g->font);
		sfText_setCharacterSize(text, 15);
		sfText_setColor        (text, sfBlack);

		frame = sfRectangleShape_create();
		sfRectangleShape_setFillColor(frame, (sfColor){255,255,255,223});
		sfRectangleShape_setOutlineColor(frame, sfBlack);
		sfRectangleShape_setOutlineThickness(frame, 1);
	}

	sfVector2f pos = {mouse.x + 24, mouse.y + 24};
	sfText_setPosition(text, pos);
	sfText_setUTF8(text, txt);

	sfFloatRect rect = sfText_getGlobalBounds(text);
	rect.left   -= 3;
	rect.top    -= 3;
	rect.width  += 8;
	rect.height += 8;

	sfVector2u size = sfRenderWindow_getSize(g->render);
	float dx = fmin(0, size.x - (rect.left+rect.width ));
	float dy = fmin(0, size.y - (rect.top +rect.height));
	pos.x += dx;
	pos.y += dy;
	rect.left += dx;
	rect.top  += dy;
	sfText_setPosition(text, pos);

	sfRectangleShape_setPosition(frame, (sfVector2f){rect.left,rect.top});
	sfRectangleShape_setSize    (frame, (sfVector2f){rect.width,rect.height});
	sfRenderWindow_drawRectangleShape(g->render, frame, NULL);

	sfRenderWindow_drawText(g->render, text, NULL);
}

void graphics_drawProgressBar(graphics_t* g, float x, float y, float w, float h, float p, char c)
{
	static sfRectangleShape* frame    = NULL;
	static sfRectangleShape* progress = NULL;
	if (frame == NULL)
	{
		frame = sfRectangleShape_create();
		sfRectangleShape_setFillColor(frame, sfTransparent);

		progress = sfRectangleShape_create();
	}

	float border = 1 + floor(h/20);
	sfRectangleShape_setOutlineThickness(frame, border);
	sfColor orange = {255, 42, 42, 255};
	sfRectangleShape_setOutlineColor(frame, c == 1 ? orange : sfWhite);

	sfColor inner;
	     if (c < 0)     inner = (sfColor){255,255,255,191};
	else if (p <= 0.25) inner = (sfColor){255,  0,  0,191};
	else if (p <= 0.50) inner = (sfColor){247,173,  0,191};
	else if (p <= 0.75) inner = (sfColor){170,170, 68,191};
	else if (p <= 1.00) inner = (sfColor){ 68,255, 68,191};
	else                inner = (sfColor){  0,  0,255,191};

	p = fmax(fmin(p, 1), 0);

	sfRectangleShape_setFillColor(progress, inner);

	sfVector2f pos = {x+border,y+border};
	sfRectangleShape_setPosition(frame, pos);
	sfRectangleShape_setPosition(progress, pos);

	sfVector2f size = {w-2*border, h-2*border};
	sfRectangleShape_setSize(frame, size);

	size.x *= p;
	sfRectangleShape_setSize(progress, size);
	sfRenderWindow_drawRectangleShape(g->render, progress, NULL);
	sfRenderWindow_drawRectangleShape(g->render, frame, NULL);
}

void graphics_drawScrollBar(graphics_t* g, float x, float y, float w, float h, float r, float p)
{
	static sfRectangleShape* cursor = NULL;
	if (cursor == NULL)
	{
		cursor = sfRectangleShape_create();
		sfRectangleShape_setFillColor(cursor, (sfColor){70,70,102,127});
	}

	r = fmin(r, 1);
	if (r != 1)
		y += p * h * (1-r);
	h *= r;
	sfRectangleShape_setSize(cursor, (sfVector2f){w,h});
	sfRectangleShape_setPosition(cursor, (sfVector2f){x,y});
	sfRenderWindow_drawRectangleShape(g->render, cursor, NULL);
}

#ifdef __WIN32__
#include <windows.h>
#include <winnls.h>
#endif
void sfText_setUTF8(sfText* text, const char* string)
{
	sfUint32 buf32[1024];
#ifdef __WIN32__
	wchar_t buf16[1024];
	MultiByteToWideChar(CP_UTF8, 0, string, -1, buf16, 1024);
	size_t n = wcslen(buf16)+1;
	for (size_t i = 0; i < n; i++)
		buf32[i] = buf16[i];
#else
	mbstowcs((wchar_t*) buf32, string, 1024);
#endif
	sfText_setUnicodeString(text, buf32);
}

char sfText_contains(sfText* text, sfVector2f point)
{
	sfFloatRect rect = sfText_getGlobalBounds(text);
	return sfFloatRect_contains(&rect, point.x, point.y);
}

char sfSprite_contains(sfSprite* sprite, sfVector2f point)
{
	sfFloatRect rect = sfSprite_getGlobalBounds(sprite);
	return sfFloatRect_contains(&rect, point.x, point.y);
}
