#pragma once

#include "btdef/config.hpp"
#include "btdef/util/basic_text_ext.hpp"

#ifndef BTDEF_UTIL_TEXT_SIZE
#define BTDEF_UTIL_TEXT_SIZE 320
#endif // BTDEF_UTIL_TEXT_SIZE

namespace btdef {
namespace util {

typedef basic_text<char, BTDEF_UTIL_TEXT_SIZE> text;

template <std::size_t N>
static text make_text(char const (&str)[N]) noexcept
{
    return text(std::cref(str));
}

} // namespace util
} // namespace btdef
