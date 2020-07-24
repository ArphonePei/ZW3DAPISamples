#include <stdio.h>
#include <string.h>
#include "..\VxApi.h"

/* Declare your function */
int FileLoadIgs(void);
int FileSaveIgs(void);

int IgsOperationInit(int format, void *data)
{
    /* Register your function with ZW3D. */
    cvxCmdFunc("FileSaveIgs", (void*)FileSaveIgs, VX_CODE_GENERAL);
    cvxCmdFunc("FileLoadIgs", (void*)FileLoadIgs, VX_CODE_GENERAL);

    return 0;
}

int IgsOperationExit(void)
{
    /* Put your cleanup code here. */
    cvxCmdFuncUnload("FileSaveIgs");
    cvxCmdFuncUnload("FileLoadIgs");
    return 0;
}

/* Save active file as "igs" format. */
/* Return 1 if function fails; 0 if it succeeds. */
int FileSaveIgs(void)
{
    int iRet = 0;
    vxLongName FileName, ExportName;
    vxPath FilePath;

    /* Make sure there is an active file. */
    cvxFileInqActive(FileName,256);
    if (!FileName[0])
    {
        cvxMsgDisp("There is no active file.");
        return 1;
    }

    /* Get the source directory for the active file. */
    cvxFileDirectory(FilePath);

    /* Set the output file path. */
    iRet = strcpy_s(ExportName, "test.igs");

    if (FilePath[0])
        iRet = cvxPathCompose(FilePath, ExportName);
    else
        iRet = strcpy_s(FilePath, "C:\\test.igs");

    if (iRet)
    {
        cvxMsgDisp("Fail to find the export path.");
        return iRet;
    }

    /* Use macro commands to save the active file as "igs" format. */
    char sMacro[sizeof(vxPath) + sizeof(vxName) + 128] = { '\0' };
    iRet = sprintf_s(sMacro, sizeof(vxPath) + sizeof(vxName) + 128,
        "[vxSend,\"!CdFileSaveAs2\", \"%s\"]", FilePath);

    if (iRet < 0)
    {
        cvxMsgDisp("Fail to save as igs format.");
        return 1;
    }

    sMacro[strlen(sMacro)] = '\0';

    iRet = cvxCmdMacro(sMacro, NULL);

    if (iRet)
        cvxMsgDisp("Fail to save as igs format.");
    else
        cvxMsgDisp("File is saved successfully.");

    return iRet;
}

/* Load specified "igs" file. */
/* Return 1 if function fails; 0 if it succeeds. */
int FileLoadIgs(void)
{
    int iRet = 0;
    vxLongName FileName;
    char PartName[] = {"IgsPart"};

    /* Make sure there is an active file. */
    cvxFileInqActive(FileName,256);
    if (!FileName[0])
    {
        cvxMsgDisp("There is no active file.");
        return 1;
    }

    /* Use macro commands to load the specified "igs" file. */
    char sMacro[sizeof(vxPath) + sizeof(vxName) + 128] = { '\0' };
    iRet = strcpy_s(sMacro, 
        "[vxSend,\"!CdFileOpen2\", \"C:\\test.igs\"]" );

    if (iRet)
        return iRet;

    sMacro[strlen(sMacro)] = '\0';

    iRet = cvxCmdMacro(sMacro, NULL);
    if (iRet)
    {
        cvxMsgDisp("Fail to load the file.");
        return iRet;
    }

    return iRet;
}
