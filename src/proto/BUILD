load("@rules_proto//proto:defs.bzl", "proto_library")

package(default_visibility = ["//visibility:public"])

cc_proto_library(
    name = "cc_nlog_proto",
    deps = [":nlog_proto"],
)

proto_library(
    name = "nlog_proto",
    srcs = glob(["*.proto"]),
    deps = [],
)

filegroup(
    name = "nlog_proto_file",
    srcs = glob(["*.proto"]),
)
