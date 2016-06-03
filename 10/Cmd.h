# ifndef Cmd_h
# define Cmd_h

# include "Object.h"

#include "Filter.h"

extern const void * const Cmd (void);

void flag (void * _self, void * filter, char flg);
int name (void * _self, const void * filter, const char * arg);

extern const void * const CmdClass (void);

# endif
