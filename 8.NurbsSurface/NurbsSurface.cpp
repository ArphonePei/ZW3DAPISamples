#include <windows.h>
#include "stdio.h"
#include "..\VxApi.h"

/* Declare your function */
int NurbsSurface(void);

int NurbsSurfaceInit(int format, void *data)
{
    vxPath ApiPath;

    /* Register your function with ZW3D. */
    cvxCmdFunc("NurbsSurface", (void*)NurbsSurface, VX_CODE_GENERAL);

	/* Get the path of the  folder that contains "NurbsSurface.dll"
	and add the folder directory to the list of search paths. */
	TCHAR szBuff[MAX_PATH];
	HMODULE module = GetModuleHandle(L"NurbsSurface.dll");
	GetModuleFileName(module, szBuff, sizeof(szBuff));
	int nLength = WideCharToMultiByte(CP_ACP, 0, szBuff,-1,NULL,0,NULL,NULL);
	WideCharToMultiByte(CP_ACP,0,szBuff,-1,ApiPath,nLength,NULL,NULL);
	vxPath ApiDir;
	cvxPathDir(ApiPath, ApiDir);
    cvxPathAdd(ApiDir);

    return 0;
}

int NurbsSurfaceExit(void)
{
    /* Put your cleanup code here. */
    cvxCmdFuncUnload("NurbsSurface");
    return 0;
}

/* Create a NURBS trimmed surface. */
/* Return 1 if function fails, else 0. */
int NurbsSurface(void)
{
    svxSurface zSurface;
    svxCurve *pTrimCurves = NULL;

    int idFace = -1;
    int iRet = 0;

    /* coordinates of control points of the outer loop */
    double outCurve_cps[][4]={ {0.0, 0.0, 0.0, 1.0},
                               {0.0, 1.0, 1.0, 1.0}, 
                               {1.0, 1.0, 1.0, 0.0},
                               {1.0, 0.0, 0.0, 0.0} };

    /* bounding box of curves in the outer loop */
    double outCurve_bnds[][4]={ {0.0, 0.0, 0.0, 1.0},
                                {0.0, 1.0, 1.0, 1.0}, 
                                {1.0, 1.0, 0.0, 1.0},
                                {0.0, 1.0, 0.0, 0.0} };

    /* knots of the outer loop */
    double outCurve_knots[]={ 0.0, 0.0, 1.0, 1.0 };

    /* coordinates of control points of the inner loop */
    double inCurve_cps[]={ 0.7000000000, 0.5000000000, 1.0000000000,
                           0.3500000000, 0.0767949192, 0.5000000000,
                           0.4000000000, 0.3267949192, 1.0000000000,
                           0.0500000000, 0.2500000000, 0.5000000000,
                           0.4000000000, 0.6732050808, 1.0000000000,
                           0.3500000000, 0.4232050808, 0.5000000000,
                           0.7000000000, 0.5000000000, 1.0000000000 };

    /* knots of the inner loop */
    double inCurve_knots[]={ 0.0000000000, 0.0000000000, 0.0000000000, 0.3333333333, 0.3333333333,
                             0.6666666667, 0.6666666667, 1.0000000000, 1.0000000000, 1.0000000000 };

    /* knots of the surface in the U direction and V direction */
    double u_knots[]={ 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0 };
    double v_knots[]={ 0.0, 0.0, 1.0, 1.0 };

    /* coordinates of control points of the surface */
    double surf_cps[]={ 80,  40,  40,
                        80,  40,   0,
                        80, -40,   0,
                        80, -40, -40,
                       200,  40,  40,
                       200,  40,   0,
                       200, -40,   0,
                       200, -40, -40 };

    /* initialize the trim curves */
    iRet = cvxMemAlloc(sizeof(svxCurve)*5, (void**)&pTrimCurves);
    if (iRet)
    {
        (void)cvxMsgDisp("Fail to allocate memory.");
        return iRet;
    }

    cvxMemZero((void*)pTrimCurves, sizeof(svxCurve)*5);

    /* Set the trimmed surface. */
    /* type of the surface */
    zSurface.Type = VX_SRF_EXTRUDE;

    /* direction of the surface's natural normal */
    zSurface.OutNormal = 0;

    /* NURB parameter space data in U direction */
    zSurface.U.closed = 0;
    zSurface.U.degree = 3;
    zSurface.U.num_knots = 8;
    zSurface.U.knots = u_knots;
    zSurface.U.bnd.min = 0.0;
    zSurface.U.bnd.max = 1.0;

    /* NURB parameter space data in V direction */
    zSurface.V.closed = 0;
    zSurface.V.degree = 1;
    zSurface.V.num_knots = 4;
    zSurface.V.knots = v_knots;
    zSurface.V.bnd.min = 0.0;
    zSurface.V.bnd.max = 1.0;

    /* NURB control point data (mm) */
    zSurface.P.dim = 3;
    zSurface.P.rat = 0;
    zSurface.P.plane = 4;
    zSurface.P.num_cp = 8;
    zSurface.P.coord = surf_cps;

    zSurface.P.box.X.min = 80;
    zSurface.P.box.X.max = 200;
    zSurface.P.box.Y.min = -40;
    zSurface.P.box.Y.max = 40;
    zSurface.P.box.Z.min = -40;
    zSurface.P.box.Z.max = 40;

    /* Set outer trim curve. */
    for (int i=0; i<4; i++)
    {
        pTrimCurves[i].Type = VX_CRV_NURB;

        pTrimCurves[i].P.rat = 0;
        pTrimCurves[i].P.dim = 2;
        pTrimCurves[i].P.plane = 2;
        pTrimCurves[i].P.num_cp = 2;
        pTrimCurves[i].P.coord = outCurve_cps[i];

        pTrimCurves[i].P.box.X.min = outCurve_bnds[i][0];
        pTrimCurves[i].P.box.X.max = outCurve_bnds[i][1];
        pTrimCurves[i].P.box.Y.min = outCurve_bnds[i][2];
        pTrimCurves[i].P.box.Y.max = outCurve_bnds[i][3];
        pTrimCurves[i].P.box.Z.min = 0.0;
        pTrimCurves[i].P.box.Z.max = 0.0;

        pTrimCurves[i].T.closed = 0;
        pTrimCurves[i].T.degree = 1;
        pTrimCurves[i].T.num_knots = 4;
        pTrimCurves[i].T.knots = outCurve_knots;
        pTrimCurves[i].T.bnd.min = 0.0;
        pTrimCurves[i].T.bnd.max = 1.0;
    }

    /* Set inner trim curve (circle). */
    pTrimCurves[4].Type = VX_CRV_NURB;

    pTrimCurves[4].P.rat = 1;
    pTrimCurves[4].P.dim = 3;
    pTrimCurves[4].P.plane = 3;
    pTrimCurves[4].P.num_cp = 7;
    pTrimCurves[4].P.coord = inCurve_cps;

    pTrimCurves[4].P.box.X.min = 0.0;
    pTrimCurves[4].P.box.X.max = 1.0;
    pTrimCurves[4].P.box.Y.min = 0.0;
    pTrimCurves[4].P.box.Y.max = 1.0;
    pTrimCurves[4].P.box.Z.min = 0.0;
    pTrimCurves[4].P.box.Z.max = 0.0;

    pTrimCurves[4].T.closed = 1;
    pTrimCurves[4].T.degree = 2;
    pTrimCurves[4].T.num_knots = 10;
    pTrimCurves[4].T.knots = inCurve_knots;
    pTrimCurves[4].T.bnd.min = 0.0;
    pTrimCurves[4].T.bnd.max = 1.0;

    pTrimCurves[4].Frame.xx = 1;
    pTrimCurves[4].Frame.yy = 1;
    pTrimCurves[4].Frame.zz = 1;
    pTrimCurves[4].Frame.identity = 1;


    /* Adds trimmed surface to the active part. */
    iRet = cvxPartFace(&zSurface, 5, pTrimCurves, 1, 0, 0.0, &idFace);
    if (iRet)
    {
        (void)cvxMsgDisp("Fail to add the surface to the active part.");
        goto CLEAN;
    }

CLEAN:
    /* Free memory of the curves. */
    cvxMemFree((void**)&pTrimCurves);

    return iRet;
}
