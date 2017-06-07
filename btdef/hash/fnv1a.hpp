// Copyright © 2017 igor . ikonopistsev at gmail
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.

#pragma once

#include "btdef/hash/basic_fnv1a.hpp"

namespace btdef {
namespace hash {

typedef basic_fnv1a<sizeof(std::size_t)> fnv1a;

} // namespace hash
} // namespace btdef
