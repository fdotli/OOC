# ifndef Wc_h
# define Wc_h

# include "Object.h"

extern const void * const Wc (void);

int wc (void * _self, const void * filter, const char * fnm, char * buf);
int printFile (void * _self, const void * filter, const char * fnm);
int printTotal (void * _self, const void * filter);

extern const void * const WcClass (void);

# endif
