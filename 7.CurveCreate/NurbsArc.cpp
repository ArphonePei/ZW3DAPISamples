#include "stdio.h"
#include "..\VxApi.h"

/* Declare your function */
int NurbsArc(void);

int NurbsArcInit(int format, void *data)
{
    /* Register your function with ZW3D. */
    cvxCmdFunc("NurbsArc", (void*)NurbsArc, VX_CODE_GENERAL);

    return 0;
}

int NurbsArcExit(void)
{
    /* Put your cleanup code here. */
    cvxCmdFuncUnload("NurbsArc");
    return 0;
}

/* Create a 3D NURBS arc. */
/* Return 1 if function fails, else 0. */
int NurbsArc(void)
{
    svxCurve zCrv;
    int idEnt = -1;
    int iRet = 0;

    /* knots of the arc */
    double knot[] = { 0.0000000000, 0.0000000000, 0.0000000000, 0.3333333333, 0.3333333333,
                      0.6666666667, 0.6666666667, 1.0000000000, 1.0000000000, 1.0000000000 };

    /* coordinates of the control points */
    double coord[] = { 40.0000000000,  0.0000000000, 0.0000000000, 1.0000000000,
                       28.2842712475, 28.2842712475, 0.0000000000, 0.7071067812,
                        0.0000000000, 40.0000000000, 0.0000000000, 1.0000000000,
                      -28.2842712475, 28.2842712475, 0.0000000000, 0.7071067812,
                      -40.0000000000,  0.0000000000, 0.0000000000, 1.0000000000,
                      -28.2842712475,-28.2842712475, 0.0000000000, 0.7071067812,
                        0.0000000000,-40.0000000000, 0.0000000000, 1.0000000000 };

    /* initialize the arc */
    cvxMemZero((void**)&zCrv, sizeof(svxCurve));

    /* type of the curve */
    zCrv.Type = VX_CRV_NURB;

    /* local frame (origin is arc center) */
    zCrv.Frame.identity = 1;
    zCrv.Frame.xx = 1;
    zCrv.Frame.yy = 1;
    zCrv.Frame.zz = 1;

    /* NURB parameter space data */
    zCrv.T.bnd.max = 1.0;
    zCrv.T.bnd.min = 0.0;
    zCrv.T.closed = 0;
    zCrv.T.degree = 2;
    zCrv.T.num_knots = 10;
    zCrv.T.knots = knot;
    zCrv.T.free_mem = 0;

    /* NURB control point data (mm) */
    zCrv.P.coord = coord;
    zCrv.P.dim = 4;
    zCrv.P.num_cp = 7;
    zCrv.P.plane = 3;
    zCrv.P.rat = 1;
    zCrv.P.free_mem = 0;

    /* adds the arc to active part */
    iRet = cvxPartCurve(&zCrv, &idEnt);

    if (iRet)
        cvxMsgDisp("Fail to display the curve.");

    return iRet;
}
