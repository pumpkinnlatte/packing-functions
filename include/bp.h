#ifndef PACKING_BP_H
#define PACKING_BP_H

#include "packing_types.h"

// Algoritmo 7
pf_int_t pf_direct_bp(const pf_int_t *B, size_t m);

// Algoritmo 8 
void pf_inverse_bp(pf_int_t alpha, size_t m, pf_int_t *B);

#endif
