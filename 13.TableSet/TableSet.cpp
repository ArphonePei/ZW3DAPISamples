#include <windows.h>
#include <time.h>
#include <string>
#include "..\VxApi.h"

#pragma comment(lib, "..\\ZW3D.lib")

char gTemplate[24] = "uiTableForm";
const int giTableFiled = 2;

int ShowForm(void);
int TableSet(int, int*);
int TableSetCb(char*, int, int);

int TableSetInit(void)
{
	cvxCmdFunc("ShowForm", (void*)ShowForm, VX_CODE_GENERAL);
	cvxCmdFunc("TableSet", (void*)TableSet, VX_CODE_GENERAL);
	cvxCmdCallback("TableSetCb", (void*)TableSetCb);

   /* Get the path of the folder that contains "TableSet.dll"
   and add the folder directory to the list of search paths. */
   vxPath ApiPath, ApiDir;
   TCHAR szBuff[MAX_PATH];
   HMODULE module = GetModuleHandle(L"TableSet.dll");
   GetModuleFileName(module, szBuff, sizeof(szBuff));
   int nLength = WideCharToMultiByte(CP_ACP, 0, szBuff, -1, NULL, 0, NULL, NULL);
   WideCharToMultiByte(CP_ACP, 0, szBuff, -1, ApiPath, nLength, NULL, NULL);
   cvxPathDir(ApiPath, ApiDir);
   cvxPathAdd(ApiDir);

	return 0;
}

int TableSetExit(void)
{
	cvxCmdFuncUnload("ShowForm");
	cvxCmdFuncUnload("TableSet");
	cvxCmdFuncUnload("TableSetCb");

	return 0;
}

int ShowForm(void)
{
   cvxFormCreate("uiTableForm", 1);
   cvxFormShow("uiTableForm");

   return 0;
}

int TableSet(int idIn, int *idOut)
   {
   char szText[64] = "\0";

   switch (idIn)
      {
      case VX_FORM_INIT:
      {
      /* Initial table data */
      cvxTableCellTextSet(gTemplate, giTableFiled, 0, 0, "ID1");
      cvxTableComboSet2(gTemplate, giTableFiled, 0, 1, "Picture1,Picture2,Picture3", "1.png,2.png,3.png");
      svxColor clr = { 0,0,255 };
      cvxTableColorSet(gTemplate, giTableFiled, 0, 2, &clr, 0, 0);
      cvxTableDateSet(gTemplate, giTableFiled, 0, 3, ((long)time((time_t *)NULL)), VX_DATE_ONLY);
      break;
      }
      default:
         break;
      }
   return 0;
   }

int TableSetCb(char* TemplateName, int Filed, int Item)
   {
   svxTableCallBackParam vsData;
   cvxMemZero(&vsData, sizeof(svxTableCallBackParam));
   char szText[128] = "\0";

   switch (abs(Filed))
      {
      case giTableFiled:
      {
      cvxTableCallBackParamGet(TemplateName, Filed, &vsData);
      switch (vsData.evt)
         {
         case VX_RIGHT_CLICK:
         {
         cvxPopupShow("AsmPopMulAln");
         break;
         }
         case VX_LEFT_CLICK:
         {
         evxTableCellType eTblType;
         cvxTableTypeGet(TemplateName, Filed, vsData.iRow, vsData.iCol, &eTblType);
         switch (eTblType)
            {
            case VX_TABLECELL_TYPE_TEXT:
            cvxTableCellTextGet(TemplateName, Filed, vsData.iRow, vsData.iCol, szText, sizeof(szText));
            break;
            default:
               break;
            }
         cvxItemSet(TemplateName, 1, 1, szText);
         break;
         }
         default:
            break;
         }
      }
      break;
      default:
         break;
      }

   return 0;
   }
