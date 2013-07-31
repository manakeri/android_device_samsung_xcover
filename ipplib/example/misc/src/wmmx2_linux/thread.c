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
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <pthread.h>
#include <sched.h>
#include <stdarg.h>
#include <sys/time.h>
#include <sys/syscall.h>
#ifndef ANDROID
#include <sys/unistd.h>
#endif
#include <sys/resource.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

/**********************************************************************
 * Thread
 ***********************************************************************/
int IPP_ThreadCreate(void *phThread, int nPriority, void *pFunc, void *pParam)
{
	pthread_attr_t          attr;
	struct sched_param      param;

	if(!phThread || !pFunc || !pParam) {
		return -1;
	}
	if(nPriority) {
		pthread_attr_init(&attr);
		pthread_attr_getschedparam(&attr, &param);
		param.sched_priority=nPriority;
		pthread_attr_setschedparam(&attr, &param);
		return pthread_create((pthread_t*)phThread, &attr, pFunc, pParam);
	} else {
		return pthread_create((pthread_t*)phThread, NULL, pFunc, pParam);
	}
}

int IPP_ThreadDestroy(void *phThread, int bWait)
{
	if((!phThread) || !(*((pthread_t*)phThread)) ) {
		return -1;
	}
	if(bWait) {
		return pthread_join(*((pthread_t*)phThread), NULL);
	} else {
#ifndef ANDROID
		return pthread_cancel(*((pthread_t*)phThread));
#else
		//Android does not have pthread_cancel, so do not care bWait flag
		return pthread_join(*((pthread_t*)phThread), NULL);
#endif
	}
}

void IPP_ThreadExit(void *ret)
{
	pthread_detach(pthread_self());
	pthread_exit(ret);
}

typedef pthread_t IPP_ThreadData;

int IPP_ThreadCreateEx( IppThread *hThread, IppThreadAttribute *pThreadAttribute, void *pFunc, void *pParam )
{
	pthread_t  *pThreadId = (pthread_t*)hThread;
	int        ret;

	if( !pFunc )
	{
		return -1;
	}
	if( !pThreadAttribute )
	{
		pthread_attr_t     attr;
		struct sched_param param;
		int                policy;
		int                detachstate;

		pthread_attr_init( &attr );

		// set policy
		if( pThreadAttribute->policy == IPP_THREAD_POLICY_RR )
		{
			policy = SCHED_RR;
		}
		else if( pThreadAttribute->policy == IPP_THREAD_POLICY_FIFO )
		{
			policy = SCHED_FIFO;
		}
		else if( pThreadAttribute->policy == IPP_THREAD_POLICY_OTHER )
		{
			policy = SCHED_OTHER;
		}
		else
		{
			return -1;
		}
		pthread_attr_setschedpolicy( &attr, policy );

		// set priority
		pthread_attr_getschedparam( &attr, &param );
		param.sched_priority = pThreadAttribute->priority;
		pthread_attr_setschedparam( &attr, &param );

		// set detatch state
		if( pThreadAttribute->detachstate == IPP_THREAD_CREATE_DETACHED )
		{
			detachstate = PTHREAD_CREATE_DETACHED;
		}
		else if( pThreadAttribute->detachstate == IPP_THREAD_CREATE_JOINABLE )
		{
			detachstate = PTHREAD_CREATE_JOINABLE;
		}
		else
		{
			return -1;
		}
		pthread_attr_setdetachstate( &attr, detachstate );

		ret = pthread_create( pThreadId, &attr, pFunc, pParam );
		pthread_attr_destroy( &attr );

		return ret;
	}
	else
	{
		return pthread_create( pThreadId, NULL, pFunc, pParam );
	}
}


int IPP_ThreadDestroyEx( IppThread *hThread, int bWait )
{
	int        ret        = 0;
	pthread_t  *pThreadId = (pthread_t*)hThread;

	if( !pThreadId )
	{
		return -1;
	}
	if( bWait )
	{
		ret = pthread_join( *pThreadId, NULL );
	}
	else
	{
#ifndef ANDROID
		ret = pthread_cancel( *pThreadId );
#else
		//Android does not have pthread_cancel, so do not care bWait flag
		ret = pthread_join( *pThreadId, NULL );
#endif
	}

	if ( ret == 0 )
	{
		*pThreadId = -1;
	}

	return ret;
}

int IPP_ThreadSetAttribute( IppThread thread, IppThreadAttribute *pThreadAttribute )
{
	struct sched_param param;
	int                policy;
	int                ret;
	pthread_t          thread_id = (pthread_t)thread;

	if ( !pThreadAttribute )
	{
		return -1;
	}
	// get schedule parameters
	ret = pthread_getschedparam( thread_id, &policy, &param );
 	if ( ret != 0 )
	{
		return -1;
	}

	// set params
	if ( pThreadAttribute->policy == IPP_THREAD_POLICY_RR )
	{
		policy = SCHED_RR;
	}
	else if ( pThreadAttribute->policy == IPP_THREAD_POLICY_FIFO )
	{
		policy = SCHED_FIFO;
	}
	else if ( pThreadAttribute->policy == IPP_THREAD_POLICY_OTHER )
	{
		policy = SCHED_OTHER;
	}
	else
	{
		return -1;
	}
	param.sched_priority=pThreadAttribute->priority;

	ret = pthread_setschedparam( thread_id, policy, &param );

	return ret;
}

int IPP_ThreadGetAttribute( IppThread thread, IppThreadAttribute *pThreadAttribute )
{
	struct sched_param param;
	int                policy;
	int                ret;
	pthread_t          thread_id = (pthread_t)thread;

	if ( !pThreadAttribute )
	{
		return -1;
	}

	// get schedule parameters
	ret = pthread_getschedparam( thread_id, &policy, &param );
	if ( ret != 0 )
	{
		return -1;
	}

	if ( policy == SCHED_RR )
	{
		pThreadAttribute->policy = IPP_THREAD_POLICY_RR;
	}
	else if ( policy == SCHED_FIFO )
	{
		pThreadAttribute->policy = IPP_THREAD_POLICY_FIFO;
	}
	else if ( policy == SCHED_OTHER )
	{
		pThreadAttribute->policy = IPP_THREAD_POLICY_OTHER;
	}

	pThreadAttribute->priority = param.sched_priority;

	return 0;

}


/** Returns a time value in 10^(-6) sec based on a clock starting at
 *  some arbitrary base. Given a call to GetTime that returns a value
 *  of n a subsequent call to GetTime made m milliseconds later should 
 *  return a value of (approximately) (n+m). This method is used, for
 *  instance, to compute the duration of call. */
int IPP_Sleep( Ipp64u uSec )
{
	int sec;
	struct timespec t, t_remain;

	sec = (int)( uSec / 1000000 );
	t.tv_sec = 0;
    t.tv_nsec = (uSec % 1000000) * 1000;

	if (sec > 0) sleep(sec);
	nanosleep(&t, &t_remain);
	 
	return 0;
}


/**********************************************************************
 * Mutex
 **********************************************************************/
int IPP_MutexCreate( void* *phMutex)
{
	static pthread_mutexattr_t oAttr;
	static pthread_mutexattr_t *pAttr = NULL;
	pthread_mutex_t *pMutex;

	IPP_MemMalloc(&pMutex, sizeof(pthread_mutex_t), 8);

	if ((NULL == pAttr) && !pthread_mutexattr_init(&oAttr)
			&& !pthread_mutexattr_settype(&oAttr, PTHREAD_MUTEX_RECURSIVE)) {
		pAttr = &oAttr;
	}

	if ( (NULL== pMutex) || pthread_mutex_init(pMutex, pAttr)) {
		IPP_MemFree(&pMutex);
		return -1;
	}
	*phMutex = (void *)pMutex;
	return 0;

}


int IPP_MutexDestroy( void* hMutex)
{
	pthread_mutex_t *pMutex = (pthread_mutex_t *)hMutex;
	if ((NULL == pMutex) || pthread_mutex_destroy(pMutex)) {
		return -1;
	}    
	IPP_MemFree(&pMutex);
	return 0;
}

int IPP_MutexLock( void* hMutex,  unsigned int mSec,  int *pbTimedOut)
{
	int err;
	pthread_mutex_t *pMutex = (pthread_mutex_t *)hMutex;
	
	if (NULL != pbTimedOut)
		*pbTimedOut = 0;
	if (NULL == pMutex)
		return -1;
	err = pthread_mutex_lock(pMutex);
	switch (err) {
		case 0:
			return 0;
		case EINVAL:
			return -1;
		case EDEADLK:
			return -1;

		default:
			return -1;
	}
	return 0;       
}

int IPP_MutexUnlock( void* hMutex)
{
	int err;
	
	pthread_mutex_t *pMutex = (pthread_mutex_t *)hMutex;
	if (NULL == pMutex)
		return -1;
	
	err = pthread_mutex_unlock(pMutex);
	switch (err) {
		case 0:
			return 0;
		case EINVAL:
			return -1;
		case EPERM:
			return -1;

		default:
			return -1;
	}
}

/**********************************************************************
 * EVENTS
 **********************************************************************/
typedef struct {
	int bSignaled;
	pthread_mutex_t mutex;
	pthread_cond_t  condition;
} IPP_ThreadEvent;

int IPP_EventCreate( void* *phEvent)
{
	IPP_ThreadEvent *pEvent;

	IPP_MemMalloc(&pEvent, sizeof(IPP_ThreadEvent), 8);
	if (NULL == pEvent)
		return -1;
	pEvent->bSignaled = 0;

	if (pthread_mutex_init(&pEvent->mutex, NULL)) {
		IPP_MemFree(&pEvent);
		return -1;
	}
	if (pthread_cond_init(&pEvent->condition, NULL)) {
		pthread_mutex_destroy(&pEvent->mutex);
		IPP_MemFree(&pEvent);
		return -1;
	}

	*phEvent = (void*)pEvent;
	return 0;

}

int IPP_EventDestroy( void* hEvent)
{
	IPP_ThreadEvent *pEvent = (IPP_ThreadEvent *)hEvent;

	if (NULL == pEvent)
		return -1;

	if (pthread_mutex_lock(&pEvent->mutex))
		return -1;

	pthread_cond_destroy(&pEvent->condition);
	pthread_mutex_unlock(&pEvent->mutex);
	pthread_mutex_destroy(&pEvent->mutex);

	IPP_MemFree(&pEvent);
	return 0;
}

int IPP_EventSet( void* hEvent)
{
	IPP_ThreadEvent *pEvent = (IPP_ThreadEvent *)hEvent;
	if (NULL == pEvent)
		return -1;

	if (pthread_mutex_lock(&pEvent->mutex))
		return -1;
	pEvent->bSignaled = 1;
	pthread_cond_signal(&pEvent->condition);
	pthread_mutex_unlock(&pEvent->mutex);

	return 0;
}

int IPP_EventWait( void* hEvent,  unsigned int mSec,  int *pbTimedOut)
{
	IPP_ThreadEvent *pEvent = (IPP_ThreadEvent *)hEvent;
	unsigned long long         timeout_us;
	struct timespec timeout;
	int             retcode;

	struct timeval now;
	gettimeofday(&now, NULL);

	if (NULL != pbTimedOut)
		*pbTimedOut = 0;

	timeout_us = now.tv_usec + 1000*mSec;
	timeout.tv_sec = now.tv_sec + timeout_us / 1000000;
	timeout.tv_nsec = (timeout_us % 1000000) * 1000;

	if (NULL == pEvent)
		return -1;
	if (pthread_mutex_lock(&pEvent->mutex))
		return -1;

	if (0 == mSec) {
		if (!pEvent->bSignaled) {
			if (NULL != pbTimedOut)
				*pbTimedOut = 1;
		}
	}
	else if (INFINITE_WAIT == mSec) {
		while (!pEvent->bSignaled) {
			pthread_cond_wait(&pEvent->condition, &pEvent->mutex);
		}
	}
	else {
		while (!pEvent->bSignaled) {
			retcode = pthread_cond_timedwait(&pEvent->condition, &pEvent->mutex, &timeout);

			if ((ETIMEDOUT == retcode) && !pEvent->bSignaled) {
				if (pbTimedOut != NULL)
					*pbTimedOut = 1;
				break;
			}
		}
	}

	pEvent->bSignaled = 0;
	pthread_mutex_unlock(&pEvent->mutex);
	return 0;
}

int IPP_EventReset( void* hEvent)
{
	IPP_ThreadEvent *pEvent = (IPP_ThreadEvent *)hEvent;
	if (NULL == pEvent)
		return -1;

	if (pthread_mutex_lock(&pEvent->mutex))
		return -1;
	pEvent->bSignaled = 0;
	pthread_mutex_unlock(&pEvent->mutex);

	return 0;
}

/* EOF */

