#include "stdio.h"
#include <cstring>
#include <set>
#include "..\VxApi.h"

#define BUFFER 256
#define INQ_OUTPUT_FORM "InqOutputWindow"

/* Declare your function */
int InqPartShpFace(int idData);
int InqPartShpEdge(int idData);
int InqFaceLoop(int idData);
int InqEdgeUVCrv(int idData);

int TopoInquiryInit(int format, void *data)
{
    vxPath ApiPath;

    /* Register your function with ZW3D. */
    cvxCmdFunc("InqPartShpFace", (void*)InqPartShpFace, VX_CODE_GENERAL);
    cvxCmdFunc("InqPartShpEdge", (void*)InqPartShpEdge, VX_CODE_GENERAL);
    cvxCmdFunc("InqFaceLoop", (void*)InqFaceLoop, VX_CODE_GENERAL);
    cvxCmdFunc("InqEdgeUVCrv", (void*)InqEdgeUVCrv, VX_CODE_GENERAL);

    /* Get the path of the "apilibs" folder that contains "TopoInquiry.dll"
       and add the folder directory to the list of search paths. */
    cvxPathApiLib("TopoInquiry", ApiPath);
    cvxPathAdd(ApiPath);

    return 0;
}

int TopoInquiryExit(void)
{
    /* Put your cleanup code here. */

    cvxCmdFuncUnload("InqPartShpFace");
    cvxCmdFuncUnload("InqPartShpEdge");
    cvxCmdFuncUnload("InqFaceLoop");
    cvxCmdFuncUnload("InqEdgeUVCrv");
    return 0;
}

/*
Get the list of indices of the faces that belong to
the specified shape in the active file.

Return 1 if function fails, else 0.
*/
int InqPartShpFace
(
int idData          /*I: Command input data*/
)
{
    int i, iRet = 0;
    int idShape;
    int iCntFace = 0, *idFaceList = NULL;
    char sBuf[BUFFER];

    iRet = cvxFormCreate(INQ_OUTPUT_FORM, 0);
    if (iRet) goto CLEAN;

    cvxItemDel(INQ_OUTPUT_FORM, 1, -1);

    /* Get the specified shape. */
    iRet = cvxDataGetEnt(idData, 1, &idShape, NULL);
    if (iRet)
    {
        cvxItemAdd(INQ_OUTPUT_FORM, 1, "Fail to get the shape.");
        /* Show information in GUI form. */
        cvxFormShow(INQ_OUTPUT_FORM);
        return iRet;
    }

    /* Get the list of indices of the faces of the specified shape. */
    iRet = cvxPartInqShapeFaces(idShape, &iCntFace, &idFaceList);
    if (iRet) goto CLEAN;

    /* Show information in GUI form. */
    for (i = 0; i < iCntFace; i++)
    {
        sprintf_s(sBuf, BUFFER, "Face %d is belonged to shape %d", idFaceList[i], idShape);
        cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);
    }

    cvxFormShow(INQ_OUTPUT_FORM);

CLEAN:
    /* Free memory of the list. */
    cvxMemFree((void**)&idFaceList);

    return iRet;
}

/*
Get a list of indices of the edges that belong to
the specified shape in the active file.

Return 1 if function fails, else 0.
*/
int InqPartShpEdge
(
int idData          /*I: Command input data*/
)
{
    int i, iRet = 0;
    int idShape;
    int iCntEdge = 0, *idEdgeList = NULL;
    char sBuf[BUFFER];

    iRet = cvxFormCreate(INQ_OUTPUT_FORM, 0);
    if (iRet) return iRet;

    cvxItemDel(INQ_OUTPUT_FORM, 1, -1);

    /* Get the specified shape. */
    iRet = cvxDataGetEnt(idData, 1, &idShape, NULL);
    if (iRet)
    {
        cvxItemAdd(INQ_OUTPUT_FORM, 1, "Fail to get the shape.");
        /* Show information in GUI form. */
        cvxFormShow(INQ_OUTPUT_FORM);
        return iRet;
    }

    /* Get a list of indices of the edges that belong to the specified shape. */
    iRet = cvxPartInqShapeEdges(idShape, &iCntEdge, &idEdgeList);
    if (iRet) return iRet;

    for (i = 0; i < iCntEdge; i++)
    {
        sprintf_s(sBuf, BUFFER, "Edge %d is belonged to shape %d", idEdgeList[i], idShape);
        cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);
    }   

    /* Show information in GUI form. */
    cvxFormShow(INQ_OUTPUT_FORM);

    /* Free memory of the list. */
    cvxMemFree((void**)&idEdgeList);

    return iRet;
}

/*
Get a list of indices of the loops that belong to a face 
in the active file.

Return 1 if function fails, else 0.
*/
int InqFaceLoop
(
int idData          /*I: Command input data*/
)
{
    char sBuf[BUFFER];
    int i, j, iRet = 0;
    int idFace = -1, iCntLoop = 0, *idLoops = NULL;
    int iCntEdge = 0, *idEdges = NULL;

    iRet = cvxFormCreate(INQ_OUTPUT_FORM, 0);
    if (iRet) return iRet;

    cvxItemDel(INQ_OUTPUT_FORM, 1, -1);

    /* Get the specified face. */
    iRet = cvxDataGetEnt(idData, 1, &idFace, NULL);
    if (iRet)
    {
        cvxItemAdd(INQ_OUTPUT_FORM, 1, "Fail to get the face.");
        /* Show information in GUI form. */
        cvxFormShow(INQ_OUTPUT_FORM);
        return iRet;
    }

    /* Get the list of indices of the loops that belong to specified face. */
    iRet = cvxPartInqFaceLoops(idFace, 1, &iCntLoop, &idLoops);
    if (iRet) goto CLEAN;

    /* Show the list in GUI form. */
    for (i = 0; i < iCntLoop; i++)
    {
        sprintf_s(sBuf, BUFFER, "Loop %d is belonged to face %d", idLoops[i], idFace);
        cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);

        /* Get the list of indices of the edges that belong to a loop on the specified face. */
        iRet = cvxPartInqLoopEdges(idLoops[i], &iCntEdge, &idEdges);
        if (iRet) goto CLEAN;

        /* Remove the common edges on the loop (ex. a cylinder face has one common edge
           at the seam */
        std::set<int> idEdgeSet;
        std::set<int>::iterator iter;

        for (j = 0; j < iCntEdge; j++)
        {
            idEdgeSet.insert(idEdges[j]);
        }

        for (iter = idEdgeSet.begin(); iter != idEdgeSet.end(); iter++)
        {
            sprintf_s(sBuf, BUFFER, "Edge %d is belonged to loop %d", *iter, idLoops[i]);
            cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);
        }

        cvxMemFree((void**)&idEdges);
    }

    /* Show information in GUI form. */
    cvxFormShow(INQ_OUTPUT_FORM);

CLEAN:
    /* Free memory of the list. */
    cvxMemFree((void**)&idLoops);
    cvxMemFree((void**)&idEdges);

    return iRet;
}

/* Get all UV curves of selected edge. */
/* Return 1 if function fails; 0 if it succeeds.*/
int InqEdgeUVCrv
(
int idData		/*I: Command input data*/
)
{
    int i, j, iRet = 0;
    char sBuf[BUFFER];
    int idEdge = -1, iCntFace = 0, *idFaceList = NULL;
    svxCurve Crv;

    iRet = cvxFormCreate(INQ_OUTPUT_FORM, 0);
    if (iRet) return iRet;

    cvxItemDel(INQ_OUTPUT_FORM, 1, -1);

    /* Get the specified edge. */
    iRet = cvxDataGetEnt(idData, 1, &idEdge, NULL);
    if (iRet)
    {
        cvxItemAdd(INQ_OUTPUT_FORM, 1, "Fail to get the edge.");
        /* Show information in GUI form. */
        cvxFormShow(INQ_OUTPUT_FORM);
        return iRet;
    }

    /* Get the list of indices of the faces connected to the specified edge. */
    iRet = cvxPartInqEdgeFaces(idEdge, &iCntFace, &idFaceList);
    if (iRet) goto CLEAN;

    for (i = 0; i < iCntFace; i++)
    {
        sprintf_s(sBuf, BUFFER, "Edge %d is belonged to face %d", idEdge, idFaceList[i]);
        cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);

        /* initialize output */
        memset((void*)&Crv, 0, sizeof(svxCurve));
        cvxMatInit(&Crv.Frame);

        /* Get the UV curve of selected edge on specified face. */
        iRet = cvxPartInqEdgeCrv(idEdge, idFaceList[i], &Crv);
        if (iRet) goto CLEAN;

        switch(Crv.Type)
        {
        case VX_CRV_LINE:
            sprintf_s(sBuf, BUFFER, "Curve type is line.");
            cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);
            sprintf_s(sBuf, BUFFER, "Line start point: (%lf, %lf, %lf).", Crv.P1.x, Crv.P1.y, Crv.P1.z);
            cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);
            sprintf_s(sBuf, BUFFER, "Line end point: (%lf, %lf, %lf).", Crv.P2.x, Crv.P2.y, Crv.P2.z);
            cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);
            break;

        case VX_CRV_ARC:
            sprintf_s(sBuf, BUFFER, "Curve type is arc.");
            cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);
            if (Crv.P1.x || Crv.P1.y || Crv.P1.z)
            {
                sprintf_s(sBuf, BUFFER, "Arc center point: (%lf, %lf, %lf).", Crv.P1.x, Crv.P1.y, Crv.P1.z);
                cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);
            }
            sprintf_s(sBuf, BUFFER, "Arc start angle(degrees): %lf", Crv.A1);
            cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);
            sprintf_s(sBuf, BUFFER, "Arc end angle(degrees): %lf", Crv.A2);
            cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);
            sprintf_s(sBuf, BUFFER, "Arc radius: %lf", Crv.R);
            cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);
            break;

        case VX_CRV_CIRCLE:
            sprintf_s(sBuf, BUFFER, "Curve type is circle.");
            cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);
            if (Crv.P1.x || Crv.P1.y)
            {
                sprintf_s(sBuf, BUFFER, "Circle center point: (%lf, %lf, %lf).", Crv.P1.x, Crv.P1.y, Crv.P1.z);
                cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);
            }
            sprintf_s(sBuf, BUFFER, "Circle radius: %lf", Crv.R);
            cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);
            break;

        case VX_CRV_NURB:
            sprintf_s(sBuf, BUFFER, "Curve type is NURBS.");
            cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);
            if (Crv.T.closed)
            {
                sprintf_s(sBuf, BUFFER, "closed curve");
                cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);
            }
            else
            {
                sprintf_s(sBuf, BUFFER, "open curve");
                cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);
            }
            sprintf_s(sBuf, BUFFER, "degree: %d", Crv.T.degree);
            cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);
            sprintf_s(sBuf, BUFFER, "boundaries of parameter space [%lf, %lf]", Crv.T.bnd.min, Crv.T.bnd.max);
            cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);
            sprintf_s(sBuf, BUFFER, "number of knots: %d", Crv.T.num_knots);
            cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);
            sprintf_s(sBuf, BUFFER, "array of knot values:");
            cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);
            for (j = 0; j < Crv.T.num_knots; j++)
            {
                sprintf_s(sBuf, BUFFER, "%lf", Crv.T.knots[j]);
                cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);
            }

            if (Crv.P.rat)
            {
                sprintf_s(sBuf, BUFFER, "rational control points");
                cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);
            }
            else
            {
                sprintf_s(sBuf, BUFFER, "non-rational control points");
                cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);
            }

            sprintf_s(sBuf, BUFFER, "control point hyper plane type: %d", Crv.P.plane);
            cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);

            sprintf_s(sBuf, BUFFER, "bounding box of control point:");
            cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);
            sprintf_s(sBuf, BUFFER, "X [%f, %f]", Crv.P.box.X.min, Crv.P.box.X.max);
            cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);
            sprintf_s(sBuf, BUFFER, "Y [%f, %f]", Crv.P.box.Y.min, Crv.P.box.Y.max);
            cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);
            sprintf_s(sBuf, BUFFER, "Z [%f, %f]", Crv.P.box.Z.min, Crv.P.box.Z.max);
            cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);

            sprintf_s(sBuf, BUFFER, "number of coordinates per control point: %d", Crv.P.dim);
            cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);

            sprintf_s(sBuf, BUFFER, "number of control point: %d", Crv.P.num_cp);
            cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);

            sprintf_s(sBuf, BUFFER, "control point coordinates:");
            cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);
            for (j = 0; j < (Crv.P.num_cp * Crv.P.dim); j++)
            {
                sprintf_s(sBuf, BUFFER, "%lf", Crv.P.coord[j]);
                cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);
            }
            break;

        default:
            cvxItemAdd(INQ_OUTPUT_FORM, 1, "Unknown curve type");
            break;
        }

        sprintf_s(sBuf, BUFFER, "Local frame:");
        cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);
        if (Crv.Frame.identity)
        {
            sprintf_s(sBuf, BUFFER, "identity matrix");
            cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);
        }
        else
        {
            sprintf_s(sBuf, BUFFER, "Cosines of X axis and origin X (xt)");
            cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);
            sprintf_s(sBuf, BUFFER, "%lf, %lf, %lf, %lf", Crv.Frame.xx, Crv.Frame.yx, Crv.Frame.zx, Crv.Frame.xt);
            cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);
            sprintf_s(sBuf, BUFFER, "Cosines of Y axis and origin Y (yt)");
            cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);
            sprintf_s(sBuf, BUFFER, "%lf, %lf, %lf, %lf", Crv.Frame.xy, Crv.Frame.yy, Crv.Frame.zy, Crv.Frame.yt);
            cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);
            sprintf_s(sBuf, BUFFER, "Cosines of Z axis and origin Z (zt)");
            cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);
            sprintf_s(sBuf, BUFFER, "%lf, %lf, %lf, %lf", Crv.Frame.xz, Crv.Frame.yz, Crv.Frame.zz, Crv.Frame.zt);
            cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);
        }
        if (i < iCntFace - 1)
        {
            sprintf_s(sBuf, BUFFER, "==================================");
            cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);
        }

        cvxCurveFree(&Crv);
    }
    cvxFormShow(INQ_OUTPUT_FORM);

CLEAN:
    /* Free memory of the list. */
    cvxMemFree((void**)&idFaceList);
    cvxCurveFree(&Crv);

    return iRet;
}
