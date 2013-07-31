/*****************************************************************************************
Copyright (c) 2010, Marvell International Ltd.
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

#ifndef __IPPLV_H__
#define __IPPLV_H__

#include "codecDef.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Calling example of get lib version function*/
/*
    ......
	char libversion[64]={'\0'};
	IppCodecStatus ret;
	ret = GetLibVersion_G722DEC(libversion,sizeof(libversion));
	if(0 == ret){
	    // get lib version successfully
	}else{
	   // fail to get lib version information
	}
	......

*/

/* Speech codec get lib version API declaration*/

IPPCODECAPI(IppCodecStatus, GetLibVersion_GSMAMRENC,(
            Ipp8u *pDst,
            Ipp32s n
            ))
IPPCODECAPI(IppCodecStatus, GetLibVersion_GSMAMRDEC,(
            Ipp8u *pDst,
            Ipp32s n
            ))
IPPCODECAPI(IppCodecStatus, GetLibVersion_AMRWBENC,(
            Ipp8u *pDst,
            Ipp32s n
            ))
IPPCODECAPI(IppCodecStatus, GetLibVersion_AMRWBDEC,(
            Ipp8u *pDst,
            Ipp32s n
            ))
IPPCODECAPI(IppCodecStatus, GetLibVersion_G711,(
            Ipp8u *pDst,
            Ipp32s n
            ))
IPPCODECAPI(IppCodecStatus, GetLibVersion_G722ENC,(
            Ipp8u *pDst,
            Ipp32s n
            ))
IPPCODECAPI(IppCodecStatus, GetLibVersion_G722DEC,(
            Ipp8u *pDst,
            Ipp32s n
            ))
IPPCODECAPI(IppCodecStatus, GetLibVersion_G7221ENC,(
            Ipp8u *pDst,
            Ipp32s n
            ))
IPPCODECAPI(IppCodecStatus, GetLibVersion_G7221DEC,(
            Ipp8u *pDst,
            Ipp32s n
            ))
IPPCODECAPI(IppCodecStatus, GetLibVersion_G723,(
            Ipp8u *pDst,
            Ipp32s n
            ))
IPPCODECAPI(IppCodecStatus, GetLibVersion_G729ENC,(
            Ipp8u *pDst,
            Ipp32s n
            ))
IPPCODECAPI(IppCodecStatus, GetLibVersion_G729DEC,(
            Ipp8u *pDst,
            Ipp32s n
            ))
IPPCODECAPI(IppCodecStatus, GetLibVersion_QCELPENC,(
            Ipp8u *pDst,
            Ipp32s n
            ))
IPPCODECAPI(IppCodecStatus, GetLibVersion_QCELPDEC,(
            Ipp8u *pDst,
            Ipp32s n
            ))

/* Audio codec get lib version API declaration */
IPPCODECAPI(IppCodecStatus, GetLibVersion_SBCENC,(
            Ipp8u *pDst,
            Ipp32s n
            ))
IPPCODECAPI(IppCodecStatus, GetLibVersion_MP3DEC,(
            Ipp8u *pDst,
            Ipp32s n
            ))
IPPCODECAPI(IppCodecStatus, GetLibVersion_AACENC,(
            Ipp8u *pDst,
            Ipp32s n
            ))
IPPCODECAPI(IppCodecStatus, GetLibVersion_AACDEC,(
            Ipp8u *pDst,
            Ipp32s n
            ))
IPPCODECAPI(IppCodecStatus, GetLibVersion_WMADEC,(
            Ipp8u *pDst,
            Ipp32s n
            ))
IPPCODECAPI(IppCodecStatus, GetLibVersion_ZSPDEC,(
            Ipp8u *pDst,
            Ipp32s n
            ))

/* Image codec get version API declaration */
IPPCODECAPI(IppCodecStatus, GetLibVersion_GIFDEC,(
            Ipp8u *pDst,
            Ipp32s n
            ))
IPPCODECAPI(IppCodecStatus, GetLibVersion_JPEGENC,(
            Ipp8u *pDst,
            Ipp32s n
            ))
IPPCODECAPI(IppCodecStatus, GetLibVersion_JPEGDEC,(
            Ipp8u *pDst,
            Ipp32s n
            ))
IPPCODECAPI(IppCodecStatus, GetLibVersion_PNGDEC,(
            Ipp8u *pDst,
            Ipp32s n
            ))

/* Video codec get lib version API declaration */
IPPCODECAPI(IppCodecStatus, GetLibVersion_H263ENC,(
            Ipp8u *pDst,
            Ipp32s n
            ))
IPPCODECAPI(IppCodecStatus, GetLibVersion_H263DEC,(
            Ipp8u *pDst,
            Ipp32s n
            ))
IPPCODECAPI(IppCodecStatus, GetLibVersion_H264ENC,(
            Ipp8u *pDst,
            Ipp32s n
            ))
IPPCODECAPI(IppCodecStatus, GetLibVersion_H264DEC,(
            Ipp8u *pDst,
            Ipp32s n
            ))
IPPCODECAPI(IppCodecStatus, GetLibVersion_MPEG2DEC,(
            Ipp8u *pDst,
            Ipp32s n
            ))
IPPCODECAPI(IppCodecStatus, GetLibVersion_MPEG4ENC,(
            Ipp8u *pDst,
            Ipp32s n
            ))
IPPCODECAPI(IppCodecStatus, GetLibVersion_MPEG4DEC,(
            Ipp8u *pDst,
            Ipp32s n
            ))
IPPCODECAPI(IppCodecStatus, GetLibVersion_VC1DEC,(
            Ipp8u *pDst,
            Ipp32s n
            ))
IPPCODECAPI(IppCodecStatus, GetLibVersion_MPEG2MvedDEC,(
            Ipp8u *pDst,
            Ipp32s n
            ))
IPPCODECAPI(IppCodecStatus, GetLibVersion_MPEG4MvedENC,(
            Ipp8u *pDst,
            Ipp32s n
            ))
IPPCODECAPI(IppCodecStatus, GetLibVersion_MPEG4MvedDEC,(
            Ipp8u *pDst,
            Ipp32s n
            ))
IPPCODECAPI(IppCodecStatus, GetLibVersion_H264MvedENC,(
            Ipp8u *pDst,
            Ipp32s n
            ))
IPPCODECAPI(IppCodecStatus, GetLibVersion_H264MvedDEC,(
            Ipp8u *pDst,
            Ipp32s n
            ))
IPPCODECAPI(IppCodecStatus, GetLibVersion_VmetaENC,(
            Ipp8u *pDst,
            Ipp32s n
            ))
IPPCODECAPI(IppCodecStatus, GetLibVersion_VmetaDEC,(
            Ipp8u *pDst,
            Ipp32s n
            ))
IPPCODECAPI(IppCodecStatus, GetLibVersion_CNMENC,(
            Ipp8u *pDst,
            Ipp32s n
            ))
IPPCODECAPI(IppCodecStatus, GetLibVersion_CNMDEC,(
            Ipp8u *pDst,
            Ipp32s n
            ))
/* Application get lib version API declaration */
IPPCODECAPI(IppCodecStatus, GetLibVersion_VoicePath,(
            Ipp8u *pDst,
            Ipp32s n
            ))
IPPAPI(IppStatus, GetLibVersion_CamEngine,(
            Ipp8u *pDst,
            Ipp32s n
            ))

/* Primitive get lib version API declaration */
IPPAPI(IppStatus, GetLibVersion_IppCam,(
            Ipp8u *pDst,
            Ipp32s n
            ))
IPPAPI(IppStatus, GetLibVersion_IppIE,(
            Ipp8u *pDst,
            Ipp32s n
            ))

IPPAPI(IppStatus, GetLibVersion_IppDeint2D,(
            Ipp8u *pDst,
            Ipp32s n
            ))
IPPAPI(IppStatus, GetLibVersion_libippsp,(
            Ipp8u *pDst,
            Ipp32s n
            ))

/* OpenMax get lib version API declaration*/
/*core*/
IPPCODECAPI(IppCodecStatus, GetLibVersion_omx_basecore,(Ipp8u *pDst, Ipp32s n))

/*audio components*/
IPPCODECAPI(IppCodecStatus, GetLibVersion_omx_aacdec,(Ipp8u *pDst, Ipp32s n))
IPPCODECAPI(IppCodecStatus, GetLibVersion_omx_aacenc,(Ipp8u *pDst, Ipp32s n))
IPPCODECAPI(IppCodecStatus, GetLibVersion_omx_amrnbdec,(Ipp8u *pDst, Ipp32s n))
IPPCODECAPI(IppCodecStatus, GetLibVersion_omx_amrnbenc,(Ipp8u *pDst, Ipp32s n))
IPPCODECAPI(IppCodecStatus, GetLibVersion_omx_amrwbdec,(Ipp8u *pDst, Ipp32s n))
IPPCODECAPI(IppCodecStatus, GetLibVersion_omx_amrwbenc,(Ipp8u *pDst, Ipp32s n))
IPPCODECAPI(IppCodecStatus, GetLibVersion_omx_mp3dec,(Ipp8u *pDst, Ipp32s n))
IPPCODECAPI(IppCodecStatus, GetLibVersion_omx_wmadec,(Ipp8u *pDst, Ipp32s n))

/*image components*/
IPPCODECAPI(IppCodecStatus, GetLibVersion_omx_jpegdec,(Ipp8u *pDst, Ipp32s n))
IPPCODECAPI(IppCodecStatus, GetLibVersion_omx_jpegenc,(Ipp8u *pDst, Ipp32s n))

/*other components*/
IPPCODECAPI(IppCodecStatus, GetLibVersion_omx_rendererpcm,(Ipp8u *pDst, Ipp32s n))
IPPCODECAPI(IppCodecStatus, GetLibVersion_omx_ivrendereryuvoverlay,(Ipp8u *pDst, Ipp32s n))
IPPCODECAPI(IppCodecStatus, GetLibVersion_omx_mp4demuxer,(Ipp8u *pDst, Ipp32s n))
IPPCODECAPI(IppCodecStatus, GetLibVersion_omx_clock,(Ipp8u *pDst, Ipp32s n))

/*video components*/
IPPCODECAPI(IppCodecStatus, GetLibVersion_omx_h264dec,(Ipp8u *pDst, Ipp32s n))
IPPCODECAPI(IppCodecStatus, GetLibVersion_omx_h264decmved,(Ipp8u *pDst, Ipp32s n))
IPPCODECAPI(IppCodecStatus, GetLibVersion_omx_h264enc,(Ipp8u *pDst, Ipp32s n))
IPPCODECAPI(IppCodecStatus, GetLibVersion_omx_h264encmved,(Ipp8u *pDst, Ipp32s n))
IPPCODECAPI(IppCodecStatus, GetLibVersion_omx_h263dec,(Ipp8u *pDst, Ipp32s n))
IPPCODECAPI(IppCodecStatus, GetLibVersion_omx_h263enc,(Ipp8u *pDst, Ipp32s n))

IPPCODECAPI(IppCodecStatus, GetLibVersion_omx_mpeg4aspdec,(Ipp8u *pDst, Ipp32s n))
IPPCODECAPI(IppCodecStatus, GetLibVersion_omx_mpeg4aspdecmved,(Ipp8u *pDst, Ipp32s n))
IPPCODECAPI(IppCodecStatus, GetLibVersion_omx_mpeg4dec,(Ipp8u *pDst, Ipp32s n))
IPPCODECAPI(IppCodecStatus, GetLibVersion_omx_mpeg4enc,(Ipp8u *pDst, Ipp32s n))
IPPCODECAPI(IppCodecStatus, GetLibVersion_omx_wmvdec,(Ipp8u *pDst, Ipp32s n))
IPPCODECAPI(IppCodecStatus, GetLibVersion_omx_wmvdecmved,(Ipp8u *pDst, Ipp32s n))
IPPCODECAPI(IppCodecStatus, GetLibVersion_omx_codadx8dec,(Ipp8u *pDst, Ipp32s n))
IPPCODECAPI(IppCodecStatus, GetLibVersion_omx_codadx8enc,(Ipp8u *pDst, Ipp32s n))
IPPCODECAPI(IppCodecStatus, GetLibVersion_omx_vmetadec,(Ipp8u *pDst, Ipp32s n))
IPPCODECAPI(IppCodecStatus, GetLibVersion_omx_vmetaenc,(Ipp8u *pDst, Ipp32s n))

/*misc*/
IPPCODECAPI(IppCodecStatus, GetLibVersion_omx_ippomxmem,(Ipp8u *pDst, Ipp32s n))
IPPCODECAPI(IppCodecStatus, GetLibVersion_omx_ippomxmemmved,(Ipp8u *pDst, Ipp32s n))

#ifdef __cplusplus
}
#endif

#endif

