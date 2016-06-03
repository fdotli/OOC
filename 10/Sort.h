# ifndef Sort_h
# define Sort_h

# include "List.h"

extern const void * const Sort (void);

void flags (void * _self, void * filter, char flag);
int line (void * _self, const void * filter, const char * fnm, char * buf);
int quit (void * _self, const void * filter);

extern const void * const SortClass (void);

# endif
