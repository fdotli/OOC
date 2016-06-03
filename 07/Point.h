# ifndef Point_h
# define Point_h

# include "Object.h"

extern const void * Point;

void move (void * _self, int dx, int dy);

void draw (const void * _self);

extern const void * PointClass;

void initPoint (void);

# endif
