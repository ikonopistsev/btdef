#include <iostream>
#include <sstream>
#include <chrono>
#include <string_view>
#include "btdef/text.hpp"

using namespace std::literals;

template <typename F, class S>
void test(const S& what, std::size_t count, F&& fn)
{
    auto counter = count;
    const auto start = std::chrono::high_resolution_clock::now();

    while (counter--) fn(counter);

    const auto stop = std::chrono::high_resolution_clock::now();

    const auto msec =
        std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    const auto nsec =
        std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);

    std::cout << what << std::endl
        << "total - " << msec.count() << " msec"
        << ", one - " << nsec.count() / count << " nsec"
        << std::endl;
}

//static const char SELECT_ALL[]
constexpr auto SELECT_ALL =
"select `source_id`,`login`,`deal`,"
"`link_source_id`,`link_login`,`link_deal`,"
"`root_source_id`,`root_login`,`root_deal`,"
"`cmd`, `volume`,`symbol`,`digits`,"
"`exec_time`,`comment`,"
"`status`,`exec_type`,`error_condition`,`reject_reason`"
" from `order`"sv;

int main()
{
    std::cout << std::to_string(std::uint64_t{2345678}) << std::endl;
    std::cout << btdef::to_text(std::uint64_t{2345678}) << std::endl;
    std::cout << std::to_string(std::uint64_t{234567898799}) << std::endl;
    std::cout << btdef::to_text(std::uint64_t{234567898799}) << std::endl;
    const std::size_t count = 10000000;
    const std::string symbol{"EURUSD"sv};

    std::ostringstream out;
    std::stringbuf buffer{std::string(4096, ' '), std::ios_base::out};
    out.rdbuf()->swap(buffer);

    test("ostringstream", count, [&](std::size_t counter) {
        out.clear();
        out.str({});

        out << SELECT_ALL
          << " where `source_id`="sv << 201
          << " and `login`="sv << 100500
          << " and `symbol`='"sv << symbol << '\''
          << " and `link_source_id`="sv << 202
          << " and `link_login`="sv << 666
          << " and `link_deal`=`deal`"sv
          << " and 'record_time'>"sv << counter
          << " limit 1"sv;
    });

    std::string str;
    test("string", count, [&](std::size_t counter) {
        str.clear();

        str = SELECT_ALL;
        str += " where `source_id`="
            + std::to_string(201)
            + " and `login`="
            + std::to_string(100500)
            + " and `symbol`='"
            + symbol + '\''
            + " and `link_source_id`="
            + std::to_string(202)
            + " and `link_login`="
            + std::to_string(666)
            + " and `link_deal`=`deal`"
            + " and 'record_time'>"
            + std::to_string(counter)
            + " limit 1";
    });

    test("string-opt", count, [&](std::size_t counter) {
        str.clear();

        str += SELECT_ALL;
        str += " where `source_id`="sv;
        str += std::to_string(201);
        str += " and `login`="sv;
        str += std::to_string(100500);
        str += " and `symbol`='"sv;
        str += symbol; str += '\'';
        str += " and `link_source_id`="sv;
        str += std::to_string(202);
        str += " and `link_login`="sv;
        str += std::to_string(666);
        str += " and `link_deal`=`deal`"sv;
        str += " and 'record_time'>"sv;
        str += std::to_string(counter);
        str += " limit 1"sv;
    });

    constexpr auto symb = "EURUSD"sv;
    btdef::util::basic_text<char, 4096> t;
    
    test("stack-string-opt", count, [&t, symb](std::size_t counter) {
        t.clear();

        t += SELECT_ALL;
        t += " where `source_id`="sv;
        t += btdef::to_text(201);
        t += " and `login`="sv;
        t += btdef::to_text(100500);
        t += " and `symbol`='"sv;
        t += symb; 
        t += '\'';
        t += " and `link_source_id`="sv;
        t += btdef::to_text(202);
        t += " and `link_login`="sv;
        t += btdef::to_text(666);
        t += " and `link_deal`=`deal`"sv;
        t += " and 'record_time'>"sv;
        t += btdef::to_text(counter);
        t += " limit 1"sv;
    });

    if (str != t)
        abort();

    return 0;
}
