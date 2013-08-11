LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_C_INCLUDES += \
	$(LOCAL_PATH)	\
	$(LOCAL_PATH)/../../../include/

#LOCAL_CFLAGS += -mabi=aapcs-linux

LOCAL_SRC_FILES:= \
	misc.c \
	wmmx2_linux/common.c \
	wmmx2_linux/perf.c  \
	wmmx2_linux/render.c  \
	wmmx2_linux/thread.c

LOCAL_PRELINK_MODULE := false

LOCAL_MODULE := libmiscgen

include $(BUILD_SHARED_LIBRARY)

