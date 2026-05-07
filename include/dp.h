#ifndef PACKING_DP_H
#define PACKING_DP_H

#include "packing_types.h"

// Algoritmo 6.
pf_int_t pf_direct_dp(const pf_int_t *W, size_t m);

// 2D DP.
pf_int_t pf_direct_dp_2d(pf_int_t w1, pf_int_t w2);

// Inverso 2D DP
void pf_inverse_dp_2d(pf_int_t alpha, pf_int_t *W);

// Inverso m-dimensional via GTP
void pf_inverse_dp(pf_int_t alpha, size_t m, pf_int_t *W);

#endif
