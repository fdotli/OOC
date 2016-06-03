# ifndef Crt_h
# define Crt_h

# include "Ic.h"

extern const void * const Crt (void);

void makeWindow (void * _self, int rows, int cols, int y, int x);
void addStr (void * _self, int y, int x, const char * s);
void crtBox (void * _self);

extern const void * const CLineOut (void);

extern const void * const CButton (void);

# endif
