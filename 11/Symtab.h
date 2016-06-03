# ifndef Symtab_h
# define Symtab_h

# include "Object.h"

#include <stddef.h>

extern const void * const Symtab (void);

void install (void * _self, const void * entry);
struct Symbol * screen (void * _self, const char * name, int lex);

# endif
