# ifndef List_h
# define List_h

# include "Object.h"

extern const void * List;

struct Object * addFirst (void * _self, const void * element);
struct Object * addLast (void * _self, const void * element);
unsigned count (const void * _self);
struct Object * lookAt (const void * _self, unsigned n);
struct Object * takeFirst (void * _self);
struct Object * takeLast (void * _self);

struct Object * add (void * _self, const void * element);
struct Object * take (void * _self);

extern const void * ListClass;

void initList (void);

# endif
