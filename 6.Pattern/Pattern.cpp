#include "stdio.h"
#include "..\VxApi.h"

int LinearPatternUtil(int);
int LinearPattern(int);
void LinearPatternEO(int, void*);      // respond mouse move message

int PatternInit(int format, void *data)
{
    vxPath ApiPath;

    // register command template functions
    cvxCmdFunc("Pattern", (void*)LinearPattern, VX_CODE_GENERAL);
    cvxCmdCallback("PatternEO", (void*)LinearPatternEO);

    // register command template file
    cvxPathApiLib("Pattern", ApiPath);
    cvxPathAdd(ApiPath);
    return(0);
}

int PatternExit(void)
{
    // put cleanup code here
    cvxCmdFuncUnload("Pattern");
    return(0);
}

int LinearPatternUtil(int idData)
{
    int iRet = 0;
    int nEnts = -1;
    int *listEnts = NULL;
    svxData zDirData;
    svxVector zDir = {1.0, 0.0, 0.0};
    svxAxis zSrc, zDst;
    double dSpace = 1.0;
    double nInstance = 1.0;

    /* initialize data struct */
    cvxMemZero((void*)&zDirData, sizeof(svxData));
    cvxMemZero((void*)&zSrc, sizeof(svxAxis));
    cvxMemZero((void*)&zDst, sizeof(svxAxis));

    /* get entities */
    iRet = cvxDataGetEnts(idData, 1, &nEnts, &listEnts);
    if (iRet) goto CLEAN;

    /* get direction */
    iRet = cvxDataGet(idData, 2, &zDirData);
    if (iRet) goto CLEAN;

    if (zDirData.isDirection)
    {
        zDir = zDirData.Dir;
    }
    else
    {
        iRet = 1;
        goto CLEAN;
    }

    /* get spacing along the direction */
    iRet = cvxDataGetNum(idData, 3, &dSpace);
    if (iRet) goto CLEAN;

    /* get number of instances */
    iRet = cvxDataGetNum(idData, 4, &nInstance);
    if (iRet) goto CLEAN;

    /* pattern selected entities */
    zSrc.Dir.x = zDst.Dir.x = 1.0;

    for (int i=1; i<(int)nInstance; i++)
    {
        zDst.Pnt.x = zSrc.Pnt.x + zDir.x * i * dSpace;
        zDst.Pnt.y = zSrc.Pnt.y + zDir.y * i * dSpace;
        zDst.Pnt.z = zSrc.Pnt.z + zDir.z * i * dSpace;

        for (int j=0; j<nEnts; j++)
        {
            int idCopyEnt = -1;
            iRet = cvxPartCopyPntToPnt(listEnts[j], &zSrc, &zDst, &idCopyEnt);
            if (iRet) goto CLEAN;
        }
    }

CLEAN:
    cvxMemFree((void**)&listEnts);
    return iRet;
}

int LinearPattern(int idData)
{
    return LinearPatternUtil(idData);
}

void LinearPatternEO(int idData, void *echoObj/* internal use */)
{
    cvxEchoStart();
    (void)LinearPatternUtil(idData);
    cvxEchoEnd();
}
