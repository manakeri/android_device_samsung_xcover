/******************************************************************************
*(C) Copyright 2009 Marvell International Ltd.
* All Rights Reserved 
******************************************************************************/

/*!
*******************************************************************************
*  \file gcu.h
*  \brief  
*      Declaration of  GCU (GC Utilities) interface
******************************************************************************/

#ifndef __GCU_H__
#define __GCU_H__

typedef unsigned int            GCUbool;
typedef unsigned int            GCUenum;
typedef int                     GCUint;
typedef unsigned int            GCUuint;
typedef float                   GCUfloat;
typedef void                    GCUvoid;

typedef void*    				GCUContext;
typedef void*    				GCUSurface;
typedef void*                   GCUFence;

typedef void*                   GCUVirtualAddr;
typedef unsigned int            GCUPhysicalAddr;

#define GCU_FALSE               0
#define GCU_TRUE                1

#ifdef __cplusplus
#	define GCU_NULL				0
#else
#	define GCU_NULL				((void *) 0)
#endif

#define GCU_MAX_UINT32          0xffffffff

#define GCU_VERSION_1_0         0x00010000;
#define GCU_VERSION_2_0         0x00020000;

#define GCU_INFINITE            0xffffffff

/*!
*********************************************************************
*   \brief
*       Data types for initialization and termination
*********************************************************************
*/
typedef struct _GCU_INIT_DATA{
    GCUenum     version;        /* [IN]  input the version you want, return the 
                                    actual version of current library.
                                 */                                 
    GCUbool     debug;          /* [IN]enable debug output or not */
    GCUuint     reserved[6];     
}GCU_INIT_DATA;


/*!
*********************************************************************
*   \brief
*       Data types for context operation
*********************************************************************
*/
typedef struct _GCU_CONTEXT_DATA{
    GCUuint     reserved[8];     
}GCU_CONTEXT_DATA;

/*!
*********************************************************************
*   \brief
*       Data types for surface operation
*********************************************************************
*/
typedef enum _GCU_SURFACE_LOCATION{
    GCU_SURFACE_LOCATION_VIDEO          =   0, /* physical continous memory     */
    GCU_SURFACE_LOCATION_VIRTUAL        =   1, /* non-physical continous memory */
    
    GCU_SURFACE_LOCATION_FORCE_UINT     =   GCU_MAX_UINT32
}GCU_SURFACE_LOCATION; 

typedef enum _GCU_SURFACE_DIMENSION{
    GCU_SURFACE_2D                      =   0,
    
    GCU_SURFACE_DIMENSION_FORCE_UINT    =   GCU_MAX_UINT32
}GCU_SURFACE_DIMENSION;        

typedef enum _GCU_FORMAT{
    /* RGB series */
    GCU_FORMAT_ARGB8888     =   0,
    GCU_FORMAT_XRGB8888     =   1,
    
    GCU_FORMAT_ARGB1555     =   2,
    GCU_FORMAT_XRGB1555     =   3,
    GCU_FORMAT_RGB565       =   4,
    
    /* BGR series */
    GCU_FORMAT_RGBA8888     =   50,
    GCU_FORMAT_ABGR8888     =   51,
    GCU_FORMAT_BGRA8888     =   52,

    /* Luminance series */
    GCU_FORMAT_L8           =   100,
    GCU_FORMAT_A8           =   101,
    
    /* YUV series */
    GCU_FORMAT_UYVY         =   200,
    GCU_FORMAT_YUY2         =   201,
    GCU_FORMAT_YV12         =   202,
    GCU_FORMAT_NV12         =   203,
    GCU_FORMAT_I420         =   204,
    GCU_FORMAT_NV16         =   205,
    GCU_FORMAT_NV21         =   206,
    GCU_FORMAT_NV61         =   207,
    
    GCU_FORMAT_UNKNOW       =   500,
    
    GCU_FORMAT_FORCE_UINT   =   GCU_MAX_UINT32
}GCU_FORMAT;

typedef struct _GCU_SURFACE_FLAG{
    union 
    {
        struct 
        {
           GCUuint preAllocVirtual     : 1;    
           GCUuint preAllocPhysical    : 1;            
           GCUuint reserved            : 30;
        }bits;
        unsigned int value;
    };
}GCU_SURFACE_FLAG;

typedef struct _GCU_ALLOC_INFO{
    unsigned int    width;                  /* [IN] allocation width, in pixels, should be 16 pixels aligned                     */
    unsigned int    height;                 /* [IN] allocation height, in pixels, should be 4 pixels aligned                     */ 
    int             stride;                 /* [IN] allocation stride, in bytes, should be 64 bytes aligned                      */     
    void*          mapInfo;                 /* [OUT] Internal used flag, for user, always set it to GCU_NULL                     */
    GCUVirtualAddr  virtualAddr;            /* [IN] virtual address of pre-allocation allocation, should be 64 bytes aligned     */
    GCUPhysicalAddr physicalAddr;           /* [IN] physical address of pre-allocation allocation, should be 64 bytes aligned    */
    GCUuint         reserved[3];    
}GCU_ALLOC_INFO;

typedef struct _GCU_SURFACE_DATA{
    GCU_SURFACE_LOCATION    location;       /* [IN] Phyiscal continous memory or physical non-countious memory   */
    GCU_SURFACE_DIMENSION   dimention;      /* [IN] Only support 2D now                                          */
    GCU_SURFACE_FLAG        flag;           /* [IN] Surface creation flag, see above definition                  */
    
    GCU_FORMAT              format;         /* [IN] Surface format                                               */
    GCUuint                 width;          /* [IN] Base layer width, should be 16 pixels aligned                */
    GCUuint                 height;         /* [IN] Base layer height, should be 4 pixels aligned                */
    GCUuint                 arraySize;      /* [IN] Surface alllication info array size. 
                                                *  For normal RGB or YUV interleaved surface, set it to 1. 
                                                *  For YUV planar surface, set it to 3.
                                                *  For mipmapped surface, set it to mipmap count.
                                                */    
    GCU_ALLOC_INFO*         pPreAllocInfos;   
    GCUuint                 reserved[4];        
}GCU_SURFACE_DATA;

typedef struct _GCU_SURFACE_LOCK_FLAG{
    union 
    {
        struct 
        {
            GCUuint preserve    : 1;        /*  preserve surface contents when you lock surface, else the content 
                                             *  you get from locked surface depends on implementation. 
                                             */ 
            GCUuint reserved    : 31;
        }bits;
        GCUuint value;
    };
}GCU_SURFACE_LOCK_FLAG;


typedef struct _GCU_SURFACE_UPDATE_FLAG{
    union 
    {
        struct 
        {
            GCUuint preAllocVirtual     : 1;    
            GCUuint preAllocPhysical    : 1;            
            GCUuint reserved            : 30;
        }bits;
        GCUuint value;
    };
}GCU_SURFACE_UPDATE_FLAG;


typedef struct _GCU_SURFACE_LOCK_DATA{
    GCUSurface      pSurface;        /* [IN] surface to lock                        */
    GCU_SURFACE_LOCK_FLAG   flag;    /* [IN] surface flag                           */
    GCUuint         arraySize;       /* [IN] specify the size of alloc info array   */
    GCU_ALLOC_INFO* pAllocInfos;     /* [OUT] pointer to alloc info array           */
    GCUuint         reserved[4];     
}GCU_SURFACE_LOCK_DATA; 
                    
typedef struct _GCU_SURFACE_UPDATE_DATA{
    GCUSurface      pSurface;           /* [IN] surface to update                       */
    GCU_SURFACE_UPDATE_FLAG flag;       /* [IN] surface flag                            */
    GCUuint         arraySize;          /* [IN] specify the size of alloc info array    */
    GCU_ALLOC_INFO* pAllocInfos;        /* [IN] pointer to alloc info array             */    
    GCUuint         reserved[4];        
}GCU_SURFACE_UPDATE_DATA;                        


/*!
*********************************************************************
*   \brief
*       Data types for rendering API
*********************************************************************
*/                        
typedef enum _GCU_ROTATION{     /* rotation is clock-wise */
    GCU_ROTATION_0              =   0x0,
    GCU_ROTATION_FLIP_H         =   0x1,
    GCU_ROTATION_FLIP_V         =   0x2,
    GCU_ROTATION_90             =   0x4,
    GCU_ROTATION_180            =   0x3,
    GCU_ROTATION_270            =   0x7,    
    
    GCU_ROTATION_FORCE_UINT     =   GCU_MAX_UINT32
}GCU_ROTATION;

typedef enum _GCU_QUALITY_TYPE{
    GCU_QUALITY_NORMAL          =   0,
    GCU_QUALITY_HIGH            =   1, /* high quality scaling when gcuBlt */     
        
    GCU_QUALITY_TYPE_FORCE_UINT =   GCU_MAX_UINT32
}GCU_QUALITY_TYPE;

/* State types for gcuSet(pCtx, GCU_STATE_TYPE, value) */
typedef enum _GCU_STATE_TYPE{
    GCU_QUALITY                 =   1,  /* Followed by GCU_QUALITY_TYPE value   */
              
    GCU_STATE_TYPE_FORCE_UINT   =   GCU_MAX_UINT32
}GCU_STATE_TYPE;

typedef enum _GCU_BLEND_MODE{
    GCU_BLEND_SRC               =   0,  /* dst = src                */
    GCU_BLEND_SRC_OVER          =   1,  /* dst = src + (1-a)* dst   */ 
    
    GCU_BLEND_MODE_FORCE_UINT   =   GCU_MAX_UINT32
}GCU_BLEND_MODE;

typedef enum _GCU_FILTER_TYPE{
    GCU_H_USER_FILTER         	=   1,	/* 	user filter on horizontal direction 					*/
    GCU_V_USER_FILTER           =   2,  /* 	user filter on vertical direction 						*/     
    GCU_BLUR_FILTER				=   4,	/*	blur filter on both horizontal and vertical direction 	*/
	
    GCU_FILTER_TYPE_FORCE_UINT  =   GCU_MAX_UINT32
}GCU_FILTER_TYPE;

typedef struct _GCU_RECT{
    GCUint left;       /* left point is included in rect   */
    GCUint top;        /* top point is included in rect    */
    GCUint right;      /* right point is not included in rect, width = right - left    */
    GCUint bottom;     /* bottom point is not included in rect, height = bottom - top  */
}GCU_RECT;

typedef struct _GCU_BLT_DATA{
    GCUSurface      pSrcSurface;        /* [IN] Src surface to blit                                  */
    GCUSurface      pDstSurface;        /* [IN] Dst surface to blit                                  */
    GCU_RECT*       pSrcRect;           /* [IN] Src sub rectangle for blit, NULL means full surface  */ 
    GCU_RECT*       pDstRect;           /* [IN] DSt sub rectangle for blit, NULL means full surface  */
    GCU_ROTATION    rotation;           /* [IN] Rotation degree when blitting src to dst             */         
    
    GCUuint         reserved[7];
}GCU_BLT_DATA;

typedef struct _GCU_BLEND_DATA{
    GCUSurface      pSrcSurface;        /* [IN] Src surface to blend                                             */
    GCUSurface      pDstSurface;        /* [IN] Dst surface to blend                                             */
    GCU_RECT*       pSrcRect;           /* [IN] Src sub rectangle to blit, NULL means full surface               */
    GCU_RECT*       pDstRect;           /* [IN] DSt sub rectangle to blit, NULL means full surface               */
    GCU_ROTATION    rotation;           /* [IN] Rotation degree when blending src to dst                         */ 
    
    GCU_BLEND_MODE  blendMode;          /* [IN] Blending mode when combine src and dst to generate final result  */                      
    GCUuint         srcGlobalAlpha;     /* [IN] 0 ~ 255 src global alpha value                                   */
    GCUuint         dstGlobalAlpha;     /* [IN] 0 ~ 255 dst global alpha value                                   */   
    
    GCUuint         reserved[4];
}GCU_BLEND_DATA;

typedef struct _GCU_FILTER_BLT_DATA{
    GCUSurface      pSrcSurface;        /* [IN] Src surface to filter                                            */
    GCUSurface      pDstSurface;        /* [IN] Dst surface to filter                                            */
    GCU_RECT*       pSrcRect;           /* [IN] Src sub rectangle to blit, NULL means full surface               */
    GCU_RECT*       pDstRect;           /* [IN] Dst sub rectangle to blit, NULL means full surface               */
    GCU_ROTATION    rotation;           /* [IN] Rotation degree when filtering src to dst                        */

    GCU_FILTER_TYPE filterType;         /* [IN] Filter type when filtering src to dst                            */
    
    GCUuint         reserved[6];
}GCU_FILTER_BLT_DATA;

typedef struct _GCU_FILL_DATA{
	GCUSurface 	pSurface;           /* [IN] Dst surface to fill                                                              */
	GCU_RECT*	pRect;              /* [IN] Dst sub rectangle to fill, NULL means full surface                               */
	GCUbool		bSolidColor;		/* [IN] Fill dst surface by solid color or pattern                                       */					
	GCUuint		color;		 		/* [IN] Solid color in 0xAARRGGBB format, each channel value is 0 ~ 255                  */
	GCUSurface  pPattern;           /* [IN] If bSolidColor is GCU_FALSE, use pPattern to fill dst. Else, pPattern is no use  */
	GCUuint		reserved[3];
}GCU_FILL_DATA;



/*!
*********************************************************************
*   \brief
*       Data types for query API
*********************************************************************
*/
typedef enum  _GCU_QUERY_NAME{
    GCU_VERSION                 =   0,
    GCU_VENDOR                  =   1,
    GCU_RENDERER                =   2,
    GCU_ERROR                   =   3,  

    GCU_QUERY_NAME_FORCE_UINT   =   GCU_MAX_UINT32   
}GCU_QUERY_NAME;

typedef enum _GCU_ERROR_TYPE{
    GCU_NO_ERROR                = 0,
    GCU_NOT_INITIALIZED         = 1,
    GCU_INVALID_PARAMETER       = 2,
    GCU_INVALID_OPERATION       = 3,
    GCU_OUT_OF_MEMORY           = 4,

    GCU_ERROR_TYPE_FORCE_UINT   = GCU_MAX_UINT32
}GCU_ERROR_TYPE;



/*check if the compiler is of C++*/
#ifdef __cplusplus 
extern "C" {
#endif


/*!
*********************************************************************
*   \brief
*       GCU interface definition
*********************************************************************
*/

/* 
 * Initialization and termination API 
 */
GCUbool gcuInitialize(GCU_INIT_DATA* pData);
GCUvoid gcuTerminate();


/* 
 * Context operation API 
 */
GCUContext gcuCreateContext(GCU_CONTEXT_DATA* pData);
GCUvoid    gcuDestroyContext(GCUContext pContext);     

 
/* 
 * Surface operation API 
 * Note : current implementation treat all surface as none-cacheable. if user specify a preallocated 
 *        cacheable buffer, he needs to flush and invalidate CPU cache properly.   
 */

/* Surface create and destroy */
GCUSurface gcuCreateSurface(GCUContext pContext, GCU_SURFACE_DATA* pData);
GCUvoid    gcuDestroySurface(GCUContext pContext, GCUSurface pSurface);

/*  Query surface information, only return basic info like width, height and format, won't return alloc info.  */
GCUbool gcuQuerySurfaceInfo(GCUContext pContext, GCUSurface pSurface, GCU_SURFACE_DATA* pData);

/*  Lock surface to get CPU accesible address, after CPU access completed, unlock the surface  */                   
GCUbool gcuLockSurface(GCUContext pContext, GCU_SURFACE_LOCK_DATA* pData);
GCUvoid gcuUnlockSurface(GCUContext pContext, GCUSurface pSurface);     

/* Update the address of pre-allocated surface address */ 
GCUbool gcuUpdateSurface(GCUContext pContext, GCU_SURFACE_UPDATE_DATA* pData);    


/* 
 * Rendering API 
 */
/* Fill surface with solid color or pattern */ 
GCUvoid gcuFill(GCUContext pContext, GCU_FILL_DATA* pData);

/* General blit, used for copy, scaling, blending and format conversion */ 
GCUvoid gcuBlit(GCUContext pContext, GCU_BLT_DATA* pData);  

/* 
 * Blend blit
 *  Blend mode  - Only support Src and SrcOver now.  
 *  Format      - Only support RGB to RGB blend, don't support YUV to RGB blend.
 *                If user want to blend YUV to RGB, he needs to convert YUV to RGB first, then blend RGB to RGB.
 *  Alpha value - Whether src or dst, alpha = pixel_alpha * global_alpha. If the format does not contain alpha 
 *                channel, pixel_alpha will be treat as 1.0f (or 255) and only use global alpha.
 */
GCUvoid gcuBlend(GCUContext pContext, GCU_BLEND_DATA* pData);


/* Set user specified filter kernel for horizontal or vertical direction */                             
GCUvoid gcuSetFilter(GCUContext       pContext,
                     GCU_FILTER_TYPE    filterType, /* GCU_H_USER_FILTER or GCU_V_USER_FILTER                       */
                     GCUint             filterSize, /* Filter coef array size, only supports filter_size == 9 now   */
                     GCUfloat*          pCoef);     /* Pointer to float point filter coef array. 
                                                     * Currently, we only support coef data range in (-2.0f, 2.0f) 
                                                     */
                                                     
/* Filter Blt, currently only support src rect size same as dst rect size, and only support one direction filter for one API call  */  
GCUvoid gcuFilterBlit(GCUContext pContext, GCU_FILTER_BLT_DATA* pData);

/* Set context state, currently only used to set rendering quality */      
GCUbool gcuSet(GCUContext pContext, GCUenum state, GCUint value);  

/* Flush previous commands to hardware, then return directly */
GCUvoid gcuFlush(GCUContext pContext);

/* Flush previous commands to hardware, wait those commands execution complete, then return directly */
GCUvoid gcuFinish(GCUContext pContext);

/*
 *  Fence API
 */

/* Create and destroy fence object */
GCUFence    gcuCreateFence(GCUContext pContext);
GCUvoid     gcuDestroyFence(GCUContext pContext, GCUFence pFence);

/* Send a fence mark to command stream */
GCUbool     gcuSendFence(GCUContext pContext, GCUFence pFence);

/* Wait the commands before corresponding gcuSendFence() completed */
GCUbool     gcuWaitFence(GCUContext pContext, GCUFence pFence, GCUuint wait_time_ms);


/* 
 * Misc API 
 */

/* Get current error code, if no error, it will return GCU_NO_ERROR. */
GCUenum     gcuGetError();

/* Query information :
 *  GCU_VERSION     :   current GCU implementation version string
 *  GCU_VENDOR      :   current GCU vendor name string, generally will be "Marvell Technology Group Ltd".
 *  GCU_RENDERER    :   current GCU hardware name string.
 *  GCU_ERROR       :   current GCU error code string.  
 */
const char* gcuGetString(GCUenum name);



/*!
*********************************************************************
*   \brief
*       GCU helper functions
*********************************************************************
*/

/* Allocate buffer in video memory, newly created buffer address returned in pVirtAddr and pPhysicalAddr  */
GCUSurface _gcuCreateBuffer(GCUContext	 pContext,
								 unsigned int        width, 
                                 unsigned int        height, 
                                 GCU_FORMAT          format, 
                                 GCUVirtualAddr*     pVirtAddr, 
                                 GCUPhysicalAddr*    pPhysicalAddr);
                              
/* 
 * Create pre-allocated buffer, the pre-allocated buffer address passed by virtualAddr and physicalAddr 
 *
 * Currently we only support below combinations :
 *  1) bPreAllocVirtual and bPreAllocPhysical are both GCU_TRUE.
 *  2) bPreAllocVirtual is GCU_TRUE but bPreAllocPhysical is GCU_FALSE.
 *
 * Currently We don't support below combinations 
 *  1) bPreAllocVirtua is GCU_FALSE but bPreAllocPhysical is GCU_TRUE 
 */
GCUSurface _gcuCreatePreAllocBuffer(GCUContext	    pContext,
                                    unsigned int        width, 
                                    unsigned int        height, 
                                    GCU_FORMAT          format,  
                                    GCUbool             bPreAllocVirtual,      
                                    GCUVirtualAddr      virtualAddr, 
                                    GCUbool             bPreAllocPhysical,
                                    GCUPhysicalAddr     physicalAddr);

/* 
 * Update pre-allocated buffer address.
 * When updating pre-alloc buffer , the bPreAllocVirtual and bPreAllocPhysical flags should be same as the 
 * values you create this buffer. Else, this function will directly return and set error.
 */ 
GCUbool    _gcuUpdatePreAllocBuffer(GCUContext	 pContext,
                                    GCUSurface          pSurface,
                                    GCUbool             bPreAllocVirtual,      
                                    GCUVirtualAddr      virtualAddr, 
                                    GCUbool             bPreAllocPhysical,
                                    GCUPhysicalAddr     physicalAddr);

/* Destroy buffer, whether pre-allocated or none-preallocated */
void       _gcuDestroyBuffer(GCUContext	 pContext, GCUSurface pSurface);




/* Create surface from bmp file, surface format is ARGB8888 */   
GCUSurface _gcuLoadRGBSurfaceFromFile(GCUContext    pContext, 
                                               const char*   filename);

/* Create surface from raw yuv file, user needs to provide format, width and height */
GCUSurface _gcuLoadYUVSurfaceFromFile(GCUContext    pContext, 
                                              const char*   filename, 
                                              GCU_FORMAT    format, 
                                              GCUuint       width, 
                                              GCUuint       height);

/* 
 * Save surface content to bmp or yuv file, file type is decided by surface format.
 * Saved file name will be prefix_index.bmp(or .yuv), index is a global counter for the 
 * times your called this function. 
 * Currenly, we only support save GCU_FORMAT_ARGB8888 or GCU_FORMAT_RGB565 to bmp.
 */ 
GCUbool    _gcuDumpSurface(GCUContext pContext, GCUSurface pSurface, const char* prefix);
    
/*check if the compiler is of C++*/
#ifdef __cplusplus 
}
#endif

#endif /* __GCU_H__ */




