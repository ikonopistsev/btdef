// Copyright © 2017 igor . ikonopistsev at gmail
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.

#pragma once

#include "btdef/util/date.hpp"
#include "btdef/conv/to_text.hpp"
#include "btdef/conv/to_string.hpp"
#include "btdef/util/time_zone.hpp"
#include "btdef/ref/string.hpp"
#include "btdef/ref/basic_string_ext.hpp"
#include "btdef/util/basic_string_ext.hpp"
#include "btdef/util/basic_text_ext.hpp"
//#include "btdef/buffer/basic.hpp"

namespace utility {

using btdef::util::time::now;
using btdef::util::time::steady;
using btdef::util::time::create_tm;
using btdef::util::time::empty_tm;
using btdef::util::tm;
using btdef::util::date;
using btdef::util::time_zone;
using btdef::util::string;
using btdef::util::text;
/*
//using buffer = btdef::buffer::basic<btdef::buffer::detail::iovec_type,
//    btdef::allocator::basic<char>>;
*/
namespace num {

using btdef::num::detail::itoa2zf;
using btdef::num::detail::itoa3zf;
using btdef::num::detail::itoa4zf;
using btdef::num::int32toa;
using btdef::num::int64toa;
using btdef::num::uint32toa;
using btdef::num::uint64toa;

} // namespace num

using btdef::num::itoa;

namespace conv {

using btdef::conv::antout;
using btdef::conv::antoutp;
using btdef::conv::antou;

} // namespace conv

using btdef::conv::to_text;
using btdef::conv::to_string;

} // namespace utility

namespace ref {

using btref::string;

} // namespace ref
