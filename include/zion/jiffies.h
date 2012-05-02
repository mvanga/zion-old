#ifndef ZION_JIFFIES_H
#define ZION_JIFFIES_H

#include <zion/types.h>

#define HZ	1000

extern volatile uint32_t jiffies;

#define time_after(a,b)         ((long)(a) - (long)(b) > 0)
#define time_before(a,b)        time_after(b,a)
#define time_after_eq(a,b)      ((long)(a) - (long)(b) >= 0)
#define time_before_eq(a,b)     time_after_eq(b,a)

#define msecs_to_jiffies(x) \
	((x + (1000 / HZ) - 1) / (1000 / HZ))

#define jiffies_to_msecs(x) \
	((1000 / HZ) * x)

void delay(int msec);

#endif
