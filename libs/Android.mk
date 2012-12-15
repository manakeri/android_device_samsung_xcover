LOCAL_PATH := $(my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_PREBUILT_LIBS := libpmemhelper.so libphycontmem.so libgcu.so libbmm.so

include $(BUILD_MULTI_PREBUILT)

