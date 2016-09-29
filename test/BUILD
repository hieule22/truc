package(default_visibility = ["//visibility:public"])

cc_test(
  name = "token_test",
  srcs = ["token_test.cc"],
  size = "small",
  deps = [
       "//src:token",
       "//third_party/gtest:gtest_main",
  ],
)

cc_test(
  name = "keywordtoken_test",
  srcs = ["keywordtoken_test.cc"],
  size = "small",
  deps = [
       "//src:keywordtoken",
       "//third_party/gtest:gtest_main",
  ],
)

cc_test(
  name = "punctoken_test",
  srcs = ["punctoken_test.cc"],
  size = "small",
  deps = [
       "//src:punctoken",
       "//third_party/gtest:gtest_main",
  ],
)

cc_test(
  name = "reloptoken_test",
  srcs = ["reloptoken_test.cc"],
  size = "small",
  deps = [
       "//src:reloptoken",
       "//third_party/gtest:gtest_main",
  ],
)

cc_test(
  name = "addoptoken_test",
  srcs = ["addoptoken_test.cc"],
  size = "small",
  deps = [
       "//src:addoptoken",
       "//third_party/gtest:gtest_main",
  ],
)

cc_test(
  name = "muloptoken_test",
  srcs = ["muloptoken_test.cc"],
  size = "small",
  deps = [
       "//src:muloptoken",
       "//third_party/gtest:gtest_main",
  ],
)

cc_test(
  name = "idtoken_test",
  srcs = ["idtoken_test.cc"],
  size = "small",
  deps = [
       "//src:idtoken",
       "//third_party/gtest:gtest_main",
  ],
)

cc_test(
  name = "numtoken_test",
  srcs = ["numtoken_test.cc"],
  size = "small",
  deps = [
       "//src:numtoken",
       "//third_party/gtest:gtest_main",
  ],
)

cc_test(
  name = "eoftoken_test",
  srcs = ["eoftoken_test.cc"],
  size = "small",
  deps = [
       "//src:eoftoken",
       "//third_party/gtest:gtest_main",
  ],
)

cc_test(
  name = "buffer_test",
  srcs = ["buffer_test.cc"],
  size = "small",
  deps = [
       "//src:buffer",
       "//third_party/gtest:gtest_main",
  ],
)

cc_test(
  name = "streambuffer_test",
  srcs = ["streambuffer_test.cc"],
  size = "small",
  deps = [
       "//src:streambuffer",
       "//third_party/gtest:gtest_main",
  ],
)

cc_test(
  name = "filebuffer_test",
  srcs = ["filebuffer_test.cc"],
  size = "small",
  deps = [
       "//src:filebuffer",
       "//third_party/gtest:gtest_main",
  ],
)

cc_test(
  name = "scanner_test",
  srcs = ["scanner_test.cc"],
  size = "small",
  deps = [
       "//src:scanner",
       "//src:streambuffer",
       "//third_party/gtest:gtest_main",
  ],
  copts = ["-std=c++14"],
)