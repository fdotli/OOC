# ifndef Ic_h
# define Ic_h

# include "Objct.h"

enum react { reject, accept };

extern const void * const Ic (void);

void wire (void * to, void * _self);
enum react gate (void * _self, const void * item);

extern const void * const IcClass (void);

extern const void * const Mux (void);

extern const void * const LineOut (void);

extern const void * const Button (void);

extern const void * const Event (void);

int kind (const void * _self);
void * data (const void * _self);

extern const void * const Calc (void);

# endif
