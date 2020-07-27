#include "stdio.h"
#include <cstring>
#include "VxApi.h"

#define INQ_OUTPUT_FORM "OutputWindow"

/* Declare your function */
int HelloWorld();
int ShowUI();
int myBox();

int MyBoxInit(int format, void *data)
{
    /* Register your function with ZW3D. */
	cvxCmdFunc("Hello", (void*)HelloWorld, VX_CODE_GENERAL);   
	cvxCmdFunc("ShowUI", (void*)ShowUI, VX_CODE_GENERAL);
	cvxCmdFunc("myBox", (void*)myBox, VX_CODE_GENERAL);
    return 0;
}

int MyBoxExit(void)
{
    /* Put your cleanup code here. */
	cvxCmdFuncUnload("Hello");
	cvxCmdFuncUnload("ShowUI");
	cvxCmdFuncUnload("myBox");
    return 0;
}

int HelloWorld()
{
	cvxMsgDisp("Hello ZW3D");
	return 0;
}

int ShowUI()
{
	int iRet = 0;
	iRet = cvxFormCreate(INQ_OUTPUT_FORM, 0);
	if (iRet) {
		iRet = -1;
		goto END;
	}
	/*add info to list control*/
	cvxItemDel(INQ_OUTPUT_FORM, 1, -1);
	cvxItemAdd(INQ_OUTPUT_FORM, 1, "Hello ZW3D");

	/* Show information in GUI form. */
	cvxFormShow(INQ_OUTPUT_FORM);
END:
	return iRet;
}

int myBox()
{
	int idData;
	svxData tempData;
	//Set the option to choose the type
	cvxDataInit("FtAllBox", &idData);
	tempData.isNumber = 1;
	tempData.NumType = VX_NUM;
	tempData.Num = 1;
	cvxDataSet(idData, 8, &tempData);

	//Set the first point
	cvxDataZero(&tempData);
	tempData.isPoint = 1;
	tempData.PntType = VX_PNT3_ABS;
	tempData.Pnt.x = tempData.Pnt.y = tempData.Pnt.z = 0.0;
	cvxDataSet(idData, 1, &tempData);
	
	//set the second point
	cvxDataZero(&tempData);
	tempData.isPoint = 1;
	tempData.PntType = VX_PNT3_ABS;
	tempData.Pnt.x = tempData.Pnt.y = tempData.Pnt.z = 15.0;
	cvxDataSet(idData, 2, &tempData);

	//Run this Z3 command
	cvxCmdExec(idData);
	return 0;
}
