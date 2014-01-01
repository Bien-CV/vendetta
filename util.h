#ifndef UTIL_H
#define UTIL_H

static inline float max(float a, float b)
{
	return a > b ? a : b;
}

#include <stdlib.h>
#include <stdio.h>
static inline void* check_alloc(size_t n, void* ptr, const char* file, int line)
{
	void* ret = realloc(ptr, n);
	if (ret == NULL)
	{
		fprintf(stderr, "Could not allocate memory at '%s' line %i\n", file, line);
		exit(1);
	}
	return ret;
}
#define CALLOC(T,N)     ( (T*) check_alloc((N)*sizeof(T), NULL, __FILE__, __LINE__) )
#define CREALLOC(P,T,N) ( (T*) check_alloc((N)*sizeof(T), P,    __FILE__, __LINE__) )

#include <wchar.h>
static inline wchar_t* strdupwcs(const char* str)
{
	wchar_t buffer[1024];
	swprintf(buffer, 1024, L"%s", str);
	return wcsdup(buffer);
}

#endif
