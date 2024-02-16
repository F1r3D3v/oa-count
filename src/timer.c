#include "timer.h"
#include "macro.h"
#include "helper.h"
#include <time.h>

clock_t start = 0, end = 0;
double diff = 0.0;
BOOL enabled = FALSE;

void StartTimer()
{
	if (enabled) return;
	enabled = TRUE;
	setTimeExceed(FALSE);
	start = clock();
}

void StopTimer()
{
	if (!enabled) return;
	end = clock();
	diff = (((double)end - start) / CLOCKS_PER_SEC);
	enabled = FALSE;
}

void ResetTimer()
{
	start = 0;
	end = 0;
	diff = 0;
	enabled = FALSE;
}

double getDuration()
{
	if (!enabled) return 0.0;
	end = clock();
	return (((double)end - start) / CLOCKS_PER_SEC);
}

double getTime()
{
	return diff;
}