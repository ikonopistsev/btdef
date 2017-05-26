#pragma once

#include <string>
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <cassert>

namespace btdef {
namespace num {
namespace fpconv {

enum {
    npowers = 87,
    steppowers = 8,
    firstpower = -348, /* 10 ^ -348 */
    expmax = -32,
    expmin = -60,
    ntensa = 20,
};

const uint64_t fracmask = 0x000FFFFFFFFFFFFFU;
const uint64_t expmask = 0x7FF0000000000000U;
const uint64_t hiddenbit = 0x0010000000000000U;
const uint64_t signmask = 0x8000000000000000U;
const int expbias = 1023 + 52;

template<typename T>
static inline T absv(T n) noexcept
{
    return ((n) < 0 ? -(n) : (n));
}

struct Fp
{
    uint64_t frac;
    int exp;
};

static Fp powers_ten[] = {
    { 18054884314459144840U, -1220 },{ 13451937075301367670U, -1193 },
    { 10022474136428063862U, -1166 },{ 14934650266808366570U, -1140 },
    { 11127181549972568877U, -1113 },{ 16580792590934885855U, -1087 },
    { 12353653155963782858U, -1060 },{ 18408377700990114895U, -1034 },
    { 13715310171984221708U, -1007 },{ 10218702384817765436U, -980 },
    { 15227053142812498563U, -954 },{ 11345038669416679861U, -927 },
    { 16905424996341287883U, -901 },{ 12595523146049147757U, -874 },
    { 9384396036005875287U,  -847 },{ 13983839803942852151U, -821 },
    { 10418772551374772303U, -794 },{ 15525180923007089351U, -768 },
    { 11567161174868858868U, -741 },{ 17236413322193710309U, -715 },
    { 12842128665889583758U, -688 },{ 9568131466127621947U,  -661 },
    { 14257626930069360058U, -635 },{ 10622759856335341974U, -608 },
    { 15829145694278690180U, -582 },{ 11793632577567316726U, -555 },
    { 17573882009934360870U, -529 },{ 13093562431584567480U, -502 },
    { 9755464219737475723U,  -475 },{ 14536774485912137811U, -449 },
    { 10830740992659433045U, -422 },{ 16139061738043178685U, -396 },
    { 12024538023802026127U, -369 },{ 17917957937422433684U, -343 },
    { 13349918974505688015U, -316 },{ 9946464728195732843U,  -289 },
    { 14821387422376473014U, -263 },{ 11042794154864902060U, -236 },
    { 16455045573212060422U, -210 },{ 12259964326927110867U, -183 },
    { 18268770466636286478U, -157 },{ 13611294676837538539U, -130 },
    { 10141204801825835212U, -103 },{ 15111572745182864684U, -77 },
    { 11258999068426240000U, -50 },{ 16777216000000000000U, -24 },
    { 12500000000000000000U,   3 },{ 9313225746154785156U,   30 },
    { 13877787807814456755U,  56 },{ 10339757656912845936U,  83 },
    { 15407439555097886824U, 109 },{ 11479437019748901445U, 136 },
    { 17105694144590052135U, 162 },{ 12744735289059618216U, 189 },
    { 9495567745759798747U,  216 },{ 14149498560666738074U, 242 },
    { 10542197943230523224U, 269 },{ 15709099088952724970U, 295 },
    { 11704190886730495818U, 322 },{ 17440603504673385349U, 348 },
    { 12994262207056124023U, 375 },{ 9681479787123295682U,  402 },
    { 14426529090290212157U, 428 },{ 10748601772107342003U, 455 },
    { 16016664761464807395U, 481 },{ 11933345169920330789U, 508 },
    { 17782069995880619868U, 534 },{ 13248674568444952270U, 561 },
    { 9871031767461413346U,  588 },{ 14708983551653345445U, 614 },
    { 10959046745042015199U, 641 },{ 16330252207878254650U, 667 },
    { 12166986024289022870U, 694 },{ 18130221999122236476U, 720 },
    { 13508068024458167312U, 747 },{ 10064294952495520794U, 774 },
    { 14996968138956309548U, 800 },{ 11173611982879273257U, 827 },
    { 16649979327439178909U, 853 },{ 12405201291620119593U, 880 },
    { 9242595204427927429U,  907 },{ 13772540099066387757U, 933 },
    { 10261342003245940623U, 960 },{ 15290591125556738113U, 986 },
    { 11392378155556871081U, 1013 },{ 16975966327722178521U, 1039 },
    { 12648080533535911531U, 1066 }
};

static inline Fp find_cachedpow10(int exp, int* k) noexcept
{
    const double one_log_ten = 0.30102999566398114;

    int approx = static_cast<int>(-(exp + npowers) * one_log_ten);
    int idx = (approx - firstpower) / steppowers;

    while (1) {
        int current = exp + powers_ten[idx].exp + 64;

        if (current < expmin) {
            idx++;
            continue;
        }

        if (current > expmax) {
            idx--;
            continue;
        }

        *k = (firstpower + idx * steppowers);

        return powers_ten[idx];
    }
}

static uint64_t tens[] = {
    10000000000000000000U, 1000000000000000000U, 100000000000000000U,
    10000000000000000U, 1000000000000000U, 100000000000000U,
    10000000000000U, 1000000000000U, 100000000000U,
    10000000000U, 1000000000U, 100000000U,
    10000000U, 1000000U, 100000U,
    10000U, 1000U, 100U,
    10U, 1U
};

static inline uint64_t get_dbits(double d) noexcept
{
    union {
        double   dbl;
        uint64_t i;
    } dbl_bits = { d };

    return dbl_bits.i;
}

static inline Fp build_fp(double d) noexcept
{
    uint64_t bits = get_dbits(d);

    Fp fp;
    fp.frac = bits & fracmask;
    fp.exp = (bits & expmask) >> 52;

    if (fp.exp) {
        fp.frac += hiddenbit;
        fp.exp -= expbias;

    }
    else {
        fp.exp = -expbias + 1;
    }

    return fp;
}

static inline void normalize(Fp* fp) noexcept
{
    while ((fp->frac & hiddenbit) == 0) {
        fp->frac <<= 1;
        fp->exp--;
    }

    int shift = 64 - 52 - 1;
    fp->frac <<= shift;
    fp->exp -= shift;
}

static inline void get_normalized_boundaries(Fp* fp, Fp* lower, Fp* upper) noexcept
{
    upper->frac = (fp->frac << 1) + 1;
    upper->exp = fp->exp - 1;

    while ((upper->frac & (hiddenbit << 1)) == 0) {
        upper->frac <<= 1;
        upper->exp--;
    }

    int u_shift = 64 - 52 - 2;

    upper->frac <<= u_shift;
    upper->exp = upper->exp - u_shift;


    int l_shift = fp->frac == hiddenbit ? 2 : 1;

    lower->frac = (fp->frac << l_shift) - 1;
    lower->exp = fp->exp - l_shift;


    lower->frac <<= lower->exp - upper->exp;
    lower->exp = upper->exp;
}

static inline Fp multiply(Fp* a, Fp* b) noexcept
{
    const uint64_t lomask = 0x00000000FFFFFFFF;

    uint64_t ah_bl = (a->frac >> 32)    * (b->frac & lomask);
    uint64_t al_bh = (a->frac & lomask) * (b->frac >> 32);
    uint64_t al_bl = (a->frac & lomask) * (b->frac & lomask);
    uint64_t ah_bh = (a->frac >> 32)    * (b->frac >> 32);

    uint64_t tmp = (ah_bl & lomask) + (al_bh & lomask) + (al_bl >> 32);
    /* round up */
    tmp += 1U << 31;

    Fp fp = {
        ah_bh + (ah_bl >> 32) + (al_bh >> 32) + (tmp >> 32),
        a->exp + b->exp + 64
    };

    return fp;
}

static inline void round_digit(char* digits, int ndigits, uint64_t delta,
    uint64_t rem, uint64_t kappa, uint64_t frac) noexcept
{
    while (rem < frac && delta - rem >= kappa &&
        (rem + kappa < frac || frac - rem > rem + kappa - frac)) {

        digits[ndigits - 1]--;
        rem += kappa;
    }
}

static inline int generate_digits(Fp* fp, Fp* upper,
    Fp* lower, char* digits, int* K) noexcept
{
    uint64_t wfrac = upper->frac - fp->frac;
    uint64_t delta = upper->frac - lower->frac;

    Fp one;
    one.frac = 1ULL << -upper->exp;
    one.exp = upper->exp;

    uint64_t part1 = upper->frac >> -one.exp;
    uint64_t part2 = upper->frac & (one.frac - 1);

    int idx = 0, kappa = 10;
    uint64_t* divp;
    /* 1000000000 */
    for (divp = tens + 10; kappa > 0; divp++) {

        uint64_t div = *divp;
        unsigned digit = static_cast<unsigned>(part1 / div);

        if (digit || idx) {
            digits[idx++] = digit + '0';
        }

        part1 -= digit * div;
        kappa--;

        uint64_t tmp = (part1 << -one.exp) + part2;
        if (tmp <= delta) {
            *K += kappa;
            round_digit(digits, idx, delta, tmp, div << -one.exp, wfrac);

            return idx;
        }
    }

    /* 10 */
    uint64_t* unit = tens + 18;

    while (true) {
        part2 *= 10;
        delta *= 10;
        kappa--;

        unsigned digit = static_cast<unsigned>(part2 >> -one.exp);
        if (digit || idx) {
            digits[idx++] = digit + '0';
        }

        part2 &= one.frac - 1;
        if (part2 < delta) {
            *K += kappa;
            round_digit(digits, idx, delta, part2, one.frac, wfrac * *unit);

            return idx;
        }

        unit--;
    }
}

static inline int grisu2(double d, char* digits, int* K) noexcept
{
    Fp w = build_fp(d);

    Fp lower, upper;
    get_normalized_boundaries(&w, &lower, &upper);

    normalize(&w);

    int k;
    Fp cp = find_cachedpow10(upper.exp, &k);

    w = multiply(&w, &cp);
    upper = multiply(&upper, &cp);
    lower = multiply(&lower, &cp);

    lower.frac++;
    upper.frac--;

    *K = -k;

    return generate_digits(&w, &upper, &lower, digits, K);
}

static inline int emit_digits(char* digits, int ndigits,
    char* dest, int K, bool neg) noexcept
{
    int exp = absv(K + ndigits - 1);

    /* write plain integer */
    if (K >= 0 && (exp < (ndigits + 7))) {
        memcpy(dest, digits, ndigits);
        memset(dest + ndigits, '0', K);

        return ndigits + K;
    }

    /* write decimal w/o scientific notation */
    if (K < 0 && (K > -7 || exp < 4)) {
        int offset = ndigits - absv(K);
        /* fp < 1.0 -> write leading zero */
        if (offset <= 0) {
            offset = -offset;
            dest[0] = '0';
            dest[1] = '.';
            memset(dest + 2, '0', offset);
            memcpy(dest + offset + 2, digits, ndigits);

            return ndigits + 2 + offset;

            /* fp > 1.0 */
        }
        else {
            memcpy(dest, digits, offset);
            dest[offset] = '.';
            memcpy(dest + offset + 1, digits + offset, ndigits - offset);

            return ndigits + 1;
        }
    }

    /* write decimal w/ scientific notation */
    ndigits = (std::min)(ndigits, 18 - neg);

    int idx = 0;
    dest[idx++] = digits[0];

    if (ndigits > 1) {
        dest[idx++] = '.';
        memcpy(dest + idx, digits + 1, ndigits - 1);
        idx += ndigits - 1;
    }

    dest[idx++] = 'e';

    char sign = K + ndigits - 1 < 0 ? '-' : '+';
    dest[idx++] = sign;

    int cent = 0;

    if (exp > 99) {
        cent = exp / 100;
        dest[idx++] = cent + '0';
        exp -= cent * 100;
    }
    if (exp > 9) {
        int dec = exp / 10;
        dest[idx++] = dec + '0';
        exp -= dec * 10;

    }
    else if (cent) {
        dest[idx++] = '0';
    }

    dest[idx++] = exp % 10 + '0';

    return idx;
}

static inline int filter_special(double fp, char* dest) noexcept
{
    if (fp == 0.0) {
        dest[0] = '0';
        return 1;
    }

    uint64_t bits = get_dbits(fp);

    bool nan = (bits & expmask) == expmask;

    if (!nan) {
        return 0;
    }

    if (bits & fracmask) {
        dest[0] = 'n'; dest[1] = 'a'; dest[2] = 'n';

    }
    else {
        dest[0] = 'i'; dest[1] = 'n'; dest[2] = 'f';
    }

    return 3;
}

struct fp
{
    char str_[24];
    std::size_t size_{0};

    fp() = default;

    const char* cbegin() const noexcept
    {
        return str_;
    }

    const char* cend() const noexcept
    {
        return str_ + size_;
    }
};

static inline std::size_t dtoa(double d, char *dest)
{
    char digits[18];

    std::size_t str_len = 0;
    bool neg = false;

    if (get_dbits(d) & signmask) {
        dest[0] = '-';
        str_len++;
        neg = true;
    }

    int spec = filter_special(d, dest + str_len);

    if (spec) {
        return str_len + spec;
    }

    int K = 0;
    int ndigits = grisu2(d, digits, &K);

    str_len += emit_digits(digits, ndigits, dest + str_len, K, neg);

    return str_len;
}

static inline fp dtofp(double d) noexcept
{
    fp result;
    result.size_ = static_cast<decltype(fp::size_)>(dtoa(d, result.str_));
    return result;
}

} // namespace fpconv
} // namespace num
} // namespace btdef
