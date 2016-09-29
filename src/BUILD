package(default_visibility = ["//visibility:public"])

cc_library(
  name = "token",
  srcs = ["token.cc"],
  hdrs = ["token.h"],
)

cc_library(
  name = "keywordtoken",
  srcs = ["keywordtoken.cc"],
  hdrs = ["keywordtoken.h"],
  deps = [":token"],
)

cc_library(
  name = "punctoken",
  srcs = ["punctoken.cc"],
  hdrs = ["punctoken.h"],
  deps = [":token"],
)