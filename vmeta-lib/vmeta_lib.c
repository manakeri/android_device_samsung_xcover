/*
 *  vmeta_lib.c
 *
 *
 * Copyright (C) 2009 Marvell International Ltd.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>

#include "vmeta_lib.h"
#include "phycontmem.h"
#include "sys/poll.h"

#define ALIGN(x,a)		__ALIGN_MASK(x,(typeof(x))(a)-1)
#define __ALIGN_MASK(x,mask)	(((x)+(mask))&~(mask))
#ifndef PAGE_SIZE
#define PAGE_SIZE				(1<<12)
#endif

// these APIs are used for vmeta driver only, not for export purpose.
#define VMETA_PRIVATE_LOCK_HANDLE "vmeta_private_lock"

static SIGN32 vmeta_private_lock();
static SIGN32 vmeta_private_unlock();
static SIGN32 vdec_os_api_get_ks(kernel_share **pp_ks);	//get kernel shared resources

// global variable
vdec_os_driver_cb_t *vdec_iface = NULL;
UNSG32 globalDbgLevel = VDEC_DEBUG_NONE;
UNSG32 syncTimeout = 500;
pthread_mutex_t pmt = PTHREAD_MUTEX_INITIALIZER;

static inline int get_bit(int nr, unsigned int *addr)
{
	unsigned int mask = 1 << nr;

	return ((*addr) & mask) != 0;
}

static inline int set_bit(int nr, unsigned int *addr)
{
	unsigned int mask = 1 << nr;
	unsigned int old;

	old = *addr;
	*addr = old | mask;

	return (old & mask) != 0;
}

static inline int clear_bit(int nr, unsigned int *addr)
{
	unsigned int mask = 1 << nr;
	unsigned int old;

	old = *addr;
	*addr = old & ~mask;

	return (old & mask) != 0;
}

//Add for hal mmap
UNSG8 vdec_os_api_rd8(UNSG32 addr)
{
	return *((volatile UNSG8 *)addr);
}

UNSG16 vdec_os_api_rd16(UNSG32 addr)
{
	return *((volatile UNSG16 *)addr);
}

UNSG32 vdec_os_api_rd32(UNSG32 addr)
{
	return *((volatile UNSG32 *)addr);
}

void vdec_os_api_wr8(UNSG32 addr, UNSG8 data)
{
	*((volatile UNSG8 *)addr) = data;
}

void vdec_os_api_wr16(UNSG32 addr, UNSG16 data)
{
	*((volatile UNSG16 *)addr) = data;
}

void vdec_os_api_wr32(UNSG32 addr, UNSG32 data)
{
	*((volatile UNSG32 *)addr) = data;
}

UNSG32 vdec_os_api_get_regbase_addr(void)
{
	vdec_os_driver_cb_t *vdec_iface = vdec_driver_get_cb();
	return (UNSG32) vdec_iface->io_mem_virt_addr;
}

//End of hal mmap

UNSG32 vdec_os_api_get_pa(UNSG32 vaddr)
{
	return ((UNSG32) phy_cont_getpa((void *)vaddr));
}

UNSG32 vdec_os_api_get_va(UNSG32 paddr)
{
	return ((UNSG32) phy_cont_getva(paddr));
}

void vdec_os_api_vfree(void *ptr)
{
	unsigned int offset = 0;
	unsigned int *paddr = NULL;

	paddr = (unsigned int *)(ptr);
	offset = *(paddr - 1);
	paddr = (unsigned int *)((unsigned int)paddr - offset);
	dbg_printf(VDEC_DEBUG_MEM, "vdec_os_api_vfree "
		   "ptr=0x%x\n paddr=0x%x offset=0x%x\n", ptr, paddr, offset);
	free((void *)paddr);
}

void *vdec_os_api_vmalloc(UNSG32 size, UNSG32 align)
{
	unsigned int *ptr = NULL;
	unsigned int tmp = 0;

	align = ALIGN(align, sizeof(int));
	size += align;
	dbg_printf(VDEC_DEBUG_MEM,
		   "vdec_os_api_vmalloc size=0x%x, align=0x%x\n", size, align);
	ptr = malloc(size);
	if (!ptr) {
		dbg_printf(VDEC_DEBUG_MEM,
			   "vdec_os_api_vmalloc not enough memory\n");
		return NULL;
	}

	tmp = (unsigned int)((unsigned int)(ptr) & (align - 1));
	tmp = (unsigned int)(align - tmp);
	ptr = (unsigned int *)((unsigned int)ptr + tmp);
	*(ptr - 1) = tmp;

	dbg_printf(VDEC_DEBUG_MEM, "vdec_os_api_vmalloc ptr: 0x%x\n", ptr);
	return ptr;
}

void vdec_os_api_dma_free(void *ptr)
{
	dbg_printf(VDEC_DEBUG_MEM, "vdec_os_api_dma_free ptr: 0x%x\n", ptr);
	phy_cont_free((void *)ptr);
}

void *vdec_os_api_dma_alloc(UNSG32 size, UNSG32 align, UNSG32 * pPhysical)
{
	unsigned int *ptr = NULL;

	if (size <= 0)
		return NULL;

	dbg_printf(VDEC_DEBUG_MEM, "vdec_os_api_dma_alloc -> size: 0x%x\n",
		   size);

	align = ALIGN(align, PAGE_SIZE);
	size = ALIGN(size, align);
	ptr = phy_cont_malloc(size, PHY_CONT_MEM_ATTR_NONCACHED);
	if (!ptr) {
		dbg_printf(VDEC_DEBUG_MEM,
			   "vdec_os_api_vmalloc not enough memory\n");
		return NULL;
	}

	*pPhysical = (unsigned long)phy_cont_getpa(ptr);

	if (((UNSG32) ptr & (align - 1)) != 0
	    || ((*pPhysical) & (align - 1)) != 0) {
		dbg_printf(VDEC_DEBUG_MEM,
			   "vdec_os_api_dma_alloc not aligned"
			   "align(0x%x) VA(0x%x) PA(0x%x)\n", align, ptr,
			   *pPhysical);
		phy_cont_free((void *)ptr);
		return NULL;
	}

	dbg_printf(VDEC_DEBUG_MEM, "vdec_os_api_dma_alloc ptr: 0x%x\n", ptr);

	return ptr;
}

void *vdec_os_api_dma_alloc_cached(UNSG32 size, UNSG32 align,
				   UNSG32 *pPhysical)
{
	unsigned int *ptr = NULL;

	if (size <= 0)
		return NULL;

	dbg_printf(VDEC_DEBUG_MEM,
		   "vdec_os_api_dma_alloc_cached -> size: 0x%x\n", size);

	align = ALIGN(align, PAGE_SIZE);
	size = ALIGN(size, align);
	ptr = phy_cont_malloc(size, PHY_CONT_MEM_ATTR_DEFAULT);
	if (!ptr) {
		dbg_printf(VDEC_DEBUG_MEM, "\tno enough memory\n");
		return NULL;
	}

	*pPhysical = (unsigned long)phy_cont_getpa(ptr);

	if (((UNSG32) ptr & (align - 1)) != 0
	    || ((*pPhysical) & (align - 1)) != 0) {
		dbg_printf(VDEC_DEBUG_MEM,
			   "vdec_os_api_dma_alloc_cached not aligned"
			   "align(0x%x) VA(0x%x) PA(0x%x)\n", align, ptr,
			   *pPhysical);
		phy_cont_free((void *)ptr);
		return NULL;
	}

	dbg_printf(VDEC_DEBUG_MEM, "vdec_os_api_dma_alloc_cached ptr: 0x%x\n",
		   ptr);

	return ptr;
}

void *vdec_os_api_dma_alloc_writecombine(UNSG32 size, UNSG32 align,
					 UNSG32 *pPhysical)
{
	unsigned int *ptr = NULL;

	if (size <= 0)
		return NULL;

	dbg_printf(VDEC_DEBUG_MEM,
		   "vdec_os_api_dma_alloc_cached -> size: 0x%x\n", size);

	align = ALIGN(align, PAGE_SIZE);
	size = ALIGN(size, align);
	ptr = phy_cont_malloc(size, PHY_CONT_MEM_ATTR_DEFAULT);
	if (!ptr) {
		dbg_printf(VDEC_DEBUG_MEM, "\tno enough memory\n");
		return NULL;
	}

	*pPhysical = (unsigned long)phy_cont_getpa(ptr);

	if (((UNSG32) ptr & (align - 1)) != 0
	    || ((*pPhysical) & (align - 1)) != 0) {
		dbg_printf(VDEC_DEBUG_MEM,
			   "vdec_os_api_dma_alloc_writecombine not aligned"
			   "align(0x%x) VA(0x%x) PA(0x%x)\n", align, ptr,
			   *pPhysical);
		phy_cont_free((void *)ptr);
		return NULL;
	}

	dbg_printf(VDEC_DEBUG_MEM,
		   "vdec_os_api_dma_alloc_writecombine ptr: 0x%x\n", ptr);

	return ptr;
}

UNSG32 vdec_os_api_flush_cache(UNSG32 vaddr, UNSG32 size,
			       enum dma_data_direction direction)
{
	int dir;
	if (direction == DMA_BIDIRECTIONAL)
		dir = PHY_CONT_MEM_FLUSH_BIDIRECTION;
	else if (direction == DMA_TO_DEVICE)
		dir = PHY_CONT_MEM_FLUSH_TO_DEVICE;
	else if (direction == DMA_FROM_DEVICE)
		dir = PHY_CONT_MEM_FLUSH_FROM_DEVICE;
	else
		return -1;

	if (0 < size)
		phy_cont_flush_cache_range((void *)vaddr, size, dir);
	else
		phy_cont_flush_cache((void *)vaddr, dir);
	return 0;
}

// enable vmeta interrupt
void vdec_os_api_irq_enable(void)
{
	int irq_on = 1;
	write(vdec_iface->uiofd, &irq_on, sizeof(int));
}

// disable vmeta interrupt
void vdec_os_api_irq_disable(void)
{
	int irq_on = 0;
	write(vdec_iface->uiofd, &irq_on, sizeof(int));
}

SIGN32 vdec_os_api_set_sync_timeout_isr(UNSG32 timeout)
{
	syncTimeout = timeout;
	return syncTimeout;
}

SIGN32 vdec_os_api_sync_event()
{
	struct pollfd ufds;
	int result = 0;

	ufds.fd = vdec_iface->uiofd;
	ufds.events = POLLIN;

	vdec_os_api_irq_enable();
	result = poll(&ufds, 1, syncTimeout);
	if (result > 0)
		return VDEC_OS_DRIVER_OK;
	else
		return -VDEC_OS_DRIVER_SYNC_TIMEOUT_FAIL;
}

//End of mem mmap
#define VMETA_VERSION_PREFIX "build-"

int get_version(char *msg)
{
	int ret;
	int version;

	FILE *file = fopen(msg, "r");

	if (!file) {
		dbg_printf(VDEC_DEBUG_ALL,
			   "Error: get_mem_size -> fopen failed\n");
		return -1;
	}

	ret = fscanf(file, VMETA_VERSION_PREFIX "%d", &version);
	if (ret < 0) {
		dbg_printf(VDEC_DEBUG_ALL,
			   "Error: get_version -> fscanf failed\n");
		version = -1;
	}

	fclose(file);
	return version;
}

UNSG32 get_mem_size(char *msg)
{
	int ret;
	UNSG32 result;
	FILE *file = fopen(msg, "r");

	if (!file) {
		dbg_printf(VDEC_DEBUG_ALL,
			   "Error: get_mem_size -> fopen failed\n");
		return -VDEC_OS_DRIVER_OPEN_FAIL;
	}

	ret = fscanf(file, "0x%x", &result);
	if (ret < 0) {
		dbg_printf(VDEC_DEBUG_ALL,
			   "Error: get_mem_size -> fscanf failed\n");
		result = 0;
	}

	fclose(file);
	return result;
}

UNSG32 get_mem_addr(char *msg)
{
	UNSG32 result;
	int ret;
	FILE *file = fopen(msg, "r");

	if (!file) {
		dbg_printf(VDEC_DEBUG_ALL,
			   "Error: get_mem_addr -> fopen failed\n");
		return -VDEC_OS_DRIVER_OPEN_FAIL;
	}

	ret = fscanf(file, "0x%x", &result);
	if (ret < 0) {
		dbg_printf(VDEC_DEBUG_ALL,
			   "Error: get_mem_addr -> fscanf failed\n");
		result = 0;
	}

	fclose(file);
	return result;
}

// init vdec os driver
SIGN32 vdec_os_driver_init(void)
{
	int ret = 0;
	int rv;

#if VMETA_LOG_ON
	FILE *fp_log;
#endif

	pthread_mutex_lock(&pmt);
	if (vdec_iface != NULL) {	// already been initiated in this process
		vdec_iface->refcount++;
		pthread_mutex_unlock(&pmt);
		return ret;
	}
#if (VMETA_LOG_ON && 0)

	fp_log = fopen(VMETA_LOG_FILE, "w");
	if (fp_log == NULL) {
		pthread_mutex_unlock(&pmt);
		return -1;
	}
	fclose(fp_log);

#endif

	// Prepare the vdec os driver control interface
	vdec_iface =
	    (vdec_os_driver_cb_t *) malloc(sizeof(vdec_os_driver_cb_t));
	memset((void *)vdec_iface, 0, sizeof(vdec_os_driver_cb_t));
	if (vdec_iface == NULL) {
		pthread_mutex_unlock(&pmt);
		return -VDEC_OS_DRIVER_INIT_FAIL;
	}
	// initialize reference count
	vdec_iface->refcount++;

	// Open the vdec uio driver
	vdec_iface->uiofd = open(UIO_DEV, O_RDWR);
	if (vdec_iface->uiofd < 0) {
		ret = -VDEC_OS_DRIVER_OPEN_FAIL;
		goto err_open_fail;
	}
	dbg_printf(VDEC_DEBUG_ALL, "vdec os driver open: %s uiofd=%d\n",
		   UIO_DEV, vdec_iface->uiofd);

	vdec_iface->kern_ver = get_version(UIO_IO_VERSION);
	if (vdec_iface->kern_ver < VMETA_KERN_MIN_VER) {
		ret = -VDEC_OS_DRIVER_VER_FAIL;
		goto err_open_fail;
	}
	dbg_printf(VDEC_DEBUG_VER, "vdec os driver kern=%d user=%s\n",
		   vdec_iface->kern_ver, VMETA_USER_VER);

	// Get the IO mem size of vPro's register
	vdec_iface->io_mem_size = get_mem_size(UIO_IO_MEM_SIZE);
	if (vdec_iface->io_mem_size <= 0) {
		ret = -VDEC_OS_DRIVER_MMAP_FAIL;
		goto err_mmap_fail;
	}
	dbg_printf(VDEC_DEBUG_MEM, "vdec os driver io mem size: 0x%x\n",
		   vdec_iface->io_mem_size);

	// Get the IO mem phy addr
	vdec_iface->io_mem_phy_addr = get_mem_addr(UIO_IO_MEM_ADDR);
	if (vdec_iface->io_mem_phy_addr <= 0) {
		ret = -VDEC_OS_DRIVER_MMAP_FAIL;
		goto err_mmap_fail;
	}
	dbg_printf(VDEC_DEBUG_MEM, "vdec os driver io mem phy addr: 0x%x\n",
		   vdec_iface->io_mem_phy_addr);

	// mmap the io mem area
	vdec_iface->io_mem_virt_addr =
	    (SIGN32) mmap(NULL, vdec_iface->io_mem_size, PROT_READ | PROT_WRITE,
			  MAP_SHARED, vdec_iface->uiofd, 0);

	if (vdec_iface->io_mem_virt_addr == -1) {
		ret = -VDEC_OS_DRIVER_MMAP_FAIL;
		goto err_mmap_fail;
	}
	dbg_printf(VDEC_DEBUG_MEM, "vdec os driver io mem map to: 0x%x\n",
		   vdec_iface->io_mem_virt_addr);

	vdec_iface->curr_op = VMETA_OP_INVALID;

	pthread_mutex_unlock(&pmt);
	return ret;

err_mmap_fail:
	if (vdec_iface->io_mem_virt_addr > 0)
		munmap((void *)vdec_iface->io_mem_virt_addr,
		       vdec_iface->io_mem_size);
	close(vdec_iface->uiofd);
err_open_fail:
	free((void *)vdec_iface);
	vdec_iface = NULL;

	pthread_mutex_unlock(&pmt);
	return ret;
}

// clean vdec os driver
SIGN32 vdec_os_driver_clean(void)
{
	pthread_mutex_lock(&pmt);

	if (vdec_iface == NULL) {
		pthread_mutex_unlock(&pmt);
		return -VDEC_OS_DRIVER_CLEAN_FAIL;
	}
	// decrease the refcount
	vdec_iface->refcount--;

	if (vdec_iface->refcount != 0) {
		dbg_printf(VDEC_DEBUG_ALL, "refcount = %d\n",
			   vdec_iface->refcount);
		pthread_mutex_unlock(&pmt);
		return 0;
	}
	// close clock and power
	if (vdec_os_api_get_user_count() <= 0) {
		vdec_os_api_clock_off();
		vdec_os_api_power_off();
		dbg_printf(VDEC_DEBUG_ALL,
			   "close vmeta power and clock in case app doesn't close\n");
	}
	// unmap memory area
	if (vdec_iface->io_mem_virt_addr > 0) {
		dbg_printf(VDEC_DEBUG_MEM,
			   "munmap with io_mem_virt_addr = 0x%x\n",
			   vdec_iface->io_mem_virt_addr);
		munmap((void *)vdec_iface->io_mem_virt_addr,
		       vdec_iface->io_mem_size);
		vdec_iface->io_mem_virt_addr = vdec_iface->io_mem_size = 0;
	}

	if (vdec_iface->kernel_share_va > 0) {
		dbg_printf(VDEC_DEBUG_MEM,
			   "munmap with kernel_share_va = 0x%x size=%d\n",
			   vdec_iface->kernel_share_va,
			   vdec_iface->kernel_share_size);
		munmap((void *)vdec_iface->kernel_share_va,
		       vdec_iface->kernel_share_size);
		vdec_iface->kernel_share_va = vdec_iface->kernel_share_size = 0;
	}

	if (vdec_iface->vdec_obj_va > 0) {
		dbg_printf(VDEC_DEBUG_MEM,
			   "munmap with vdec_obj_va = 0x%x size=%d\n",
			   vdec_iface->vdec_obj_va, vdec_iface->vdec_obj_size);
		munmap((void *)vdec_iface->vdec_obj_va,
		       vdec_iface->vdec_obj_size);
		vdec_iface->vdec_obj_va = vdec_iface->vdec_obj_size = 0;
	}
	// close fd
	if (vdec_iface->uiofd > 0) {
		close(vdec_iface->uiofd);
		dbg_printf(VDEC_DEBUG_ALL, "uio close\n");
	}
	// free vdec_iface
	if (vdec_iface != NULL) {
		free((void *)vdec_iface);
		dbg_printf(VDEC_DEBUG_ALL, "free vdec_iface\n");
		vdec_iface = NULL;
	}

	dbg_printf(VDEC_DEBUG_ALL, "vmeta clean done\n");
	pthread_mutex_unlock(&pmt);
	return 0;
}

/* display debug message */
int dbg_printf(UNSG32 dbglevel, const char *format, ...)
{
	char dbgBuf[256] = { '\0' };
	va_list var;
#if VMETA_LOG_ON
	FILE *fp_log = fopen(VMETA_LOG_FILE, "a+");
	if (fp_log == NULL) {
		return -1;
	}
#endif

	if (VDEC_DEBUG_NONE == globalDbgLevel)
		goto DBG_EXIT;
	else {
		va_start(var, format);
		vsprintf(dbgBuf, format, var);
		va_end(var);

		if (VDEC_DEBUG_ALL & globalDbgLevel)
			goto DBG_PRINT;
		else if ((VDEC_DEBUG_MEM & globalDbgLevel)
			 && (dbglevel == VDEC_DEBUG_MEM))
			goto DBG_PRINT;
		else if ((VDEC_DEBUG_LOCK & globalDbgLevel)
			 && (dbglevel == VDEC_DEBUG_LOCK))
			goto DBG_PRINT;
		else if ((VDEC_DEBUG_VER & globalDbgLevel)
			 && (dbglevel == VDEC_DEBUG_VER))
			goto DBG_PRINT;
		else if ((VDEC_DEBUG_POWER & globalDbgLevel)
			 && (dbglevel == VDEC_DEBUG_POWER))
			goto DBG_PRINT;
		else
			goto DBG_EXIT;
	}
DBG_PRINT:
#if VMETA_LOG_ON
	fprintf(fp_log, "%s", dbgBuf);
#else
	printf("%s", dbgBuf);
#endif

DBG_EXIT:
#if VMETA_LOG_ON
	fclose(fp_log);
#endif
	return 0;
}

/* vdec driver get cb */
vdec_os_driver_cb_t *vdec_driver_get_cb(void)
{
	return vdec_iface;
}

SIGN32 vdec_os_api_get_hw_obj_addr(UNSG32 *vaddr, UNSG32 size)
{
	UNSG32 io_mem_size;
	SIGN32 io_mem_virt_addr;
	UNSG32 ret = VDEC_OS_DRIVER_OK;

	if (vdec_iface->vdec_obj_va > 0) {
		dbg_printf(VDEC_DEBUG_MEM, "Already get vdec obj\n");
		*vaddr = vdec_iface->vdec_obj_va;
		return VDEC_OS_DRIVER_OK;
	}

	io_mem_size = get_mem_size(UIO_IO_VMETA_OBJ_SIZE);
	if (io_mem_size <= 0 || io_mem_size < size) {
		ret = -VDEC_OS_DRIVER_MMAP_FAIL;
		dbg_printf(VDEC_DEBUG_MEM,
			   "vdec_os_api_get_hw_obj_addr error: get_mem_size io_mem_size=%d, requested size=%d!!!\n",
			   io_mem_size, size);
		goto get_vdec_obj_fail;
	}
	dbg_printf(VDEC_DEBUG_MEM,
		   "vdec_os_api_get_hw_obj_addr: get_mem_size io_mem_size=%d, requested size=%d\n",
		   io_mem_size, size);

	io_mem_virt_addr = (SIGN32) mmap(NULL, size,
					 PROT_READ | PROT_WRITE, MAP_SHARED,
					 vdec_iface->uiofd,
					 UIO_IO_VMETA_OBJ_INDEX *
					 getpagesize());
	if (io_mem_virt_addr == -1) {
		ret = -VDEC_OS_DRIVER_MMAP_FAIL;
		dbg_printf(VDEC_DEBUG_MEM,
			   "vdec_os_api_get_hw_obj_addr: mmap hw obj error\n");
		goto get_vdec_obj_fail;
	}

	dbg_printf(VDEC_DEBUG_MEM,
		   "UIO_IO_VMETA_OBJ_ADDR virtual 0x%x size=%d\n",
		   io_mem_virt_addr, io_mem_size);

	*vaddr = io_mem_virt_addr;
	vdec_iface->vdec_obj_va = io_mem_virt_addr;
	vdec_iface->vdec_obj_size = size;

get_vdec_obj_fail:
	return ret;
}

SIGN32 vdec_os_api_get_hw_context_addr(UNSG32 *paddr, UNSG32 *vaddr,
				       UNSG32 size, SIGN32 flag)
{
	UNSG32 io_mem_size;
	UNSG32 io_mem_addr;

	UNSG32 ret = VDEC_OS_DRIVER_OK;

	if (vdec_iface->hw_context_pa != 0) {
		dbg_printf(VDEC_DEBUG_MEM, "Already get hw context\n");
		*paddr = vdec_iface->hw_context_pa;
		return VDEC_OS_DRIVER_OK;
	}

	io_mem_size = get_mem_size(UIO_IO_HW_CONTEXT_SIZE);
	if (io_mem_size <= 0 || io_mem_size < size) {
		ret = -VDEC_OS_DRIVER_MMAP_FAIL;
		dbg_printf(VDEC_DEBUG_MEM,
			   "vdec_os_api_get_hw_context_addr error: get_mem_size io_mem_size=%d, requested size=%d!!!\n",
			   io_mem_size, size);
		goto get_hw_context_fail;
	}
	dbg_printf(VDEC_DEBUG_MEM,
		   "vdec_os_api_get_hw_context_addr: get_mem_size io_mem_size=%d, requested size=%d\n",
		   io_mem_size, size);

	io_mem_addr = get_mem_addr(UIO_IO_HW_CONTEXT_ADDR);
	if (io_mem_addr <= 0) {
		ret = -VDEC_OS_DRIVER_MMAP_FAIL;
		dbg_printf(VDEC_DEBUG_MEM,
			   "vdec_os_api_get_hw_context_addr: get_mem_addr error\n");
		goto get_hw_context_fail;
	}

	*((UNSG32 *) paddr) = io_mem_addr;
	vdec_iface->hw_context_pa = io_mem_addr;

	dbg_printf(VDEC_DEBUG_MEM, "UIO_IO_HW_CONTEXT_ADDR: 0x%08x\n",
		   *((UNSG32 *) paddr));

get_hw_context_fail:
	return ret;
}

SIGN32 vdec_os_api_get_ks(kernel_share **pp_ks)
{
	UNSG32 io_mem_size;
	SIGN32 io_mem_virt_addr;

	UNSG32 ret = VDEC_OS_DRIVER_OK;

	if (vdec_iface->kernel_share_va != 0) {
		dbg_printf(VDEC_DEBUG_MEM, "Already get kernel share\n");
		return 0;
	}

	io_mem_size = get_mem_size(UIO_IO_KERNEL_SHARE_SIZE);
	if (io_mem_size <= 0) {
		ret = -VDEC_OS_DRIVER_MMAP_FAIL;
		dbg_printf(VDEC_DEBUG_MEM,
			   "vdec_os_api_get_ks: get_mem_size error\n");
		goto get_vos_fail;
	}
	dbg_printf(VDEC_DEBUG_MEM,
		   "vdec_os_api_get_ks: get_mem_size io_mem_size=%d\n",
		   io_mem_size);

	io_mem_virt_addr = (SIGN32) mmap(NULL, io_mem_size,
					 PROT_READ | PROT_WRITE, MAP_SHARED,
					 vdec_iface->uiofd,
					 UIO_IO_KERNEL_SHARE_INDEX *
					 getpagesize());
	if (io_mem_virt_addr == -1) {
		ret = -VDEC_OS_DRIVER_MMAP_FAIL;
		dbg_printf(VDEC_DEBUG_MEM,
			   "vdec_os_api_get_ks: mmap ks error\n");
		goto get_vos_fail;
	}

	dbg_printf(VDEC_DEBUG_MEM,
		   "kernel share virtual address: 0x%x size=%d \n",
		   io_mem_virt_addr, io_mem_size);

	*pp_ks = (kernel_share *) io_mem_virt_addr;
	vdec_iface->kernel_share_va = (UNSG32) io_mem_virt_addr;
	vdec_iface->kernel_share_size = io_mem_size;

get_vos_fail:
	return ret;
}

int find_user_id(id_instance *list)	//return unoccupied id
{
	int i;
	for (i = 0; i < MAX_VMETA_INSTANCE; i++) {
		if (get_bit(VMETA_STATUS_BIT_USED, &(list[i].status)) == 0) {
			set_bit(VMETA_STATUS_BIT_USED, &(list[i].status));
			return i;
		}
	}
	return -VDEC_OS_DRIVER_USER_ID_FAIL;
}

SIGN32 vdec_os_api_force_ini(void)
{
	kernel_share *p_ks;
	vdec_os_driver_cb_t *p_cb = vdec_driver_get_cb();

	if (p_cb->kernel_share_va == 0) {
		vdec_os_api_get_ks(&p_ks);
	} else {
		p_ks = (kernel_share *) p_cb->kernel_share_va;
	}

	vmeta_private_lock();
	memset(p_ks, 0, sizeof(kernel_share));
	p_ks->active_user_id = MAX_VMETA_INSTANCE;
	vmeta_private_unlock();

	ioctl(vdec_iface->uiofd, VMETA_CMD_UNLOCK);

	return 0;
}

SIGN32 vdec_os_api_get_user_id(void)
{
	kernel_share *p_ks;
	SIGN32 ret = -1;
	vdec_os_driver_cb_t *p_cb = vdec_driver_get_cb();

	if (p_cb->kernel_share_va == 0) {
		vdec_os_api_get_ks(&p_ks);
	} else {
		p_ks = (kernel_share *) p_cb->kernel_share_va;
	}

	vmeta_private_lock();
	ret = find_user_id(p_ks->user_id_list);
	if (ret < 0) {
		dbg_printf(VDEC_DEBUG_ALL,
			   "vdec_os_api_get_user_id: find_user_id error\n");
	}
	vmeta_private_unlock();

	return ret;

}

SIGN32 vdec_os_api_free_user_id(SIGN32 user_id)
{
	kernel_share *p_ks;
	vdec_os_driver_cb_t *p_cb = vdec_driver_get_cb();

	if (p_cb->kernel_share_va == 0) {
		dbg_printf(VDEC_DEBUG_ALL,
			   "vdec_os_api_free_user_id error: not init yet\n");
		return VDEC_OS_DRIVER_USER_ID_FAIL;
	}

	if (user_id >= MAX_VMETA_INSTANCE || user_id < 0) {
		dbg_printf(VDEC_DEBUG_ALL,
			   "vdec_os_api_free_user_id error: exceeds max user_id\n");
		return VDEC_OS_DRIVER_USER_ID_FAIL;
	}
	p_ks = (kernel_share *) p_cb->kernel_share_va;

	vmeta_private_lock();

	clear_bit(VMETA_STATUS_BIT_REGISTED,
		  &(p_ks->user_id_list[user_id].status));
	clear_bit(VMETA_STATUS_BIT_USED, &(p_ks->user_id_list[user_id].status));

	vmeta_private_unlock();

	return VDEC_OS_DRIVER_OK;
}

void *vmeta_thread_monitor(void *pmd)
{
	kernel_share *p_ks;
	vdec_os_driver_cb_t *p_cb = vdec_driver_get_cb();
	pthread_attr_t pat;
	int detach_attr;
	struct monitor_data *p_md = NULL;

	pthread_mutex_lock(&pmt);

	if (p_cb->kernel_share_va == 0 || pmd == NULL) {
		dbg_printf(VDEC_DEBUG_ALL,
			   "vdec_os_api_free_user_id error: not init yet\n");
		free(pmd);
		pthread_detach(pthread_self());
		pthread_mutex_unlock(&pmt);
		return (void *)-1;
	}

	p_md = malloc(sizeof(struct monitor_data));
	if (p_md == NULL) {
		free(pmd);
		pthread_detach(pthread_self());
		pthread_mutex_unlock(&pmt);
		return (void *)-1;
	}

	memcpy(p_md, pmd, sizeof(struct monitor_data));
	dbg_printf(VDEC_DEBUG_LOCK, "ori 0x%x 0x%x pt=0x%x user_id=0x%x\n",
		   ((struct monitor_data *)pmd)->pt,
		   ((struct monitor_data *)pmd)->user_id, p_md->pt,
		   p_md->user_id);

	p_ks = (kernel_share *) p_cb->kernel_share_va;

	if (pthread_getattr_np(p_md->pt, &pat) != 0) {
		dbg_printf(VDEC_DEBUG_LOCK, "get thread attr failed\n");
		goto tag_monitor_fail;
	}

	if (pthread_attr_getdetachstate(&pat, &detach_attr) != 0) {
		dbg_printf(VDEC_DEBUG_LOCK, "get detach attr failed\n");
		goto tag_monitor_fail;
	}

	if (detach_attr == PTHREAD_CREATE_DETACHED) {
		dbg_printf(VDEC_DEBUG_LOCK,
			   "The thread is a detached thread, cannot join\nexit monitor!\n");
		goto tag_monitor_fail;
	}

	dbg_printf(VDEC_DEBUG_LOCK, "wait for pt=0x%x\n", p_md->pt);
	pthread_mutex_unlock(&pmt);

	pthread_join(p_md->pt, NULL);
	dbg_printf(VDEC_DEBUG_LOCK, "pt=0x%x is killed user_id(%d)\n", p_md->pt,
		   p_md->user_id);

	pthread_mutex_lock(&pmt);
	p_cb = vdec_driver_get_cb();

	if (p_cb) {
		if (p_cb->kernel_share_va) {
			p_ks = (kernel_share *) p_cb->kernel_share_va;
			if (p_md->user_id == p_ks->active_user_id) {
				dbg_printf(VDEC_DEBUG_LOCK,
					   "vmeta thread exit abnormally, instance id=%d lock flag=%d\n",
					   p_md->user_id, p_ks->lock_flag);
				if (p_ks->lock_flag == VMETA_LOCK_ON) {
					vdec_os_api_unlock(p_md->user_id);
					p_ks->lock_flag = VMETA_LOCK_FORCE_INIT;
				}
				p_ks->active_user_id = MAX_VMETA_INSTANCE;
			}

			if (p_ks->user_id_list[p_md->user_id].status != 0) {
				dbg_printf(VDEC_DEBUG_LOCK,
					   "vmeta thread exit abnormally, clear instance(%d)\n",
					   p_md->user_id);
				memset(&(p_ks->user_id_list[p_md->user_id]),
				       0x0, sizeof(id_instance));
				p_ks->ref_count--;
			}
		}
	}

	free(p_md);
	free(pmd);
	pthread_attr_destroy(&pat);

	dbg_printf(VDEC_DEBUG_MEM, "thread monitor exit\n");
	pthread_detach(pthread_self());
	pthread_mutex_unlock(&pmt);
	return (void *)0;

tag_monitor_fail:
	pthread_mutex_unlock(&pmt);
	free(p_md);
	free(pmd);
	pthread_detach(pthread_self());

	pthread_attr_destroy(&pat);
	return (void *)-1;

}

SIGN32 vdec_os_api_register_user_id(SIGN32 user_id)
{
	kernel_share *p_ks;
	vdec_os_driver_cb_t *p_cb = vdec_driver_get_cb();
	pthread_t tmp;
	struct monitor_data *p_md;

	if (user_id >= MAX_VMETA_INSTANCE || user_id < 0) {
		dbg_printf(VDEC_DEBUG_ALL,
			   "vdec_os_api_register_user_id error: exceeds max user_id\n");
		return VDEC_OS_DRIVER_USER_ID_FAIL;
	}

	if (p_cb->kernel_share_va == 0) {
		vdec_os_api_get_ks(&p_ks);
	} else {
		p_ks = (kernel_share *) p_cb->kernel_share_va;
	}

	if (set_bit
	    (VMETA_STATUS_BIT_REGISTED,
	     &(p_ks->user_id_list[user_id].status)) == 1) {
		dbg_printf(VDEC_DEBUG_ALL,
			   "vdec_os_api_register_user_id error: user id has already been registered\n");
		return VDEC_OS_DRIVER_USER_ID_FAIL;
	}

	p_md = (struct monitor_data *)malloc(sizeof(struct monitor_data));	//This is freed monitor function
	if (p_md == NULL) {
		dbg_printf(VDEC_DEBUG_ALL,
			   "vdec_os_api_register_user_id error: OOM\n");
		return VDEC_OS_DRIVER_USER_ID_FAIL;
	}
	p_ks->ref_count++;
	p_ks->user_id_list[user_id].pid = getpid();
	p_md->pt = pthread_self();
	p_ks->user_id_list[user_id].pt = (unsigned int)p_md->pt;
	p_md->user_id = user_id;

	pthread_create(&tmp, NULL, vmeta_thread_monitor, p_md);
	ioctl(p_cb->uiofd, VMETA_CMD_REG_UNREG, (unsigned long)user_id);

	dbg_printf(VDEC_DEBUG_LOCK,
		   "pid=%d,pt=0x%x are monitored user_id(%d)\n",
		   p_ks->user_id_list[user_id].pid,
		   p_ks->user_id_list[user_id].pt, user_id);

	return VDEC_OS_DRIVER_OK;

}

SIGN32 vdec_os_api_unregister_user_id(SIGN32 user_id)
{
	kernel_share *p_ks;
	vdec_os_driver_cb_t *p_cb = vdec_driver_get_cb();

	if (user_id >= MAX_VMETA_INSTANCE || user_id < 0) {
		dbg_printf(VDEC_DEBUG_ALL,
			   "vdec_os_api_unregister_user_id error: exceeds max user_id\n");
		return VDEC_OS_DRIVER_USER_ID_FAIL;
	}

	if (p_cb->kernel_share_va == 0) {
		dbg_printf(VDEC_DEBUG_ALL,
			   "vdec_os_api_unregister_user_id error: not init yet\n");
		return VDEC_OS_DRIVER_USER_ID_FAIL;
	} else {
		p_ks = (kernel_share *) p_cb->kernel_share_va;
	}

	if (clear_bit
	    (VMETA_STATUS_BIT_REGISTED,
	     &(p_ks->user_id_list[user_id].status)) == 0) {
		dbg_printf(VDEC_DEBUG_ALL,
			   "vdec_os_api_unregister_user_id error: user id[%d] has not been registered\n",
			   user_id);
		return VDEC_OS_DRIVER_USER_ID_FAIL;
	}

	p_ks->ref_count--;
	ioctl(p_cb->uiofd, VMETA_CMD_REG_UNREG, (unsigned long)user_id);
	return VDEC_OS_DRIVER_OK;
}

SIGN32 vmeta_private_lock()
{
	ioctl(vdec_iface->uiofd, VMETA_CMD_PRIV_LOCK,
	      (unsigned long)0xffffffff);
	return 0;
}

SIGN32 vmeta_private_unlock()
{
	ioctl(vdec_iface->uiofd, VMETA_CMD_PRIV_UNLOCK);
	return 0;
}

SIGN32 vdec_os_api_get_user_count(void)
{
	vdec_os_driver_cb_t *p_cb = vdec_driver_get_cb();
	kernel_share *p_ks;

	if (p_cb == NULL) {
		dbg_printf(VDEC_DEBUG_ALL,
			   "vdec_os_api_get_user_count error: point is NULL\n");
		return -1;
	}

	if (p_cb->kernel_share_va == 0) {
		vdec_os_api_get_ks(&p_ks);
	} else {
		p_ks = (kernel_share *) p_cb->kernel_share_va;
	}

	dbg_printf(VDEC_DEBUG_ALL, "get_user_count=%d\n", p_ks->ref_count);

	return p_ks->ref_count;
}

SIGN32 vdec_os_api_lock(SIGN32 user_id, long to_ms)
{
	vdec_os_driver_cb_t *p_cb = vdec_driver_get_cb();
	kernel_share *p_ks;
	SIGN32 ret;
	struct timeval tv;
	struct timezone tz;

	if (p_cb == NULL) {
		dbg_printf(VDEC_DEBUG_ALL,
			   "vdec_os_api_lock error: point is NULL\n");
		return LOCK_RET_ERROR_UNKNOWN;
	}
	p_ks = (kernel_share *) p_cb->kernel_share_va;

	if (p_ks->active_user_id == user_id) {
		dbg_printf(VDEC_DEBUG_LOCK,
			   "lock same user=%d, lock_flag=%d,ref_count=%d\n",
			   user_id, p_ks->lock_flag, p_ks->ref_count);
		return LOCK_RET_ME;	//just return since they are the same caller
	} else if (p_ks->lock_flag == VMETA_LOCK_ON) {	/*Here, we can handle the second lock is not released by the first one */
		gettimeofday(&tv, &tz);
		vmeta_private_lock();
		if (tv.tv_sec > p_ks->lock_start_tv.tv_sec + 3) {
			dbg_printf(VDEC_DEBUG_LOCK, "force lock to others\n");
			dbg_printf(VDEC_DEBUG_LOCK, "interval sec=%ld us=%ld\n",
				   tv.tv_sec - p_ks->lock_start_tv.tv_sec,
				   tv.tv_usec - p_ks->lock_start_tv.tv_usec);
			p_ks->lock_start_tv.tv_sec = tv.tv_sec;
			p_ks->lock_start_tv.tv_usec = tv.tv_usec;
			vmeta_private_unlock();

			vdec_os_api_unregister_user_id(p_ks->active_user_id);
			vdec_os_api_free_user_id(p_ks->active_user_id);

			vmeta_private_lock();
			p_ks->active_user_id = user_id;
			vmeta_private_unlock();

			return LOCK_RET_FORCE_TO_OTHERS;
		}

		vmeta_private_unlock();
	}

	ret = ioctl(vdec_iface->uiofd, VMETA_CMD_LOCK, (unsigned long)to_ms);
	if (ret != 0) {
		dbg_printf(VDEC_DEBUG_LOCK, "lock timeout\n");
		return LOCK_RET_ERROR_TIMEOUT;
	}

	vmeta_private_lock();

	gettimeofday(&tv, &tz);
	p_ks->lock_start_tv.tv_sec = tv.tv_sec;
	p_ks->lock_start_tv.tv_usec = tv.tv_usec;

	p_ks->active_user_id = user_id;
	if (p_ks->lock_flag == VMETA_LOCK_FORCE_INIT) {
		p_ks->lock_flag = VMETA_LOCK_ON;
		vmeta_private_unlock();
		return LOCK_RET_FORCE_INIT;
	}
	p_ks->lock_flag = VMETA_LOCK_ON;
	vmeta_private_unlock();

	return LOCK_RET_OHTERS_NORM;
}

SIGN32 vdec_os_api_unlock(SIGN32 user_id)
{
	vdec_os_driver_cb_t *p_cb = vdec_driver_get_cb();
	kernel_share *p_ks;
	int ret;

	if (p_cb == NULL) {
		dbg_printf(VDEC_DEBUG_LOCK,
			   "vdec_os_api_unlock error: point is NULL\n");
		return LOCK_RET_ERROR_UNKNOWN;
	}

	p_ks = (kernel_share *) p_cb->kernel_share_va;
	vmeta_private_lock();
	if (p_ks->active_user_id == user_id) {
		p_ks->active_user_id = MAX_VMETA_INSTANCE;
		p_ks->lock_flag = VMETA_LOCK_OFF;
	} else {
		dbg_printf(VDEC_DEBUG_LOCK,
			   "vdec_os_api_unlock error: unlock other user id %d; active_user_id is %d\n",
			   user_id, p_ks->active_user_id);
		vmeta_private_unlock();
		return LOCK_RET_ERROR_UNKNOWN;
	}
	vmeta_private_unlock();

	ret = ioctl(vdec_iface->uiofd, VMETA_CMD_UNLOCK);
	dbg_printf(VDEC_DEBUG_LOCK, "ID: %d after unlock\n", user_id);
	if (ret != 0) {
		dbg_printf(VDEC_DEBUG_LOCK, "vdec_os_api_unlock ioctl error\n");
		return LOCK_RET_ERROR_UNKNOWN;
	}

	return LOCK_RET_OHTERS_NORM;
}

SIGN32 vdec_os_api_power_on(void)
{
	SIGN32 ret = 0;

	if (vdec_iface == NULL) {
		return -1;
	}
	ret = ioctl(vdec_iface->uiofd, VMETA_CMD_POWER_ON);

	return ret;
}

SIGN32 vdec_os_api_power_off(void)
{
	SIGN32 ret = 0;

	if (vdec_iface == NULL) {
		return -1;
	}
	ret = ioctl(vdec_iface->uiofd, VMETA_CMD_POWER_OFF);

	return ret;
}

SIGN32 vdec_os_api_clock_on(void)
{
	SIGN32 ret = 0;

	if (vdec_iface == NULL) {
		return -1;
	}
	ret = ioctl(vdec_iface->uiofd, VMETA_CMD_CLK_ON);

	return ret;
}

SIGN32 vdec_os_api_clock_off(void)
{
	SIGN32 ret = 0;

	if (vdec_iface == NULL) {
		return -1;
	}
	ret = ioctl(vdec_iface->uiofd, VMETA_CMD_CLK_OFF);

	return ret;
}

SIGN32 vdec_os_driver_version(SIGN8 *ver_str)
{
	vdec_os_driver_cb_t *p_cb = vdec_driver_get_cb();

	if (p_cb == NULL || ver_str == NULL) {
		dbg_printf(VDEC_DEBUG_VER,
			   "vdec_os_driver_version error: point is NULL\n");
		return -VDEC_OS_DRIVER_VER_FAIL;
	}

	strcpy(ver_str, VMETA_USER_VER);
	dbg_printf(VDEC_DEBUG_VER, "vmeta user lib ver=%s\n", ver_str);

	return VDEC_OS_DRIVER_OK;
}

/*
vco bitmap
31~24		23~16	15~8		0~7
reserved	OP		step	flags
OP: vmeta working OP table defined in kernel space
step:  delta op level;
flags:  0-set OP for vmeta; 1-increase/decrease op by the value in step; 2-max op; 3-min op
e.g.
a) set vmeta as OP3 vco = 0x00030000
b) set vmeta higher op by 1 step vco = 0x00000101
*/
SIGN32 vdec_os_api_clock_switch(UNSG32 vco)
{
	SIGN32 ret = 0;

	if (vdec_iface == NULL) {
		return -1;
	}
	dbg_printf(VDEC_DEBUG_POWER, "vdec_os_api_clock_switch vco= 0x%08x\n",
		   vco);
	ret =
	    ioctl(vdec_iface->uiofd, VMETA_CMD_CLK_SWITCH, (unsigned long)vco);

	return ret;
}

/*
OP Definition(Based on H264)
resolution <= VGA           -- 1~7
WVGA < resolution <=720p    -- 8~13
resolution > 720p           -- 14
*/
static SIGN32 _vmeta_get_default_op(vmeta_user_info_t *p_info, SIGN32 *max_op)
{
	SIGN32 op = -1;
	int reso;

	reso = p_info->width * p_info->height;
	if (reso <= RESO_VGA_SIZE) {
		if (p_info->usertype == 0) {
			op = VMETA_OP_VGA;
			*max_op = VMETA_OP_VGA_MAX;
		} else {
			op = VMETA_OP_VGA_ENC;
			*max_op = VMETA_OP_VGA_ENC_MAX;
		}
	} else if (reso <= RESO_720P_SIZE) {
		op = VMETA_OP_720P;
		*max_op = VMETA_OP_720P_MAX;
	} else {
		op = VMETA_OP_1080P;
		*max_op = VMETA_OP_1080P_MAX;
	}

	if (p_info->strm_fmt == 4 && reso <= RESO_720P_SIZE) {	//H263, it need higher OP
		op = VMETA_OP_720P;
		*max_op = VMETA_OP_720P_MAX;
	}

	return op;
}

static SIGN32 vmeta_power_op(id_instance *list, int num)
{
	SIGN32 op = VMETA_OP_MAX;
	SIGN32 max_op;
	int id;

	if (num > 1)
		return VMETA_OP_MAX;

	for (id = 0; id < MAX_VMETA_INSTANCE; id++)
		if (list[id].status)
			break;	//found the user id

	if (id >= MAX_VMETA_INSTANCE)
		return -1;

	op = _vmeta_get_default_op(&(list[id].info), &max_op);
	dbg_printf(VDEC_DEBUG_POWER, "vmeta_power_op = %d\n", op);
	return op;
}

SIGN32 vdec_os_api_update_user_info(SIGN32 user_id, vmeta_user_info_t *info)
{
	kernel_share *p_ks;
	vdec_os_driver_cb_t *p_cb = vdec_driver_get_cb();
	SIGN32 op;
	unsigned int vco = 0;
	SIGN32 ret = 0;
	SIGN32 min_op, max_op, target_op;;
	SIGN32 step = 0;

	dbg_printf(VDEC_DEBUG_POWER, "vdec_os_api_update_user_info\n");
	if (info == NULL)
		return -VDEC_OS_DRIVER_UPDATE_FAIL;

	if (p_cb->kernel_share_va == 0) {
		dbg_printf(VDEC_DEBUG_POWER,
			   "vdec_os_api_update_user_info error: not init yet\n");
		return -VDEC_OS_DRIVER_UPDATE_FAIL;
	} else {
		p_ks = (kernel_share *) p_cb->kernel_share_va;
	}

	/*-99: expect lowest perf, -1: expect lower perf, 0: default perf,
	 1: expect higher perf, 99: expect highest perf*/
	switch (info->perf_req) {
	case (0):
		if (user_id < 0) {
			/* This means the upper layer is single instance usage */
			/* So we assume there is one user here */
			user_id = 0;
			p_ks->user_id_list[0].status =
			    (1 << VMETA_STATUS_BIT_USED) | (1 <<
							    VMETA_STATUS_BIT_REGISTED);
			p_ks->ref_count = 1;
		} else if (user_id >= MAX_VMETA_INSTANCE) {
			dbg_printf(VDEC_DEBUG_POWER,
				   "vdec_os_api_update_user_info: exceeds max user_id\n");
			return -VDEC_OS_DRIVER_UPDATE_FAIL;
		}
		vmeta_private_lock();
		memcpy(&p_ks->user_id_list[user_id].info, info,
		       sizeof(vmeta_user_info_t));
		op = vmeta_power_op(p_ks->user_id_list, p_ks->ref_count);
		p_ks->user_id_list[user_id].info.curr_op = op;
		vmeta_private_unlock();
		if (op < 0) {
			dbg_printf(VDEC_DEBUG_POWER, "cannot set correct op\n");
			return -VDEC_OS_DRIVER_UPDATE_FAIL;
		}
		if (info->usertype == 0)
			op++;
		vco = 0 | op << 16;
		break;
	case (99):
		vco = 2;
		break;
	case (-99):
		vco = 3;
		break;
	default:
		step = info->perf_req;

		/*We must hold the OP not lower than the default OP */
		if (p_cb->curr_op < VMETA_OP_MIN
		    || p_cb->curr_op > VMETA_OP_MAX) {
			dbg_printf(VDEC_DEBUG_POWER,
				   "[vmeta] error: unknown currunt OP, "
				   "set default OP first\n");
			return -VDEC_OS_DRIVER_UPDATE_FAIL;
		}
		min_op = _vmeta_get_default_op(info, &max_op);
		target_op = p_cb->curr_op + step;
		if (step > 0 && target_op > max_op) {
			dbg_printf(VDEC_DEBUG_POWER,
				   "[vmeta] warning: cannot exceed max op\n");
			dbg_printf(VDEC_DEBUG_POWER,
				   "[vmeta] step %d, target %d,max %d, min %d\n",
				   step, target_op, max_op, min_op);
			return 0;
		} else if (step < 0 && target_op < min_op) {
			dbg_printf(VDEC_DEBUG_POWER,
				   "[vmeta] warning: cannot exceed min op\n");
			dbg_printf(VDEC_DEBUG_POWER,
				   "[vmeta] step %d, target %d,max %d, min %d\n",
				   step, target_op, max_op, min_op);
			return 0;
		} else if (step == 0) {
			dbg_printf(VDEC_DEBUG_POWER,
				   "[vmeta] warning: cannot exceed min op\n");
			dbg_printf(VDEC_DEBUG_POWER,
				   "[vmeta] step %d, target %d,max %d, min %d\n",
				   step, target_op, max_op, min_op);
			return 0;
		}
		vco = (1 | (step << 8)) & 0xffff;
		dbg_printf(VDEC_DEBUG_POWER, "increase step =%d\n", step);
		break;
	}

	ret = vdec_os_api_clock_switch(vco);
	//dbg_printf(VDEC_DEBUG_POWER,"increase result current/return(%d/%d)\n",p_cb->curr_op,info->curr_op);
	/*Special handle for increase/decrease OP */
	dbg_printf(VDEC_DEBUG_POWER, "flag=0x%x\n", vco & 0xff);
	if ((vco & 0xff) == 1) {
		dbg_printf(VDEC_DEBUG_POWER, "after increase ret=%d step=%d\n",
			   ret, step);
		if (ret < 0) {
			if (step > 0)
				info->curr_op = VMETA_OP_MAX;
			else
				info->curr_op = VMETA_OP_MIN;
			ret = 0;
		} else {
			p_cb->curr_op += (SIGN32) step;
			info->curr_op = p_cb->curr_op;
		}
		dbg_printf(VDEC_DEBUG_POWER,
			   "increase result current/return(%d/%d)\n",
			   p_cb->curr_op, info->curr_op);
	} else {
		if (ret >= VMETA_OP_MIN && ret <= VMETA_OP_MAX) {
			p_cb->curr_op = ret;
			info->curr_op = ret;
		}
	}

	return ret;
}
