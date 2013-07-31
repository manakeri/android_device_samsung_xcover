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


#ifndef _IPPIE_H
#define _IPPIE_H

#include "ippdefs.h"
#include "codecDef.h"

typedef struct _IppUsmfilterParam {
	float fAmount;
    float fRadius;
    int iThreshold;
} IppUsmfilterParam;

typedef enum _IppiFormat {
    /* BGR, B is at low bits, R is at high bits */
    FORMAT_BGRA,	// Alpha channel is at the highest bits
    FORMAT_BGR888,
    FORMAT_BGR555,
    FORMAT_BGRA5551,
    FORMAT_BGR565,
    FORMAT_GRAY8,

    /* RGB, R is at low bits, B is at high bits */
    FORMAT_RGBA,	// Alpha channel is at the highest bits
    FORMAT_RGB888,
    FORMAT_RGB555,
    FORMAT_RGBA5551,
    FORMAT_RGB565,

	/* YUV */
	FORMAT_YUV420PLANAR,
	FORMAT_YUYVPACKED,
	FORMAT_UYVYPACKED,	// U, Y, V, Y are at the 1st, 2nd, 3rd and 4th byte accordingly
} IppiFormat;


IPPAPI(IppStatus, ippiUSMSharpen, (Ipp8u *pSrcRGB, unsigned int phAddrSrcRGB, int iSrcStep, IppiFormat eSrcFormat,
	   Ipp8u *pDstRGB, unsigned int phAddrDstRGB, int iDstStep, IppiFormat eDstFormat,
	   int iWidth, int iHeight,
	   float fRadius, int iThreshold, float fAmount,
	   float *pReserved1, int iReserved2, Ipp16s *pReserved3))



/*****************************************************************
*	Deinterlace Related Deinitions
*****************************************************************/

typedef void IppDeinterlaceState;

#define DEINT_OFF		0
#define DEINT_9TAP		1
#define DEINT_VT		2

#define BASE_ALIGNMENT		64		/* base address alignment (in bytes) */
#define STRIDE_ALIGNMENT	8		/* stride alignment (in pixel) */

/////////////////////////////////////////////////////////////////////////////
//	Video Post Processing for deinterlace / resize and color space conversion
//
//	All supported conversions are listed as below:
//	-----------------------------------------------------------------------
//	Input        Output             Deinterlace     Resize		Comment
//	-----------------------------------------------------------------------
//	420p/422i    RGB(A)xxx(x)       vt              off			Not suggested
//	420p/422i    RGB(A)xxx(x)       off/9tap        on			Two-GC-steps (vertical and horizontal filter blit)
//	420p/422i    <same as input>    9tap            off			Single-GC-step (vertical filter blit)
//	-----------------------------------------------------------------------
/////////////////////////////////////////////////////////////////////////////

IPPAPI(IppStatus, ippiDeinterlaceResizeCsc_InitAlloc,
	   (int iSrcWidth, int iSrcHeight, IppiFormat iSrcFormat,
	   int iDstWidth, int iDstHeight, IppiFormat iDstFormat,
	   int iDeinterlace,
	   MiscGeneralCallbackTable *pCallbackTable, IppDeinterlaceState **ppDeinterlaceState))

// IndexHint ranges [0, 31], smaller or larger values will be wrap around into this range.
// For same combination of pSrc[]/pSrcPhyAddr[]/iSrcStep[], pls use the same IndexHint (0~31).
// Use a different IndexHint causes unneccasary GC buffer creating.
// For each combination of pSrc[]/pSrcPhyAddr[]/iSrcStep[], pls use a different IndexHint (0~31).
// Sharing IndexHint causes GC buffer recreating.
// There's no problem if an out dated IndexHint is re-used for other buffers
// iSrcIndexHint and iDstIndexHint are irrelevant
IPPAPI(IppStatus, ippiDeinterlaceResizeCsc_Process,
	   (Ipp8u *pSrc[], void *pSrcPhyAddr[], int iSrcStep[], int iSrcIndexHint,
	   Ipp8u *pDst[], void *pDstPhyAddr[], int iDstStep[], int iDstIndexHint,
	   void* pGCFence /* Reserved */,
	   IppDeinterlaceState *pDeinterlaceState))

IPPAPI(IppStatus, ippiDeinterlaceResizeCsc_Wait,
	   (void* pGCFence /* Reserved */, IppDeinterlaceState *pDeinterlaceState))

IPPAPI(IppStatus, ippiDeinterlaceResizeCsc_Free, (IppDeinterlaceState **ppDeinterlaceState))




#endif
/* EOF */


