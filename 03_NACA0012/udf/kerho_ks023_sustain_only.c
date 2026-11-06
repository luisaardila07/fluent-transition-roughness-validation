#include "udf.h"

/*
KERHO / NACA0012 — MANUAL-K BASE

This UDF sets ONLY:
1) SST sustaining sources
2) Localized Standard Wall equivalent sand-grain roughness Ks = 0.23 mm

It deliberately DOES NOT set the Transition-SST geometric roughness K.
K will be changed manually later in Fluent.

Strip:
  c = 0.5334 m
  x/c = 0.00612 ... 0.02580
  upper surface only

Wall:
  Ks = 0.23 mm
  Cs is set by the Python script to 0.50
*/

#define BETA_STAR   0.09
#define BETA_FS     0.0828
#define K_AMB       1.7565992468929561e-03
#define OMEGA_AMB   1.2024996219454037e+01

DEFINE_SOURCE(sustain_k, c, t, dS, eqn)
{
    real rho = C_R(c,t);
    dS[eqn] = 0.0;
    return BETA_STAR * rho * OMEGA_AMB * K_AMB;
}

DEFINE_SOURCE(sustain_omega, c, t, dS, eqn)
{
    real rho = C_R(c,t);
    dS[eqn] = 0.0;
    return BETA_FS * rho * OMEGA_AMB * OMEGA_AMB;
}

DEFINE_PROFILE(kerho_ks_023mm, thread, position)
{
    face_t f;
    real xc[ND_ND];

    const real x_start = 0.003264408; /* 0.00612*c */
    const real x_end   = 0.013761720; /* 0.02580*c */
    const real Ks      = 0.000230000; /* 0.23 mm */

    begin_f_loop(f, thread)
    {
        F_CENTROID(xc, f, thread);

        if ((xc[0] >= x_start) &&
            (xc[0] <= x_end) &&
            (xc[2] > 0.0))
        {
            F_PROFILE(f, thread, position) = Ks;
        }
        else
        {
            F_PROFILE(f, thread, position) = 0.0;
        }
    }
    end_f_loop(f, thread)
}
