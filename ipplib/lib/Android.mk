LOCAL_PATH:=$(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_PREBUILT_LIBS := \
	libcodecaacdec.a \
	libcodecaacenc.a \
	libcodecmp3dec.a \
	libcodech263dec.a \
	libcodech264dec.a \
	libcodecmpeg4dec.a \
	libcodecmpeg2dec.a \
	libcodecwmvdec.a \
	libcodecamrnbdec.a \
	libcodecamrnbenc.a \
	libcodecamrwbdec.a \
	libcodecamrwbenc.a \
	libcodecg711.a \
	libcodecjpegdec.a \
	libcodecjpegenc.a \
	libcodecmpeg4enc.a \
	libcodech264enc.a \
	libcodech263enc.a \
	libvmetahal.a \
	libcodecwmadec.a \
	libcodecvmetaenc.a \
	libcodecvmetadec.a \
	libippcam.a \
	libippsp.a \
	libippvp.a

LOCAL_MODULE_TAGS := optional
include $(BUILD_MULTI_PREBUILT)

MY_IPP_STATIC_LIBRARIES := \
	libcodecaacdec \
	libcodecaacenc \
	libcodecmp3dec \
	libcodech263dec \
	libcodech264dec \
	libcodecmpeg4dec \
	libcodecmpeg2dec \
	libcodecwmvdec \
	libcodecamrnbdec \
	libcodecamrnbenc \
	libcodecamrwbdec \
	libcodecamrwbenc \
	libcodecg711 \
	libcodecjpegdec \
	libcodecjpegenc \
        libcodecmpeg4enc \
        libcodech264enc \
        libcodech263enc \
	libvmetahal \

define translate-a-to-so
$(foreach t,$(1), \
  $(eval include $(CLEAR_VARS)) \
  $(eval LOCAL_WHOLE_STATIC_LIBRARIES := $(t)) \
  $(eval LOCAL_MODULE := $(t)) \
  $(eval LOCAL_PRELINK_MODULE := false) \
  $(eval LOCAL_MODULE_TAGS := optional) \
  $(eval include $(BUILD_SHARED_LIBRARY)) \
 )
endef

$(call translate-a-to-so, $(MY_IPP_STATIC_LIBRARIES))

include $(CLEAR_VARS)
LOCAL_WHOLE_STATIC_LIBRARIES := libcodecwmadec
LOCAL_SRC_FILES := rand.c
LOCAL_PRELINK_MODULE := false
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libcodecwmadec
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_WHOLE_STATIC_LIBRARIES := libcodecvmetaenc
LOCAL_SHARED_LIBRARIES += libvmeta libvmetahal
LOCAL_SRC_FILES := getpagesize.c
LOCAL_PRELINK_MODULE := false
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libcodecvmetaenc
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_WHOLE_STATIC_LIBRARIES := libcodecvmetadec
LOCAL_SHARED_LIBRARIES += libvmetahal libvmeta
LOCAL_SRC_FILES := getpagesize.c
LOCAL_PRELINK_MODULE := false
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libcodecvmetadec
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_WHOLE_STATIC_LIBRARIES := libippsp
LOCAL_SHARED_LIBRARIES := libmiscgen
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libippsp
LOCAL_PRELINK_MODULE := false
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_WHOLE_STATIC_LIBRARIES := libippvp 
LOCAL_SHARED_LIBRARIES := libmiscgen libippsp
LOCAL_MODULE := libippvp
LOCAL_MODULE_TAGS := optional
LOCAL_PRELINK_MODULE := false
include $(BUILD_SHARED_LIBRARY)
