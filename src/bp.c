#include "bp.h"
#include "arith.h"

// Algoritmo 7.
pf_int_t pf_direct_bp(const pf_int_t *B, size_t m) {
    if (m == 0) return 0;

    // Encontrar beta e iota 
    pf_int_t beta = B[0];
    size_t iota = 1;
    for (size_t i = 2; i <= m; i++) {
        if (B[i - 1] > beta) {
            beta = B[i - 1];
            iota = i;
        }
    }

    // Contribucion de clase. phi_1 + phi_2 - phi_3
    pf_int_t phi1 = 1, phi2 = 1, phi3 = 1;
    for (size_t i = 1; i <= m; i++) {
        phi1 *= beta;
        phi2 *= (beta + 1);
        if (i < iota) phi3 *= beta;
        else          phi3 *= (beta + 1);
    }


    pf_int_t alpha;
    size_t i;
    if (iota == 1) {
        alpha = (m >= 2) ? B[1] : 0;   // b_2
        i = 3;
    } else {
        alpha = B[0];                   // b_1
        i = 2;
    }

    // Contribucion del vector
    while (i <= m) { //<
        if (i < iota) {
            alpha = alpha * beta + B[i - 1];
        }
        if (i > iota) {
            alpha = alpha * (beta + 1) + B[i - 1];
        }
        i++;
    }

    // Sumar la contribucion de clase.
    alpha += phi1 + phi2 - phi3;
    return alpha;
}

// Algoritmo 8. InverseBP(alpha, m, B).
void pf_inverse_bp(pf_int_t alpha, size_t m, pf_int_t *B) {
    if (m == 0) return;

    pf_int_t beta = pf_iroot(alpha, (pf_int_t)m);

    pf_int_t phi1 = 1, phi2 = 1;
    for (size_t i = 1; i <= m; i++) {
        phi1 *= beta;
        phi2 *= (beta + 1);
    }

    // Determinar iota probando contribuciones de clase crecientes.
    pf_int_t phi3 = phi2;
    size_t iota = 1;
    while (beta > 0 && phi1 + phi2 - phi3 * beta / (beta + 1) <= alpha) { //<
        iota++;
        phi3 = phi3 * beta / (beta + 1);
    }

    alpha -= (phi1 + phi2 - phi3);
    B[iota - 1] = beta;

    // Decodificar el resto en bases mixtas 
    for (size_t i = m; i >= 1; i--) {
        if (i > iota) {
            B[i - 1] = alpha % (beta + 1);
            alpha /= (beta + 1);
        } else if (i < iota) {
            B[i - 1] = alpha % beta;
            alpha /= beta;
        }
        if (i == 1) break;
    }
}
