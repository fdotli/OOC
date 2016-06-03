# ifndef List_h
# define List_h

# include "Objct.h"

extern const void * const List (void);

struct Objct * addFirst (void * _self, const void * element);
struct Objct * addLast (void * _self, const void * element);
unsigned count (const void * _self);
struct Objct * lookAt (const void * _self, unsigned n);
struct Objct * takeFirst (void * _self);
struct Objct * takeLast (void * _self);

struct Objct * add (void * _self, const void * element);
struct Objct * take (void * _self);

extern const void * const ListClass (void);

# endif
