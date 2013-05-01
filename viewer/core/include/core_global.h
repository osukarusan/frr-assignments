// See Qt Reference documentation, "Creating Shared Libraries"
#ifndef _CORE_GLOBAL_H_
#define _CORE_GLOBAL_H_

#include <QtCore/QtGlobal>

#if defined(CORE_LIBRARY)
#  define CORE_EXPORT Q_DECL_EXPORT
#else
#  define CORE_EXPORT Q_DECL_IMPORT
#endif

#define PI	3.14159265358979f   //3.14159265358979323846
#define	DEG2RAD PI/180.0f
#define RAD2DEG 180.0f/PI

#define TOLERANCE	 1.0e-9

#define ABS(a)      (((a)<0) ? -(a) : (a))
#define SIGN(a)     (((a)<0) ? -1 : 1)
#define SWAP(a,b)   { a^=b; b^=a; a^=b; }
#define SQR(a)      ((a)*(a))

// Finds the smallest power of 2 greater or equal to x.
inline unsigned int pow2roundup(unsigned int x)
{
  --x;
  x |= x >> 1;
  x |= x >> 2;
  x |= x >> 4;
  x |= x >> 8;
  x |= x >> 16;
  return x+1;
}

#endif

