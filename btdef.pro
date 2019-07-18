TEMPLATE = lib

CONFIG -= qt
CONFIG -= app_bundle
CONFIG += staticlib c++14 warn_on

TARGET = btdef

CONFIG(release, debug|release) {
    DEFINES += NDEBUG
}

INCLUDEPATH += ../

HEADERS += \
    allocator/basic.hpp \
    allocator/basic_pool.hpp \
    allocator/wrapper.hpp \
    conv/string_traits.hpp \
    conv/to_hex.hpp \
    conv/to_hex_text.hpp \
    conv/to_string.hpp \
    conv/to_text.hpp \
    hash/basic_fnv1a.hpp \
    hash/fnv1a.hpp \
    num/dtoap.hpp \
    num/fpconv.hpp \
    num/itoa.hpp \
    ref/basic_string.hpp \
    ref/basic_string_ext.hpp \
    ref/string.hpp \
    util/basic_string.hpp \
    util/basic_string_ext.hpp \
    util/basic_text.hpp \
    util/basic_text_ext.hpp \
    util/date.hpp \
    util/string.hpp \
    util/text.hpp \
    util/time.hpp \
    util/time_zone.hpp \
    util/tm.hpp \
    util/util.hpp \
    btdef.hpp \
    config.hpp \
    conv.hpp \
    date.hpp \
    hash.hpp \
    num.hpp \
    ref.hpp \
    string.hpp \
    text.hpp
