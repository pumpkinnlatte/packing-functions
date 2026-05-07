#include "gtp.h"
#include "arith.h"

// Algoritmo 1. DirectGTP(V)
pf_int_t pf_direct_gtp(const pf_int_t *V, size_t m) {
    pf_int_t alpha = 0;
    for (size_t i = 1; i <= m; i++) {
        alpha += pf_binomial(V[i - 1], i);
    }
    return alpha;
}

// Algoritmo 3. InverseGTP(alpha, m, V).
void pf_inverse_gtp(pf_int_t alpha, size_t m, pf_int_t *V) {
    // Calcular f = m!
    pf_int_t f = 1;
    for (size_t k = 1; k <= m; k++) f *= k;

    for (size_t i = m; i >= 1; i--) {
        pf_int_t a_i = pf_iroot(f * alpha, (pf_int_t)i);
        pf_int_t v_i = a_i;
        pf_int_t delta = pf_binomial(v_i, i);

        // Avanzar v_i mientras C(v_i + 1, i) quepa en alpha.
        for (;;) {
            pf_int_t next = pf_next_binomial(delta, v_i, i);
            if (next > alpha) break;
            delta = next;
            v_i++;
        }

        V[i - 1] = v_i;
        alpha -= delta;
        f /= i;

        if (i == 1) break;  // size_t no puede bajar de 0; salida explicita
    }
}

// Biyeccion GTP DP.
void pf_gtp_to_dp(const pf_int_t *V, size_t m, pf_int_t *W) {
    if (m == 0) return;
    W[0] = V[0];
    for (size_t i = 1; i < m; i++) {
        W[i] = V[i] - V[i - 1] - 1;
    }
}

// Biyeccion DP  GTP.
void pf_dp_to_gtp(const pf_int_t *W, size_t m, pf_int_t *V) {
    pf_int_t s = 0;
    for (size_t i = 1; i <= m; i++) {
        s += W[i - 1];
        V[i - 1] = (i - 1) + s;
    }
}

// Algoritmo 5. AllSubsets(k, t, V).
void pf_all_subsets(pf_int_t k, size_t t, pf_subset_cb cb, void *user) {
    if (t == 0 || k < t) return;

    // Buffer en stack
    pf_int_t V[64];
    if (t > 64) return;  // limite practico

    for (size_t i = 0; i < t; i++) V[i] = (pf_int_t)i;

    size_t omega = (k == t) ? 1 : t;

    while (V[t - 1] != k || omega != 1) {
        cb(V, t, user);

        V[t - 1] += 1;
        if (V[t - 1] == k && omega != 1) {
            V[omega - 2] += 1;
            for (size_t i = omega - 1; i < t; i++) {
                V[i] = V[i - 1] + 1;
            }
            if (V[omega - 2] == k - t + omega - 2) omega -= 1;
            else                                   omega = t;
        }
    }
}
