licenses(["notice"])

cc_library(
  name = "gtest_main",
  includes = ["include"],
  srcs = glob(
      ["src/*.cc"],
      exclude = ["src/gtest-all.cc"]
    ),
  hdrs = glob([
      "include/**/*.h",
      "src/*.h"
  ]),
  copts = ["-Ithird_party/gtest"],
  linkopts = ["-pthread"],
  visibility = ["//visibility:public"],
)