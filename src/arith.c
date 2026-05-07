#include "arith.h"

pf_int_t gcd_u64(pf_int_t a, pf_int_t b) {
    while (b != 0) {
        pf_int_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

// Algoritmo 2. Coeficiente Binomial C(k, r)
pf_int_t pf_binomial(pf_int_t k, pf_int_t r) {
    if (k < r) return 0;

    pf_int_t b = 1;
    for (pf_int_t i = 1; i <= r; i++) {
        b = b * (k - i + 1) / i;
    }
    return b;
}

// Algoritmo 4. Next Binomial C(k+1, r)
pf_int_t pf_next_binomial(pf_int_t p, pf_int_t k, pf_int_t r) {
    if (k + 1 <  r) return 0;
    if (k + 1 == r) return 1;
    return p * (k + 1) / (k + 1 - r);
}

pf_int_t pf_isqrt(pf_int_t x) {
    if (x < 2) return x;

    pf_int_t lo = 1;
    pf_int_t hi = (pf_int_t)1u << 32;

    while (lo < hi) {
        pf_int_t mid = lo + (hi - lo + 1) / 2;
        if (mid <= x / mid) lo = mid;
        else                hi = mid - 1;
    }
    return lo;
}

static pf_int_t pow_saturating(pf_int_t base, pf_int_t i, pf_int_t limit) {
    pf_int_t r = 1;
    for (pf_int_t k = 0; k < i; k++) {
        if (r > limit / base) return limit + 1;
        r *= base;
    }
    return r;
}

pf_int_t pf_iroot(pf_int_t x, pf_int_t i) {
    if (i == 1) return x;
    if (i == 2) return pf_isqrt(x);
    if (x < 2)  return x;

    pf_int_t lo = 1;
    pf_int_t hi = (pf_int_t)1u << ((64 + i - 1) / i);

    while (lo < hi) {
        pf_int_t mid = lo + (hi - lo + 1) / 2;
        if (pow_saturating(mid, i, x) <= x) lo = mid;
        else                                hi = mid - 1;
    }
    return lo;
}

