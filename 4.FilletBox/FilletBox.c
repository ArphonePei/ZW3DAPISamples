#include "stdio.h"
#include"float.h"
#include "..\VxApi.h"


int FilletBox(int idData);

int FilletBoxInit(int format, void *data)
{
    vxPath ApiPath;
    cvxCmdFunc("FilletBox", (void*)FilletBox, VX_CODE_GENERAL);

    /* register command template file */
    cvxPathApiLib("FilletBox", ApiPath);
    cvxPathAdd(ApiPath);

    return(0);
}

int FilletBoxExit(void)
{
    /* put your cleanup code here */
    cvxCmdFuncUnload("FilletBox");
    return(0);
}

int FilletBox(int idData)
{
    int idShape = -1, idPlane = -1, idFilletBox = -1, idFilletFace = -1;
    int ret = 0, i = 0, nOp = -1, nBoxFaceCnt = 0, nEdgeCnt = 0;
    int *listFaces = NULL, *listEdges = NULL;

    double dRadius = 0.0, dDistance = 0.0, dMinDist = DBL_MAX;

    svxPoint *pListPrjPnts = NULL;
    svxPoint zSeedPnt;
    svxMatrix zPlaneMat;
    svxBoxData zBoxData;

    zBoxData.Combine = VX_BOOL_NONE;
    zBoxData.idPlane = -1;

    nOp = cvxOpCount();

    /* get input data*/
    if (ret = cvxDataGetNum(idData, 1 ,&zBoxData.Center.x))
        goto END;
    if (ret = cvxDataGetNum(idData, 2 ,&zBoxData.Center.y))
        goto END;
    if (ret = cvxDataGetNum(idData, 3 ,&zBoxData.Center.z))
        goto END;
    if (ret = cvxDataGetNum(idData, 4 ,&zBoxData.X))
        goto END;
    if (ret = cvxDataGetNum(idData, 5 ,&zBoxData.Y))
        goto END;
    if (ret = cvxDataGetNum(idData, 6 ,&zBoxData.Z))
        goto END;
    if (ret = cvxDataGetNum(idData, 7 ,&dRadius))
        goto END;
    if (ret = cvxDataGetEnt(idData, 8, &idPlane, NULL))
        idPlane = 0;

    zBoxData.idPlane = idPlane;

    /* create a box */
    if (ret = cvxPartBox(&zBoxData,&idShape ))
    {
        cvxMsgDisp("can't create the box");
        goto END;
    }

    /* get the point near to the top face */
    if (0 < idPlane)
    {
        if (ret = cvxEntMatrix(idPlane, &zPlaneMat))
        {
            cvxMsgDisp("can't get the matrix");
            goto END;
        }
        zPlaneMat.xt = 0.0;
        zPlaneMat.yt = 0.0;
        zPlaneMat.zt = 0.0;
        zSeedPnt.x=zBoxData.Center.x;
        zSeedPnt.y=zBoxData.Center.y;
        zSeedPnt.z=zBoxData.Center.z+(zBoxData.Z)/2;
        (void)cvxPntTransform(&zPlaneMat, &zSeedPnt);
    }
    else
    {
        zSeedPnt.x=zBoxData.Center.x;
        zSeedPnt.y=zBoxData.Center.y;
        zSeedPnt.z=zBoxData.Center.z+(zBoxData.Z)/2;
    }

    /* project point to each face */
    if (ret = cvxPartInqShapeFaces(idShape, &nBoxFaceCnt, &listFaces) )
    {
        cvxMsgDisp("can't get the faces");
        goto END;
    }
    if (ret = cvxMemAlloc(sizeof(svxPoint)*nBoxFaceCnt, (void**)&pListPrjPnts) )
    {
        cvxMsgDisp("can't allocate memory for project points");
        goto END;
    }

    /* find the top face */
    for (i=0; i<nBoxFaceCnt; i++)
    {
        if (ret = cvxPntProject(&zSeedPnt, listFaces[i], &pListPrjPnts[i]))
        {
            cvxMsgDisp("can't get project point");
            goto END;
        }

        dDistance = cvxPntDist(&zSeedPnt, &pListPrjPnts[i]);

        /* get the closest face */
        if (dDistance < dMinDist)
        {
            idFilletFace = listFaces[i];
            dMinDist = dDistance;
        }
    }

    /* get edges */
    if (ret = cvxPartInqFaceEdges(idFilletFace, &nEdgeCnt ,&listEdges))
    {
        cvxMsgDisp("can't get top face's edges");
        goto END;
    }

    /* fillet operation */
    if (ret = cvxPartFillet(nEdgeCnt, listEdges ,dRadius))
    {
        cvxMsgDisp("fillet failed");
        goto END;
    }

    /* get the shape id */
    idFilletBox = cvxEntNew(nOp, VX_ENT_SHAPE);

END:
    cvxMemFree((void**)&listFaces);
    cvxMemFree((void**)&listEdges);
    cvxMemFree((void**)&pListPrjPnts);
    return 0;
}
