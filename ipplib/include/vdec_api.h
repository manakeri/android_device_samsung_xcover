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

#ifndef VDEC_API_H_
#define VDEC_API_H_

#ifdef __cplusplus
extern "C"
{
#endif


//---------------------------------------------------------------------------
//              Macros
//---------------------------------------------------------------------------
#ifndef TYPEDEF_STRUCT
#define TYPEDEF_STRUCT(x_)      typedef struct x_ x_; struct x_
#endif


//---------------------------------------------------------------------------
//              VMeta Common Data Structures
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//! \brief      Basic Constant Parameters of VMeta (read-only)
//---------------------------------------------------------------------------
TYPEDEF_STRUCT(vmeta_const_t)
{
    unsigned int    hw_version;                 //!< VMeta H/W version supported by this library
    unsigned int    rsvd;                       //!< Reserved.
    char           *version_string;             //!< Version string of the vmeta library.
    unsigned int    vmeta_context_size;         //!< Size (in bytes) of the vmeta context for software.
    unsigned int    vmeta_hw_context_size;      //!< Size (in bytes) of the vmeta context for hardware.
    unsigned int    vdec_strm_context_size;     //!< Size (in bytes) of the decoder stream context for software.
    unsigned int    vdec_strm_hw_context_size;  //!< Size (in bytes) of the decoder stream context for hardware.
    unsigned int    venc_strm_context_size;     //!< Size (in bytes) of the encoder stream context.
    unsigned int    venc_strm_hw_context_size;  //!< Size (in bytes) of the encoder stream context for hardware.
};

//---------------------------------------------------------------------------
//! \brief      Basic Constant Parameters of Video Decoder (read-only)  //Obsolete, for backward compatibity only
//---------------------------------------------------------------------------
TYPEDEF_STRUCT(vdec_const_t)
{
    unsigned int    version;                    //!< Reserved.
    unsigned int    build;                      //!< Reserved.
    char           *version_string;             //!< Version string of the video decoder library.
    unsigned int    vdec_context_size;          //!< Size (in bytes) of the video decoder context for software.
    unsigned int    vdec_hw_context_size;       //!< Size (in bytes) of the video decoder context for hardware.
    unsigned int    strm_context_size;          //!< Size (in bytes) of the stream context for software.
    unsigned int    strm_hw_context_size;       //!< Size (in bytes) of the stream context for hardware.
    unsigned int    rsvd0;                      //!< Reserved.
    unsigned int    rsvd1;                      //!< Reserved.
};

//---------------------------------------------------------------------------
//! \brief      Basic Constant Parameters of Video Encoder (read-only)  //Obsolete, for backward compatibity only
//---------------------------------------------------------------------------
TYPEDEF_STRUCT(venc_const_t)
{
    unsigned int    version;                    //!< Reserved.
    unsigned int    build;                      //!< Reserved.
    char           *version_string;             //!< Version string of the video encoder library.
    unsigned int    venc_context_size;          //!< Size (in bytes) of the video encoder context.
    unsigned int    venc_hw_context_size;       //!< Size (in bytes) of the video decoder context for hardware.
    unsigned int    rsvd0;                      //!< Reserved.
    unsigned int    rsvd1;                      //!< Reserved.
    unsigned int    strm_context_size;          //!< Size (in bytes) of the stream context.
    unsigned int    strm_hw_context_size;       //!< Size (in bytes) of the stream context for hardware.
};



//---------------------------------------------------------------------------
//! \brief      The Virtual external DMA's interface callback routines; 
//---------------------------------------------------------------------------
TYPEDEF_STRUCT(dma_drv_cfg_t)
{
    void           *dma_obj;                 //!< The external DMA object; 
    void           *dma_init;                //!< Function pointer for external dma init; 
    void           *dma_config;              //!< Function pointer for external dma config; 
    void           *dma_clear;               //!< Function pointer for external dma clear; 
    void           *dma_push_cmd;            //!< Function pointer for push a dma cmd; 
    void           *dma_get_cmdq_space;      //!< Function pointer for get cmdQ space and wptr; 
    void           *dma_get_cmdq_count;      //!< Function pointer for get cmdQ counter and rptr; 
    void           *dma_enable_empty_intr;   //!< Function pointer for enable empty interrupt;  
    void           *dma_disable_empty_intr;  //!< Function pointer for disable empty interrupt; 
    void           *dma_check_empty_intr;    //!< Function pointer to check DMA empty interrupt status; 
};



//---------------------------------------------------------------------------
//! \brief      Stream-independent VMeta Settings
//---------------------------------------------------------------------------
TYPEDEF_STRUCT(vmeta_config_t)
{
    void           *user_data;              //!< User-defined decoder context data, not used by the video decoder.
    void           *internal_data;          //!< Reserved. For video decoder internal use only.
    unsigned int    io_base;                //!< Base address of the video decoder I/O space.
    unsigned int    fast_io_base;           //!< Reserved. For video decoder internal use only.
    unsigned int    hw_context_addr;        //!< Base address (4KB-aligned) of the video decoder context buffer for hardware.
    void           *hw_context_vaddr;       //!< Virtual address (4KB-aligned) of the video decoder context buffer for hardware.
    void           *io_rd32;                //!< Function pointer for 32-bit I/O read.
    void           *io_wr32;                //!< Function pointer for 32-bit I/O write.
    void           *reg32_read;             //!< Function pointer to access 32-bit register space.  //Reserved for future improvements
    void           *reg32_write;            //!< Function pointer to access 32-bit register space.  //Reserved for future improvements
    void           *reg32_fill;             //!< Function pointer to access 32-bit register space.  //Reserved for future improvements
    void           *reg32_copy;             //!< Function pointer to access 32-bit register space.  //Reserved for future improvements
    void           *mem32_read;             //!< Function pointer to access 32-bit physical memory. //Reserved for future improvements
    void           *mem32_write;            //!< Function pointer to access 32-bit physical memory. //Reserved for future improvements
    void           *mem32_fill;             //!< Function pointer to access 32-bit physical memory. //Reserved for future improvements
    void           *mem32_copy;             //!< Function pointer to access 32-bit physical memory. //Reserved for future improvements
    dma_drv_cfg_t  dma_drv_cfg;             //!< Function pointer sets for external fake DMA driver implementation;   
    void           *dbg_panic;              //!< Reserved. For video decoder internal use only.
    void           *dbg_printf;             //!< Reserved. For video decoder internal use only.
};
typedef vmeta_config_t vdec_config_t;
typedef vmeta_config_t venc_config_t;


//---------------------------------------------------------------------------
//! \brief      VMeta Return Codes
//---------------------------------------------------------------------------
//!             All API functions return non-negative code on succuess
//!             and negative error code on failure.
//---------------------------------------------------------------------------

#define VMETA_RC_FIF_NOT_ZERO		     30006  // Not a fatal error; first_mb_in_slice != 0 in the first slice when ASO if off.
#define VMETA_RC_UNSUPPORTED_SYNTAX      30005  //Not a fatal error; Informational
#define VMETA_RC_INTERNAL_ERROR         -30001
#define VMETA_RC_INVALID_ARGUMENT       -30002
#define VMETA_RC_INVALID_CONFIG         -30003
#define VMETA_RC_UNSUPPORTED_FORMAT     -30004

#define VDEC_RC_FIF_NOT_ZERO            VMETA_RC_FIF_NOT_ZERO
#define VDEC_RC_UNSUPPORTED_SYNTAX      VMETA_RC_UNSUPPORTED_SYNTAX     //Obsolete, for backward compatibity only
#define VDEC_RC_INTERNAL_ERROR          VMETA_RC_INTERNAL_ERROR         //Obsolete, for backward compatibity only
#define VDEC_RC_INVALID_ARGUMENT        VMETA_RC_INVALID_ARGUMENT       //Obsolete, for backward compatibity only
#define VDEC_RC_INVALID_CONFIG          VMETA_RC_INVALID_CONFIG         //Obsolete, for backward compatibity only
#define VDEC_RC_UNSUPPORTED_FORMAT      VMETA_RC_UNSUPPORTED_FORMAT     //Obsolete, for backward compatibity only

#define VENC_RC_INTERNAL_ERROR          VMETA_RC_INTERNAL_ERROR         //Obsolete, for backward compatibity only
#define VENC_RC_INVALID_ARGUMENT        VMETA_RC_INVALID_ARGUMENT       //Obsolete, for backward compatibity only
#define VENC_RC_INVALID_CONFIG          VMETA_RC_INVALID_CONFIG         //Obsolete, for backward compatibity only
#define VENC_RC_UNSUPPORTED_FORMAT      VMETA_RC_UNSUPPORTED_FORMAT     //Obsolete, for backward compatibity only


//---------------------------------------------------------------------------
// VMeta Common API
//---------------------------------------------------------------------------

const vmeta_const_t *vmeta_get_const(void);
int     vmeta_open                  (void *vmeta_cntxt, vmeta_config_t *vmeta_config);
int     vmeta_configure             (void *vmeta_cntxt, vmeta_config_t *vmeta_config);
int     vmeta_close                 (void *vmeta_cntxt);
int     vmeta_switch_stream         (void *strm_cntxt, void *next_strm_cntxt);  //Obsolete, for backward compatibity only


//---------------------------------------------------------------------------
//              Video Decoder Data Structures
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//! \brief      Per-stream Decoder Settings
//---------------------------------------------------------------------------
TYPEDEF_STRUCT(vdec_strm_config_t)
{
    void           *user_data;              //!< User-defined stream context data, not used by the video decoder.
    void           *internal_data;          //!< Reserved. For video decoder internal use only.
    void           *vdec_cntxt;             //!< Pointer to the video decoder context.
    void           *base_strm;              //!< Pointer to the stream context of the base stream. Only used in H.264 MVC.
    unsigned int    format;                 //!< Stream format ID.
    unsigned int    hw_context_addr;        //!< Base address (4KB-aligned) of the stream context buffer for hardware.
    void           *hw_context_vaddr;       //!< Virtual adress (4KB-aligned) of the stream context buffer for hardware.
    unsigned int    strm_pool_addr;         //!< Base address of the stream buffer pool.
    unsigned int    strm_pool_size;         //!< Size (in bytes) of the stream buffer pool.
    unsigned int    mon_width;              //!< Preferred display buffer/monitor width in pixels. 0 = auto.
    unsigned int    mon_height;             //!< Preferred display buffer/monitor height in pixels. 0 = auto.
    unsigned int    min_strm_size;          //!< Reserved. For video decoder internal use only.
    unsigned int    no_reordering;          //!< Nonzero to disable the display buffer reordering.
    unsigned int    ref_padding_mode;       //!< Padding mode for reference picture (MPEG4 only).
                                            //      0: auto, 1: aligned pic boundary, 2: actual pic boundary
    unsigned int    pp_scan_mode;           //!< Source scan mode (for post-processing only):
                                            //      0: auto, 1: force progressive, 2: force interlaced
    unsigned int    pp_filter_mode;         //!< Up-sampling filter mode (for post-processing only):
                                            //      0: auto, 1: line repeat, 2: multi-tap filter
    unsigned int    pp_hscale;              //!< Horizontal down-scaling ratio (JPEG only)
                                            //      0 or 1: no scaling, 2: 1/2, 4: 1/4, 8: 1/8
    unsigned int    pp_vscale;              //!< Vertical down-scaling ratio (JPEG only)
                                            //      0 or 1: no scaling, 2: 1/2, 4: 1/4, 8: 1/8
    unsigned char   dblk_mode;              //!< Advanced setting for deblocking filter
                                            //      0=auto; 1=normal; 2=ref-only; 3=disabled;
    unsigned char   mvc_mode;               //!< Advanced setting for H.264 MVC support
                                            //      0=auto; 1=disabled; 2=enabled;
    unsigned char   aso_mode;               //!< Advanced setting for H.264 ASO support
                                            //      0=auto; 1=disabled; 2=enabled;
	unsigned char   encry_flag;             //!< 0: if clear ES in DDR;  else: encrypted ES in DDR;         
 
    unsigned int    roi_x;                  //!< ROI (region of interest) start location in horizontal direction (after down-scaling, unit in pixel) (JPEG only)
	unsigned int    roi_y;                  //!< ROI start location in vertical direction (after down-scaling, unit in pixel) (JPEG only)
	unsigned int    roi_w;                  //!< ROI width  (after down-scaling, unit in pixel) (JPEG only)
	unsigned int    roi_h;                  //!< ROI height (after down-scaling, unit in pixel) (JPEG only)

	unsigned int    slc_row;
	unsigned int    frame_in_mode;          //!< 0: stream-in mode; else: frame-in mode;  
};

//---------------------------------------------------------------------------
//! \brief      Sequence Descriptor
//---------------------------------------------------------------------------
TYPEDEF_STRUCT(vdec_seq_desc_t)
{
    void           *user_data;              //!< User-defined data, not used by the video decoder.
    void           *internal_data;          //!< Reserved. For video decoder internal use only.
    unsigned int    max_width;              //!< Maximum coded picture width (in pixels) of the whole sequence.
    unsigned int    max_height;             //!< Maximum coded picture height (in pixels) of the whole sequence.
    unsigned int    dis_offset;             //!< Offset (in bytes) of valid data in a display frame buffer.
    unsigned int    dis_stride;             //!< Stride (in bytes) of the display frame buffer.
    unsigned int    is_new_seq;             //!< Nonzero if it is a new sequence (not a repeated sequence header).
    unsigned int    is_intl_seq;            //!< Nonzero if it is an interlaced sequence. Not always reliable.
    unsigned int    dis_buf_size;           //!< Size (in bytes) of the display frame buffer.
    unsigned int    ref_buf_size;           //!< Size (in bytes) of the reference frame buffer.
    unsigned int    num_dis_bufs;           //!< Informative. Number of display frame buffers required by reordering.
    unsigned int    num_ref_bufs;           //!< Informative. Number of reference frame buffers required by decoding.
    unsigned int    frame_rate_num;         //!< Numerator of the frame rate, if available.
    unsigned int    frame_rate_den;         //!< Denominator of the frame rate, if available.
    unsigned int    sar_width;              //!< Horizontal size of the sample aspect ratio, if available.
    unsigned int    sar_height;             //!< Vertical size of the sample aspect ratio, if available.
    unsigned int    src_video_format;       //!< Source video format, if available.
                                            //      0: unknown, 'COMP': component,
                                            //      'PAL': PAL, 'NTSC': NTSC, 'SECA': SECAM, 'MAC': MAC
    unsigned int    level_idc; 
    unsigned int    max_reorder_frames;     //!< Reserved. For video decoder internal use only.
};

//---------------------------------------------------------------------------
//! \brief      Common Physcical DRAM Memory Descriptor
//---------------------------------------------------------------------------
TYPEDEF_STRUCT(vdec_buf_desc_t)
{
    void           *user_data;              //!< User-defined data, not used by the video decoder.
    void           *internal_data;          //!< Reserved. For video decoder internal use only.
    unsigned int    type;                   //!< Type of the buffer.
    unsigned int    paddr;                  //!< Base physical address of the buffer.
    void           *vaddr;                  //!< Base virtual address of the buffer.
    unsigned int    data_size;              //!< Size (in bytes) of the valid data.
    unsigned int    buf_size;               //!< Size (in bytes) of the buffer.
    unsigned int    id;                     //!< Buffer ID assigned by the buffer allocator, not used by the video decoder.
    unsigned int    sn;                     //!< Reserved. For video decoder internal use only.
    unsigned int    pos;                    //!< Reserved. For video decoder internal use only.
    void           *side_info;              //!< Side information to further describe the buffer content.
};

//---------------------------------------------------------------------------
//! \brief      Decoder Picture Descriptor
//---------------------------------------------------------------------------
TYPEDEF_STRUCT(vdec_pic_desc_t)
{
    void           *user_data;              //!< User-defined data, not used by the video decoder.
    void           *internal_data;          //!< Reserved. For video decoder internal use only.
    unsigned short  coded_width;            //!< Width (in pixels) of the coded frame.
    unsigned short  coded_height;           //!< Height (in pixels) of the coded frame.
    unsigned short  h_scaling;              //!< Horizontal up-scaling ratio of the coded frame. 0: no scaling.
    unsigned short  v_scaling;              //!< Vertical up-scaling ratio the coded frame. 0: no scaling.
    unsigned short  dis_width;              //!< Width (in pixels) of the display window.
    unsigned short  dis_height;             //!< Height (in pixels) of the display window.
    short           dis_left;               //!< Horizontal offset (in pixels) of the display window relative to the coded picture.
    short           dis_top;                //!< Vertical offset (in pixels) of the display window relative to the coded picture.
    unsigned int    total_mbs_in_pic;       //!< Total number of macroblcoks in the picture.
    unsigned int    error_mbs_in_pic;       //!< Total number of error macroblcoks in the picture.

	unsigned char   is_there_idr;           //!< Nonzero if there is no idr frame for current frame.
    unsigned char   is_valid;               //!< Nonzero if it is a valid picture.
    unsigned char   is_skipped;             //!< Nonzero if it the picture is skipped by user.
	unsigned char   is_aborted;             //!< Nonzero if it is aborted by decoder, for pictures which can(should) not be decoded totally.
    unsigned char   is_idr;                 //!< Nonzero if it is an IDR picture.
    unsigned char   is_ref;                 //!< Nonzero if it is a reference picture.
    unsigned char   is_fld;                 //!< Nonzero if it is a field picture.
    unsigned char   is_btm;                 //!< Nonzero if it is a bottom field picture.
    unsigned char   is_intl;                //!< Nonzero if it is interlaced (field or interlaced frame picture).
    unsigned char   is_coded;               //!< Nonzero if it is a coded picture (not a skip picture).
	unsigned char   is_open_gop_b;          //!< Nonzero if it is a B picture before I pic (in display order) at the start of an open gop.
	unsigned char   pd_top_field_first;     //!< Pull-down parameter. Nonzero if the top field needs to be output first.
    unsigned char   pd_repeat_first_field;  //!< Pull-down parameter. Nonzero if the first field needs to be repeated.
    unsigned char   pd_repeat_frame_count;  //!< Pull-down parameter. Number of times the whole frame needs to be repeated.
    unsigned char   reserved2;              //!< Reserved.
    unsigned int    ps_num_vectors;         //!< Number of available pan-scan vectors.
    unsigned short  ps_width[4];            //!< Width (in pixels) of the pan-scan window.
    unsigned short  ps_height[4];           //!< Height (in pixels) of the pan-scan window.
    int             ps_left[4];             //!< Horizontal offset (in 1/16 pixels) of the pan-scan window relative to the diplay window.
    int             ps_top[4];              //!< Vertical offset (in 1/16 pixels) of the pan-scan window relative to the diplay window.
    unsigned int    pic_type;               //!< Unified picture type ('I', 'P', 'B').
    unsigned int    native_pic_type;        //!< Native picture type defined by the video standard.
    int             poc;                    //!< Picture order count of the current picture.
    int             fld_poc[2];             //!< [0]: top poc, [1]: bottom poc, only valid for interlaced frame picture.
    unsigned int    stream_pos;             //!< 29-bit offset (in bytes) of the picture start code in the input stream.
    unsigned int    coded_pic_idx;          //!< Picture index in decoding order.
    unsigned int    output_delay;           //!< Reserved. For video decoder internal use only.
    unsigned int    chksum_data[8];         //!< Reserved. For video decoder internal use only.
    unsigned int    perf_cntr[4];           //!< Reserved. For video decoder internal use only.
    unsigned int    internal_flags;         //!< Reserved. For video decoder internal use only.
};

//---------------------------------------------------------------------------
//! \brief      Side Information for Video Display Buffer
//---------------------------------------------------------------------------
TYPEDEF_STRUCT(vdec_dbuf_side_info_t)
{
    vdec_pic_desc_t pic[2];                 //!< Picture descriptor pair. [0]: frame/top, [1]: bottom.
};

//---------------------------------------------------------------------------
//! \brief      Decoder Video Buffer Descriptor
//---------------------------------------------------------------------------
TYPEDEF_STRUCT(vdec_vid_buf_desc_t)
{
    void           *user_data;              //!< User-defined data, not used by the video decoder.
    void           *internal_data;          //!< Reserved. For video decoder internal use only.
    unsigned int    rsvd1;                  //!< Reserved. For video decoder internal use only.
    unsigned int    base_addr;              //!< Base address of the frame buffer.
    void           *rsvd2;                  //!< Reserved. For video decoder internal use only.
    unsigned int    rsvd3;                  //!< Reserved. For video decoder internal use only.
    unsigned int    rsvd4;                  //!< Reserved. For video decoder internal use only.
    unsigned int    id;                     //!< Buffer ID assigned by the buffer allocator, not used by the video decoder.
    unsigned int    sn;                     //!< Reserved. For video decoder internal use only.
    unsigned int    rsvd5;                  //!< Reserved. For video decoder internal use only.
    void           *rsvd6;                  //!< Reserved. For video decoder internal use only.
    unsigned int    tctx_addr;              //!< Reserved. For video decoder internal use only.
    vdec_pic_desc_t pic[2];                 //!< Picture descriptor pair. [0]: frame/top, [1]: bottom.
};

//---------------------------------------------------------------------------
//! \brief      Decoder Stream Buffer Descriptor
//---------------------------------------------------------------------------
TYPEDEF_STRUCT(vdec_strm_buf_desc_t)
{
    void           *user_data;              //!< User-defined data, not used by the video decoder.
    void           *internal_data;          //!< Reserved. For video decoder internal use only.
    unsigned int    rsvd1;                  //!< Reserved. For video decoder internal use only.
    unsigned int    addr;                   //!< Base address of the stream buffer.
    void           *rsvd2;                  //!< Reserved. For video decoder internal use only.
    unsigned int    size;                   //!< Size (in bytes) of the stream buffer.
    unsigned int    rsvd3;                  //!< Reserved. For video decoder internal use only.
    unsigned int    id;                     //!< Buffer ID assigned by the buffer allocator, not used by the video decoder.
    unsigned int    sn;                     //!< Reserved. For video decoder internal use only.
    unsigned int    pos;                    //!< Reserved. For video decoder internal use only.
    void           *rsvd4;                  //!< Reserved. For video decoder internal use only.
    unsigned int    flags;                  //!< Reserved. For video decoder internal use only.
};

//---------------------------------------------------------------------------
//! \brief      MPEG4 GMC Frame Level Parameters
//---------------------------------------------------------------------------
TYPEDEF_STRUCT(vdec_gmc_param_t)
{
    short           gmc_warp_point_x[3];
    short           gmc_warp_point_y[3];

    unsigned char   sprite_warp_point_num;
    unsigned char   sprite_warp_accuracy;
    unsigned char   sprite_bright_change;
    unsigned char   rounding_ctrl;

    vdec_buf_desc_t    *cur_pic_ref_buf;
    vdec_buf_desc_t    *cur_pic_dis_buf;
    vdec_buf_desc_t    *ref_pic_ref_buf;
    vdec_buf_desc_t    *gmc_buf;
};

//---------------------------------------------------------------------------
//! \brief      Per-stream Decoder Status and Event Information
//---------------------------------------------------------------------------
TYPEDEF_STRUCT(vdec_strm_status_t)
{
    void           *user_data;              //!< User-defined stream context data, not used by the video decoder.
    void           *internal_data;          //!< Reserved. For video decoder internal use only.
    int             event_id;               //!< Event ID.
    unsigned int    event_args[2];          //!< Event arguments.
    unsigned int    wait_for_int;           //!< Nonezero if decoder is waiting for interrupt.
    unsigned int    safe_for_switch;        //!< Reserved. For video decoder internal use only.
    unsigned int    strm_pos;               //!< 29-bit stream position (in bytes) in which the event occurs, if applicable.
    vdec_seq_desc_t    *seq_desc;           //!< Pointer to a vdec_seq_desc_t structure.
    vdec_pic_desc_t    *pic_desc;           //!< Pointer to a vdec_pic_desc_t structure.
    vdec_gmc_param_t   *gmc_param;          //!< Pointer to a vdec_gmc_param_t structure.

    //Parameters for VDEC_EVENT_SYNCWORD
    int             hdr_idx;                //!< Index of sequence level headers; Negative if not a sequence level header. <300 if H.264; <8 if other codecs.
    unsigned int    hdr_len;                //!< Length (in bytes) of the sequence level header.
};

//---------------------------------------------------------------------------
//! \brief      Decoder Event IDs
//---------------------------------------------------------------------------
enum
{
    // No event

    VDEC_EVENT_NONE = 0,

    // Stream events

    VDEC_EVENT_START_OF_STREAM,
    VDEC_EVENT_END_OF_STREAM,
    VDEC_EVENT_START_OF_SEQUENCE,
    VDEC_EVENT_END_OF_SEQUENCE,
    VDEC_EVENT_START_OF_GOP,
    VDEC_EVENT_END_OF_GOP,
    VDEC_EVENT_START_OF_FRAME,
    VDEC_EVENT_END_OF_FRAME,
    VDEC_EVENT_START_OF_PICTURE,
    VDEC_EVENT_END_OF_PICTURE,
	VDEC_EVENT_START_OF_SLICE,          // for JPEG only
	VDEC_EVENT_END_OF_SLICE,            // for JPEG only
    VDEC_EVENT_SYNCWORD,
    VDEC_EVENT_SWITCH_POINT,

    // Buffer management events

    VDEC_EVENT_NEED_BUF,            //arg0: buffer type; arg1: buffer size
    VDEC_EVENT_RELEASE_BUF,         //arg0: buffer type

    VDEC_EVENT_ALLOC_ALL,
    VDEC_EVENT_FREE_ALL,
    VDEC_EVENT_ALLOC_STR_BUF,
    VDEC_EVENT_ALLOC_REF_BUF,
    VDEC_EVENT_ALLOC_DIS_BUF,
    VDEC_EVENT_CHECK_STR_BUF,
    VDEC_EVENT_CHECK_REF_BUF,
    VDEC_EVENT_CHECK_DIS_BUF,

	VDEC_EVENT_FLUSH_DCACHE,

    // End of event definitions

    VDEC_EVENT_RESERVED
};


//---------------------------------------------------------------------------
// Video Decoder API
//---------------------------------------------------------------------------

const vdec_const_t *vdec_get_const  (void);                                             //Obsolete, for backward compatibity only
int     vdec_open                   (void *vdec_cntxt, vdec_config_t *vdec_config);     //Obsolete, for backward compatibity only
int     vdec_configure              (void *vdec_cntxt, vdec_config_t *vdec_config);     //Obsolete, for backward compatibity only
int     vdec_close                  (void *vdec_cntxt);                                 //Obsolete, for backward compatibity only
int     vdec_switch_stream          (void *strm_cntxt, void *next_strm_cntxt);          //Obsolete, for backward compatibity only
int     vdec_stream_switch_out      (void *strm_cntxt);
int     vdec_stream_switch_in       (void *strm_cntxt);

int     vdec_open_stream            (void *strm_cntxt, vdec_strm_config_t *strm_config);
int     vdec_configure_stream       (void *strm_cntxt, vdec_strm_config_t *strm_config);
int     vdec_close_stream           (void *strm_cntxt);
int     vdec_push_buffer            (void *strm_cntxt, unsigned int type, vdec_buf_desc_t *desc);
int     vdec_pop_buffer             (void *strm_cntxt, unsigned int type, vdec_buf_desc_t **desc);

int     vdec_push_stream_buffer     (void *strm_cntxt, vdec_strm_buf_desc_t *desc);
int     vdec_pop_stream_buffer      (void *strm_cntxt, vdec_strm_buf_desc_t **desc);
int     vdec_push_display_buffer    (void *strm_cntxt, vdec_vid_buf_desc_t *desc);
int     vdec_pop_display_buffer     (void *strm_cntxt, vdec_vid_buf_desc_t **desc);
int     vdec_push_reference_buffer  (void *strm_cntxt, vdec_vid_buf_desc_t *desc);
int     vdec_pop_reference_buffer   (void *strm_cntxt, vdec_vid_buf_desc_t **desc);

int     vdec_decode_stream          (void *strm_cntxt, vdec_strm_status_t **strm_status);
int     vdec_skip                   (void *strm_cntxt, int to);
int     vdec_save_context           (void *strm_cntxt, void *backup_strm_cntxt);
int     vdec_load_context           (void *strm_cntxt, void *backup_strm_cntxt);


//---------------------------------------------------------------------------
//              Video Encoder Data Structures
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//! \brief      Per-stream Encoder Settings
//---------------------------------------------------------------------------
TYPEDEF_STRUCT(venc_strm_config_t)
{
    void           *user_data;              //!< User-defined stream context data, not used by the video encoder.
    void           *internal_data;          //!< Reserved. For video encoder internal use only.
    void           *venc_cntxt;             //!< Pointer to the video encoder context
    unsigned int    format;                 //!< Stream format ID.
    unsigned int    hw_context_addr;        //!< Base address (1KB-aligned) of the stream context buffer for hardware.

    // per-stream parameters necessary for encoding intialization
    unsigned int    src_yuv_format;         //!< YUV format of input video source, e.g. YUV422
    unsigned int    use_1stpass;            //!< Whether enable first-pass encoding, i.e. 1/4 x 1/4 mostion search
    unsigned int    frm_width;              //!< Number of pels
    unsigned int    frm_height;             //!< Number of lines
    unsigned int    frm_only_flag;          //!< Frame only flag
    unsigned int    field_flag;             //!< Frame or field
    unsigned int    mbaff_enable;           //!< Enable mbaff
    unsigned int    poc_type;
    unsigned int    entropy_mode;           //!< 0:cavlc, 1:cabac
    unsigned int    transform_8x8_flag;     //!< Enable 8x8 transform flag
    unsigned int    ref_num_in_buf;         //!< Reference buffer size
    
    unsigned int    dis_offset;             //!< Offset (in bytes) of valid data in a display frame buffer.
    unsigned int    dis_stride;             //!< Stride (in bytes) of the display frame buffer.
    unsigned int    frame_rate_num;         //!< Numerator of the frame rate, if available.
    unsigned int    frame_rate_den;         //!< Denominator of the frame rate, if available.
    unsigned int    sar_width;              //!< Horizontal size of the sample aspect ratio, if available.
    unsigned int    sar_height;             //!< Vertical size of the sample aspect ratio, if available.
    unsigned int    src_video_format;       //!< Source video format, if available.
                                            //      0: unknown, 'COMP': component,
                                            //      'PAL': PAL, 'NTSC': NTSC, 'SECA': SECAM, 'MAC': MAC

    unsigned int    enc_mode;               //!< Encoding mode, see 'ENC_MODE_XXX'
    unsigned int    gop_type;               //!< GOP structure, see '__RCGOP_xxx'
    unsigned int    gop_size;               //!< Max frame count in a GOP
    unsigned int    open_gop;               //!< If open-GOP allowed
    unsigned int    switch_gop;             //!< Enable adaptive sub-GOP switching
    unsigned int    scene_change;           //!< Enable adaptive scene change
    unsigned int    init_qp;                //!< initial (constant) qp index if bit rate control is disabled
    unsigned int    vbv_size;               //!< VBV buffer size (bit), 0 to disable vbv control
    unsigned int    hrd_rate;               //!< HRD (Hypothetical Reference Decoder) filling speed (bps)
    unsigned int    min_rate;               //!< Minimum target bit rate (bps)
    unsigned int    max_rate;               //!< Maximum target bit rate (bps), CBR if (min_rate == max_rate && min_rate == target_rate)
	unsigned int    dri_mb_rows;            //!< (JPEG only) Number of MB rows in the restart interval (slice).
											//          0 (default) - disable restart interval
											//          1~(65535/mbs_in_row)     - MB rows in one restart interval (slice)

    int             rc_sel;                 //!< Rate-control strategy
                                            //      0 thru 4: select from the preset rate-control parameter sets
                                            //                0 to minimize the quality fluctuation
                                            //                4 to minimize the bit-rate fluctuation
                                            //     -1: disable the preset parameter set and configure the individual parameters.

    // The following rate-control parameters need to be set only if rc_sel is -1
    unsigned int    min_qp_idx;             //!< minimum qp index
    unsigned int    max_qp_idx;             //!< maximum qp index
    unsigned int    tracking_window;        //!< Rate control tracking window size in 1/10 second (default is 50 i.e. 5 seconds)
    unsigned int    short_term_min_rate;    //!< upper-bound (bps) for short-term bit-rate
    unsigned int    short_term_max_rate;    //!< lower-bound (bps) for short-term bit-rate
    unsigned int    short_term_window;      //!< short-term window size (1/10-sec)

	unsigned int    par;                    //!< pixel aspect ratio:
											// 0000: forbidden
											// 0001: 1:1(square)
											// 0010: 12:11(cif for 4:3 pictures)
											// 0011: 10:11(525-type for 4:3 picture)
											// 0100: 16:11 (CIF stretched for 16:9 picture)
											// 0101: 40:33 (525-type stretched for 16:9 picture)
											// 0110-1110: Reserved
											// 1111: Extended PAR
};

//---------------------------------------------------------------------------
//! \brief      Per-stream Encoder information input for first pass 
//---------------------------------------------------------------------------
TYPEDEF_STRUCT(venc_1stpass_param_t)
{
    unsigned int  pic_type;   //!< picture type, I, P, B, P SKIP, we need to think how to use B type
    unsigned int  pic_sn;     //!< picture serial number, handle re-order when B frame is used
    unsigned int  pic_qp;
};

//---------------------------------------------------------------------------
//! \brief      Per-stream Encoder status: output of first pass 
//---------------------------------------------------------------------------
TYPEDEF_STRUCT(venc_1stpass_stat_t)
{
    unsigned int  sum_sad;       //!< Sum of all down-sampled macro-blocks SAD 
    unsigned int  sum_sd;        //!< Sum of all down-sampled macro-blocks SD 
    unsigned int  num_intra_mbs; //!< Number of intra MBs in the 1st pass encode.
};

//---------------------------------------------------------------------------
//! \brief      Per-stream Encoder information: input for second pass 
//---------------------------------------------------------------------------
TYPEDEF_STRUCT(venc_2ndpass_param_t)
{
    unsigned int  pic_type;    //!< picture type
    unsigned int  pic_sn;      //!< picutre serial number
    unsigned int  pic_qp;      //!< QP index for the picture
};

//---------------------------------------------------------------------------
//! \brief      Per-stream Encoder status: output of second pass 
//---------------------------------------------------------------------------
TYPEDEF_STRUCT(venc_2ndpass_stat_t)
{
    unsigned int  strm_len;       //!< encoded bit stream length
    unsigned int  num_intra_mbs;  //!< Number of Intra MBs after 2nd pass encode
    unsigned int  sum_sad;		  //!< This SAD is sad_fullres.
};


//---------------------------------------------------------------------------
//! \brief      Encoder Sequence Descriptor
//---------------------------------------------------------------------------
TYPEDEF_STRUCT(venc_seq_desc_t)
{
    void           *user_data;              //!< User-defined data, not used by the video decoder.
    void           *internal_data;          //!< Reserved. For video decoder internal use only.
    unsigned int    is_intl_seq;            //!< Nonzero if it is an interlaced sequence. Not always reliable.
    unsigned int    src_yuv_format;         //!< Source YUV format (0: 420 tile, 1: UYVY 422 ...)
    unsigned int    width;                  //!< Image width for this sequence
    unsigned int    height;                 //!< Image height for this sequence
    unsigned int    vid_buf_size;           //!< Size (in bytes) of original 420 tiled video buffer size
    unsigned int    vid422_buf_size;        //!< Size (in bytes) of original 422 raster video buffer size
    unsigned int    ref_buf_size;           //!< Size (in bytes) of the reference buffer size (including reference, down sample video, temporal context)
    unsigned int    ds_buf_size;            //!< Size (in bytes) of the downsampled buffer size 
    unsigned int    num_ref_bufs;           //!< Informative. Number of reference frame buffers required by decoding.
};


//---------------------------------------------------------------------------
//! \brief      Encoder Picture Descriptor
//---------------------------------------------------------------------------
TYPEDEF_STRUCT(venc_pic_desc_t)
{
    void           *user_data;              //!< User-defined data, not used by the video decoder.
    void           *internal_data;          //!< Reserved. For video decoder internal use only.
    unsigned short  coded_width;            //!< Coded picture width (in pixels).
    unsigned short  coded_height;           //!< Coded picture height (in pixels).
    unsigned char   is_idr;                 //!< Nonzero if it is an IDR picture.
    unsigned char   is_ref;                 //!< Nonzero if it is a reference picture.
    unsigned char   is_fld;                 //!< Nonzero if it is a field picture.
    unsigned char   is_btm;                 //!< Nonzero if it is a bottom field picture.
    unsigned int    pic_type;               //!< Picture coding type ('I', 'P', 'B').
    int             poc;                    //!< Picture order count of the current picture.
    int             fld_poc[2];             //!< [0]: top poc, [1]: bottom poc, only valid for interlaced frame picture.
    unsigned int    stream_pos;             //!< 29-bit offset (in bytes) of the picture start code in the input stream.
    unsigned int    coded_pic_bits;         //!< Size (in bits) of the coded picture.
    unsigned int    seq_hdr_bits;           //!< Size (in bits) of the sequence header (if any) in coded picture.
    unsigned int    pic_hdr_bits;           //!< Size (in bits) of the picture header (if any) in coded picture.
    unsigned int    coded_pic_idx;          //!< Picture index in decoding order.
    unsigned int    chksum_data[8];         //!< Reserved. For video decoder internal use only.
    unsigned int    perf_cntr[4];           //!< Reserved. For video decoder internal use only.
};


//---------------------------------------------------------------------------
//! \brief      Encoder Video Buffer Descriptor
//---------------------------------------------------------------------------
TYPEDEF_STRUCT(venc_vid_buf_desc_t)
{
    void           *user_data;              //!< User-defined buffer context data
    void           *internal_data;          //!< Reserved. For video decoder internal use only.
    unsigned int    base_addr;              //!< Base address of the frame buffer in vMeta internal tiled format
    unsigned int    id;                     //!< Buffer ID assigned by the buffer allocator
    unsigned int    sn;                     //!< Reserved. For video decoder internal use only.
    unsigned int    tctx_addr;              //!< Reserved. For video encoder internal use only.
    venc_pic_desc_t pic[2];                 //!< Picture descriptor pair. [0]: frame/top, [1]: bottom.
};


//---------------------------------------------------------------------------
//! \brief      Encoder Stream Buffer Descriptor
//---------------------------------------------------------------------------
TYPEDEF_STRUCT(venc_strm_buf_desc_t)
{
    void           *user_data;              //!< User-defined buffer context data
    void           *internal_data;          //!< Reserved. For video decoder internal use only.
    unsigned int    addr;                   //!< Base address of the stream buffer.
    unsigned int    size;                   //!< Size (in bytes) of the stream buffer.
    unsigned int    valid_data;             //!< Size (in bytes) of the valid bitstream 
    unsigned int    flags;                  //!< Bit 0 is set if last stream buffer of picture. Bit 1 is set if last stream buffer of slice (bit 1 for JPEG only).
    unsigned int    id;                     //!< Buffer ID assigned by the buffer allocator
    unsigned int    sn;                     //!< Reserved. For video decoder internal use only.
    unsigned int    pos;                    //!< Reserved. For video decoder internal use only.
};


//---------------------------------------------------------------------------
//! \brief      Per-stream Encoder Status and Event Information
//---------------------------------------------------------------------------
TYPEDEF_STRUCT(venc_strm_status_t)
{
    void           *user_data;              //!< User-defined stream context data
    void           *internal_data;          //!< reserved for internal use
    int             event_id;               //!< Event ID.
    unsigned int           wait_for_int;    //!< None zero if encoder is waiting for interrupt.
    unsigned int           safe_for_switch; //!< Nonezero if decoder is in a safe state for context switch.
    unsigned int           strm_pos;        //!< 29-bit stream position (in bytes) in which the event occurs, if applicable.
    venc_seq_desc_t        *seq_desc;       //!< Pointer to a venc_seq_desc_t structure.
    venc_pic_desc_t        *pic_desc;       //!< Pointer to a venc_pic_desc_t structure.
    venc_1stpass_param_t   *param_1stpass;  //!< Pointer to the first pass parameters  (input).
    venc_1stpass_stat_t    *stat_1stpass;   //!< Pointer to the first pass statistics  (output).
    venc_2ndpass_param_t   *param_2ndpass;  //!< Pointer to the second pass parameters (input).
    venc_2ndpass_stat_t    *stat_2ndpass;   //!< Pointer to the second pass statistics (output).
};


//---------------------------------------------------------------------------
//! \brief      Encoder Event IDs
//---------------------------------------------------------------------------
enum
{
    // No event

    VENC_EVENT_NONE = 0,

    // Stream events

    VENC_EVENT_START_OF_STREAM,
    VENC_EVENT_END_OF_STREAM,

    //Picture events
    VENC_EVENT_INPUT_REFORMAT_END,
    VENC_EVENT_END_OF_PICTURE,
    VENC_EVENT_FIRST_PASS_START,
    VENC_EVENT_FIRST_PASS_END,
    VENC_EVENT_SECOND_PASS_START,
    VENC_EVENT_SECOND_PASS_END,

    // Buffer management events

    VENC_EVENT_ALLOC_ALL,
    VENC_EVENT_FREE_ALL,
    VENC_EVENT_ALLOC_STR_BUF,
    VENC_EVENT_ALLOC_REF_BUF,
    VENC_EVENT_ALLOC_VID_BUF,
    VENC_EVENT_ALLOC_VID422_BUF,
    VENC_EVENT_ALLOC_DS_BUF,
    VENC_EVENT_CHECK_STR_BUF,
    VENC_EVENT_CHECK_REF_BUF,
    VENC_EVENT_CHECK_VID_BUF,
    VENC_EVENT_CHECK_VID422_BUF,
    VENC_EVENT_CHECK_DS_BUF,
    VENC_EVENT_SWITCH_POINT,

    // End of event definitions

    VENC_EVENT_RESERVED
};

enum
{
	VENC_YUV420_TILE = 0,
	VENC_YUV422_UYVY,
	VENC_YUV422_YUYV,
	VENC_YUV420_PLANE,
	VENC_YUV422_PLANE,
	VENC_YUV420_SP,
    VENC_YUV420_SPA,
    VENC_INPUT_UNSUPPORTED
};

//Rate Control Parameters
#define __RCGOP_I       0       // I... only
#define __RCGOP_IP      1       // IP... only
#define __RCGOP_IBP     2       // IBP... with adaptive {BP}->{PP} sub-GOP decision
#define __RCGOP_IBBP    3       // IBBP... with adaptive {BBP}->{PPP} sub-GOP decision
#define __RCGOP_IBBBP   4       // IBBBP... with adaptive {BBBP}->{BP/PP,BP/PP} sub-GOP decision
#define __RCGOP_IBbbP   5       // IBbbP... with adaptive {BbBP}->{BP/PP,BP/PP} sub-GOP decision

enum
{
    VENC_PIC_TYPE_I = 0,        // Intra only
    VENC_PIC_TYPE_P,            // Forward prediction
    VENC_PIC_TYPE_PSKIP,        // Forward prediction, SKIP all the MB, in Panic mode
    VENC_PIC_TYPE_B,            // Bi Prediction
    VENC_PIC_TYPE_BREF,         // Bi Prediction and reconstruct as reference
    VENC_PIC_TYPE_NA,           // non-supported
};

enum
{
    VENC_MODE_EXPRESS = 0,
    VENC_MODE_TURBO,
    VENC_MODE_EXPERT,
    VENC_MODE_UNSUPPORTED
};

#define SIZEOF_COMMON_PACKET_HEADER (16)

//---------------------------------------------------------------------------
// Video Encoder API
//---------------------------------------------------------------------------

const venc_const_t *venc_get_const  (void);                                             //Obsolete, for backward compatibity only
int     venc_open                   (void *venc_cntxt, venc_config_t *venc_config);     //Obsolete, for backward compatibity only
int     venc_configure              (void *vdec_cntxt, venc_config_t *venc_config);     //Obsolete, for backward compatibity only
int     venc_close                  (void *venc_cntxt);                                 //Obsolete, for backward compatibity only
int     venc_switch_stream          (void *strm_cntxt, void *next_strm_cntxt);          //Obsolete, for backward compatibity only
int     venc_stream_switch_out      (void *strm_cntxt);
int     venc_stream_switch_in       (void *strm_cntxt);

int     venc_open_stream            (void *strm_cntxt, venc_strm_config_t *strm_config);
int     venc_configure_stream       (void *strm_cntxt, venc_strm_config_t *strm_config);                //Reserved for future improvements
int     venc_close_stream           (void *strm_cntxt);
int     venc_push_buffer            (void *strm_cntxt, unsigned int type, vdec_buf_desc_t *desc);       //Reserved for future improvements
int     venc_pop_buffer             (void *strm_cntxt, unsigned int type, vdec_buf_desc_t **desc);      //Reserved for future improvements

int     venc_push_video_buffer      (void *strm_cntxt, venc_vid_buf_desc_t *desc);
int     venc_pop_video_buffer       (void *strm_cntxt, venc_vid_buf_desc_t **desc);
int     venc_push_video422_buffer   (void *strm_cntxt, venc_vid_buf_desc_t *desc);
int     venc_pop_video422_buffer    (void *strm_cntxt, venc_vid_buf_desc_t **desc);
int     venc_push_stream_buffer     (void *strm_cntxt, venc_strm_buf_desc_t *desc);
int     venc_pop_stream_buffer      (void *strm_cntxt, venc_strm_buf_desc_t **desc);
int     venc_push_reference_buffer  (void *strm_cntxt, venc_vid_buf_desc_t *desc);
int     venc_pop_reference_buffer   (void *strm_cntxt, venc_vid_buf_desc_t **desc);
int     venc_push_ds_buffer         (void *strm_cntxt, venc_vid_buf_desc_t *desc);
int     venc_pop_ds_buffer          (void *strm_cntxt, venc_vid_buf_desc_t **desc);

int     venc_encode_stream          (void *strm_cntxt, venc_strm_status_t **strm_status);


//---------------------------------------------------------------------------
// Callback Functions to Access Register Space
//          ra:     register address

//---------------------------------------------------------------------------
// unsigned int my_io_rd32          (unsigned int ra);
// void         my_io_wr32          (unsigned int ra, unsigned int val);

//---------------------------------------------------------------------------
// Callback Functions to Access Register Space      //Reserved for future improvements
//          ra:     register address
//          n:      number of 32-bit words
//---------------------------------------------------------------------------
// void     my_reg32_read       (unsigned int ra, unsigned int n, unsigned int *buf, void *user_data);
// void     my_reg32_write      (unsigned int ra, unsigned int n, const unsigned int *buf, void *user_data);
// void     my_reg32_fill       (unsigned int ra, unsigned int n, unsigned int val, void *user_data);
// void     my_reg32_copy       (unsigned int ra_dst, unsigned int ra_src, unsigned int n, void *user_data_src);

//---------------------------------------------------------------------------
// Callback Functions to Access Physical Memory     //Reserved for future improvements
//---------------------------------------------------------------------------
//          pa:     physical address
//          n:      number of 32-bit words
//---------------------------------------------------------------------------
// void     my_mem32_read       (unsigned int pa, unsigned int n, unsigned int *buf, void *user_data);
// void     my_mem32_write      (unsigned int pa, unsigned int n, const unsigned int *buf, void *user_data);
// void     my_mem32_fill       (unsigned int pa, unsigned int n, unsigned int val, void *user_data);
// void     my_mem32_copy       (unsigned int pa_dst, unsigned int pa_src, unsigned int n, void *user_data_dst, void *user_data_src);

//---------------------------------------------------------------------------
// Optional Callback Functions for Debug
//---------------------------------------------------------------------------
// void     my_dbg_panic        (int code);
// int      my_dbg_printf       (const char *format, ...);


#ifdef __cplusplus
}
#endif

#endif
