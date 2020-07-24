#include "..\VxApi.h"

#pragma comment(lib, "..\\ZW3D.lib")

static int giEntFld = 1;
static int giMatFld = 2;

int MoveEntities(int idData);
int MoveEntitiesCb(char* from, int field, int iddata);
void MoveEntitiesEcho(int idData, void *echo);

int MoveEntitiesInit()
{
	cvxCmdFunc("MoveEntities", (void*)MoveEntities, VX_CODE_GENERAL);
	cvxCmdFunc("MoveEntitiesEcho", (void*)MoveEntitiesEcho, VX_CODE_GENERAL);
	cvxCmdCallback("MoveEntitiesCb", (void*)MoveEntitiesCb);
	return 0;
}

int MoveEntitiesExit()
{
	cvxCmdFuncUnload("MoveEntities");
	cvxCmdFuncUnload("MoveEntitiesEcho");
	cvxCmdFuncUnload("MoveEntitiesCb");
	return 0;
}

int MoveEntities(int idData)
{
	int iErr = 0;

	/* Get entities list form entity field */
	int *listEnt = nullptr;
	int iCnt = 0;
	cvxDataGetEnts(idData, giEntFld, &iCnt, &listEnt);

	/* Get matrix form matrix field */
	svxMatrix Mat;
	cvxDataGetMatrix(idData, giMatFld, &Mat);

	/* Move selected entities to specified position */
	iErr = cvxPartMoveTransformList(iCnt, listEnt, &Mat);

	/* Hide aux frame */
	cvxAuxFrameShow(-1, 0, 0);

	return iErr;
}

int MoveEntitiesCb(char* from, int field, int iddata)
{
	/* Show aux frame */
	cvxAuxFrameShow(iddata, giEntFld, giMatFld);
	return 0;
}

void MoveEntitiesEcho(int idData, void *echo)
{
	cvxEchoStart();

	/* Get entities list form entity field */
	int *listEnt = nullptr;
	int iCnt = 0;
	cvxDataGetEnts(idData, giEntFld, &iCnt, &listEnt);

	/* Get matrix form matrix field */
	svxMatrix Mat;
	cvxDataGetMatrix(idData, giMatFld, &Mat);

	/* Move selected entities to specified position */
	cvxPartMoveTransformList(iCnt, listEnt, &Mat);

	cvxEchoEnd();
	return;
}
