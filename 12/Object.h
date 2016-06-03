# ifndef Object_h
# define Object_h

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

typedef void (* Method) ();
void * retrieve (FILE * fp);

extern const void * const Object (void);

const struct Class * classOf (const void * _self);
size_t sizeOf (const void * _self);
int isA (const void * _self, const void * class);
int isOf (const void * _self, const void * class);
void * cast (const void * class, const void * _self);
Method respondsTo (const void * _self, const char * tag);

void * ctor (void * _self, va_list * app);
void * dtor (void * _self);
int puto (const void * _self, FILE * fp);
void delete (void * _self);
void * geto (void * _self, FILE * fp);

struct Object * new (const void * _self, ...);

extern const void * const Class (void);

struct Object * allocate (const void * _self);
const struct Class * super (const void * _self);
const char * nameOf (const void * _self);

# endif
