#include "stdio.h"
#include <cstring>
#include "..\VxApi.h"

#define BUFFER 256
#define INQ_OUTPUT_FORM "InqOutputWindow"

/* Declare your function */
int InqRoot(int idData);
int InqPartComp(int idData);
int InqPartVar(int idData);

int BaseInquiryInit(int format, void *data)
{
    vxPath ApiPath;

    /* Register your function with ZW3D. */
    cvxCmdFunc("InqRoot", (void*)InqRoot, VX_CODE_GENERAL);
    cvxCmdFunc("InqPartComp", (void*)InqPartComp, VX_CODE_GENERAL);
    cvxCmdFunc("InqPartVar", (void*)InqPartVar, VX_CODE_GENERAL);

    /* Get the path of the "apilibs" folder that contains "BaseInquiry.dll"
       and add the folder directory to the list of search paths. */
    cvxPathApiLib("BaseInquiry", ApiPath);
    cvxPathAdd(ApiPath);

    return 0;
}

int BaseInquiryExit(void)
{
    /* Put your cleanup code here. */
    cvxCmdFuncUnload("InqRoot");
    cvxCmdFuncUnload("InqPartComp");
    cvxCmdFuncUnload("InqPartVar");
    return 0;
}

/* Inquire active root object information. */
/* Return 1 if function fails, else 0. */
int InqRoot(int idData)
{
    int i, idObj, nObjs = 0, iRet = 0;
    char sBuf[BUFFER];
    vxLongName ActFile, *ObjList = NULL;
    evxRootType ObjType;

    iRet = cvxFormCreate(INQ_OUTPUT_FORM, 0);
    if (iRet) goto CLEAN;

    cvxItemDel(INQ_OUTPUT_FORM, 1, -1);

    /* Make sure there is an active file. */
    cvxFileInqActive(ActFile, 256);
    if (!ActFile[0])
    {
        cvxItemAdd(INQ_OUTPUT_FORM, 1, "No active root object.");
        /* Show information in GUI form. */
        cvxFormShow(INQ_OUTPUT_FORM);
        return 1;
    }

    /* Get the list of root object. */
    iRet = cvxRootList(NULL, &nObjs, &ObjList);
    if (iRet) goto CLEAN;

    sprintf_s(sBuf, BUFFER, "No. of root objects: %d.", nObjs);
    cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);

    for (i=0; i < nObjs; i++)
    {
        if (cvxRootId(ObjList[i], &idObj, &ObjType))
            continue;

        /* application MUST reliably pair calls to cvxRootActivate2() function */
        iRet = cvxRootActivate2(ActFile, ObjList[i]);
        if (iRet) goto CLEAN;

        sprintf_s(sBuf, BUFFER, "The id of root object %s is %d.", ObjList[i], idObj);
        cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);

        iRet = cvxRootActivate2(NULL, NULL);
        if (iRet) goto CLEAN;
    }

    /* Show information in GUI form. */
    cvxFormShow(INQ_OUTPUT_FORM);

CLEAN:
    /* Free memory of the list. */
    cvxMemFree((void**)&ObjList);

    return iRet;	
}

/*
Get the lists of path and root object name of the components that belong 
to the active part.
Return 1 if function fails, else 0.
*/
int InqPartComp(int idData)
{
    int i, nComps = 0, iRet = 0;
    char sBuf[BUFFER];
    vxPath *CompPath = NULL;
    vxLongName ActFile, *CompList = NULL;

    iRet = cvxFormCreate(INQ_OUTPUT_FORM, 0);
    if (iRet) goto CLEAN;

    cvxItemDel(INQ_OUTPUT_FORM, 1, -1);

    /* Make sure there is an active file. */
    cvxFileInqActive(ActFile, 256);
    if (!ActFile[0])
    {
        cvxItemAdd(INQ_OUTPUT_FORM, 1, "No active root object.");
        /* Show information in GUI form. */
        cvxFormShow(INQ_OUTPUT_FORM);
        return 1;
    }

    /* Add the list of root object information to the GUI form. */
    if (iRet = cvxPartInqCompsInfo(NULL, NULL, &nComps, &CompPath, &CompList))
    {
        goto CLEAN;
    }
    else
    {
        iRet = cvxFormCreate(INQ_OUTPUT_FORM, 0);
        if (iRet) goto CLEAN;

        cvxItemDel(INQ_OUTPUT_FORM, 1, -1);

        if (nComps == 0)
        {
            cvxItemAdd(INQ_OUTPUT_FORM, 1, "There is no component.");
        }
        else
        {
            for (i=0; i<nComps; i++)
            {
                sprintf_s(sBuf, BUFFER, "The path of component %s:", CompList[i]);
                cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);
                cvxItemAdd(INQ_OUTPUT_FORM, 1, CompPath[i]);									
            }
        }
    }

    /* Show the information in the GUI form. */
    cvxFormShow(INQ_OUTPUT_FORM);

CLEAN:
    /* Free memory of the list. */
    cvxMemFree((void**)&CompPath);
    cvxMemFree((void**)&CompList);

    return iRet;
}

/*
Get the list of variables that belong to the specified part.
Note: Only variables that created with "!CdPartEqnNew" or "!CdVarDis"
      commands can be inquired.
Return 1 if function fails, else 0.
*/
int InqPartVar(int idData)
{
    int i, iRet = 0, nVars = 0;
    char sBuf[BUFFER];
    svxVariable *Vars = NULL;

    /* Get the list of variables that belong to the active part. */
    if (iRet = cvxPartInqVars(NULL, NULL, &nVars, &Vars))
    {
        goto CLEAN;
    }
    else
    {
        iRet = cvxFormCreate(INQ_OUTPUT_FORM, 0);
        if (iRet) goto CLEAN;

        cvxItemDel(INQ_OUTPUT_FORM, 1, -1);

        /* Show information in message area. */
        if (!nVars)
        {
            cvxItemAdd(INQ_OUTPUT_FORM, 1, "There is no variable.");
            goto CLEAN;
        }
        else
        {
            for (i=0; i<nVars; i++)
            {
                sprintf_s(sBuf, BUFFER, "%s = %lf", Vars[i].Name, Vars[i].Value);
                cvxItemAdd(INQ_OUTPUT_FORM, 1, sBuf);
                if (!Vars[i].Expression)
                    cvxItemAdd(INQ_OUTPUT_FORM, 1, Vars[i].Expression);
            } 
        }
        /* Show the information in the GUI form. */
        cvxFormShow(INQ_OUTPUT_FORM);
    }

CLEAN:
    /* Free memory of the list. */
    cvxMemFree((void**)&Vars);

    return iRet;
}
