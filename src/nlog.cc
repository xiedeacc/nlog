#include "src/nlog.h"

#include "google/protobuf/text_format.h"
#include "src/proto/log.pb.h"

using namespace std;

namespace nlog {

LogManager::LogManager() {}
LogManager::~LogManager() {}

LogManager &LogManager::Instance() {
  static LogManager instance_;
  return instance_;
}

void LogManager::CreateRotateLogger(const nlog::Conf &item) {
  ::spdlog::file_event_handlers event_handlers;
  event_handlers.after_open = [base_filename = item.file()](
                                  const spdlog::filename_t &filename,
                                  std::FILE *file_stream) {
    char resolved_path[PATH_MAX];
    if (NULL != realpath(filename.c_str(), resolved_path)) {
      remove(base_filename.c_str());
      int ret = symlink(resolved_path, base_filename.c_str());
    }
  };

  spdlog::sink_ptr sink = make_shared<spdlog::sinks::hourly_file_sink_mt>(
      item.file(), false, item.max_files(), event_handlers);
  // if (item.has_rotating_minutes()) {
  //   sink = make_shared<spdlog::sinks::minutely_file_sink_mt>(
  //       item.file(), item.rotating_minutes(), false, item.max_files(),
  //       event_handlers);
  // } else {
  //   sink = make_shared<spdlog::sinks::hourly_file_sink_mt>(item.file(),
  //   false, item.max_files(),
  //                                                          event_handlers);
  // }

  shared_ptr<spdlog::logger> logger;
  if (item.mode() == MODE_ASYNC) {
    auto tp = spdlog::thread_pool();
    if (item.has_queue_size() && item.has_thread_size()) {
      tp = make_shared<spdlog::details::thread_pool>(item.queue_size(),
                                                     item.thread_size());
      pool_.emplace(item.name(), tp);
    }
    logger = make_shared<spdlog::async_logger>(item.name(), sink, tp);
  } else {
    logger = make_shared<spdlog::logger>(item.name(), sink);
  }
  spdlog::register_logger(logger);

  if (item.has_level()) {
    logger->set_level(static_cast<spdlog::level::level_enum>(item.level()));
  }
  if (item.has_pattern()) {
    logger->set_pattern(item.pattern());
  }
  if (item.has_flush_severity()) {
    logger->flush_on(
        static_cast<spdlog::level::level_enum>(item.flush_severity()));
  }
  if (item.has_is_default() && item.is_default()) {
    spdlog::set_default_logger(logger);
  }
}

int LogManager::Init(const string &file) {
  ifstream in(file);
  string content =
      string((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());

  LogConf conf;
  if (!google::protobuf::TextFormat::ParseFromString(content, &conf)) {
    cout << "failed to parse " << file << endl;
    return -1;
  }

  // global setting
  if (conf.has_global()) {
    auto &global = conf.global();

    if (global.has_level()) {
      spdlog::set_level(static_cast<spdlog::level::level_enum>(global.level()));
    }
    if (global.has_pattern()) {
      spdlog::set_pattern(global.pattern());
    }
    if (global.has_flush_interval()) {
      spdlog::flush_every(chrono::seconds(global.flush_interval()));
    }
    if (global.has_flush_severity()) {
      spdlog::flush_on(
          static_cast<spdlog::level::level_enum>(global.flush_severity()));
    }
    if (global.has_queue_size() && global.has_thread_size()) {
      spdlog::init_thread_pool(global.queue_size(), global.thread_size());
    }
  }

  for (auto &item : conf.loggers()) {
    CreateRotateLogger(item);
  }
  return 0;
}

void LogManager::Shutdown() { spdlog::shutdown(); }

shared_ptr<spdlog::logger> LogManager::Get(const string &name) {
  return spdlog::get(name);
}
}  // namespace nlog
