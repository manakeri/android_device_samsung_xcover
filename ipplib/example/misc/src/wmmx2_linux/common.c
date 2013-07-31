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

#ifdef _IPP_MEM_CHECK
#include "mmcheck.h"
#endif

/////////////////////////////////////////////////////////////////////////////////
// Part 1 Memory operation implementation under Linux OS
/////////////////////////////////////////////////////////////////////////////////


/* Use 4 bytes to record real memory address */
#define REALADDR_OFFSET		4

/* Use 1 byte to record alignment value*/
#define ALIGNMENT_OFFSET	5
/* Get the total size of memory to allocate */
int GetTotalSizeToAlloc(int size, unsigned char align)
{
    int nTotalSize = size + align + ALIGNMENT_OFFSET;

#ifdef _IPP_MEM_CHECK
	AdjustMemSizeForMemCheck(&nTotalSize);
#endif

	return nTotalSize;
}

/* Get the address to return for external usage */
void GetAddressToReturn(char* pAddr_actual, char** pAddr_for_external, unsigned char align)
{
	char* pAddr_actual_add_offset = pAddr_actual + ALIGNMENT_OFFSET;
    *pAddr_for_external = (char*)( (unsigned int)pAddr_actual_add_offset - ((unsigned int)pAddr_actual_add_offset & (align -1)) + align);

#ifdef _IPP_MEM_CHECK
	AdjustMemAddressForMemCheck(pAddr_for_external);
#endif

}

/* Get the alignment value of the momery block at ALIGNMENT_OFFSET bytes before the return address */
void GetAlignValue(char* pAddr_for_external,unsigned char * align)
{
	*align = *(pAddr_for_external - ALIGNMENT_OFFSET);
}

/* Save the alignment value of the momery block at ALIGNMENT_OFFSET bytes before the return address */
void SaveAlignValue(char* pAddr_for_external,unsigned char align)
{
	*(pAddr_for_external - ALIGNMENT_OFFSET) = align;
}
/* Save the actual starting address of the momery block at REAL_ADDR_LEN bytes before the return address */
void SaveActualAddress(char* pAddr_actual, char* pAddr_for_external)
{
    //*((unsigned int*)(pAddr_for_external - REALADDR_OFFSET)) = (unsigned int)pAddr_actual;
	*(pAddr_for_external-1)	= (unsigned char)((unsigned int)pAddr_actual & 0xff);
	*(pAddr_for_external-2) = (unsigned char)(((unsigned int)pAddr_actual >> 8 ) & 0xff);
	*(pAddr_for_external-3) = (unsigned char)(((unsigned int)pAddr_actual >> 16) & 0xff);
	*(pAddr_for_external-4) = (unsigned char)(((unsigned int)pAddr_actual >> 24) & 0xff);
}

/* Get the actual starting address of the momery block from REAL_ADDR_LEN bytes before the return address */
void GetActualAddress(char* pAddr_for_external,char** pAddr_actual)
{
    //*pAddr_actual = (char*)(*(unsigned int*)(pAddr_for_external - REALADDR_OFFSET));
	unsigned int tmp = 0;
	tmp += (*(pAddr_for_external-1)) & 0xff;
	tmp += (*(pAddr_for_external-2) << 8 ) & 0xff00;
	tmp += (*(pAddr_for_external-3) << 16 ) & 0xff0000;
	tmp += (*(pAddr_for_external-4) << 24 ) & 0xff000000;
	*pAddr_actual = tmp;
}


/* Initialize for the memory test */
void IPP_InitMemCheck()
{

#ifdef _IPP_MEM_CHECK
	InitMemCheck();
#endif

	return;
}

/* Deinitialize for the memory test */
int IPP_DeinitMemCheck()
{

#ifdef _IPP_MEM_CHECK
	return DestroyMemCheck();	//0 means everything is OK
#endif

	return 0;
}

/* Aligned malloc
// return IPP_OK if success
// return IPP_FAIL if failure
*/
int IPP_MemMalloc(void **ppDstBuf, int size, unsigned char align)
{
	char* pAddr_actual, *pAddr_for_external;
	int nActualSize;

	if(size < 0)
	{
		IPP_Log(NULL,"w","can not IPP_MemMalloc with size = %d\n",size);
		*ppDstBuf = NULL;
		return IPP_FAIL;
	}

	if(size == 0)
		size = 8;

    nActualSize = GetTotalSizeToAlloc(size, align);  /* get actual size to malloc */

    pAddr_actual = (char*)malloc(nActualSize); /* alloc memory here */
	if(NULL == pAddr_actual)
	{
		IPP_Log(NULL,"w","can not malloc with size = %d\n",nActualSize);
		*ppDstBuf = NULL;
		return IPP_FAIL;
	}

	GetAddressToReturn(pAddr_actual, &pAddr_for_external, align);		// get the address to return

#ifdef _IPP_MEM_CHECK
	MallocCheck(pAddr_actual, pAddr_for_external, nActualSize, size);
#endif

    SaveActualAddress(pAddr_actual, pAddr_for_external);/* save actual address for correctly free */
	SaveAlignValue(pAddr_for_external, align);

	*ppDstBuf = (void*)pAddr_for_external;

	return IPP_OK;
}

/* Aligned calloc
// return IPP_OK if success
// return IPP_FAIL if failure
*/
int IPP_MemCalloc(void **ppDstBuf, int size, unsigned char align)
{
	char* pAddr_actual, *pAddr_for_external;
	int nActualSize;

	if(size < 0)
	{
		IPP_Log(NULL,"w","can not IPP_MemCalloc with size = %d\n",size);
		*ppDstBuf = NULL;
		return IPP_FAIL;
	}

	if(size == 0)
		size = 8;

    nActualSize = GetTotalSizeToAlloc(size, align);/* get actual size to malloc */

    pAddr_actual = (char*)calloc(nActualSize,1);/* alloc memory here */
	if(NULL == pAddr_actual)
	{
		IPP_Log(NULL,"w","can not malloc with size = %d\n",nActualSize);
		*ppDstBuf = NULL;
		return IPP_FAIL;
	}

	GetAddressToReturn(pAddr_actual, &pAddr_for_external, align);		// get the address to return

#ifdef _IPP_MEM_CHECK
	MallocCheck(pAddr_actual, pAddr_for_external, nActualSize, size);
#endif

	SaveActualAddress(pAddr_actual, pAddr_for_external);	// save actual address for correctly free
	SaveAlignValue(pAddr_for_external, align);

	*ppDstBuf = (void*)pAddr_for_external;

	return IPP_OK;
}

/* Free for IPP_MemMalloc, IPP_MemCalloc and IPP_MemRealloc
// return IPP_OK if success
// return IPP_FAIL if failure
*/
int IPP_MemFree(void ** ppSrcBuf)
{
	char* pAddr_actual;
	GetActualAddress((char*)(*ppSrcBuf),&pAddr_actual);	// get actual address to be free

#ifdef _IPP_MEM_CHECK
	FreeCheck(pAddr_actual,(char*)(*ppSrcBuf));
#endif

    free(pAddr_actual);/* free the memory */

	*ppSrcBuf = NULL;
	return IPP_OK;
}

/* Memory realloc without alignment
// If newsize > oldsize, will realloc
// If newsize <= oldsize, no need to realloc
// return IPP_OK if success
// return IPP_FAIL if failure
*/
int IPP_MemRealloc(void **ppSrcBuf, int oldsize, int newsize)
{
	int ret = 0;
    int nActualSize, bAlloc;
    char* pAddr_actual_old, *pAddr_actual_new,*pAddr_for_external_new;
	unsigned char align;

    if (newsize <= oldsize)/* if new size <= old size, return */
	{
		return IPP_OK;
	}
	else
	{
        if(NULL == *ppSrcBuf || 0 == oldsize)/*alloc buffer for the first time */
        {
			return IPP_MemMalloc(ppSrcBuf,newsize,1);
        }

        GetAlignValue((char*)(*ppSrcBuf), &align);	/* get alignment setting for this buffer */

#ifdef _IPP_MEM_CHECK
		IPP_MemMalloc(&pAddr_actual_new,newsize,align);
		if(pAddr_actual_new == NULL)
		{
			*ppSrcBuf = NULL;
			return IPP_FAIL;
		}
		memcpy(pAddr_actual_new,*ppSrcBuf,oldsize);
		IPP_MemFree(ppSrcBuf);
		*ppSrcBuf = (void*)pAddr_actual_new;
#else
		GetActualAddress((char*)(*ppSrcBuf),&pAddr_actual_old);		/* get actual addr for this buffer*/
		nActualSize = GetTotalSizeToAlloc(newsize, align);			/* get actual size to malloc */
		pAddr_actual_new = realloc(pAddr_actual_old,nActualSize);	/* try to realloc a new buffer*/
		if(pAddr_actual_new == NULL)
		{
			*ppSrcBuf = NULL;
			return IPP_FAIL;
		}

		if(pAddr_actual_new == pAddr_actual_old)		/*if the new addr equal old addr, nothing to do*/
		{
			//nothing to do
		}
		else	/*if the new addr not equal to old addr*/
		{
			pAddr_for_external_new = pAddr_actual_new + ((char*)(*ppSrcBuf) - pAddr_actual_old);	/*get addr for external in the new buffer*/

			if( ((unsigned int)pAddr_for_external_new & (unsigned int)(align - 1)) == 0)		/*if the addr for external still has align, return it*/
			{
				SaveActualAddress(pAddr_actual_new,pAddr_for_external_new);		/*only need to record the new actual addr*/
				*ppSrcBuf = pAddr_for_external_new;
			}
			else	/*if the addr for external is not align, new another buffer, copy the data to it, and then free the last buffer*/
			{
				IPP_MemMalloc(ppSrcBuf,newsize,align);
				if(*ppSrcBuf == NULL)
					return IPP_FAIL;
				memcpy(*ppSrcBuf,pAddr_for_external_new,oldsize);
				free(pAddr_actual_new);
			}
		}
#endif

		return IPP_OK;
	}
}

int IPP_PysicalMemTest()
{
	#ifdef _IPP_MEM_CHECK
	return IPP_PysicalMem_Check();
	#else
	return IPP_OK;
	#endif
}

/////////////////////////////////////////////////////////////////////////////////
// Part 3 Logging implementation under Linux OS
/////////////////////////////////////////////////////////////////////////////////

#define MAX_BUF_SIZE 1024

#define DEFAULT_LOGFILE "codeclog.txt"
// Logging function
// If set logfile to NULL will log message to a test system default log stream
// If set logfile to other names, they will treat as output files
void IPP_Log(char* logfile, char* mode, char* message, ...)
{
#ifndef LOG_OFF
    IPP_FILE* file;
    char buf[MAX_BUF_SIZE];
    int buflen;
    va_list args;

    if (logfile == NULL)
    {
        file = IPP_Fopen(DEFAULT_LOGFILE, mode);
    }
    else
    {
        file = IPP_Fopen(logfile, mode);
    }

    va_start(args, message);
    memset(buf,0,MAX_BUF_SIZE);
    buflen = vsprintf(buf,message, args);
    va_end(args);
    fprintf(stdout,"%s",buf);
    fflush(stdout);

    if(file != NULL)
	{
		IPP_Fwrite(buf,buflen, 1, file);
		IPP_Fclose(file);
	}
#endif
}

/////////////////////////////////////////////////////////////////////////////////
// Part 4 File operation implementation under Linux OS
/////////////////////////////////////////////////////////////////////////////////

// Open a file
IPP_FILE* IPP_Fopen(const char * filename, const char * mode)
{
	return fopen(filename, mode);
}

// Close a file
int IPP_Fclose(IPP_FILE* file)
{
	return fclose((FILE*)file);
}

// Read data from a file
int IPP_Fread(void * buffer, int size, int count, IPP_FILE* file)
{
	return fread(buffer, size, count, (FILE*)file);
}

// Write data to a file
int IPP_Fwrite(const void * buffer, int size, int count, IPP_FILE* file)
{
	return fwrite(buffer, size, count, (FILE*)file);
}

// Tests for end-of-file on a file
int IPP_Feof(IPP_FILE* file)
{
	return feof((FILE*)file);
}

// Moves the file pointer to a specified location
int IPP_Fseek(IPP_FILE* file, long offset, int origin)
{
	return fseek((FILE*)file, offset, origin);
}

// Gets the current position of a file pointer
long IPP_Ftell(IPP_FILE* file)
{
	return ftell((FILE*)file);
}

char *IPP_Fgets(char *str, int n, IPP_FILE* file)
{
	return fgets(str, n, (FILE*)file);
}

int IPP_Fgetc(IPP_FILE* file)
{
	return getc((FILE*)file);
}

int IPP_Fungetc(char c, IPP_FILE* file)
{
	return ungetc(c, (FILE*)file);
}

void *IPP_Memset(void *buffer, int c, int count)
{
	return memset(buffer, c, count);
}

void *IPP_Memmove(void *dst, const void *src, int n)
{
	return memmove(dst, src, n);
}

int IPP_Memcmp(void* src1, void* src2, int len)
{
	return memcmp(src1,src2,len);
}

void* IPP_Memcpy(void* dst, void* src, int len)
{
	return memcpy(dst,src,len);
}
void IPP_Fprintf(IPP_FILE* file, const char *format, ...)
{
	char buf[MAX_BUF_SIZE];
	va_list args;

	buf[MAX_BUF_SIZE-1] = '\0';

        va_start(args, format);
        vsprintf(buf,format, args);
        va_end(args);
        fprintf((FILE*)file, "%s", buf);

}

int IPP_Fflush(IPP_FILE *stream)
{
    return fflush(stream);
}

void IPP_Fscanf(IPP_FILE* file, const char *format, ...)
{
    va_list args;

    va_start(args, format);
    vfscanf((FILE*)file, format, args);
    va_end(args);
}
void IPP_Printf(const char* format, ...)
{
	char buf[MAX_BUF_SIZE];
	va_list args;

	buf[MAX_BUF_SIZE-1] = '\0';

	va_start(args, format);
	vsprintf(buf,format, args);
	va_end(args);
	fprintf(stdout, "%s", buf);
}

int IPP_Strcmp(const char *s1, const char *s2)
{
	return strcmp(s1, s2);
}

char *IPP_Strcpy(char *dst, const char *src)
{
	return strcpy(dst, src);
}

char *IPP_Strstr(char* s1, char* s2)
{
	return strstr(s1, s2);
}

int IPP_Strlen(char* s)
{
	return strlen(s);
}

char *IPP_Strncpy(char *dst, const char *src, int n)
{
	return strncpy(dst, src, n);
}

int IPP_Strncmp(char * s1, char * s2, int n )
{
	return strncmp(s1,s2,n);
}

char *IPP_Strcat(char *dst,const char *src)
{
	return strcat(dst,src);
}

int IPP_Atoi(const char *s)
{
	return atoi(s);
}

float IPP_Atof(const char *s)
{
	return atof(s);
}

float IPP_Log10(float x)
{
	return log10(x);
}

int IPP_Strtol(char * s1, char * s2, int n )
{
	return atoi(s1);
}


/* EOF */

