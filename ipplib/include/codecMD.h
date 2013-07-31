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
#ifndef _CODECMD_H_
#define	_CODECMD_H_

/* General Codec external header file*/
#include "codecDef.h"


#ifdef __cplusplus
extern  "C"
{
#endif

/* ========================== Data Type Definition ========================= */
typedef IppCodecStatus (*IppMidiCallback)();

/* ========================== Constant Definition ========================== */
/* waveform data format flags */
#define IPP_MIDI_MONO				1
#define IPP_MIDI_STEREO				2

#define IPP_MIDI_8BPS				8		
#define	IPP_MIDI_16BPS				16

//4 sampling rate: 8000,11025,22050,44100
#define	IPP_MIDI_8000_HZ			0
#define	IPP_MIDI_11025_HZ			1
#define	IPP_MIDI_22050_HZ			4			
#define	IPP_MIDI_44100_HZ			7

/* stream access control parameter values */
#define	IPP_MIDI_STREAM_MEMRES		0	/* contiguous block, memory resident */
#define IPP_MIDI_STREAM_CB			1	/* piecewise random access, uses CB
                                        // interface */
/* type of access to shared memory */
#define	IPP_MIDI_MEM_READONLY		0	/* read only access to shared memory */
#define	IPP_MIDI_MEM_READWRITE		1	/* read write access to shared memory */

/* status of shared memory */										
#define	IPP_MIDI_MEM_CREATED		0	/* newly allocated by the calling process */
#define	IPP_MIDI_MEM_RESIDENT		1	/* allocated by previous processes */

/* wavetable callback seek parameter values */
#define	IPP_MIDI_CB_SEEK_SET		0	/* seek offset from first byte */
#define	IPP_MIDI_CB_SEEK_CUR		1	/* seek offset from current byte */
#define	IPP_MIDI_CB_SEEK_END		2	/* seek offset from last byte */

/* frame length in milliseconds */		
#define IPP_MIDI_FRAME_LEN		1

/* midi events type */
#define	 MI_NONE				0x00
#define  MI_TEMPO				0x10
#define  MI_EOT					0x20
#define  MI_NOTEOFF             0x80
#define  MI_NOTEON              0x90
#define  MI_POLYPRESSURE        0xA0
#define  MI_CONTROLCHANGE		0xB0
#define  MI_PATCHCHANGE         0xC0
#define  MI_CHANPRESSURE        0xD0
#define  MI_PITCHBEND           0xE0
#define  MI_SYSEX               0xF0

/* ======================== Functional Macro Definition ===================== */

/* ============================ Table Declarations ========================= */

/* ========================== Structures Definitions ======================= */

/* 1. midi source media stream struct */
typedef struct _IppMidiStream
{
	void	*pId;			/* bitstream identifier */
	void	*pBuf;			/* bitstream buffer */
	int		bufLen;			/* bitstream buffer length, in bytes */
	void	*pCurByte;		/* pointer to the current bitstream byte */
	int		curBitOffset;	/* index of the current bit in *pCurByte */
	int		mode;			/* IPP_MIDI_STREAM_YYYY, i.e., callback or memory resident */
} IppMidiStream;

/* 2. midi output sound definition */
typedef struct
{
	void	*pSndFrame;
	int		sndSampleRate;
	int		sndChannelNum;
	int		sndBitsPerSample;
	int		sndLen;	
}	IppMidiOutSound;

/* Callback table */
typedef struct _IppMidiDecoderCBTable
{
	IppMidiCallback midiOpenWT;		/* wt_open() callback for wavetableMode==IPP_MIDI_WT_CB */
	IppMidiCallback midiCloseWT;	/* wt_close() callback for wavetableMode==IPP_MIDI_WT_CB */
	IppMidiCallback midiReadWT;		/* wt_block read() callback for wavetableMode==IPP_MIDI_WT_CB */
	IppMidiCallback midiSeekWT;		/* wt_seek() callback for wavetableMode==IPP_MIDI_WT_CB */
	IppMidiCallback midiTellWT;		/* wt_tell() callback for wavetableMode==IPP_MIDI_WT_CB */
	IppMidiCallback midiOpenMF;		/* input stream open() callback */
	IppMidiCallback midiCloseMF;	/* input stream close() callback */
	IppMidiCallback midiReadMF;		/* input stream read() callback */
	IppMidiCallback midiSeekMF;		/* input stream seek() callback */
	IppMidiCallback midiTellMF;		/* input stream tell() callback */
	IppMidiCallback midiMemAlloc;	/* memory allocation callback */
	IppMidiCallback midiMemFree;	/* memory release callback */
} IppMidiDecoderCBTable;

/* memory share attributes struct */
typedef struct _IppMidiMemShareAttributes
{
	int		shmId;			/* identifier of the shared memory blocks */
	int		shmFlag;		/* IPP_MIDI_MEM_READONLY or IPP_MIDI_MEM_READWRITE */
	int		shmStatus;		/* IPP_MIDI_MEM_CREATED or IPP_MIDI_MEM_RESIDENT */
	void	*pShmReserved;	/* reserved for users */
} IppMidiMemShareAttributes;

/* 3. Midi decoder configuration options struct */
typedef struct _IppMidiDecoderParams
{
	int		pcmMode;						/* IPP_MIDI_MONO or IPP_MIDI_STEREO */
	int		pcmBitsPerSample;				/* IPP_MIDI_8BPS or IPP_MIDI_16BPS */
	int		pcmSampleRate;					/* IPP_MIDI_XXXXX_HZ, e.g., IPP_MIDI_22050_HZ */
	int		numFramesPerBatch;				/* 1,2,3,... */
	int		polyphony;						/* 24 - 48 */
	int		volL;							/* 0 - 127, 0 = mute, 127 = no attenuation */
	int		volR;							/* 0 - 127, 0 = mute, 127 = no attenuation */
	int		reverbEnable;					/* 0 - disable, 1 - enable */
	int		chorusEnable;					/* 0 - disable, 1 - enable */
	IppMidiDecoderCBTable	callbackTable;  /* Callback table */

} IppMidiDecoderParams;

/* 4. Midi event struct*/
typedef struct 
{
	Ipp32s deltaTime;
	Ipp8u channel, type, data1, data2;
	void *pExt;
}IppMidiEvent;

/* 5. Midi event decoder configuration options struct */
typedef struct
{
	Ipp32u	division;
	Ipp32u	tempo; 

} IppMidiEventSynParams;


extern IppCodecStatus DecoderInitAlloc_MIDI (IppMidiStream	*pSrcWavetable,
IppMidiOutSound	 *pDstPcmAudio, IppMidiDecoderParams *pSrcMidiConfig, 
void **ppDstDecoderState);

/* APIs for stream decoder */
extern IppCodecStatus DecoderOpenStream_MIDI(IppMidiStream	*pSrcMidiBits,
											 IppMidiDecoderParams *pSrcMidiConfig,
											 void  *pSrcDstDecoderState);

extern IppCodecStatus DecodeStream_MIDI(IppMidiStream *pSrcMidiBits,
IppMidiOutSound	*pDstPcmAudio, IppMidiStream *pSrcWavetable,
IppMidiDecoderParams *pSrcMidiConfig, void *pSrcDstDecoderState);

extern IppCodecStatus DecoderCloseStream_MIDI(IppMidiStream *pSrcMidiBits,
IppMidiDecoderParams *pSrcMidiConfig, void *pSrcDstDecoderState);

/* APIs for event decoder*/
extern IppCodecStatus DecoderEventInit_MIDI(IppMidiEventSynParams *pEventParms,
	void  *pSrcDstDecoderState);

extern IppCodecStatus DecodeEvent_MIDI(IppMidiEvent midiEvent,
IppMidiOutSound	*pDstPcmAudio, IppMidiStream *pSrcWavetable,
IppMidiDecoderParams *pSrcMidiConfig, void *pSrcDstDecoderState);

extern IppCodecStatus DecoderFree_MIDI(IppMidiStream *pSrcWavetable,
IppMidiDecoderParams *pSrcMidiConfig, void **ppSrcDecoderState);

#ifdef __cplusplus
}
#endif

#endif
