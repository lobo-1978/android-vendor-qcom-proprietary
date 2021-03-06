#ifndef ADEC_SVR_H
#define ADEC_SVR_H

/* Copyright (c) 2008, 2017 Qualcomm Technologies, Inc.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 */
#ifdef __cplusplus
extern "C" {
#endif
#define D_DEBUG
#include <pthread.h>
#include <sched.h>
#ifdef D_DEBUG
#ifdef _ANDROID_
#include <utils/Log.h>
#include "common_log.h"
#define LOG_TAG "OMX_EVRC_DEC"
#define DEBUG_PRINT_ERROR LOGE
#define DEBUG_PRINT LOGI
#define DEBUG_DETAIL LOGI
#else
#define DEBUG_PRINT(args...) printf("%s:%d ", __FUNCTION__, __LINE__); \
    printf(args)
#define DEBUG_PRINT_ERROR(args...) printf("%s:%d ", __FUNCTION__, __LINE__); \
    printf(args)
#define DEBUG_DETAIL(args...) printf("%s:%d ", __FUNCTION__, __LINE__); \
    printf(args)
#endif
#else
#ifdef _ANDROID_
#include <utils/Log.h>
#include "common_log.h"
#define LOG_TAG "OMX_EVRC_DEC"
#define DEBUG_PRINT_ERROR
#define DEBUG_PRINT
#define DEBUG_DETAIL
#else
#define DEBUG_PRINT(args...)
#define DEBUG_PRINT_ERROR(args...)
#define DEBUG_DETAIL(args...)
#endif
#endif

typedef void (*message_func)(void* client_data, unsigned char id);

struct evrc_ipc_info
{
    pthread_t    thr;
    int          pipe_in;
    int          pipe_out;
    int          dead;
    message_func process_msg_cb;
    void         *client_data;
    char         thread_name[128];
};

/**
 @brief This function starts command server

 @param cb pointer to callback function from the client
 @param client_data reference client wants to get back
  through callback
 @return handle to command server
 */
struct evrc_ipc_info *omx_evrc_thread_create(message_func cb,
    void* client_data,
    char *th_name);


/**
 @brief This function stop command server

 @param svr handle to command server
 @return none
 */
void omx_evrc_thread_stop(struct evrc_ipc_info *evrc_ipc);


/**
 @brief This function post message in the command server

 @param svr handle to command server
 @return none
 */
void omx_evrc_post_msg(struct evrc_ipc_info *evrc_ipc,
                          unsigned char id);

#ifdef __cplusplus
}
#endif

#endif /* ADEC_SVR */
