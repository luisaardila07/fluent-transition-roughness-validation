#include "udf.h"

/*
===============================================================================
SST sustaining-turbulence source terms for the smooth NACA0012 validation case
===============================================================================

Purpose
-------
Prevent artificial free-stream decay of k and omega while preserving the
experimental target:

    Tu_amb = 0.1 % = 0.001
    TVR    = mu_t/mu = 10

Exact values obtained from the verified smooth validation case:

    U_ref   = 34.22084400764108 m/s
    rho_ref = 1.225044359779965 kg/m^3
    mu_ref  = 1.789532371177454e-05 Pa s

Ambient SST state:

    k_amb = 3/2 * (U_ref * Tu_amb)^2
          = 1.7565992468929561e-03 m^2/s^2

    omega_amb = rho_ref*k_amb/(mu_ref*TVR)
              = 1.2024996219454037e+01 1/s

Canonical SST-sust source form:

    S_k     = beta_star * rho * omega_amb * k_amb
    S_omega = beta_2    * rho * omega_amb^2

with:

    beta_star = 0.09
    beta_2    = 0.0828  (freestream SST value, F1 -> 0)

At rho = rho_ref:

    S_k     ~= 2.3289060e-03 kg/(m s^3)
    S_omega ~= 1.4667363e+01 kg/(m^3 s^2)

Notes
-----
1. DEFINE_SOURCE is called by Fluent cell-by-cell; no begin_c_loop is needed.
2. Local density C_R(c,t) is used, while k_amb and omega_amb remain fixed.
3. These sources are independent of the local transported variable, so dS=0.
4. This file contains NO roughness UDF. The intended first test is:
       K = 0, Ks = 0
   using the already-converged smooth Transition-SST solution.
*/

#define BETA_STAR   0.09
#define BETA_FS     0.0828

#define K_AMB       1.7565992468929561e-03
#define OMEGA_AMB   1.2024996219454037e+01


DEFINE_SOURCE(sustain_k, c, t, dS, eqn)
{
    real rho = C_R(c,t);
    real source;

    source = BETA_STAR * rho * OMEGA_AMB * K_AMB;

    dS[eqn] = 0.0;

    return source;
}


DEFINE_SOURCE(sustain_omega, c, t, dS, eqn)
{
    real rho = C_R(c,t);
    real source;

    source = BETA_FS * rho * OMEGA_AMB * OMEGA_AMB;

    dS[eqn] = 0.0;

    return source;
}
