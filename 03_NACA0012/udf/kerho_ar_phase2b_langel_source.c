#include "udf.h"
#include "math.h"

/*
 * KERHO / NACA0012 - PHASE 2B LANGEL ROUGHNESS COUPLING
 *
 * Adds ONLY the roughness contribution to Fluent's native transported
 * Re_theta_t source equation:
 *
 *   S_rough = -c_theta_t * rho/T * b * F_Ar
 *
 * Fluent keeps its native baseline source term. A_r is UDS-0 from the
 * already-converged Phase-2A solution.
 *
 * Langel final roughness model: Eqs. 6.28-6.31.
 * Standard Langtry-Menter auxiliary definitions are used for F_theta_t,
 * F(lambda_theta), T, Tu and lambda_theta.
 */

#define PI_CONST        3.14159265358979323846
#define C_THETA_T       0.03
#define C_E2            50.0
#define AR_CORR_MAX     2000.0
#define TU_MIN          0.027
#define RETHETA_MIN     20.0
#define LAMBDA_MIN     -0.1
#define LAMBDA_MAX      0.1
#define SMALL_POS       1.0e-20
#define SMALL_U2        1.0e-12
#define SOURCE_SCALE    1.0

static real clamp_r(real x, real lo, real hi)
{
    if (x < lo) return lo;
    if (x > hi) return hi;
    return x;
}

static real F_lambda(real lambda_theta, real Tu)
{
    real f;

    lambda_theta = clamp_r(lambda_theta, LAMBDA_MIN, LAMBDA_MAX);
    Tu = MAX(Tu, TU_MIN);

    if (lambda_theta <= 0.0)
    {
        real p = 12.986*lambda_theta
               + 123.66*lambda_theta*lambda_theta
               + 405.689*lambda_theta*lambda_theta*lambda_theta;

        f = 1.0 + p * exp(-pow(Tu/1.5, 1.5));
    }
    else
    {
        f = 1.0
          + 0.275*(1.0 - exp(-35.0*lambda_theta))*exp(-(Tu/0.5));
    }

    return MAX(f, 1.0e-8);
}

static real Re_theta_base(real Tu)
{
    real r;

    Tu = MAX(Tu, TU_MIN);

    if (Tu <= 1.3)
        r = 1173.51 - 589.428*Tu + 0.2196/(Tu*Tu);
    else
        r = 331.50*pow(Tu - 0.5658, -0.671);

    return MAX(r, RETHETA_MIN);
}

/* dU/ds = (u_m u_n / U^2) du_m/dx_n */
static real local_dUds(cell_t c, Thread *t, real u, real v, real w, real U2)
{
    real *gu;
    real *gv;
    real dUds;

    if (U2 <= SMALL_U2)
        return 0.0;

    if (THREAD_STORAGE(t, SV_U_G) == NULL ||
        THREAD_STORAGE(t, SV_V_G) == NULL)
        return 0.0;

    gu = C_U_G(c,t);
    gv = C_V_G(c,t);

#if RP_3D
    {
        real *gw;

        if (THREAD_STORAGE(t, SV_W_G) == NULL)
            return 0.0;

        gw = C_W_G(c,t);

        dUds = (
              u*(u*gu[0] + v*gu[1] + w*gu[2])
            + v*(u*gv[0] + v*gv[1] + w*gv[2])
            + w*(u*gw[0] + v*gw[1] + w*gw[2])
        )/U2;
    }
#else
    dUds = (
          u*(u*gu[0] + v*gu[1])
        + v*(u*gv[0] + v*gv[1])
    )/U2;
#endif

    return dUds;
}

/* Iterative solution of the standard implicit Re_theta_t equilibrium correlation. */
static real solve_lambda_theta(real rho, real mu, real U, real dUds, real Tu)
{
    int n;
    real base;
    real Re_eq;
    real lambda_theta = 0.0;

    if (rho <= SMALL_POS || mu <= SMALL_POS || U <= 1.0e-8)
        return 0.0;

    Tu = MAX(Tu, TU_MIN);
    base = Re_theta_base(Tu);
    Re_eq = base;

    for (n = 0; n < 30; ++n)
    {
        real theta_t;
        real f;
        real new_Re_eq;

        theta_t = Re_eq*mu/(rho*U);
        lambda_theta = (rho*theta_t*theta_t/mu)*dUds;
        lambda_theta = clamp_r(lambda_theta, LAMBDA_MIN, LAMBDA_MAX);

        f = F_lambda(lambda_theta, Tu);
        new_Re_eq = MAX(base*f, RETHETA_MIN);

        /* Relax the fixed-point iteration for robustness. */
        Re_eq = 0.5*Re_eq + 0.5*new_Re_eq;
    }

    return lambda_theta;
}

/* Standard Langtry-Menter F_theta_t, Langel Eqs. 5.18-5.20. */
static real local_Ftheta(cell_t c, Thread *t,
                         real rho, real mu, real U2,
                         real Re_theta_tilde, real gamma)
{
    real d;
    real omega_sst;
    real Omega;
    real Re_omega;
    real Fwake;
    real delta;
    real f1;
    real f2;
    real Ftheta;

    if (rho <= SMALL_POS || mu <= SMALL_POS || U2 <= SMALL_U2)
        return 1.0;

    d = C_WALL_DIST(c,t);
    omega_sst = MAX(C_O(c,t), 0.0);
    Omega = Rotationrate_Mag(c,t);

    Re_theta_tilde = MAX(Re_theta_tilde, RETHETA_MIN);
    gamma = clamp_r(gamma, 0.0, 1.0);

    Re_omega = rho*omega_sst*d*d/mu;
    Fwake = exp(-pow(Re_omega/1.0e5, 2.0));

    delta = 375.0*Omega*mu*Re_theta_tilde*d/(rho*U2);

    if (delta > SMALL_POS)
    {
        real ratio = d/delta;
        f1 = (ratio < 50.0) ? Fwake*exp(-pow(ratio,4.0)) : 0.0;
    }
    else
    {
        f1 = 0.0;
    }

    f2 = 1.0 - pow((C_E2*gamma - 1.0)/(C_E2 - 1.0), 2.0);

    Ftheta = MIN(MAX(f1, f2), 1.0);
    return clamp_r(Ftheta, 0.0, 1.0);
}

/* Langel Eq. 6.28. */
static real local_FAr(real Ar_raw, real Ftheta, real Flambda)
{
    real Ar;
    real first_term;
    real second_term;

    /* Numerical protection only for the correlation evaluation.
       The transported UDS field itself is not modified. */
    Ar = clamp_r(Ar_raw, 0.0, AR_CORR_MAX);

    if (Ar <= 0.0)
        return 0.0;

    first_term = 4.0*pow(Ar,0.42)*Ftheta;

    if (Flambda <= 1.0)
        second_term = 4.0*Ar*(1.0/Flambda)*(1.0 - Ftheta);
    else
        second_term = 4.0*Ar*pow(1.0/Flambda,0.6)*(1.0 - Ftheta);

    return MAX(first_term + second_term, 0.0);
}

/* Langel Eq. 6.31. */
static real local_b(real Re_theta_tilde)
{
    real b = 0.5*sin((PI_CONST/155.0)*Re_theta_tilde
                     - (97.0*PI_CONST/155.0)) + 0.5;

    return clamp_r(b, 0.0, 1.0);
}

/*
 * Additive Langel roughness source for Fluent's native Re_theta_t equation.
 * Fluent retains the baseline source internally; only the extra sink is
 * returned here.
 */
DEFINE_SOURCE(ar_retheta_source_langel, c, t, dS, eqn)
{
    real rho;
    real mu;
    real k;
    real u, v, w;
    real U2, U;
    real Tu;
    real dUds;
    real lambda_theta;
    real Flambda;
    real Re_theta_tilde;
    real gamma;
    real Ftheta;
    real Ar_raw;
    real FAr;
    real b;
    real Tscale;
    real source;

    dS[eqn] = 0.0;

    rho = C_R(c,t);
    mu = C_MU_L(c,t);

    if (rho <= SMALL_POS || mu <= SMALL_POS)
        return 0.0;

    u = C_U(c,t);
    v = C_V(c,t);
#if RP_3D
    w = C_W(c,t);
#else
    w = 0.0;
#endif

    U2 = u*u + v*v + w*w;
    if (U2 <= SMALL_U2)
        return 0.0;

    U = sqrt(U2);
    k = MAX(C_K(c,t), 0.0);
    Tu = 100.0*sqrt(2.0*k/3.0)/U;
    Tu = MAX(Tu, TU_MIN);

    dUds = local_dUds(c,t,u,v,w,U2);
    lambda_theta = solve_lambda_theta(rho,mu,U,dUds,Tu);
    Flambda = F_lambda(lambda_theta,Tu);

    Re_theta_tilde = C_STORAGE_R(c,t,SV_RETHETA);
    gamma = C_STORAGE_R(c,t,SV_INTERMIT);

    Ftheta = local_Ftheta(c,t,rho,mu,U2,Re_theta_tilde,gamma);

    Ar_raw = C_UDSI(c,t,0);
    FAr = local_FAr(Ar_raw,Ftheta,Flambda);

    if (FAr <= 0.0)
        return 0.0;

    b = local_b(Re_theta_tilde);

    Tscale = 500.0*mu/(rho*U2);
    if (Tscale <= SMALL_POS)
        return 0.0;

    source = -SOURCE_SCALE*C_THETA_T*rho/Tscale*b*FAr;

    return source;
}