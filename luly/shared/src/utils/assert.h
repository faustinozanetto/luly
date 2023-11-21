#pragma once

#include <filesystem>

#define LY_ASSERT(expr)                                                                                                            \
if (!(expr))                                                                                                                   \
{                                                                                                                              \
LY_ERROR("Assertion failed: {} in file {} line {}", #expr, std::filesystem::path(__FILE__).filename().string(), __LINE__); \
__debugbreak();                                                                                                            \
}

#define LY_ASSERT_MSG(expr, msg)                                                                                                             \
if (!(expr))                                                                                                                             \
{                                                                                                                                        \
LY_ERROR("Assertion failed: {} ({}) in file {} line {}", #expr, msg, std::filesystem::path(__FILE__).filename().string(), __LINE__); \
__debugbreak();                                                                                                                      \
}
