# ifndef Point_h
# define Point_h

# include "Object.h"

extern const void * const Point (void);

void move (void * _self, int dx, int dy);

void draw (const void * _self);

extern const void * const PointClass (void);

# endif
