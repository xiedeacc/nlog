#include "src/nlog.h"

#include <signal.h>

#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "gtest/gtest.h"
using namespace std;

static void sig_usr(int signo) {
  std::cout << "SIGSEGV!" << std::endl;
  spdlog::shutdown();
  exit(0);
}

TEST(LogManager, LOG_INFO) {
  if (nlog::LogManager::Instance().Init("./log.conf")) {
    cout << "spdlog init failed !";
    return;
  }

  auto logger = nlog::LogManager::Instance().Get("request");
  int n = 0;
  while (true) {
    LOG_INFO("{}", n++);
    this_thread::sleep_for(1s);
  }
  return;
}
