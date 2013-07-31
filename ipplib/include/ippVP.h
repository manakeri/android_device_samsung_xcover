/******************************************************************************
//      (C) Copyright 2008 Marvell International Ltd.
//          All Rights Reserved	
//
//  Description:
//      voicepath library external header
//
********************************************************************************/

#ifndef _IPPVP_H_
#define _IPPVP_H_

#include "codecDef.h"

#ifdef __cplusplus
extern "C" {
#endif

#define VOICEPATH_SAMPLEPERFRAME	160
#define VOICEPATH_INPUTOUTPUTALIGN	8

typedef enum {
	CFG_ESNOECHO_MODE,
	CFG_ES_MODE,
	CFG_SP_MODE,
	CFG_HF_MODE,
	CFG_ECHO64MS_MODE,
	CFG_ECHO128MS_MODE,
}CFG_AUXAEC_MODE;

typedef enum {
	CFG_TX_PATH,
	CFG_RX_PATH,
	CFG_BOTH_PATH,
}CFG_PATH;

typedef struct {
	//AEC
	int				auxaec_mode;
	int				path;
}voicepath_cfg_t;

typedef enum {
	
	//Change CFG_AUXAEC_MODE for TX and RX. If the new mode is the same as last mode, nothing happens.
	//example:
	//CFG_AUXAEC_MODE	newMode = CFG_ES_MODE;
	//VoicePathSendCmd(pVPState, VOICEPATH_CMD_CHANGEAUXAECMODE, &newMode, NULL);
	VOICEPATH_CMD_CHANGEAUXAECMODE,

	VOICEPATH_CMD_COUNT,
} VPCMD;

typedef struct
{
    unsigned short noOfTaps;
    unsigned short params[31];
	unsigned char  description[16];
} ACM_EqParam;


//ICAT EXPORTED STRUCT
typedef struct
{
    unsigned short nsMaxSupress;
    unsigned short nsMaxNoise;
	unsigned char  description[16];
} ACM_NSParam;


//ICAT EXPORTED STRUCT
typedef struct
{
  signed short   sideToneGain;
  signed short   Tap0;
  signed short   Tap1;
  unsigned char  description[16];
} ACM_SideToneParam;

//ICAT EXPORTED STRUCT
typedef struct
{
  unsigned short mode;
  unsigned short sampleDelay;
  unsigned short ecLmsBurst;
  /* atention factor */
  unsigned short  attenuationFactorValid; //update from ACM code, change from unsigned char to unsigned short
  unsigned short attenuationFactor1;
 /*echo to noise */
  unsigned short  echoToNoiseThValid;  //update from ACM code, change from unsigned char to unsigned short
  unsigned short echoToNoiseThresFactor;
  unsigned short maxAgcGain_Tx;
  unsigned short maxAgcGain_Rx;
  /* min envelop Prms */
  unsigned short minEnvelopeTx;
  unsigned short minEnvelopeRx;
  unsigned short lowAmbientNoise;
  unsigned short level1_Tx;
  unsigned short level1_Rx;
  /* drc gain */
  unsigned short drcGainFactor;
  unsigned short maxDrcGain;
  unsigned short additionalGain;
  unsigned char  description[16];
  /* init period for Tx */
  unsigned short initPeriodTx;
  /* EXT Analog Gain */
  unsigned short refThExp;
  unsigned short peakInThExp;
  unsigned short resRefThExp;
  /* init period for Rx */
  unsigned short initPeriodRx;
} ACM_EC_AVCParam ;

#define DEVICE_TABLE_FILENAME  "audio_device.nvm"
#define EQ_TABLE_FILENAME      "audio_eq.nvm"
#define NS_TABLE_FILENAME  	   "audio_ns.nvm"
#define SIDETONE_FILENAME      "audio_sidetone.nvm"
#define EC_AND_AVC_FILENAME    "audio_ec_and_avc.nvm"
#define CTM_FILENAME    	   "audio_ctm.nvm"
#define EC_FIR_TAPS_SP_FILENAME   "audio_ec_fir_taps_sp.nvm"
#define TOOL_DEVICE_TABLE_FILENAME  "audio_tool_device.nvm"

typedef enum
{
  EQ_TABLE = 0,
  NS_TABLE,
  SIDE_TONE_TABLE,
  EC_AVC_TABLE,
  EC_FIR_TAPS_SP_TABLE,
  CTM_TABLE,
  DEVICE_TABLE,

  MAX_FDI_TABLE_TYPES
}FdiTableTypeName;

IppCodecStatus VoicePathInit(voicepath_cfg_t* pVPCfg, void** ppVPState, MiscGeneralCallbackTable* pCallbackTable);
IppCodecStatus VoicePathTx(void* pVPState, Ipp16s* pSrcDstSpeech, Ipp16s* pSrcRefSpeech, Ipp16s nSampleCnt);
IppCodecStatus VoicePathRx(void* pVPState, Ipp16s* pSrcDstSpeech, Ipp16s nSampleCnt);
IppCodecStatus VoicePathFree(void** ppVPState);
IppCodecStatus VoicePathSendCmd(void* pVPState, int cmd, void* pCmdValue);
void VoicePathSetParams(int tableType, int mode, int path, void* pParms);

#ifdef __cplusplus
}
#endif

#endif    /* #ifndef _IPPVP_H_ */

/* EOF */
