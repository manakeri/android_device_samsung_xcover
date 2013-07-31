/*
 * (C) Copyright 2010 Marvell International Ltd.
 * All Rights Reserved
 */


#ifndef __PMEM_HELPER_LIB_H__
#define __PMEM_HELPER_LIB_H__

#ifdef __cplusplus
extern "C"
{
#endif

struct pmem_handle_mrvl
{
	int fd;
	unsigned int size;
	void* va;
	void* pa;
};

typedef struct pmem_handle_mrvl PMEM_HANDLE_MRVL;

#define MARVELL_PMEMDEV_NAME_CACHEBUFFERED	"/dev/pmem"	//cacheable & buffered
#define MARVELL_PMEMDEV_NAME_NONCACHED		"/dev/pmem_adsp"	//non-cacheable & non-buffered
#define MARVELL_PMEMDEV_NAME_WC		"/dev/pmem_wc"		//non-cacheable & buffered

#define PMEM_FLUSH_BIDIRECTION		0
#define PMEM_FLUSH_TO_DEVICE		1
#define PMEM_FLUSH_FROM_DEVICE		2

struct pmem_handle_mrvl* pmem_malloc(int size, const char* devname);	//return handle. if NULL, fail
int pmem_free(struct pmem_handle_mrvl* handle);		//return 0 is ok, other value fail
void pmem_flush_cache(int pmem_fd, unsigned long offset, unsigned long size, int dir);	//dir should be PMEM_FLUSH_BIDIRECTION, PMEM_FLUSH_TO_DEVICE or PMEM_FLUSH_FROM_DEVICE


#ifdef __cplusplus
}
#endif

#endif
