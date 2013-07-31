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

#define RESERVED_PARAM(par) (par)=(par)

/* return actual flush bytes */
int miscgStreamFlush(void *pStream, void *pStreamHandler, int nAvailBytes, int flag)
{ 
	IPP_FILE *fp = (IPP_FILE*)pStreamHandler;
    int bytesWritten, total=0;
    int retryTimes = 5;
    RESERVED_PARAM(flag);

    for(;retryTimes>0;retryTimes--) {
        bytesWritten = IPP_Fwrite((char*)pStream+total, 1, nAvailBytes-total, fp);
        total += bytesWritten;
        if(total>=nAvailBytes) {
            break;
        }
    }
    return total;
}

/* Read data from a file */
int miscFileRead(void ** pbuffer, int size, int count, IPP_FILE* file)
{
    return IPP_Fread(*pbuffer, size, count, file);
}

/* init callback table */
int miscInitGeneralCallbackTable(MiscGeneralCallbackTable **ppDstCBTable)
{
    MiscGeneralCallbackTable *pTable = NULL;
    
    int ret = IPP_MemMalloc((void**)&pTable,sizeof(MiscGeneralCallbackTable),4);
    if (IPP_FAIL == ret || NULL == pTable) {
		IPP_Log(NULL,"w","Fails to allocate callback tables!\n");
        return -1;
    }

	pTable->fMemMalloc = (MiscMallocCallback)IPP_MemMalloc;
	pTable->fMemCalloc = (MiscCallocCallback)IPP_MemCalloc;
    pTable->fMemFree   = (MiscFreeCallback)IPP_MemFree;
	pTable->fStreamRealloc = (MiscStreamReallocCallback)IPP_MemRealloc;
    pTable->fStreamFlush   = (MiscStreamFlushCallback)miscgStreamFlush;
	pTable->fFileSeek = (MiscFileSeekCallback)IPP_Fseek;
	pTable->fFileRead = (MiscFileReadCallback)miscFileRead;
	pTable->fFileWrite = (MiscWriteFileCallBack)IPP_Fwrite;
    (*ppDstCBTable) = pTable;

    return 0;
}

/* free callback table */
int miscFreeGeneralCallbackTable(MiscGeneralCallbackTable **ppSrcCBTable)
{
    if (NULL != (*ppSrcCBTable)) {
        IPP_MemFree ((void**)ppSrcCBTable);
        (*ppSrcCBTable) = NULL;
    }
	return 0;
}

void IppStatusMessage(IppCodecStatus status, char* message)
{
	char err[128]={0};
	
	if(NULL == message)
		return;

	IPP_Memset(message,0,MAX_MESG_LEN);
	switch(status)
	{
		case IPP_STATUS_INIT_ERR:
			IPP_Strcpy(message,"init is failed");
			break;
		case IPP_STATUS_BUFFER_UNDERRUN:
			IPP_Strcpy(message,"buffer is under-run");
			break;
		case IPP_STATUS_FRAME_ERR:
			IPP_Strcpy(message,"frame is error");
			break;
		case IPP_STATUS_FRAME_HEADER_INVALID:
			IPP_Strcpy(message,"frame header is invalid");
			break;
		case IPP_STATUS_FRAME_UNDERRUN:
			IPP_Strcpy(message,"frame is under-run");
			break;
		
		case IPP_STATUS_MP4_SHORTHEAD:
			IPP_Strcpy(message,"mp4 short head");
			break;
	
		case IPP_STATUS_DTMF_NOTSUPPORTEDFS:
			IPP_Strcpy(message,"not supported sample rate");
			break;

		case IPP_STATUS_STREAMFLUSH_ERR:
			IPP_Strcpy(message,"stream flush error");
			break;
		case IPP_STATUS_BUFOVERFLOW_ERR:
			IPP_Strcpy(message,"buffer overflow error");
			break;
		case IPP_STATUS_NOTSUPPORTED_ERR:
			IPP_Strcpy(message,"not supported error");
			break;
		case IPP_STATUS_MISALIGNMENT_ERR:
			IPP_Strcpy(message,"misaligment error");
			break;
		case IPP_STATUS_BITSTREAM_ERR:
			IPP_Strcpy(message,"bit stream error");
			break;
		case IPP_STATUS_INPUT_ERR:
			IPP_Strcpy(message,"input error");
			break;
		case IPP_STATUS_SYNCNOTFOUND_ERR:
			IPP_Strcpy(message,"sync not found error");
			break;
		case IPP_STATUS_BADARG_ERR:
			IPP_Strcpy(message,"bad arguments error");
			break;
		case IPP_STATUS_NOMEM_ERR:
			IPP_Strcpy(message,"no memory error");
			break;
		case IPP_STATUS_ERR:
			IPP_Strcpy(message,"common status error");
			break;

		case IPP_STATUS_NOTSUPPORTED:
			IPP_Strcpy(message,"not supported warning");
			break;

		case IPP_STATUS_FATAL_ERR:
			IPP_Strcpy(message,"fatal error");
			break;

		default:
			IPP_Strcpy(message,"undefined error");
	}
}