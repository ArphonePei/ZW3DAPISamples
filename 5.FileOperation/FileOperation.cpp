#include "stdio.h"
#include "../VxApi.h"

/* Declare your function */
int FileCreate(void);
int FileCopy(void);

int FileOperationInit(int format, void *data)
{
    /* Register your function with ZW3D. */
    cvxCmdFunc("FileCreate", (void*)FileCreate, VX_CODE_GENERAL);
    cvxCmdFunc("FileCopy", (void*)FileCopy, VX_CODE_GENERAL);

    return 0;
}

int FileOperationExit(void)
{
    /* Put your cleanup code here. */
    cvxCmdFuncUnload("FileCreate");
    cvxCmdFuncUnload("FileCopy");
    return 0;
}

/* Create a file and add a part and a sheet to it. */
/* Return 1 if function fails, else 0. */
int FileCreate(void)
{
    int iRet = 0;
    char FileName[] = {"APITestFile.Z3"};
    char PartName[] = {"APITestPart"};
    char DrawName[] = {"APITestDwg"};

    /* Create a new file with the specified name and make it active. */
    iRet = cvxFileNew(FileName);
    if (iRet)
    {
        cvxMsgDisp("Fail to create a new file.");
        return iRet;
    }

    /* Create and insert a part object to active file. */
    iRet = cvxRootAdd(VX_ROOT_PART, PartName, NULL);
    if (iRet)
    {
        cvxMsgDisp("Fail to add part.");
        return iRet;
    }

    /* Exit the active part. */
    cvxPartExit();

    /* Create and insert a sheet object to active file. */
    iRet = cvxRootAdd(VX_ROOT_SHEET, DrawName, NULL);
    if (iRet)
    {
        cvxMsgDisp("Fail to add drawing.");
        return iRet;
    }

    /* Exit the active sheet. */
    cvxPartExit();

    /* Save file. */
    iRet = cvxFileSaveAs(FileName);
    if (iRet)
    {
        cvxMsgDisp("Fail to save.");
        return iRet;
    }

    /* Close active file. */
    cvxFileClose();
    return iRet;
}

/* Copy the active file. */
/* Return 1 if function fails, else 0. */
int FileCopy(void)
{
    int iRet = 0;
    char SourceFile[] = {"APISourceFile.Z3"};
    char SourceRoot[] = {"APISourcePart"};
    char DestFile[] = {"APICopyFile.Z3"};
    char DestRoot[] = {"APICopyPart"};
    svxBoxData Box;
    int idShape = -1;

    /* initialize memory */
    cvxMemZero((void*)&Box, sizeof(Box));

    Box.Center.x = 0.0;
    Box.Center.y = 0.0;
    Box.Center.z = 0.0;
    Box.X = 10.0;
    Box.Y = 10.0;
    Box.Z = 10.0;
    Box.Combine = VX_BOOL_ADD;
    Box.idPlane = 0;

    /* Create a new file with the specified name and make it active. */
    iRet = cvxFileNew(SourceFile);
    if (iRet)
    {
        cvxMsgDisp("Fail to create source file.");
        return iRet;
    }

    /* Create and insert a part object in the source file. */
    iRet = cvxRootAdd(VX_ROOT_PART, SourceRoot, NULL);
    if (iRet)
    {
        cvxMsgDisp("Fail to add part.");
        return iRet;
    }

    /* Creates a box shape and adds it to the source file. */
    iRet = cvxPartBox(&Box, &idShape);
    if (iRet)
    {
        cvxMsgDisp("Fail to create box.");
        return iRet;
    }

    /* Exit the active part. */
    cvxPartExit();

    /* Save file. */
    iRet = cvxFileSaveAs(SourceFile);
    if (iRet)
    {
        cvxMsgDisp("Fail to save.");
        return iRet;
    }

    cvxFileClose();

    /* Create another file. */
    iRet = cvxFileNew(DestFile);
    if (iRet)
    {
        cvxMsgDisp("Fail to create the destination file.");
        return iRet;
    }

    /* Copy the part to newly created file. */
    iRet = cvxRootCopy(SourceFile, SourceRoot, DestFile, DestRoot, 0, 1);
    if (iRet)
    {
        cvxMsgDisp("Fail to copy the root object.");
        return iRet;
    }

    /* Exit the active part. */
    cvxPartExit();

    /* Save file. */
    iRet = cvxFileSaveAs(DestFile);
    if (iRet)
    {
        cvxMsgDisp("Fail to save.");
        return iRet;
    }

    return iRet;
}
