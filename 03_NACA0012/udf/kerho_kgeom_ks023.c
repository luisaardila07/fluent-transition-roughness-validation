#include "udf.h"

/*
  Kerho NACA 0012 localized roughness
  -----------------------------------
  Physical strip:
      c = 0.5334 m
      x/c = 0.00612 ... 0.0258
      upper surface only

  Transition-SST geometric roughness:
      Kgeom = 0.35 mm

  Standard wall equivalent sand-grain roughness:
      Ks = 0.23 mm  (midpoint diagnostic value)

  IMPORTANT:
  - DEFINE_TRANS_GEOMRGH acts in the VOLUME for the Transition-SST
    roughness correlation.
  - DEFINE_PROFILE acts on the WALL for the Standard Roughness model.
*/

DEFINE_TRANS_GEOMRGH(kerho_geom_rgh_ks023, c, t)
{
    real xc[ND_ND];
    real geomrgh = 0.0;

    const real x_start = 0.003264408;  /* 0.00612*c */
    const real x_end   = 0.013761720;  /* 0.0258*c */
    const real z_max   = 0.050000000;  /* comfortably beyond BL */
    const real k_geom  = 0.000350000;  /* 0.35 mm */

    C_CENTROID(xc, c, t);

    if ((xc[0] >= x_start) &&
        (xc[0] <= x_end) &&
        (xc[2] > 0.0) &&
        (xc[2] <= z_max))
    {
        geomrgh = k_geom;
    }

    return geomrgh;
}


DEFINE_PROFILE(kerho_ks_023mm, thread, position)
{
    face_t f;
    real xc[ND_ND];

    const real x_start = 0.003264408;  /* 0.00612*c */
    const real x_end   = 0.013761720;  /* 0.0258*c */
    const real ks      = 0.000230000;  /* 0.23 mm */

    begin_f_loop(f, thread)
    {
        F_CENTROID(xc, f, thread);

        if ((xc[0] >= x_start) &&
            (xc[0] <= x_end) &&
            (xc[2] > 0.0))
        {
            F_PROFILE(f, thread, position) = ks;
        }
        else
        {
            F_PROFILE(f, thread, position) = 0.0;
        }
    }
    end_f_loop(f, thread)
}
