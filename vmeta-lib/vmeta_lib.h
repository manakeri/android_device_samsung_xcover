#ifndef VDEC_OS_DRIVER_H
#define VDEC_OS_DRIVER_H
#include <sys/poll.h>
#include <semaphore.h>
#include "uio_vmeta.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define VDEC_DEBUG_ALL 0x1
#define VDEC_DEBUG_MEM 0x2
#define VDEC_DEBUG_LOCK 0x4
#define VDEC_DEBUG_VER 0x8
#define VDEC_DEBUG_POWER 0x10
#define VDEC_DEBUG_NONE 0x0

#define UIO_DEV "/dev/uio0"
#define UIO_IO_MEM_SIZE "/sys/class/uio/uio0/maps/map0/size"
#define UIO_IO_MEM_ADDR "/sys/class/uio/uio0/maps/map0/addr"
#define UIO_IO_VERSION "/sys/class/uio/uio0/version"

#define UIO_IO_HW_CONTEXT_SIZE "/sys/class/uio/uio0/maps/map1/size"
#define UIO_IO_HW_CONTEXT_ADDR "/sys/class/uio/uio0/maps/map1/addr"

#define UIO_IO_VMETA_OBJ_SIZE "/sys/class/uio/uio0/maps/map2/size"
#define UIO_IO_VMETA_OBJ_ADDR "/sys/class/uio/uio0/maps/map2/addr"
#define UIO_IO_VMETA_OBJ_INDEX 2

#define UIO_IO_KERNEL_SHARE_SIZE "/sys/class/uio/uio0/maps/map3/size"
#define UIO_IO_KERNEL_SHARE_ADDR "/sys/class/uio/uio0/maps/map3/addr"
#define UIO_IO_KERNEL_SHARE_INDEX 3

#define VMETA_SHARED_LOCK_HANDLE "vmeta_shared_lock"

#define VMETA_KERN_MIN_VER	5
#define VMETA_USER_VER		"build-006"
//---------------------------------------------------------------------------
// Macros
//---------------------------------------------------------------------------
#ifndef UNSG32
#define UNSG32 unsigned int
#endif

#ifndef SIGN32
#define SIGN32 int
#endif

#ifndef UNSG16
#define UNSG16 unsigned short
#endif

#ifndef SIGN16
#define SIGN16 short
#endif

#ifndef UNSG8
#define UNSG8 unsigned char
#endif

#ifndef SIGN8
#define SIGN8 char
#endif


#ifndef _ASM_LINUX_DMA_MAPPING_H
#define _ASM_LINUX_DMA_MAPPING_H
enum dma_data_direction {
	DMA_BIDIRECTIONAL = 0,
	DMA_TO_DEVICE = 1,
	DMA_FROM_DEVICE = 2,
	DMA_NONE = 3,
};
#endif

#define VMETA_STATUS_BIT_USED		0
#define VMETA_STATUS_BIT_REGISTED	1

#ifndef VMETA_OP_MAX
#define VMETA_OP_MAX		15
#define VMETA_OP_MIN		0
#define VMETA_OP_VGA		1
#define VMETA_OP_720P		8
#define VMETA_OP_1080P		14
#define VMETA_OP_VGA_MAX	(VMETA_OP_720P-1)
#define VMETA_OP_720P_MAX	(VMETA_OP_1080P-1)
#define	VMETA_OP_1080P_MAX	VMETA_OP_1080P
#define VMETA_OP_INVALID -1
#endif

#define RESO_VGA_SIZE		(480*640)
#define RESO_WVGA_SIZE		(480*800)
#define RESO_720P_SIZE		(720*1280)
#define RESO_1080P_SIZE		(1080*1920)
//---------------------------------------------------------------------------
// Driver initialization API
//---------------------------------------------------------------------------
SIGN32 vdec_os_driver_init(void);
SIGN32 vdec_os_driver_clean(void);
SIGN32 vdec_os_driver_version(SIGN8 *ver_str);

//---------------------------------------------------------------------------
// Memory operation API
//---------------------------------------------------------------------------
void * vdec_os_api_dma_alloc(UNSG32 size, UNSG32 align, UNSG32 * pPhysical);
void * vdec_os_api_dma_alloc_writecombine(UNSG32 size, UNSG32 align, UNSG32 * pPhysical);
void * vdec_os_api_dma_alloc_cached(UNSG32 size, UNSG32 align, UNSG32 * pPhysical);

void vdec_os_api_dma_free(void *ptr);
void *vdec_os_api_vmalloc(UNSG32 size, UNSG32 align);		// always return VA and can't be translated to PA
void vdec_os_api_vfree(void *ptr);
UNSG32 vdec_os_api_get_va(UNSG32 paddr);
UNSG32 vdec_os_api_get_pa(UNSG32 vaddr);

UNSG32 vdec_os_api_flush_cache(UNSG32 vaddr, UNSG32 size, enum dma_data_direction direction);

SIGN32 vdec_os_api_get_hw_obj_addr(UNSG32* vaddr, UNSG32 size);
SIGN32 vdec_os_api_get_hw_context_addr(UNSG32* paddr, UNSG32* vaddr, UNSG32 size, SIGN32 flag);

//---------------------------------------------------------------------------
// Mem/IO R/W API
//---------------------------------------------------------------------------
UNSG8 vdec_os_api_rd8(UNSG32 addr);
UNSG16 vdec_os_api_rd16(UNSG32 addr);
UNSG32 vdec_os_api_rd32(UNSG32 addr);
void vdec_os_api_wr8(UNSG32 addr, UNSG8 data);
void vdec_os_api_wr16(UNSG32 addr, UNSG16 data);
void vdec_os_api_wr32(UNSG32 addr, UNSG32 data);
UNSG32 vdec_os_api_get_regbase_addr(void);			// return VA

//---------------------------------------------------------------------------
// Interrupt register API
//---------------------------------------------------------------------------
SIGN32 vdec_os_api_set_sync_timeout_isr(UNSG32 timeout);
SIGN32 vdec_os_api_sync_event();

//---------------------------------------------------------------------------
// multi-instance operation APIs
//---------------------------------------------------------------------------
SIGN32 vdec_os_api_get_user_id(void);
SIGN32 vdec_os_api_free_user_id(SIGN32 user_id);
SIGN32 vdec_os_api_register_user_id(SIGN32 user_id);
SIGN32 vdec_os_api_unregister_user_id(SIGN32 user_id);

SIGN32 vdec_os_api_update_user_info(SIGN32 user_id, vmeta_user_info_t *info);
SIGN32 vdec_os_api_get_user_count(void);

//return: Former user unlock vmeta normally/abnormally/forced
SIGN32 vdec_os_api_lock(SIGN32 user_id, long to_ms);
SIGN32 vdec_os_api_unlock(SIGN32 user_id);

//---------------------------------------------------------------------------
// Power operation APIs
//---------------------------------------------------------------------------
SIGN32 vdec_os_api_power_on(void);
SIGN32 vdec_os_api_power_off(void);
SIGN32 vdec_os_api_suspend_check(void);
SIGN32 vdec_os_api_suspend_ready(void);
SIGN32 vdec_os_api_clock_on(void);
SIGN32 vdec_os_api_clock_off(void);

typedef enum _VMETA_CLOCK_OP{
	VMETA_CLOCK_L0 = 0,
	VMETA_CLOCK_L1
}VMETA_CLOCK_OP;

SIGN32 vdec_os_api_clock_switch(UNSG32 vco);

typedef enum _VPRO_CODEC_ERROR_CODE_ {
	VDEC_OS_DRIVER_OK = 0,
	VDEC_OS_DRIVER_INIT_FAIL,
	VDEC_OS_DRIVER_OPEN_FAIL,
	VDEC_OS_DRIVER_NO_SYS_MEM_FAIL,
	VDEC_OS_DRIVER_MEM_POOL_INIT_FAIL,
	VDEC_OS_DRIVER_MMAP_FAIL,
	VDEC_OS_DRIVER_SYNC_TIMEOUT_FAIL,
	VDEC_OS_DRIVER_IO_CONTROL_FAIL,
	VDEC_OS_DRIVER_ALREADY_INIT_FAIL,
	VDEC_OS_DRIVER_CLEAN_FAIL,
	VDEC_OS_DRIVER_USER_ID_FAIL,
	VDEC_OS_DRIVER_VER_FAIL,
	VDEC_OS_DRIVER_UPDATE_FAIL
}VPRO_DEC_ERROR_CODE;

typedef enum _LOCK_RET_CODE {
	LOCK_RET_ERROR_TIMEOUT = -9999,
	LOCK_RET_ERROR_UNKNOWN,
	LOCK_RET_OHTERS_NORM = 0,
	LOCK_RET_NULL,
	LOCK_RET_ME,
	LOCK_RET_FORCE_INIT,
	LOCK_RET_FORCE_TO_OTHERS,
}LOCK_RET_CODE;

/* display debug message */
#define VMETA_LOG_ON 0
#define VMETA_LOG_FILE "/data/vmeta_dbg.log"
int dbg_printf(UNSG32 dbglevel, const char* format, ...);

typedef sem_t lock_t;
//---------------------------------------------------------------------------
// the control block of vdec os driver
//---------------------------------------------------------------------------
typedef struct vdec_os_driver_cb_s
{
	int uiofd;			// the uio file descriptor
	UNSG32 io_mem_phy_addr;		// the physical addr of io register base
	SIGN32 io_mem_virt_addr;	// the reg base addr that maped from kernel
	UNSG32 io_mem_size;		// the size of io mem area
	int refcount;			// reference count in current process
	SIGN32 vdec_obj_va;
	UNSG32 vdec_obj_size;
	UNSG32 hw_context_pa;
	SIGN32 kernel_share_va;
	UNSG32 kernel_share_size;
	int kern_ver;	//vmeta kernel version
	SIGN32 curr_op;
} vdec_os_driver_cb_t;

struct monitor_data{
	pthread_t pt;
	SIGN32 user_id;
};

/* vdec driver get cb */
vdec_os_driver_cb_t *vdec_driver_get_cb(void);


#ifdef __cplusplus
}
#endif
#endif
