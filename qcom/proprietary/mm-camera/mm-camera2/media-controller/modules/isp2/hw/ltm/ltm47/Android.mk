#======================================================================
#makefile for libmmcamera2_isp2_ltm47.so form mm-camera2
#======================================================================
ifeq ($(call is-vendor-board-platform,QCOM),true)
BOARD_PLATFORM_LIST := msm8996
BOARD_PLATFORM_LIST += apq8098_latv
BOARD_PLATFORM_LIST += msm8998
BOARD_PLATFORM_LIST += sdm660
BOARD_PLATFORM_LIST += $(TRINKET)
ifeq ($(call is-board-platform-in-list,$(BOARD_PLATFORM_LIST)),true)

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_LDFLAGS := $(mmcamera_debug_lflags)

LOCAL_CFLAGS :=  -DAMSS_VERSION=$(AMSS_VERSION) \
  $(mmcamera_debug_defines) \
  $(mmcamera_debug_cflags)

LOCAL_CFLAGS  += -Werror

ifeq ($(32_BIT_FLAG), true)
LOCAL_32_BIT_ONLY := true
endif

LOCAL_MMCAMERA_PATH := $(LOCAL_PATH)/../../../../../../

LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_C_INCLUDES += $(LOCAL_MMCAMERA_PATH)/includes/

LOCAL_C_INCLUDES += $(LOCAL_MMCAMERA_PATH)/server-tuning/tuning/

LOCAL_C_INCLUDES += $(LOCAL_MMCAMERA_PATH)/media-controller/includes/

LOCAL_C_INCLUDES += $(LOCAL_MMCAMERA_PATH)/media-controller/mct/bus/
LOCAL_C_INCLUDES += $(LOCAL_MMCAMERA_PATH)/media-controller/mct/controller/
LOCAL_C_INCLUDES += $(LOCAL_MMCAMERA_PATH)/media-controller/mct/event/
LOCAL_C_INCLUDES += $(LOCAL_MMCAMERA_PATH)/media-controller/mct/module/
LOCAL_C_INCLUDES += $(LOCAL_MMCAMERA_PATH)/media-controller/mct/object/
LOCAL_C_INCLUDES += $(LOCAL_MMCAMERA_PATH)/media-controller/mct/pipeline/
LOCAL_C_INCLUDES += $(LOCAL_MMCAMERA_PATH)/media-controller/mct/port/
LOCAL_C_INCLUDES += $(LOCAL_MMCAMERA_PATH)/media-controller/mct/stream/
LOCAL_C_INCLUDES += $(LOCAL_MMCAMERA_PATH)/media-controller/mct/tools/

LOCAL_C_INCLUDES += $(LOCAL_MMCAMERA_PATH)/media-controller/modules/includes/
LOCAL_C_INCLUDES += $(LOCAL_CHROMATIX_PATH)
LOCAL_C_INCLUDES += $(LOCAL_MMCAMERA_PATH)/media-controller/modules/stats/q3a/

LOCAL_C_INCLUDES += $(LOCAL_MMCAMERA_PATH)/media-controller/modules/isp2/common/
LOCAL_C_INCLUDES += $(LOCAL_MMCAMERA_PATH)/media-controller/modules/isp2/hw/sub_module

LOCAL_C_INCLUDES += $(SRC_CAMERA_HAL_DIR)/QCamera2/stack/common

LOCAL_HEADER_LIBRARIES := libutils_headers

LOCAL_C_INCLUDES += $(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ/usr/include
LOCAL_C_INCLUDES += $(TARGET_OUT_INTERMEDIATES)/include/mm-camera-interface

ifeq ($(call is-board-platform-in-list, msm8996),true)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../adrc/
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include47
LOCAL_C_INCLUDES += $(LOCAL_PATH)/0309
LOCAL_C_INCLUDES += $(LOCAL_MMCAMERA_PATH)/media-controller/modules/isp2/module/isp47
else ifeq ($(call is-board-platform-in-list,sdm660 msm8998 apq8098_latv $(TRINKET)),true)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../adrc/
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include47
LOCAL_C_INCLUDES += $(LOCAL_PATH)/0310
LOCAL_C_INCLUDES += $(LOCAL_MMCAMERA_PATH)/media-controller/modules/isp2/module/isp48
endif

ifeq ($(TARGET_COMPILE_WITH_MSM_KERNEL),true)
LOCAL_ADDITIONAL_DEPENDENCIES := $(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ/usr
endif

LOCAL_SRC_FILES        := module_ltm47.c ltm47.c \
                          ../adrc/ltm_algo.c

ifeq ($(CHROMATIX_VERSION), 0310)
    LOCAL_SRC_FILES += 0310/ltm47_0310.c
else
    LOCAL_SRC_FILES += 0309/ltm47_0309.c
endif

LOCAL_MODULE           := libmmcamera_isp_ltm47
include $(SDCLANG_COMMON_DEFS)
LOCAL_SHARED_LIBRARIES := libcutils \
                          libmmcamera2_mct \
                          libmmcamera_isp_sub_module \
                          libmmcamera_dbg \
                          libmmcamera2_isp_modules

ifeq ($(OEM_CHROMATIX), true)
LOCAL_C_INCLUDES += $(LOCAL_EXTEN_ISP_INCLUDES)
LOCAL_SRC_FILES  += ../../../../../../../../mm-camera-ext/mm-camera2/media-controller/modules/isp2/ltm47_ext.c
LOCAL_CFLAGS += -DOVERRIDE_FUNC=1
endif
ifeq ($(MM_DEBUG),true)
LOCAL_SHARED_LIBRARIES += liblog
endif

LOCAL_MODULE_OWNER := qti
LOCAL_PROPRIETARY_MODULE := true

include $(BUILD_SHARED_LIBRARY)

endif # if 8996
endif # is-vendor-board-platform,QCOM
