# ifndef Symbol_h
# define Symbol_h

# include "Object.h"

extern const void * const Symbol (void);

const char * name (const void * _self);
int lex (const void * _self);

extern const void * const Reserved (void);

extern const void * const Var (void);

double value (const void * _self);
double setvalue (void * _self, double value);

void move (void * _self, void * _from);

extern const void * const VarClass (void);

extern const void * const Const (void);

extern const void * const Fun (void);

void setfun (void * _self, void * fun);
double funvalue (void * _self, double value);

typedef double (* function) (double);

extern const void * const Math (void);

double mathvalue (const void * _self, double value);

# endif
