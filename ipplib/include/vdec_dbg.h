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

#ifndef VDEC_DBG_H_
#define VDEC_DBG_H_


#include "vdec_api.h"


#ifdef __cplusplus
extern "C"
{
#endif


//---------------------------------------------------------------------------
// Macros
//---------------------------------------------------------------------------
#ifndef TYPEDEF_STRUCT
#define TYPEDEF_STRUCT(x_)      typedef struct x_ x_; struct x_
#endif

//---------------------------------------------------------------------------
// Stream-independent VMeta Internal Settings
//---------------------------------------------------------------------------
TYPEDEF_STRUCT(vmeta_icfg_t)
{
    unsigned char clkgt_mode;   // 0=auto; 1=enable clk gating; 2=clk always on;
    unsigned char rsvd[3];
    unsigned int internal_flags;
};
typedef vmeta_icfg_t vdec_icfg_t;
typedef vmeta_icfg_t venc_icfg_t;

//---------------------------------------------------------------------------
// Per-stream Internal Settings
//---------------------------------------------------------------------------
TYPEDEF_STRUCT(vdec_strm_icfg_t)
{
    unsigned char parser_mode;      // 0=auto; 1=single-pass; 2=dual-pass;
    unsigned char dblk_mode;        // 0=auto; 1=normal; 2=ref-only; 3=disabled;
    unsigned char bump_mode;        // 0=auto; 1=normal; 2=greedy;
    unsigned char csw_mode;         // 0=auto; 1=per frame; 2=any time;
    unsigned char force_ref_out;    // 0=disabled; 1=enabled;
    unsigned char mvc_mode;         // 0=auto; 1=disabled; 2=enabled;
    unsigned char aso_mode;         // 0=auto; 1=disabled; 2=enabled;
    unsigned char divx_it_mode;     // 0=auto; 1=disabled; 2=enabled;
    unsigned char rsvd[3];
    unsigned char mb_timeout;       // 0=auto; ~0=disabled;
    unsigned short fewd_wmax;       // 0=auto;
    unsigned short fewd_imax;       // 0=auto;
    unsigned short bewd_wmax;       // 0=auto;
    unsigned short bewd_imax;       // 0=auto;
    unsigned short eopwd_wmax;      // 0=auto;
    unsigned short eopwd_imax;      // 0=auto;
};

//---------------------------------------------------------------------------
// Performance Counters
//---------------------------------------------------------------------------
TYPEDEF_STRUCT(vmeta_perf_t)
{
    unsigned int mb_cnt;
    unsigned int slc_cnt;
    unsigned int f0_run;
    unsigned int f0_stl;
    unsigned int f1_run;
    unsigned int f1_stl;
    unsigned int f2_run;
    unsigned int f2_stl;
    unsigned int f3_run;
    unsigned int f3_stl;
    unsigned int p0_run;
    unsigned int p0_stl;
    unsigned int p0_bbl;
    unsigned int p0_sem;
    unsigned int p0_ins;
    unsigned int p1_run;
    unsigned int p1_stl;
    unsigned int p1_bbl;
    unsigned int p1_sem;
    unsigned int p1_ins;
};
typedef vmeta_perf_t vdec_perf_t;
typedef vmeta_perf_t venc_perf_t;

//---------------------------------------------------------------------------
// VMeta Statistics
//---------------------------------------------------------------------------
TYPEDEF_STRUCT(vmeta_stat_t)
{
    unsigned int dummy;
};

//---------------------------------------------------------------------------
// VMeta Stream Debug Object
//---------------------------------------------------------------------------
TYPEDEF_STRUCT(vmeta_strm_dbg_t)
{
    void *strm;
    unsigned int strm_id;
};

//---------------------------------------------------------------------------
// VMeta Decoder Debug Object
//---------------------------------------------------------------------------
TYPEDEF_STRUCT(vmeta_dbg_t)
{
    void *vmeta;
    vmeta_strm_dbg_t strm_dbg[4];
    unsigned int trace_line;
    char *trace_file;
    unsigned int sp_base;
    unsigned int sp_max;
    int dec_func_entered;
    int buf_func_entered;
    vmeta_stat_t *stat;
    vmeta_perf_t *perf;
    unsigned int agk_en;
    unsigned int agk_delay0;
    unsigned int agk_delay1;
    unsigned int agk_wait;
    unsigned int wdlog[64][6];
    unsigned int wdlog_idx;
};


extern vmeta_dbg_t *g_vmeta_dbg;

enum
{
    ICFG_CPU_PARSER     = 1 << 0,
    ICFG_CPU_SYNPRO     = 1 << 1,
    ICFG_CPU_P3CTL      = 1 << 2,
    ICFG_NO_MB          = 1 << 3,
    ICFG_NO_ASP         = 1 << 4,
    ICFG_CABAC_ONLY     = 1 << 5,
	ICFG_CPU_MEECTL     = 1 << 6,
};

//---------------------------------------------------------------------------
// Video Decoder Reserved EVENT/STATE IDs
//---------------------------------------------------------------------------
enum
{
    VDEC_TOTAL_API_EVENTS = VDEC_EVENT_RESERVED,

    // Decoder states

    VDEC_STATE_WAIT_FOR_START,
    VDEC_STATE_WAIT_FOR_SLICE_DONE,
    VDEC_STATE_WAIT_FOR_PICTURE_DONE,

    // End of event definitions

    VDEC_TOTAL_EVENTS
};


int vdec_debug (void *strm_cntxt, void *x);
unsigned int vmeta_dbg_get_sp(void);


#ifdef __cplusplus
}
#endif

#endif