#include <stdio.h>
#include <string.h>
#include "..\VxApi.h"

/* Declare your function */
int FileExportImg(void);
int FileExportPdf(void);

int FileExportInit(int format, void *data)
{
    /* Register your function with ZW3D. */
    cvxCmdFunc("FileExportImg", (void*)FileExportImg, VX_CODE_GENERAL);
    cvxCmdFunc("FileExportPdf", (void*)FileExportPdf, VX_CODE_GENERAL);

    return 0;
}

int FileExportExit(void)
{
    /* Put your cleanup code here. */
    cvxCmdFuncUnload("FileExportImg");
    cvxCmdFuncUnload("FileExportPdf");
    return 0;
}

/* Save the active file as "png" format. */
/* Return 1 if function fails, else 0. */
int FileExportImg(void)
{
    int iRet = 0;
    vxLongName FileName;
	vxLongName ExportName;
    vxPath FilePath;
    svxImgData ImgData;

    /* Make sure there is an active file. */
    cvxFileInqActive(FileName,256);
    if (!FileName[0])
    {
        (void)cvxMsgDisp("There is no active file.");
        return 1;
    }

    /* Get the source directory for the active file. */
    cvxFileDirectory(FilePath);

    /* Set the output file path. */
    iRet = strcpy_s(ExportName, "test.png");

    if (FilePath[0])
        iRet = cvxPathCompose(FilePath, ExportName);
    else
        iRet = strcpy_s(FilePath, "test.png");

    if (iRet)
    {
        cvxMsgDisp("Fail to find the export path.");
        return iRet;
    }

    /* Initialize the output data. */
    cvxMemZero((void*)&ImgData, sizeof(ImgData));

    /* format of the output data */
    ImgData.Type = VX_EXPORT_IMG_TYPE_PNG;

    /* background color */
    //ImgData.Background.r = 0;
    //ImgData.Background.g = 0;
    //ImgData.Background.b = 0;

    /* background mode */
    ImgData.BkgndMode = VX_EXPORT_IMG_BKGND_MODE_CURRENT;

    /* color mode */
    ImgData.ColorMode = VX_EXPORT_IMG_COLOR_MODE_24BITS;

    /* range mode */
    ImgData.RangeMode = VX_EXPORT_IMG_RANGE_MODE_NORMAL;

    /* size of the image (pixel)*/
    ImgData.Height = 1000;
    ImgData.Width = 1000;

    /* compression rate (JPEG only) */
    //pImgData.Quality = 10;

    /* Export the file. */
    iRet = cvxFileExport(VX_EXPORT_TYPE_IMG, FilePath, (void*)&ImgData);
    if (iRet)
        cvxMsgDisp("Fail to save as png format.");
    else
        cvxMsgDisp("File is saved successfully.");

    return iRet;
};

/* Save the active file as "pdf" format. */
/* Return 1 if function fails; 0 if it succeeds. */
int FileExportPdf(void)
{
    int iRet = 0;
    vxLongName FileName;
	vxLongName ExportName;
    vxPath FilePath;
    svxPdfData PdfData;

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
    iRet = strcpy_s(ExportName, "test.pdf");

    if (FilePath[0])
        iRet = cvxPathCompose(FilePath, ExportName);
    else
        iRet = strcpy_s(FilePath, "test.pdf");

    if (iRet)
    {
        cvxMsgDisp("Fail to find the export path.");
        return iRet;
    }

    /* Initialize the output data. */
    cvxMemZero((void*)&PdfData, sizeof(PdfData));

    /* author */
    //PdfData.Author = ;

    /* color */
    //PdfData.Color = ;

    /* description */
    //PdfData.Description = ;

    /* dot per inch*/
    //PdfData.Dpi = ;

    /* font */
    //PdfData.Font = ;

    /* height of the paper (pixel) */
    PdfData.PaperHeight = 300;

    /* width of the paper (pixel) */
    PdfData.PaperWidth = 220;

    /* password */
    //PdfData.Password = ;

    /* range mode */
    PdfData.RangeMode = VX_EXPORT_PDF_RANGE_MODE_NORMAL;

    /**/
    //PdfData.Rect = ;

    /* subject */
    //PdfData.Subject = ;

    /* title */
    //PdfData.Title = ;

    /* type of pdf */
    PdfData.Type = VX_EXPORT_PDF_TYPE_OBJECT;

    /* Export the file. */
    iRet = cvxFileExport(VX_EXPORT_TYPE_PDF, FilePath, (void*)&PdfData);

    if (iRet)
        cvxMsgDisp("Fail to save as pdf format.");
    else
        cvxMsgDisp("File is saved successfully.");

    return iRet;
};
