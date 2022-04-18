/*
 * Copyright (c) 2017 Qualcomm Technologies, Inc.
 * All Rights Reserved.
 * Confidential and Proprietary - Qualcomm Technologies, Inc.
 *
 * Not a Contribution.
 * Apache license notifications and license are retained
 * for attribution purposes only.
 */
//
// Copyright 2016 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "async_fd_watcher.h"

#include <condition_variable>
#include <map>
#include <mutex>
#include <thread>
#include <vector>
#include "fcntl.h"
#include "sys/select.h"
#include "unistd.h"
#include <utils/Log.h>
#include "logger.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "vendor.qti.bluetooth@1.0-async_fd_watcher"

static const int INVALID_FD = -1;

namespace android {
namespace hardware {
namespace bluetooth {
namespace V1_0 {
namespace implementation {

int AsyncFdWatcher::WatchFdForNonBlockingReads(
  int file_descriptor, const ReadCallback& on_read_fd_ready_callback)
{
  // Add file descriptor and callback
  {
    std::unique_lock<std::mutex> guard(internal_mutex_);
    watched_fds_[file_descriptor] = on_read_fd_ready_callback;
  }

  // Start the thread if not started yet
  return TryStartThread();
}

int AsyncFdWatcher::ConfigureTimeout(
  const std::chrono::milliseconds timeout,
  const TimeoutCallback& on_timeout_callback)
{
  // Add timeout and callback
  {
    std::unique_lock<std::mutex> guard(timeout_mutex_);
    timeout_cb_ = on_timeout_callback;
    timeout_ms_ = timeout;
  }

  NotifyThread();
  return 0;
}

void AsyncFdWatcher::StopWatchingFileDescriptors()
{
  StopThread();
}

AsyncFdWatcher::AsyncFdWatcher()
{
  timeout_ms_ = std::chrono::milliseconds(1000);
  timeout_cb_ = nullptr;
}

AsyncFdWatcher::~AsyncFdWatcher()
{
}

// Make sure to call this with at least one file descriptor ready to be
// watched upon or the thread routine will return immediately
int AsyncFdWatcher::TryStartThread()
{
  if (std::atomic_exchange(&running_, true)) return 0;

  // Set up the communication channel
  int pipe_fds[2];
  if (pipe2(pipe_fds, O_NONBLOCK)) return -1;

  notification_listen_fd_ = pipe_fds[0];
  notification_write_fd_ = pipe_fds[1];

  thread_ = std::thread([this]() { ThreadRoutine(); });
  if (!thread_.joinable()) return -1;

  return 0;
}

int AsyncFdWatcher::StopThreadRoutine()
{
  if (!std::atomic_exchange(&running_, false)) return 0;
  NotifyThread();
  return 0;
}

int AsyncFdWatcher::StopThread()
{
  if (!std::atomic_exchange(&running_, false)) return 0;

  NotifyThread();
  if (std::this_thread::get_id() != thread_.get_id()) {
    thread_.join();
  }

  ALOGW("%s: stopped the work thread", __func__);

  close(notification_listen_fd_);
  close(notification_write_fd_);

  {
    std::unique_lock<std::mutex> guard(internal_mutex_);
    watched_fds_.clear();
  }

  {
    std::unique_lock<std::mutex> guard(timeout_mutex_);
    timeout_cb_ = nullptr;
  }

  return 0;
}

int AsyncFdWatcher::NotifyThread()
{
  uint8_t buffer[] = { 0 };

  if (TEMP_FAILURE_RETRY(write(notification_write_fd_, &buffer, 1)) < 0) {
    return -1;
  }
  return 0;
}

void AsyncFdWatcher::ThreadRoutine()
{

  while (running_) {
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(notification_listen_fd_, &read_fds);
    int max_read_fd = INVALID_FD;
    for (auto& it : watched_fds_) {
      FD_SET(it.first, &read_fds);
      max_read_fd = std::max(max_read_fd, it.first);
    }

    struct timeval timeout;
    struct timeval* timeout_ptr = NULL;
    if (timeout_ms_ > std::chrono::milliseconds(0)) {
      timeout.tv_sec = timeout_ms_.count() / 1000;
      timeout.tv_usec = (timeout_ms_.count() % 1000) * 1000;
      timeout_ptr = &timeout;
    }

    // Wait until there is data available to read on some FD.
    int nfds = std::max(notification_listen_fd_, max_read_fd);
#ifdef DUMP_RINGBUF_LOG
    Logger::Get()->UpdateRxEventTag(RX_PRE_SELECT_CALL_BACK);
#endif
    int retval = select(nfds + 1, &read_fds, NULL, NULL, timeout_ptr);
#ifdef DUMP_RINGBUF_LOG
    Logger::Get()->UpdateRxEventTag(RX_POST_SELECT_CALL_BACK);
#endif

    // There was some error.
    if (retval < 0) continue;

    // Timeout.
    if (retval == 0) {
      // Allow the timeout callback to modify the timeout.
      TimeoutCallback saved_cb;
      {
        std::unique_lock<std::mutex> guard(timeout_mutex_);
        if (timeout_ms_ > std::chrono::milliseconds(0))
          saved_cb = timeout_cb_;
      }
      if (saved_cb != nullptr)
        saved_cb();
      continue;
    }

    // Read data from the notification FD.
    if (FD_ISSET(notification_listen_fd_, &read_fds)) {
      char buffer[] = { 0 };
      TEMP_FAILURE_RETRY(read(notification_listen_fd_, buffer, 1));
      continue;
    }

    // Invoke the data ready callbacks if appropriate.
    std::vector<decltype(watched_fds_) ::value_type> saved_callbacks;
    {
      std::unique_lock<std::mutex> guard(internal_mutex_);
      for (auto& it : watched_fds_) {
        if (FD_ISSET(it.first, &read_fds)) {
          saved_callbacks.push_back(it);
        }
      }
    }

    for (auto& it : saved_callbacks) {
      if (it.second) {
        it.second(it.first);
      }
    }
  }
  ALOGE("%s: End of AsyncFdWatcher::ThreadRoutine", __func__);
}

} // namespace implementation
} // namespace V1_0
} // namespace bluetooth
} // namespace hardware
} // namespace android
