#include <zion/jiffies.h>
#include <zion/types.h>

volatile uint32_t jiffies = 0;

void delay(int msec)
{
	uint32_t delay = jiffies + msecs_to_jiffies(msec);
	while (time_before(jiffies, delay));
}

