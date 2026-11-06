#include "udf.h"
#include "math.h"

/* =====================================================================
   KERHO / NACA0012 — LANGEL A_r(k_s+) — PHASE 2A

   Passive transport:
       Gamma_Ar = 30 * (mu + mu_t)

   The A_r,w(x) table below was precomputed from the converged flow:
       k_s+ = rho*u_tau*k_s/mu
       A_r,w = 2000/[1 + exp(-(k_s+ - 13.5))]

   Flow and turbulence remain frozen during this Phase-2A run.
   ===================================================================== */

#define SIGMA_AR 30
#define X_START_M 3.2644079999999999e-03
#define X_END_M   1.3761720000000000e-02

#define N_AR_TAB 11

static const real AR_X_TAB[N_AR_TAB] = {
    3.7556824747973158e-03,
    4.3215439644454928e-03,
    4.9508206935687292e-03,
    5.6480231867651332e-03,
    6.4178363051285818e-03,
    7.2651532369465404e-03,
    8.1950826722332268e-03,
    9.2129846072389401e-03,
    1.0324462075131161e-02,
    1.1535435150228673e-02,
    1.2852123043797386e-02
};

static const real AR_W_TAB[N_AR_TAB] = {
    1.9999999948962177e+03,
    1.9999999999905019e+03,
    1.9999999999993361e+03,
    1.9999999999997965e+03,
    1.9999999999998713e+03,
    1.9999999999998793e+03,
    1.9999999999998543e+03,
    1.9999999999997885e+03,
    1.9999999999996287e+03,
    1.9999999999992926e+03,
    1.9999999999976530e+03
};


static real interp_ar_wall(real x)
{
    int j;

    if (x <= AR_X_TAB[0])
        return AR_W_TAB[0];

    if (x >= AR_X_TAB[N_AR_TAB-1])
        return AR_W_TAB[N_AR_TAB-1];

    for (j = 0; j < N_AR_TAB - 1; ++j)
    {
        if (x >= AR_X_TAB[j] && x <= AR_X_TAB[j+1])
        {
            real dx = AR_X_TAB[j+1] - AR_X_TAB[j];
            real a;

            if (fabs(dx) < 1.0e-30)
                return AR_W_TAB[j];

            a = (x - AR_X_TAB[j]) / dx;

            return AR_W_TAB[j]
                 + a * (AR_W_TAB[j+1] - AR_W_TAB[j]);
        }
    }

    return 0.0;
}


DEFINE_DIFFUSIVITY(ar_diffusivity_phase2a, c, t, i)
{
    real mu_l = C_MU_L(c,t);
    real mu_t = C_MU_T(c,t);

    if (mu_t < 0.0)
        mu_t = 0.0;

    return SIGMA_AR * (mu_l + mu_t);
}


DEFINE_PROFILE(ar_wall_bc_phase2a, t, i)
{
    face_t f;
    real xf[ND_ND];

    begin_f_loop(f,t)
    {
        F_CENTROID(xf,f,t);

#if RP_3D
        if ((xf[0] >= X_START_M) &&
            (xf[0] <= X_END_M) &&
            (xf[2] > 0.0))
            F_PROFILE(f,t,i) = interp_ar_wall(xf[0]);
        else
            F_PROFILE(f,t,i) = 0.0;
#else
        if ((xf[0] >= X_START_M) &&
            (xf[0] <= X_END_M) &&
            (xf[1] > 0.0))
            F_PROFILE(f,t,i) = interp_ar_wall(xf[0]);
        else
            F_PROFILE(f,t,i) = 0.0;
#endif
    }
    end_f_loop(f,t)
}
