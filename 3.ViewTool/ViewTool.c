#include "stdio.h"
#include "..\VxApi.h"

#define Buffer 256
#define InformationBox "InformationBox"

int gsVisible = 0;
int gsFrozen = 0;
int gsDispMode = 1;
int gsDispAttribute = 0;

/* command function declaration */
int LayerInquire(int idData);
int LayerSet(int idData);
int LayerAdd(int idData);
int LayerDel(int idData);
int EntColorGet(int idData);
int EntColorSet(int idData);
void ViewGet(void);
int ViewSet(int idData);
int DispModeSet(int idData);
int DispColorSet(int idData);
void MsgAreaClose(void);
void MsgAreaOpen(void);
void PromptEnable(void);
void PromptDisable(void);

int ViewToolInit(int format, void *data)
{
    vxPath ApiPath;

    /* register your functions with ZW3D */
    cvxCmdFunc("LayerInquire", (void*)LayerInquire, VX_CODE_GENERAL);
    cvxCmdFunc("LayerSet", (void*)LayerSet, VX_CODE_GENERAL);
    cvxCmdFunc("LayerAdd", (void*)LayerAdd, VX_CODE_GENERAL);
    cvxCmdFunc("LayerDel", (void*)LayerDel, VX_CODE_GENERAL);
    cvxCmdFunc("EntColorGet", (void*)EntColorGet, VX_CODE_GENERAL);
    cvxCmdFunc("EntColorSet", (void*)EntColorSet, VX_CODE_GENERAL);
    cvxCmdFunc("ViewGet", (void*)ViewGet, VX_CODE_GENERAL);
    cvxCmdFunc("ViewSet", (void*)ViewSet, VX_CODE_GENERAL);
    cvxCmdFunc("DispModeSet", (void*)DispModeSet, VX_CODE_GENERAL);
    cvxCmdFunc("DispColorSet", (void*)DispColorSet, VX_CODE_GENERAL);
    cvxCmdFunc("MsgAreaClose", (void*)MsgAreaClose, VX_CODE_GENERAL);
    cvxCmdFunc("MsgAreaOpen", (void*)MsgAreaOpen, VX_CODE_GENERAL);
    cvxCmdFunc("PromptEnable", (void*)PromptEnable, VX_CODE_GENERAL);
    cvxCmdFunc("PromptDisable", (void*)PromptDisable, VX_CODE_GENERAL);

    /*register global variables*/
    cvxCmdVariable("gsVisible", (void*)&gsVisible);
    cvxCmdVariable("gsFrozen", (void*)&gsFrozen);
    cvxCmdVariable("gsDispMode", (void*)&gsDispMode);
    cvxCmdVariable("gsDispAttribute", (void*)&gsDispAttribute);
   
    /* Get the path of the "apilibs" folder that contains "ViewTool.dll"
    and add the folder directory to the list of search paths. */
    cvxPathApiLib("ViewTool", ApiPath);
    cvxPathAdd(ApiPath);

    return(0);
}

int ViewToolExit(void)
{
    /* put your cleanup code here */
    cvxCmdFuncUnload("LayerInquire");
    cvxCmdFuncUnload("LayerSet");
    cvxCmdFuncUnload("LayerAdd");
    cvxCmdFuncUnload("LayerDel");
    cvxCmdFuncUnload("EntColorGet");
    cvxCmdFuncUnload("EntColorSet");
    cvxCmdFuncUnload("ViewGet");
    cvxCmdFuncUnload("ViewSet");
    cvxCmdFuncUnload("DispModeSet");
    cvxCmdFuncUnload("DispColorSet");
    cvxCmdFuncUnload("MsgAreaClose");
    cvxCmdFuncUnload("MsgAreaOpen");
    cvxCmdFuncUnload("PromptEnable");
    cvxCmdFuncUnload("PromptDisable");
    return(0);
}

/* inquire specified layer information */
int LayerInquire(int idData)
{
    int ret = 0;
    int IsVisible = 0, IsFrozen = 0;
    vxName LyrName = "";

    if (ret = cvxFormCreate(InformationBox, 0))
        return 0;

    cvxItemDel(InformationBox, 1, -1);

    if (ret = cvxDataGetText(idData, 1, 32, LyrName))
    {
        cvxMsgDisp("layer does not exist");
        return 0;
    }

    if (ret = cvxLayerStateGet(LyrName, &IsVisible, &IsFrozen))
    {
        cvxMsgDisp("failed to inquire layer state");
        return 0;
    }

    if (1 == IsVisible)
    {
        if (1 == IsFrozen)
        {   
            cvxItemAdd(InformationBox, 1, "the layer is visible and frozen");
        }
        else 
        {
            cvxItemAdd(InformationBox, 1, "the layer is visible and active");
        }
    }
    if (0 == IsVisible)
    {
        if (1 == IsFrozen)
        {
            cvxItemAdd(InformationBox, 1, "the layer is invisible and frozen");
        }
        else
        {
            cvxItemAdd(InformationBox, 1, "the layer is invisible and frozen");
        }
    }
    cvxFormShow(InformationBox);
    return 0;
}

/* set specified layer state */
int LayerSet(int idData)
{
    int ret = 0;
    vxName LyrName;

    if (ret = cvxDataGetText(idData, 1, 32, LyrName))
    {
        cvxMsgDisp("can't get the name");
        return 0;
    }

    if (ret =cvxLayerStateSet(LyrName, gsVisible, gsFrozen))
    {
        cvxMsgDisp("can't set the state");
        return 0;
    }
    return 0;
}

/* create a layer */
int LayerAdd(int idData)
{
    int ret = 0;
    vxName LyrName;
    if (ret = cvxDataGetText(idData, 1, 32, LyrName))
    {
        cvxMsgDisp("can't get the layer");
        return 0;
    }
    if (ret = cvxLayerAdd(LyrName))
    {
        cvxMsgDisp("can't add the layer");
        return 0;
    }
    return 0;
}

/* delete the specified layer */
int LayerDel(int idData)
{
    int ret = 0;
    vxName LyrName;
    if (ret = cvxDataGetText(idData, 1, 32, LyrName))
    {
        cvxMsgDisp("can't get the data");
        return 0;
    }
    if (ret = cvxLayerDel(LyrName))
    {
        cvxMsgDisp("can't delete the layer");
        return 0;
    }
    return 0;
}

/* get entity color */
int EntColorGet(int idData)
{
    int ret = 0;
    int idEntity;
    evxColor Color;

    if (ret = cvxFormCreate(InformationBox, 0))
        return 0;

    cvxItemDel(InformationBox, 1, -1);

    if (ret = cvxDataGetEnt(idData, 1, &idEntity, NULL))
    {
        cvxMsgDisp("can't get the entity");
        return 0;
    }
    if (ret = cvxEntColorGet(idEntity, &Color))
    {
        cvxMsgDisp("can't get the color");
        return 0;
    }

    switch(Color)
    {
    case VX_COLOR_RGB: 
        cvxMsgDisp("color is rgb"); 
        cvxItemAdd(InformationBox, 1, "color is rgb");
        break;

    case VX_COLOR_NULL: 
        cvxMsgDisp("color is null"); 
        cvxItemAdd(InformationBox, 1, "color is null");
        break;

    case VX_COLOR_GREEN: 
        cvxMsgDisp("color is GREEN");
        cvxItemAdd(InformationBox, 1, "color is green");
        break;	

    case VX_COLOR_RED:
        cvxMsgDisp("color is RED");
        cvxItemAdd(InformationBox, 1, "color is red");
        break;

    case VX_COLOR_BROWN: 
        cvxMsgDisp("color is BROWN");
        cvxItemAdd(InformationBox, 1, "color is brown");
        break;

    case VX_COLOR_GOLDENROD: 
        cvxMsgDisp("color is GOLDENROD");
        cvxItemAdd(InformationBox, 1, "color is goldenrod");
        break;

    case VX_COLOR_MED_BLUE: 
        cvxMsgDisp("color is MED BLUE"); 
        cvxItemAdd(InformationBox, 1, "color is med blue");
        break;

    case VX_COLOR_DARK_MAGENTA:
        cvxMsgDisp("color is DARK MAGENTA"); 
        cvxItemAdd(InformationBox, 1, "color is dark magenta");
        break;

    case VX_COLOR_DARK_GREY:
        cvxMsgDisp("color is DARK GREY");
        cvxItemAdd(InformationBox, 1, "color is dark grey");
        break;

    case VX_COLOR_DARK_BLUE:
        cvxMsgDisp("color is DARK BLUE"); 
        cvxItemAdd(InformationBox, 1, "color is dark blue");
        break;

    case VX_COLOR_VIOLET: 
        cvxMsgDisp("color is VIOLET"); 
        cvxItemAdd(InformationBox, 1, "color is violet");
        break;

    case VX_COLOR_LIGHT_GREEN: 
        cvxMsgDisp("color is LIGHT GREEN"); 
        cvxItemAdd(InformationBox, 1, "color is green");
        break;

    case VX_COLOR_LIGHT_BLUE: 
        cvxMsgDisp("color is LIGHT BLUE");
        cvxItemAdd(InformationBox, 1, "color is light blue");
        break;

    case VX_COLOR_ROSE: 
        cvxMsgDisp("color is ROSE");
        cvxItemAdd(InformationBox, 1, "color is rose");
        break;

    case VX_COLOR_LIGHT_MAGENTA:
        cvxMsgDisp("color is LIGHT MAGENTA");
        cvxItemAdd(InformationBox, 1, "color is light magenta");
        break;

    case VX_COLOR_LIGHT_GREY: 
        cvxMsgDisp("color is light grey");
        cvxItemAdd(InformationBox, 1, "color is light grey");
        break;

    case VX_COLOR_BLACK: 
        cvxMsgDisp("color is black"); 
        cvxItemAdd(InformationBox, 1, "color is black");
        break;

    case VX_COLOR_WHITE: 
        cvxMsgDisp("color is WHITE"); 
        cvxItemAdd(InformationBox, 1, "color is white");
        break;

    case VX_COLOR_YELLOW:
        cvxMsgDisp("color is YELLOW"); 
        cvxItemAdd(InformationBox, 1, "color is yellow");
        break;

    case VX_COLOR_MED_GREY: 
        cvxMsgDisp("color is MED GREY"); 
        cvxItemAdd(InformationBox, 1, "color is med grey");
        break;

    default: break;
    }
    cvxFormShow(InformationBox);
    return 0;
}

/* set specified entity color */
int EntColorSet(int idData)
{
    int ret = 0;
    int nCnt = 0;
    int *idEntities = NULL;
    double ColorNum;

    if (ret = cvxDataGetEnts(idData, 1, &nCnt, &idEntities ))
    {
        cvxMsgDisp("can't get the data");
        return 0;
    }
    if (ret = cvxDataGetNum(idData, 2, &ColorNum))
    {
        cvxMsgDisp("default color");
        ColorNum = VX_COLOR_BLACK;
    }

    if (ret = cvxEntColorSet((evxColor)ColorNum, nCnt, idEntities))
    {
        cvxMsgDisp("can't set the color");
        return 0;
    }

    return 0;
}

/*get the view information*/
void ViewGet(void)
{
    int ret = 0;
    svxMatrix matrix;
    double dViewExtent = 200.0;
    char sViewExtent[Buffer] = {""};
    char sOrigin[Buffer] = {""};
    char sAxis[Buffer] = {""};

    if (ret = cvxFormCreate(InformationBox, 0))
        return;

    cvxItemDel(InformationBox, 1, -1);

    /* output view extent information */
    cvxViewGet(&matrix, &dViewExtent);
    sprintf_s(sViewExtent, Buffer, "view extent is : %lf mm", dViewExtent);
    cvxItemAdd(InformationBox, 1 ,sViewExtent);

    /* output origin of active view */
    sprintf_s(sOrigin, Buffer, "view origin is (%lf, %lf, %lf)", matrix.xt, matrix.yt, matrix.zt);
    cvxItemAdd(InformationBox, 1, sOrigin);

    /* output x-axis of active view */
    sprintf_s(sAxis, Buffer, "view x axis is (%lf, %lf, %lf)", matrix.xx, matrix.yx, matrix.zx);
    cvxItemAdd(InformationBox, 1, sAxis);

    /* output y-axis of active view */
    sprintf_s(sAxis, Buffer, "view y axis is (%lf, %lf, %lf)", matrix.xy, matrix.yy, matrix.zy);
    cvxItemAdd(InformationBox, 1, sAxis);
    
    /* output z-axis of active view */
    sprintf_s(sAxis, Buffer, "view z axis is (%lf, %lf, %lf)", matrix.xz, matrix.yz, matrix.zz);
    cvxItemAdd(InformationBox, 1, sAxis);

    /* show the information window */
    cvxFormShow(InformationBox);
}

/* set view */
int ViewSet(int idData)
{
    int ret = 0;
    double dViewExtent = 200.0;
    svxPoint origin = {0.0, 0.0, 0.0};

    svxData axisDir;
    svxMatrix matrix;
    
    /* initialization */
    cvxMatInit(&matrix);

    /* get view origin */
    if (ret = cvxDataGetPnt(idData, 1, &origin))
    {
        cvxMsgDisp("can't get view origin input");
        return 0;
    }
    else
    {
        matrix.xt = origin.x;
        matrix.yt = origin.y;
        matrix.zt = origin.z;
    }

    /* get view x axis */
    if (ret=cvxDataGet(idData, 2, &axisDir) || !axisDir.isDirection)
    {
        cvxMsgDisp("can't get view x axis input");
        return 0;
    }
    else
    {
        matrix.xx = axisDir.Dir.x;
        matrix.yx = axisDir.Dir.y;
        matrix.zx = axisDir.Dir.z;
    }

    /* get view y axis */
    if (ret=cvxDataGet(idData, 3, &axisDir) || !axisDir.isDirection)
    {
        cvxMsgDisp("can't get view y axis input");
        return 0;
    }
    else
    {
        matrix.xy = axisDir.Dir.x;
        matrix.yy = axisDir.Dir.y;
        matrix.zy = axisDir.Dir.z;
    }

    /* get view z axis */
    if (ret=cvxDataGet(idData, 4, &axisDir) || !axisDir.isDirection)
    {
        cvxMsgDisp("can't get view z axis input");
        return 0;
    }
    else
    {
        matrix.xz = axisDir.Dir.x;
        matrix.yz = axisDir.Dir.y;
        matrix.zz = axisDir.Dir.z;
    }

    /* make sure "identity" flag associated with matrix is set correctly */
    cvxMatSetIdentity(&matrix);

    /* get view extent */
    if (ret = cvxDataGetNum(idData, 5, &dViewExtent))
    {
        cvxMsgDisp("can't get view extent input");
        return 0;
    }

    /* set view */
    cvxViewSet(&matrix, dViewExtent);

    return 0;
}

/* display mode set */
int DispModeSet(int idData)
{
    cvxDispModeSet(gsDispMode);
    return 0;
}

/* set the display color */
int DispColorSet(int idData)
{
    int ret = 0;
    evxColor color;
    double IdColor;
    if (ret = cvxDataGetNum(idData, 2, &IdColor))
    {
        cvxMsgDisp("set default color");
        IdColor = VX_COLOR_BLACK;
    }
    color = (evxColor)IdColor;
    cvxDispColorSet(gsDispAttribute, color);
    return 0;
}

/* open the message area */
void MsgAreaOpen(void)
{
    cvxMsgAreaOpen();
}

/* close the message area */
void MsgAreaClose(void)
{
    cvxMsgAreaClose();
}

/* enable display of command prompts */
void PromptEnable(void)
{
    cvxPromptEnable();
    cvxMsgDisp("command prompts are enable");
}

/* disable display of command prompts */
void PromptDisable(void)
{
    cvxPromptDisable();
    cvxMsgDisp("command prompts are disable");
}
