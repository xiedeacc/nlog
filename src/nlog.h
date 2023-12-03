#pragma once

#include <limits.h>
#include <stdlib.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include "spdlog/async.h"
#include "spdlog/cfg/env.h"
#include "spdlog/common.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/hourly_file_sink.h"
#include "spdlog/spdlog.h"
//#include "spdlog/sinks/minutely_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"

namespace nlog {
class Conf;
class LogManager final {
 public:
  LogManager(const LogManager &) = delete;
  LogManager(LogManager &&) = delete;
  LogManager &operator=(const LogManager &) = delete;
  LogManager &operator=(LogManager &&) = delete;

  LogManager();
  ~LogManager();

  int Init(const std::string &file);
  void Shutdown();
  std::shared_ptr<spdlog::logger> Get(const std::string &name);

  static LogManager &Instance();

 private:
  void CreateRotateLogger(const nlog::Conf &);

  std::unordered_map<std::string, std::shared_ptr<spdlog::details::thread_pool>>
      pool_;
};
}  // namespace nlog

#define NLOG_LEVEL spdlog::get_level()

// default is stdout, call spdlog::set_default_logger to change
#define LOG_TRACE(...)                                                     \
  do {                                                                     \
    SPDLOG_LOGGER_CALL(spdlog::default_logger_raw(), spdlog::level::trace, \
                       __VA_ARGS__);                                       \
  } while (0);
#define LOG_DEBUG(...)                                                     \
  do {                                                                     \
    SPDLOG_LOGGER_CALL(spdlog::default_logger_raw(), spdlog::level::debug, \
                       __VA_ARGS__);                                       \
  } while (0);
#define LOG_INFO(...)                                                     \
  do {                                                                    \
    SPDLOG_LOGGER_CALL(spdlog::default_logger_raw(), spdlog::level::info, \
                       __VA_ARGS__);                                      \
  } while (0);
#define LOG_WARN(...)                                                     \
  do {                                                                    \
    SPDLOG_LOGGER_CALL(spdlog::default_logger_raw(), spdlog::level::warn, \
                       __VA_ARGS__);                                      \
  } while (0);
#define LOG_ERROR(...)                                                   \
  do {                                                                   \
    SPDLOG_LOGGER_CALL(spdlog::default_logger_raw(), spdlog::level::err, \
                       __VA_ARGS__);                                     \
  } while (0);
#define LOG_CRITICAL(...)                                                     \
  do {                                                                        \
    SPDLOG_LOGGER_CALL(spdlog::default_logger_raw(), spdlog::level::critical, \
                       __VA_ARGS__);                                          \
  } while (0);
