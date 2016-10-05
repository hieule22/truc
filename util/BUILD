package(default_visibility = ["//visibility:public"])

cc_library(
  name = "ptr_util",
  hdrs = ["ptr_util.h"],
)

cc_test(
  name = "ptr_util_test",
  srcs = ["ptr_util_test.cc"],
  deps = [
       ":ptr_util",
       "//third_party/gtest:gtest_main",
  ],
)