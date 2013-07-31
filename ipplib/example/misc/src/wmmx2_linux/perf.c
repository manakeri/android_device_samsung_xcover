/***************************************************************************************** 
Copyright (c) 2009, Marvell International Ltd. 
All Rights Reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Marvell nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY MARVELL ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL MARVELL BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************************/


#include "misc.h"
#include <sys/time.h>
#include <sys/syscall.h>
#include <sys/resource.h>
#include <time.h>
#ifndef ANDROID
#include <sys/unistd.h>
#endif
int g_Frame_Num[2];
long long g_Tot_Time[2];
long long g_Tot_DeviceTime[2];
long long g_Tot_CPUTime[2];
/////////////////////////////////////////////////////////////////////////////////
// Part 2 Performance test implementation under Linux OS
/////////////////////////////////////////////////////////////////////////////////

/* individual data structure */
typedef struct {
	int available;
	long long start_time;
	long long accumulated_time;
	IPP_COUNTER_FUNC pStart;
	IPP_COUNTER_FUNC pStop;
} IPP_Counter_Info;

/* performance counter array */
static IPP_Counter_Info g_perf_counter[MAX_PERFORMANCE_INDEX];

/* Initialize for performance counter */
void IPP_InitPerfCounter()
{
	int i;
	for (i=0; i<MAX_PERFORMANCE_INDEX; i++)
	{
		g_perf_counter[i].available = 1;
		g_perf_counter[i].pStart = IPP_TimeGetTickCount;
		g_perf_counter[i].pStop  = IPP_TimeGetTickCount;
	}

	g_Frame_Num[IPP_AUDIO_INDEX] = 0;
	g_Frame_Num[IPP_VIDEO_INDEX] = 0;
	g_Tot_Time[IPP_AUDIO_INDEX] = 0;
	g_Tot_Time[IPP_VIDEO_INDEX] = 0;
	g_Tot_DeviceTime[IPP_AUDIO_INDEX] = 0;
	g_Tot_DeviceTime[IPP_VIDEO_INDEX] = 0;

}

/* Get an index for performance counting,return -1 means fail */
void IPP_GetPerfCounter(int* index, IPP_COUNTER_FUNC pStart, IPP_COUNTER_FUNC pStop)
{
	int i;
	for (i=0; i<MAX_PERFORMANCE_INDEX; i++)
	{
		if (g_perf_counter[i].available == 1) {
			*index = i;
			g_perf_counter[i].available = 0;
			if(pStart) {
				g_perf_counter[i].pStart = pStart;
			} else {
				g_perf_counter[i].pStart = IPP_TimeGetTickCount;
			}
			if(pStop) {
				g_perf_counter[i].pStop  = pStop;
			} else {
				g_perf_counter[i].pStop  = IPP_TimeGetTickCount;
			}
			return;
		}
	}

    /* no available counter */
	*index = -1;
}

/* Release an index for performance counting */
void IPP_FreePerfCounter(int index)
{
	g_perf_counter[index].available = 1;
}

/* Reset the specified performance counter to 0 */
void IPP_ResetPerfCounter(int index)
{
	g_perf_counter[index].accumulated_time = 0;
}

/* Start performance counting for specified counter */
void IPP_StartPerfCounter(int index)
{
	g_perf_counter[index].start_time = g_perf_counter[index].pStart();
}

/* Stop performance counting for specified counter */
void IPP_StopPerfCounter(int index)
{
	g_perf_counter[index].accumulated_time += g_perf_counter[index].pStop() - g_perf_counter[index].start_time;
}

/* Get the performance value from specified counter */
long long IPP_GetPerfData(int index)
{
	return g_perf_counter[index].accumulated_time;
}

/* DeInitialize for performance counter */
void IPP_DeinitPerfCounter()
{
    /*nothing to do */
}
long long IPP_TimeGetTickCount()
{
	struct timeval g_tv;
	struct timezone g_tz;
	gettimeofday(&g_tv, &g_tz);
	return (long long)g_tv.tv_sec * 1000000 + g_tv.tv_usec;
}

#ifndef ANDROID
#define getrusage(a,b) syscall(SYS_getrusage,a,b)
#endif

long long IPP_TimeGetThreadTime()
{
#if 0
	struct rusage r_usage;
	getrusage(RUSAGE_SELF, &r_usage);
	return r_usage.ru_utime.tv_sec * 1000000 + r_usage.ru_utime.tv_usec + r_usage.ru_stime.tv_sec * 1000000 + r_usage.ru_stime.tv_usec;
#else
    struct timespec t;
    int err;
#ifdef ANDROID
    err = clock_gettime(CLOCK_THREAD_CPUTIME_ID,&t);
#else
	if (sysconf(_POSIX_THREAD_CPUTIME)){
      err = clock_gettime(CLOCK_THREAD_CPUTIME_ID,&t);
    }
#endif

    return t.tv_sec*1000000 + t.tv_nsec/1000;
#endif
}
