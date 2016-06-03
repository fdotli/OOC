# ifndef Object_h
# define Object_h

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

extern const void * Object;

void delete (void * _self);
const void * classOf (const void * _self);
size_t sizeOf (const void * _self);

void * ctor (void * _self, va_list * app);
void * dtor (void * _self);
int differ (const void * _self, const void * b);
int puto (const void * _self, FILE * fp);

extern const void * Class;

struct Object * new (const void * _self, ...);
const void * super (const void * _self);

# endif
