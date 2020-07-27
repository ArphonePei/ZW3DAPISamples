#include "stdio.h"
#include <cstring>
#include "VxApi.h"

#define INQ_OUTPUT_FORM "SetColor.ui"

/* Declare your function */
int SetColor(int idData);


int SetColorInit(int format, void *data)
{
    /* Register your function with ZW3D. */
	cvxCmdFunc("SetColor", (void*)SetColor, VX_CODE_GENERAL);
	vxPath ApiPath;
	cvxPathApiLib("SetColor", ApiPath);
	cvxPathAdd(ApiPath);
    return 0;
}

int SetColorExit(void)
{
    /* Put your cleanup code here. */
	cvxCmdFuncUnload("SetColor");
    return 0;
}

int SetColor(int idData)
{
	int idEnt = 0;
	int idParent = 0;
	vxName entName = { 0 };
	int iRet = cvxDataGetEnt(idData, 1, &idEnt, NULL);
	char sMsg[64];
	sprintf_s(sMsg, "iRet=%d", iRet);
	cvxMsgDisp(sMsg);
	if (cvxEntClassNum(idEnt)==VX_ENT_FACE)
	{
		cvxMsgDisp("in cvxEntClassNum");
		svxFaceAt att;
		cvxMemZero(&att, sizeof(att));
		cvxPartInqFaceAt(idEnt, &att);
		att.front_color.r = 0;
		att.front_color.g = 255;
		att.front_color.b = 255;
		cvxPartSetFaceAt(1, &idEnt, &att);
	}
	else {
		cvxMsgDisp("type is not a eFace");
	}
	return 0;
}
