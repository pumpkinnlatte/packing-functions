#ifndef PACKING_GTP_H
#define PACKING_GTP_H

#include "packing_types.h"

// Algoritmo 1.
pf_int_t pf_direct_gtp(const pf_int_t *V, size_t m);

// Algoritmo 3. 
void pf_inverse_gtp(pf_int_t alpha, size_t m, pf_int_t *V);

// Biyeccion GTP DP
void pf_gtp_to_dp(const pf_int_t *V, size_t m, pf_int_t *W);

// Biyeccion DP GTP
void pf_dp_to_gtp(const pf_int_t *W, size_t m, pf_int_t *V);

// Algoritmo 5.AllSubsets(k, t).
typedef void (*pf_subset_cb)(const pf_int_t *V, size_t t, void *user);

void pf_all_subsets(pf_int_t k, size_t t, pf_subset_cb cb, void *user);

#endif