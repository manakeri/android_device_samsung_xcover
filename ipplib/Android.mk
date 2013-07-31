LOCAL_PATH := $(call my-dir)

MARVELL_IPP_TOP := $(LOCAL_PATH)

include $(CLEAR_VARS)

include $(MARVELL_IPP_TOP)/lib/Android.mk
include $(MARVELL_IPP_TOP)/example/misc/src/Android.mk

