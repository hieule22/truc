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

cc_library(
  name = "symbol_table",
  srcs = ["symbol_table.cc"],
  hdrs = ["symbol_table.h"],
)

cc_library(
  name = "register",
  srcs = ["register.cc"],
  hdrs = ["register.h"],
)

cc_library(
  name = "register_allocator",
  srcs = ["register_allocator.cc"],
  hdrs = ["register_allocator.h"],
  deps = [":register"],
)

cc_library(
  name = "operand",
  srcs = ["operand.cc"],
  hdrs = ["operand.h"],
  deps = [":register"],
)

cc_library(
  name = "emitter",
  srcs = ["emitter.cc"],
  hdrs = ["emitter.h"],
  deps = [":register"],
)

cc_library(
  name = "parser",
  srcs = ["parser.cc"],
  hdrs = ["parser.h"],
  deps = [
       ":scanner",
       ":symbol_table",
       ":keywordtoken",
       ":punctoken",
       ":reloptoken",
       ":addoptoken",
       ":muloptoken",
       ":idtoken",
       ":numtoken",
       ":eoftoken",
       ":register",
       ":register_allocator",
       ":emitter",
       ":operand",
  ],
)

cc_binary(
  name = "test_scanner",
  srcs = ["test_scanner.cc"],
  deps = [":scanner"],
)

cc_binary(
  name = "truc",
  srcs = ["truc.cc"],
  deps = [":parser"],
)