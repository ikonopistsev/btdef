#include "btdef/date.hpp"
#include "btdef/text.hpp"
#include <iostream>
#include <string>
#include <vector>

int main()
{
    std::vector<char> vv;
    vv.resize(1, 'v');
    auto t = btdef::date::now();
    auto text = t.to_json();

    btdef::date d{text};
    std::string s{text};
    std::cout << t << ' ' << s << std::endl;

    using namespace std::literals;
    btdef::text t2 = "1234"sv;
    std::cout << t << ' ' << t2 << std::endl;

    t2 += text;
    t2 += s;

    std::string_view v = t2;
    std::cout << v << std::endl;

    return 0;
}

