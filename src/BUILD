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

cc_library(
  name = "reloptoken",
  srcs = ["reloptoken.cc"],
  hdrs = ["reloptoken.h"],
  deps = [":token"],
)

cc_library(
  name = "addoptoken",
  srcs = ["addoptoken.cc"],
  hdrs = ["addoptoken.h"],
  deps = [":token"],
)

cc_library(
  name = "muloptoken",
  srcs = ["muloptoken.cc"],
  hdrs = ["muloptoken.h"],
  deps = [":token"],
)

cc_library(
  name = "idtoken",
  srcs = ["idtoken.cc"],
  hdrs = ["idtoken.h"],
  deps = [":token"],
)

cc_library(
  name = "numtoken",
  srcs = ["numtoken.cc"],
  hdrs = ["numtoken.h"],
  deps = [":token"],
)

cc_library(
  name = "eoftoken",
  srcs = ["eoftoken.cc"],
  hdrs = ["eoftoken.h"],
  deps = [":token"],
)

cc_library(
  name = "buffer",
  srcs = ["buffer.cc"],
  hdrs = ["buffer.h"],
)

cc_library(
  name = "scanner",
  srcs = ["scanner.cc"],
  hdrs = ["scanner.h"],
  deps = [
       ":buffer",
       ":token",
       ":keywordtoken",
       ":punctoken",
       ":reloptoken",
       ":addoptoken",
       ":muloptoken",
       ":numtoken",
       ":idtoken",
       ":eoftoken",
  ],
)

cc_binary(
  name = "test_scanner",
  srcs = ["test_scanner.cc"],
  deps = [":scanner"],
)