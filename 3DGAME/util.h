/***********************************************
 * [util.h]
 * ½yºÏévÊý
 * GP11A341 24 ˆñYòG
 ***********************************************/

#ifndef __UTIL_H__
#define __UTIL_H__

#define SAFE_RELEASE(p)			{ if(p) { (p)->Release(); p = NULL; } }
#define SAFE_DELETE(p)			{ if(p) { delete p; p = NULL; } }
#define FREE(p)					{ if(p) { free(p); p = NULL; } }
#define GET_RANDOM(a, b)		((a) + rand() / (RAND_MAX / ((b) - (a) + 1) + 1))
#define DICE(n)					(rand() < (RAND_MAX + 1u) / (n))

#endif // !__UTIL_H__
