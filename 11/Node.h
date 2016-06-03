# ifndef Node_h
# define Node_h

# include "Object.h"

extern const void * const Node (void);

void sunder (void * _self);

double exec (const void * _self);

extern const void * const NodeClass (void);

void reclaim (const void * _self, Method how);

extern const void * const Monad (void);

extern const void * const Dyad (void);

extern const void * const Number (void);

extern const void * const Val (void);

extern const void * const Global (void);

extern const void * const Parm (void);

extern const void * const Unary (void);

extern const void * const Minus (void);

extern const void * const Ref (void);

extern const void * const Assign (void);

extern const void * const Builtin (void);

extern const void * const User (void);

extern const void * const Binary (void);

extern const void * const Add (void);

extern const void * const Sub (void);

extern const void * const Mult (void);

extern const void * const Div (void);

# endif
