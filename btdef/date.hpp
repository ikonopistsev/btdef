#pragma once

#include "btdef/util/date.hpp"

namespace btdef {
namespace time {

using btdef::util::time::now;
using btdef::util::time::steady;
using btdef::util::time::create_tm;
using btdef::util::time::empty_tm;
using btdef::util::time::empty_tm_dst;

} // namespace time

using btdef::util::tm;
using btdef::util::date;

} // namspace btdef

namespace utility {

using btdef::time::now;
using btdef::time::steady;
using btdef::time::create_tm;
using btdef::util::time::empty_tm;
using btdef::util::time::empty_tm_dst;
using btdef::util::tm;
using btdef::util::date;

} // namespace utility
