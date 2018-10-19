#ifndef BOOL_H
#define BOOL_H

// This file is to bridge the gap of the bool type that our compiler supports,
// but the typical C compiler does not.

#define bool int
#define true 1
#define false 0

#endif // BOOL_H
