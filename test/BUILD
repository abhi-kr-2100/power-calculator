load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
  name = "token-test",
  size = "small",
  srcs = ["token_test.cpp"],
  deps = ["@com_google_googletest//:gtest_main", "//token:token"],
)

cc_test(
  name = "parser-test",
  size = "small",
  srcs = ["parser_test.cpp"],
  deps = ["@com_google_googletest//:gtest_main", "//parser:parser"],
)

cc_test(
  name = "primary-test",
  size = "small",
  srcs = ["primary_test.cpp"],
  deps = ["@com_google_googletest//:gtest_main", "//primary:primary"],
)
