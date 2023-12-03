load("//tools:cpplint.bzl", "cpplint")
load("//tools:build.bzl", "cc_test")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "nlog",
    srcs = ["nlog.cc"],
    hdrs = ["nlog.h"],
    deps = [
        "//src/proto:cc_nlog_proto",
        "@com_github_fmtlib_fmt//:fmt",
        "@com_github_glog_glog//:glog",
        "@spdlog",
    ],
)

cc_test(
    name = "nlog_test",
    srcs = [
        "nlog_test.cc",
    ],
    data = [
        "//test_data:log.conf",
    ],
    deps = [
        ":nlog",
        "@com_google_googletest//:gtest",
    ],
)

cpplint()
