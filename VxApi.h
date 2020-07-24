/*
** (C) Copyright 2012, ZWSOFT., Proprietary and Trade Secret
*********************************************************************
**
** MAIN HEADER FILE FOR THE ZW3D API
**
** Note -- Use the pipe "|" and exclamation mark "!" characters 
**         in the comments that label header file sections as
**         shown. They are required to properly generate HTML
**         documentation for the contents of this file.
**         "*!" is used to delimit main sections of information.
**         "*|" is used to delimit sub-sections.
**         The "file: filename.c" comment lines are required to
**         establish links in the HTML documentation.
**
**			  Add names of new functions to "driver/supp/zw3d.def"
**			  and to release notes (Api.INFO).  Recompile API docs
**			  using ~ApiDoc.
*********************************************************************
**
*/

#pragma once
#ifndef VX_API_H   /* this ifndef surrounds entire header */
#define VX_API_H

/*
*********************************************************************
*! 0. Data Types and Structures
*********************************************************************
*/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*! api version number
*******************************************/

#define VX_API_VERSION 2400

/*| basic data types
*******************************************/

/* object or file name (null-terminated text string) */
typedef char vxName[32];

/* long object or file name (null-terminated text string) */
typedef char vxLongName[256];

/* complete file path (null-terminated text string) */
typedef char vxPath[256];

/* arithmetic expression (null-terminated text string) */
typedef char vxExp[256];

/* variable expression (null-terminated text string) */
typedef char vxVarExp[1024];

/* 32 bit unsigned integer */
typedef unsigned int  vxUint32;

/* integer tree widget item id */
typedef int vxTreeItemId;

/*| math/geometry/topology information
*******************************************/

/* limits */
typedef struct svxLimit
   {
   double min;
   double max;
   } svxLimit;

/* 3D bounding box */
typedef struct svxBndBox
   {
   svxLimit X;
   svxLimit Y;
   svxLimit Z;
   } svxBndBox;

/* 2D point */
typedef struct svxPoint2
   {
   double x;
   double y;
   } svxPoint2;

/* 2D point(single precision floating point) */
typedef struct svxPoint2f
   {
   float x;
   float y;
   } svxPoint2f;

/* 3D point */
typedef struct svxPoint
   {
   double x;
   double y;
   double z;
   } svxPoint;

/* 3D point (single precision floating point) */
typedef struct svxPointf
   {
   float x;
   float y;
   float z;
   } svxPointf;

/* 2D vector */
typedef struct svxVector2
   {
   double x;
   double y;
   } svxVector2;

/* 3D vector */
typedef struct svxVector
   {
   double x;
   double y;
   double z;
   } svxVector;

/* 3D axis */
typedef struct svxAxis
   {
   svxPoint Pnt;     /* point */
   svxVector Dir;    /* direction */
   } svxAxis;

/* 3D transformation matrix or coordinate frame (local to world) */
typedef struct svxMatrix
   {
   char identity;          /* 1 if identity matrix, else 0 */
   double xx, yx, zx, xt;  /* cosines of X axis and origin X (xt) */
   double xy, yy, zy, yt;  /* cosines of Y axis and origin Y (yt) */
   double xz, yz, zz, zt;  /* cosines of Z axis and origin Z (zt) */
   } svxMatrix;

/* 3D triangle */
typedef struct svxTriangle
   {
   svxPoint Pnt[3];     /* triangle vertices */
   svxVector Normal;    /* outward-facing normal */
   } svxTriangle;

/* curve/surface extension options */
typedef enum evxExtendOpt
   {
   VX_EXTEND_UNDEFINED = 0,/* linear extend is the default when extension is undefined */
   VX_EXTEND_LINEAR = 1,
   VX_EXTEND_CIRCULAR = 2,
   VX_EXTEND_REFLECT = 3
   } evxExtendOpt;

/* curve types */
typedef enum evxCurveType
   {
   VX_CRV_UNDEFINED = 0,
   VX_CRV_LINE = 1,
   VX_CRV_ARC = 2,
   VX_CRV_CIRCLE = 3,
   VX_CRV_NURB = 4
   } evxCurveType;

typedef struct svxParameters
   {
   int closed;          /* 1=closed curve, 0=open curve */
   int degree;          /* degree (order = deg + 1) */
   int num_knots;       /* number of knots */
   svxLimit bnd;        /* boundaries of parameter space */
   double *knots;       /* array of knot values */
   int free_mem;        /* 1 if memory referenced by "knots" should
                        ** be free'd by cvxCurveFree()
                        */
   } svxParameters;

typedef struct svxControlPoints
   {
   int rat;             /* 1=RATIONAL, 0=NONRATIONAL control points  */
   int dim;             /* number of coordinates per control point (1-4) */
   int plane;           /* cp hyper plane type: 1:pnt, 2:line, 3:plane  */
   int num_cp;          /* number of control points in list */
   svxBndBox box;       /* bounding box of control points */
   double *coord;      	/* control point coordinates  (possibly weighted) */
                        /* if RATIONAL, points are of form (wx,wy,wz,w) */
   int free_mem;        /* 1 if memory referenced by "coord" should
                        ** be free'd by cvxCurveFree()
                        */
   } svxControlPoints;

/* 2D or 3D curvilinear geometry */
typedef struct svxCurve
   {
   evxCurveType Type;   /* curve type */
   svxMatrix Frame;     /* local frame (origin is center of circle/arc) */

   svxPoint P1, P2;     /* line start/end points */
   double A1, A2;       /* arc start/end angles (degrees) */
   double R;            /* circle/arc radius */

   svxParameters T;     /* NURB parameter space data */
   svxControlPoints P;  /* NURB control point data (mm) */
   } svxCurve;

typedef struct svxEvalCurv
   {
   int level;              /* level of evaluation                       */
   svxPoint pnt;           /* level 0 - x,y,z (or u,v)                  */
   svxPoint deriv_1;       /* level 1 - x',y' (or u',v')                */
   svxPoint deriv_2;       /* level 2 - x'',y'' (or u'',v'')            */
   svxPoint deriv_3;       /* level 3 - x''',y''',z''' (or u''',v''')   */
   } svxEvalCurv;


/* surface types */
typedef enum evxSurfaceType
   {
   VX_SRF_PLANE,
   VX_SRF_SPHERE,
   VX_SRF_CONE,
   VX_SRF_GRID,
   VX_SRF_CYLINDER,
   VX_SRF_EXTRUDE,
   VX_SRF_REVOLVE,
   VX_SRF_SWEEP_NOT_USED,
   VX_SRF_GEN,
   VX_SRF_OFFSET,
   VX_SRF_MESH,
   VX_SRF_RULED,
   VX_SRF_HEMIS,
   VX_SRF_FRUSTRUM,
   VX_SRF_DRAFT,
   VX_SRF_DRAFT_NPLN,
   VX_SRF_DRAFT_NUV,
   VX_SRF_ELLIPSOID,
   VX_SRF_3PT_PLANE,
   VX_SRF_BILIN_CNVX,
   VX_SRF_EXTRUDE_DRFT,
   VX_SRF_REVOLVE_PLN,
   VX_SRF_TORUS,
   VX_SRF_SMD_THCK_FC,
   VX_SRF_BEND_GEN,
   VX_SRF_PLANE_STAT_FC,
   VX_SRF_GEN_STAT_FC,
   VX_SRF_CIR_OFF,
   VX_SRF_VAR_OFF,
   VX_SRF_THRD_THDFC,
   VX_SRF_THRD_NTHDFC,
   VX_SRF_THRD_CONFC
   } evxSurfaceType;

/* 3D NURB surface (untrimmed) */
typedef struct svxSurface
   {
   evxSurfaceType Type; /* surface type */
   int OutNormal;       /* 1 if the surface's natural normal points towards
                        ** the outside of the shape it is associated with;
                        ** 0 if the surface's natural normal points inward.
                        */
   svxParameters U;     /* NURB parameter space data in U direction */
   svxParameters V;     /* NURB parameter space data in V direction */
   svxControlPoints P;  /* NURB control point data (mm) */

   } svxSurface;

/* variable types */
typedef enum evxVarType
   {
   VX_VAR_NUM = 0,      /* number */
   VX_VAR_STR = 1,      /* string */
   VX_VAR_PNT = 2,      /* point */
   VX_VAR_VEC = 3       /* vector */
   } evxVarType;

/* sub types of numeric variable */
typedef enum evxVarNumType
   {
   VX_VAR_CONS = 0,        /* constant number (without unit) */
   VX_VAR_DST = 1,         /* distance (evxUnitType) */
   VX_VAR_ANG = 2,         /* angle (evxAngUnitType) */
   VX_VAR_MASS = 3,        /* mass (evxMassUnitType) */
   VX_VAR_DENSITY = 4,     /* density (evxDensityUnitType) */
   VX_VAR_AREA = 5,        /* area (evxUnitType) */
   VX_VAR_VOLUME = 6,      /* volume (evxUnitType) */
   VX_VAR_MASS_MOMENT = 7  /* mass moment (evxMassMomentUnitType) */
   } evxVarNumType;

/* numeric variable */
typedef struct svxVariable
   {
   vxName Name;         /* variable name */
   double Value;        /* value is not required if Expression is defined */
   svxPoint pnt_val;    /* the value of point is not required if Expression is defined */
   vxVarExp Expression;    /* optional expression (Expression[0]=0 if undefined) */
   char type;           /* variable type (value of evxVarType) */
   char sub_type;       /* sub type of numeric variable (value of evxVarNumType) */
   char unit_type;      /* unit type (see evxVarNumType), evxUnitType for point type */
   } svxVariable;

/* system of linear units */
typedef enum evxUnitType
   {
   VX_UNIT_MICRON = 0,     /* micron */
   VX_UNIT_MILLIMETER = 1, /* millimeter */
   VX_UNIT_CENTIMETER = 2, /* centimeter */
   VX_UNIT_METER = 3,      /* meter */
   VX_UNIT_INCH = 4,       /* inch */
   VX_UNIT_FOOT = 5,       /* foot */
   VX_UNIT_YD = 6,         /* yard */
   VX_UNIT_DM = 7,         /* decimeter */
   VX_UNIT_MIL = 10        /* mil */
   } evxUnitType;

/* system of angle units */
typedef enum evxAngUnitType
   {
   VX_UNIT_DEG = 8,        /* degree */
   VX_UNIT_RAD = 9         /* radian */
   } evxAngUnitType;

/* system of mass units */
typedef enum evxMassUnitType
   {
   VX_UNIT_GM = 11,        /* gram */
   VX_UNIT_KG = 12,        /* kilogram */
   VX_UNIT_OZ = 13,        /* ounce */
   VX_UNIT_LB = 14         /* pound */
   } evxMassUnitType;

/* system of density units */
typedef enum evxDensityUnitType
   {
   VX_UNIT_DEN_KG_M3 = 0,     /* kg/m3 */
   VX_UNIT_DEN_KG_MM3 = 1,    /* kg/mm3 */
   VX_UNIT_DEN_G_MM3 = 2,     /* g/mm3 */ 
   VX_UNIT_DEN_G_CM3 = 3,     /* g/cm3 */
   VX_UNIT_DEN_LB_IN3 = 4,    /* lb/in3 */
   VX_UNIT_DEN_LB_FT3 = 5,    /* lb/ft3 */
   VX_UNIT_DEN_LB_YD3 = 6,    /* lb/yd3 */
   VX_UNIT_DEN_OZ_IN3 = 7,    /* oz/in3 */
   VX_UNIT_DEN_OZ_FT3 = 8,    /* oz/ft3 */
   VX_UNIT_DEN_OZ_YD3 = 9,     /* oz/yd3 */
   VX_UNIT_DEN_KG_DM3 = 10,     /* kg/dm3 */
   VX_UNIT_DEN_G_DM3 = 11,     /* g/dm3 */
   } evxDensityUnitType;

/* system of mass moment units */
typedef enum evxMassMomentUnitType
   {
   VX_UNIT_MASSMOMENT_KGMM2 = 0,    /* kg.mm2 */
   VX_UNIT_MASSMOMENT_KGM2 = 1,     /* kg.m2 */
   VX_UNIT_MASSMOMENT_GMM2 = 2,     /* g.mm2 */ 
   VX_UNIT_MASSMOMENT_GCM2 = 3,     /* g.cm2 */
   VX_UNIT_MASSMOMENT_LBIN2 = 4,    /* lb.in2 */
   VX_UNIT_MASSMOMENT_LBFT2 = 5,    /* lb.ft2 */
   VX_UNIT_MASSMOMENT_LBYD2 = 6,    /* lb.yd2 */
   VX_UNIT_MASSMOMENT_OZIN2 = 7,    /* oz.in2 */
   VX_UNIT_MASSMOMENT_OZFT2 = 8,    /* oz.ft2 */
   VX_UNIT_MASSMOMENT_OZYD2 = 9,     /* oz.yd2 */
   VX_UNIT_MASSMOMENT_KGDM2 = 10,    /* kg.dm2 */
   VX_UNIT_MASSMOMENT_GDM2 = 11,    /* g.dm2 */
   } evxMassMomentUnitType;

/* face trim-curve option */
typedef enum evxFaceTrim
   {
   VX_TRIM_NONE = 0, /* ignore all trim boundaries */
   VX_TRIM_ALL = 1,  /* use all face trim boundaries */
   VX_TRIM_OUTER = 2 /* use only the outer trim boundaries */
   } evxFaceTrim;

/* point location */
typedef enum evxPntLocation
   {
   VX_PNT_OUT = -1,  /* point lies outside the boundary */
   VX_PNT_ON = 0,    /* point lies on the boundary */
   VX_PNT_IN = 1,    /* point lies inside the boundary */
   VX_PNT_NULL = -2  /* unable to classify point location */
   } evxPntLocation;

/* pre-edge information */
typedef struct svxPreEdge
   {
   svxLimit Limits;  /* limits of pre-edge within its parameter space */
   int idLoop;       /* id of parent loop */
   int idEdge;       /* id of parent edge */
   int idNext;       /* id of next pre-edge in loop direction */
	int idGeom;       /* id of uv curve geometry */
	int EdgeDir;	   /* direction relative to edge (1=same, -1=opposite) */
	int LoopDir;	   /* direction relative to loop (1=same, -1=opposite) */
   } svxPreEdge;


/*| gui/display data
*******************************************/

/* standard colors (see cvxColorRGB for RGB color components) */
typedef enum evxColor
   {
   VX_COLOR_RGB = -1,
   VX_COLOR_NULL = 0,
   VX_COLOR_GREEN = 1,
   VX_COLOR_RED = 2,
   VX_COLOR_BROWN = 3,
   VX_COLOR_GOLDENROD = 4,
   VX_COLOR_MED_BLUE = 5,
   VX_COLOR_DARK_MAGENTA = 6,
   VX_COLOR_DARK_GREY = 7,
   VX_COLOR_DARK_BLUE = 8,
   VX_COLOR_VIOLET = 9,
   VX_COLOR_LIGHT_GREEN = 10,
   VX_COLOR_LIGHT_BLUE = 11,
   VX_COLOR_ROSE = 12,
   VX_COLOR_LIGHT_MAGENTA = 13,
   VX_COLOR_LIGHT_GREY = 14,
   VX_COLOR_WHITE = 15,
   VX_COLOR_BLACK = 16,
   VX_COLOR_YELLOW = 17,
   VX_COLOR_MED_GREY = 18,
   VX_COLOR_PURPLE = 19
   } evxColor;

/* RGB color (each component defined over 0 to 255) */
typedef struct svxColor
   {
   unsigned char r;  /* red color component (0-255) */
   unsigned char g;  /* green color component (0-255) */
   unsigned char b;  /* blue color component (0-255) */
   } svxColor;

/* RGB color (floating point components defined over 0.0 to 1.0) */
typedef struct svxColorf
   {
   float r;  /* red color component (0.0-1.0) */
   float g;  /* green color component (0.0-1.0) */
   float b;  /* blue color component (0.0-1.0) */
   } svxColorf;

/* display mode */
typedef enum evxDispMode
   {
   VX_DISP_WIRE = 1,
   VX_DISP_SHADE = 2,
   VX_DISP_HIDD_APPROX = 3,
   VX_DISP_HIDD_EXACT = 4,
   VX_DISP_EDGE = 5
   } evxDispMode;

/* display (i.e. GUI) item */
typedef enum evxDispItem
   {
   VX_DISP_SCREEN = 0,
   VX_DISP_PREVIEW = 1,
   VX_DISP_FORMS = 2,
   VX_DISP_ALL = 3
   } evxDispItem;

/* display attribute types */
typedef enum evxDispAttrib
   {
   VX_DISP_POINT = 0,
   VX_DISP_LINE = 1,
   VX_DISP_FACE = 2
   } evxDispAttrib;

/* visual display item */
typedef enum evxVisualDispItem
   {
   VX_VISUAL_DISP_FRAME_TRIAD = 0,       /* frame triad display in visual manager */  
   VX_VISUAL_DISP_COMPASS_TRIAD = 1,     /* compass triad display in visual manager */  
   VX_VISUAL_DISP_TRIAD_TRANS = 2        /* show hidden in visual manager */ 
   } evxVisualDispItem;

/* standard 3D view orientations */
typedef enum evxViewStd
   {
   VX_VIEW_FRONT = 0,
   VX_VIEW_BACK = 1,
   VX_VIEW_TOP = 2,
   VX_VIEW_BOTTOM = 3,
   VX_VIEW_RIGHT = 4,
   VX_VIEW_LEFT = 5,
   VX_VIEW_ISO = 6,
   VX_VIEW_AUX = 7,
   VX_VIEW_PLANE = 8
   } evxViewStd;

/* cursor display type */
typedef enum evxCursorType
   {
   VX_CURSOR_STD = 0,            /* standard arrow cursor */
   VX_CURSOR_MOUSE = 1,          /* mouse shaped cursor */
   VX_CURSOR_BUSY = 2,           /* cursor used to indicate system is busy */
   VX_CURSOR_WRENCH = 3,         /* wrench shaped cursor */
   VX_CURSOR_PLIERS = 4,         /* cursor shaped like pliers */
   VX_CURSOR_SCREW_DRIVER = 5,   /* screw driver shaped cursor */
   VX_CURSOR_HAMMER = 6,         /* cursor shaped like a hammer */
   VX_CURSOR_TOGGLE = 7,         /* cursor with two semi-circular arrows */
   VX_CURSOR_HAND = 8,           /* cursor shaped like a hand */
   VX_CURSOR_WRENCH_PLUS = 9     /* wrench shaped cursor with plus */
   } evxCursorType;

/* root object preview mode */
typedef enum evxPreviewMode
   {
   VX_PREVIEW_OFF = 0,
   VX_PREVIEW_GEOM = 1,
   VX_PREVIEW_ATTRIB = 2
   } evxPreviewMode;

/* point input options */
typedef enum evxPntInpOpt
   {
   VX_INP_PNT_GENERAL = 0,
   VX_INP_PNT_ON_ENTITY = 1,
   VX_INP_PNT_ON_CURVE = 2,
   VX_INP_PNT_ON_EDGE = 3,
   VX_INP_PNT_ON_CURVE_OR_EDGE = 4,
   VX_INP_PNT_ON_ANY_FACE = 5,
   VX_INP_PNT_ON_FIELD_1_FACE = 6,
   VX_INP_PNT_END = 7,
   VX_INP_PNT_ASSEMBLY = 8,
   VX_INP_PNT_DIRECTION = 9,
   VX_INP_PNT_ON_CRV_OR_PROF = 10,
   VX_INP_PNT_ASSEM_FACE = 11,
   VX_INP_PNT_ASSEM_EDGE = 12,
   VX_INP_PNT_ASSEM_CURVE = 13
   } evxPntInpOpt;

/* entity input options */
typedef enum evxEntInpOpt
   {
   VX_INP_UNDEFINED = 0,
   VX_INP_GENERAL_3D = 1,
   VX_INP_GENERAL_2D = 2,
   VX_INP_SHAPE = 3,
   VX_INP_FACE = 4,
   VX_INP_SHAPE_OR_FACE = 5,
   VX_INP_EDGE = 6,
   VX_INP_CURVE = 7,
   VX_INP_EDGE_OR_CURVE = 8,
   VX_INP_CURVE_LIST = 9,
   VX_INP_PARTING_CURVE = 10,
   VX_INP_POINT = 11,
   VX_INP_FEATURE = 12,
   VX_INP_SKETCH = 13,
   VX_INP_DATUM_PLANE = 14,
   VX_INP_REF_PLANE = 15,
   VX_INP_3D_BLOCK = 16,
   VX_INP_LIGHT = 17,
   VX_INP_LAYER = 18,
   VX_INP_REF_GEOM = 19,
   VX_INP_WIREFRAME = 20,
   VX_INP_MOVE = 21,
   VX_INP_BLANK = 22,
   VX_INP_ERASE = 23,
   VX_INP_INQUIRE = 24,
   VX_INP_3D_GEOM = 25,
   VX_INP_TEXT = 26,
   VX_INP_PROF = 27,
   VX_INP_COMP = 28,
   VX_INP_ASM_SHAPE = 29,
   VX_INP_FACE_ON_SHAPE = 30
   } evxEntInpOpt;

/* response to user prompt */
typedef enum evxResponse
   {
   VX_CANCEL = -1,
   VX_NO = 0,
   VX_YES = 1,
   VX_YES_ALL = 2
   } evxResponse;

/* dialog form type of user prompt */
typedef enum evxDialogType
   {
   VX_DIALOG_SAVE_AS = 1

   } evxDialogType;

/* light source type */
typedef enum evxLightType
	{
   V_LIGHT_NULL = 0,    /* undefined */
   V_LIGHT_AMB = 1,     /* ambient light source */
   V_LIGHT_DIR = 2,     /* directional light source */
   V_LIGHT_POS = 3,     /* positional light source */
   V_LIGHT_SPOT = 4,    /* spot light source */
   V_LIGHT_EYE = 5      /* light source located at view position */
	} evxLightType;

/* light source */
typedef struct svxLight
   {
   vxName name;         /* light source name */
   evxLightType type;   /* light source type */
   svxPoint pos;        /* light position in definition space */
   svxPoint dir;        /* light direction in definition space */
   svxPoint target_pnt; /* target point for spot light */
   float intensity;     /* light intensity (0.0-1.0) */
   float angle_1;     	/* spotlight cutoff angle (0-180 deg) */
   float angle_2;       /* spotlight constant intensity angle (0-180 deg) */
   float exp;           /* spotlight exponent (0-128)*/
   float c1,c2,c3;      /* attenuation coefficients */
   svxColor color;      /* light color (rgb) */
   char off;            /* 1-light is OFF, else ON */
   char shadow;         /* 1-cast shadows, else 0 */
   } svxLight;

/* face display attributes */
typedef struct svxFaceAt
   {
   unsigned char reflectance; /* reflectance */
   unsigned char refraction;  /* refraction */
   unsigned char exp;         /* specular reflection exponent (0-128) */
   unsigned char trans;       /* transparency setting (0-100) */
   unsigned char ambient;     /* ambient reflection coeff. (0-100) */
   unsigned char diffuse;     /* diffuse reflection coeff. (0-100) */
   unsigned char specular;    /* specular reflection coeff. (0-100) */
   unsigned char emission;    /* emission coeff. (0-100) */
   svxColor front_color;      /* color of frontside */
   svxColor back_color;       /* color of backside */
   svxColor spec_color;       /* specular color */
   } svxFaceAt;

/* table attributes */
typedef struct svxTableAt
   {
   int Row, Col;     /* specification of row/column that attributes are applied to; 
                     ** Row > 0 and Col > 0 identifies a specific cell in the table;
                     ** Row > 0 and Col = 0 identifies an entire row;
                     ** Row = 0 and Col > 0 identifies an entire column;
                     ** Row = 0 and Col = 0 identifies the entire table.
                     */

   int RowHeight;    /* row height; 0 to ignore; -1 for default height */
   int ColWidth;     /* column width; 0 to ignore; -1 for default height */

   int SetBgColor;   /* 1 to set background color of table cells specified by
                     ** "Row" and "Col"; 0 to ignore.
                     */
   svxColor BgColor; /* background color if "SetBgColor = 1" */

   int SetFgColor;   /* 1 to set foreground color of table cells specified by
                     ** "Row" and "Col"; 0 to ignore.
                     */
   svxColor FgColor; /* foreground color if "SetFgColor = 1" */

   int SortRows;     /* 1 to sort rows on column header; -1 to not sort; 0 to ignore */

   int AutoSize;     /* 1 to auto-size all rows to the size of the largest item; 
                     ** 2 to auto-size all columns to the size of the largest item; 
                     ** 3 to auto-size all rows and columns; -1 to disable auto-size;
                     ** 0 to ignore.
                     */

   int AllowResize;  /* 1 to allow rows to be resized; 2 to allow columns to be resized;
                     ** 3 to allow both to be resized; -1 to disable resize; 0 to ignore.
                     */

   int Editable;     /* 1 to make cells specified by "Row" and "Col" editable; 
                     ** -1 to make specified cells not editable; 0 to ignore.
                     */
   } svxTableAt;

/* table cell types */
typedef enum evxTableCellType
   {
   VX_TABLECELL_TYPE_TEXT = 0,
   VX_TABLECELL_TYPE_COMBOBOX = 1,
   VX_TABLECELL_TYPE_CHECKBOX = 2,
   VX_TABLECELL_TYPE_INDEX_COMBOBOX = 3,
   VX_TABLECELL_TYPE_INPUT_COMBOBOX = 4,
   VX_TABLECELL_TYPE_TEXT_BUTTON = 5,
   VX_TABLECELL_TYPE_DATE = 6,
   VX_TABLECELL_TYPE_COLOR = 7,
   VX_TABLECELL_TYPE_NUMBER = 8,
   } evxTableCellType;

/* table widget event types */
typedef enum evxTableEvent
   {
   VX_LEFT_CLICK = 2,       /* left button click (mouse) */
   VX_RIGHT_CLICK = 4,      /* right button click (mouse) */
   VX_LEFT_DOUBLE_CLICK,    /* left button double click (mouse) */
   VX_KEY_UP_DOWN,          /* key up and down (keyboard) */
   VX_HOT_KEY = 9,		     /* hotkey */
   VX_SORT = 15,            /* sort event */
   VX_DATA_CHANGED,         /* data changed event */
   VX_CELL_BUTTON_CLICKED,  /* button in table cell clicked event */
   VX_MOUSE_MOVE = 21,
   } evxTableEvent;

typedef enum evxDateType
   {
   VX_DATE_WITH_TIME,
   VX_DATE_ONLY,
   } evxDateType;

typedef struct svxTableCellData
   {
   union svxCellData
      {
      struct svxTextData
         {
         const char* pImage;     //In
         char* buf;              //In/Out
         int nBufSize;           //In
         const char* btnToolTip; //In
         int btnWidth;           //In
         int btnHeight;          //In
         } textData;
      struct svxComboData
         {
         const char* pImageList; //In
         const char* pTextList;  //In
         char* pSelTextBuf;      //In/Out
         int nBufSize;           //In
         int iSel;               //In/Out
         } comboData;
      struct svxCheckData
         {
         const char* psLabel; //In
         int iCheck;          //In/Out
         } checkBoxData;
      struct svxDateData
         {
         int secsFromEpoch;   //In/Out
         evxDateType dateType; //In
         } dateData;
      struct svxColorData
         {
         unsigned char type;    //In: 0-true color, 1-index color, 2-include FROM PART
         int clrIndex;    //In/Out
         int r, g, b;     //In/Out
         } colorData;
      struct svxNumData
         {
         int iMin;      //In
         int iMax;      //In
         char* buf;     //In/Out
         int nBufSize;  //In
         }numData;
      } cellData;
   int iAttrData;
   struct svxCompareData
      {
      int iType;           //In 1-integer, 2-string
      int iInteger;        //In
      const char* pString; //In
      int iKeyCol;         //In
      } compareData;
   int idx;
   int txtMaxLen;
   const char * placeHolderText;
   void* validator;
   } svxTableCellData;

/* keyboard event */
typedef struct svxKeyEvent
   {
   unsigned int modifier;  /* Synchronous key:CTRL, SHIFT, ALT, or WIN key etc. */
   unsigned int keyCode;   /* A~Z, F1~F12, 0~9 etc, mapping to Microsoft virtual key directly */
   } svxKeyEvent;

/* table widget callback function parameters */
typedef struct svxTableCallBackParam
   {
   evxTableEvent evt;
   svxKeyEvent key;
   int iRow;
   int iCol;
   svxTableCellData cellData;
   int* pResult;
   int iSortSequenceSize;  //size of aSortSequence
   int* aSortSequence;
   } svxTableCallBackParam;

/* texture map option */
typedef enum evxTextureOpt
   {
   V_TEXTURE_NULL = 0,
   V_TEXTURE_BLEND = 1,
   V_TEXTURE_DECAL = 2,
   V_TEXTURE_MODULATE = 3,
   V_TEXTURE_REPLACE = 4
   } evxTextureOpt;

/* texture map origin point with respect to texture map rectangle */
typedef enum evxTextureLoc
   {
   V_TEXTURE_LOWER_LEFT = 0,
   V_TEXTURE_UPPER_LEFT = 1,
   V_TEXTURE_UPPER_RIGHT = 2,
   V_TEXTURE_LOWER_RIGHT = 3,
   V_TEXTURE_CENTER = 4
   } evxTextureLoc;

/* texture attribute */
typedef struct svxTextureAt
   {
   evxTextureOpt opt;      /* texture option (zero if undefined) */
   vxPath file;            /* texture path/file name (blank if undefined) */
   unsigned char dimen;    /* texture dimension (1-3) */
   unsigned char repeat;   /* 1 to repeat texture, else 0 */
   float trans;            /* texture transparency (0 to 1.0) */
   double height;          /* texture height (mm) */
   double width;           /* texture width (mm) */
   double angle;           /* texture angle (deg) */
   svxPoint origin;        /* texture origin */
   int use_embed;          /* as an input, 1 means a pre-existing embedded
                           ** image takes precedence over file, 0 means file
                           ** replaces any pre-existing embedded image; as
                           ** an output, 1 means an embedded image exists
                           ** for file, 0 means there is no embedded image. */
   evxTextureLoc location; /* texture location relative to origin */
   } svxTextureAt;

/* shader attribute */
typedef struct svxShaderAt
   {
   vxName name;         /* shader name (blank if undefined) */
   vxPath file;         /* shader path/file name (blank if undefined) */
   svxColor color[4];   /* shader colors */
   double param[10];    /* shader parameters */
   } svxShaderAt;

/* layer information */
typedef enum
   {
   VX_LAYER_INFOR_NAME = 0x00000001,
   VX_LAYER_INFOR_DESC = 0x00000002,
   VX_LAYER_INFOR_ID = 0x00000004,
   VX_LAYER_INFOR_VISIBLE = 0x00000008,
   VX_LAYER_INFOR_FREEZE = 0x000000010,
   VX_LAYER_INFOR_DISABLE = 0x00000020,
   VX_LAYER_INFOR_COLOR = 0x00000040,
   VX_LAYER_INFOR_LINETYPE = 0x00000080,
   VX_LAYER_INFOR_LINEWIDTH = 0x0000100,
   VX_LAYER_INFOR_NUMOBJ = 0x0000200,
   VX_LAYER_INFOR_FLTR = 0x0000400,
   VX_LAYER_INFOR_ALL = 0xFFFFFFFF,
   }evxInfoLayerFlag;

typedef struct svxLayerInfo
   {
   int mask;
   char sName[32];
   char sDesc[512];
   unsigned char uLayerId;
   unsigned char uVisible;
   unsigned char uFrozen;
   unsigned char uDisable;
   unsigned char uColor;
   unsigned char uLineType;
   unsigned char ulineWidth;
   char category[512];      /* category, like this CURVES;SHEETS */
   int iNumObj;
   }svxLayerInfo;

/* display state settings */
typedef int vxDispState[10];

/* 3D facets defined by triangle strips */
typedef struct svxFacets
   {
   int numTriStrip;     /* number of triangle strips in TriStrip list */
   int *TriStrip;       /* list of integer indices that define triangle strips
                        **
                        ** This list has the format "num_vertices_in_strip_1,
                        ** index_of_vertex_1,index_of_vertex_2,...,index_of_vertex_N,
                        ** num_vertices_in_strip_2,..." where an index is an offset
                        ** into the list of Vertex/Normal/ST/UV/RGB coordinates, numbered
                        ** from 0.  A triangle strip defined by N vertices defines 
                        ** N-2 triangles.  For example, if a triangle strip is
                        ** defined by 5 vertices whose indices are {0,1,2,3,4}, 
                        ** then it contains 3 triangles defined by vertices {0,1,2}, 
                        ** {2,1,3} and {2,3,4} (when using data from a triangle strip,
                        ** note that vertex order is implicitly reversed for alternating
                        ** triangles when calculating the implied normal direction
                        ** of each triangle's plane). */
   int numVertex;       /* number of vertices in Vertex list */
   svxPointf *Vertex;   /* list of vertex coordinates (x,y,z) */
   svxPointf *Normal;   /* list of vertex normals (i,j,k unit direction vector);
                        ** Normal is optional and may be NULL */
   svxPoint2f *ST;      /* list of vertex texture coordinates (s,t from 0.0 to 1.0);
                        ** ST is optional and may be NULL */
   svxFaceAt At;        /* optional display attributes */
   svxTextureAt Tx;     /* optional texture attributes */
   svxShaderAt Sh;      /* optional shader attributes */
   svxPoint2f *UV;      /* list of UV coordinates corresponding to vertex coordinates;
                        ** UV is optional and may be NULL */
   svxColorf *RGB;      /* list of vertex colors (r,g,b components from 0.0 to 1.0;
                        ** see cvxColorfToColor() for conversion to svxColor);
                        ** RGB is optional and may be NULL
                        ** (NOTE: The RGB field is a placeholder for a future enhancement
                        ** and is not yet supported.  Colors assigned to this field will
                        ** be ignored when a facet block is displayed.) */
   } svxFacets;

typedef struct
	{
	vxName Name;			/* view name */
   svxMatrix Matrix;  	/* view matrix (i.e. transform) */
   float Extent;			/* view extent */
   float FrontDist;		/* distance from view origin to front clip plane */
   float BackDist;		/* distance from view origin to back clip plane */
   float PRP_Dist;		/* distance from view origin to projection reference point (PRP) */
   char Display;			/* 1-display this view, 0-don't display this view */
   char Clip;				/* 1-clip to front/back plane, 0-don't clip */
   char Project;			/* projection method, 0-parallel, 1-perspective */
	} svxView;


/*| ui tree widget management
*******************************************/

/* ui alignment types */
typedef enum
	{
	VX_ALIGN_CENTER = 0,
	VX_ALIGN_LEFT = 1,
	VX_ALIGN_RIGHT = 2,
	} evxAlignFlag;

/* tree widget item types */
typedef enum
	{
	VX_ITEM = 0,
	VX_FINISHMARK = 1
	} evxTreeItemType;

/* tree widget item options */
#define  VX_INVALID_ITEM             ((vxTreeItemId)-1)
#define  VX_INVISIBLE_ROOT_ITEM      ((vxTreeItemId)0)
#define  VX_INSERT_LAST              ((vxTreeItemId)-1)  //Insert at last position 
#define  VX_INSERT_FIRST             ((vxTreeItemId)0)   //Insert node at first position

/* tree widget event types */
typedef enum
   {
   VX_EMPTY = 0,
   VX_LEFT_DOWN,            /* left mouse click down */
   VX_LEFT_UP,              /* left mouse click up */
   VX_MIDDLE_DOWN,          /* middle mouse click up */
   VX_MIDDLE_UP,            /* middle mouse click up */
   VX_RIGHT_DOWN,           /* right mouse click down */
   VX_RIGHT_UP,             /* right mouse click up */
   VX_BOX_SELECTION_DONE,   /* box selection over */
   VX_LABEL_CHANGED,        /* label name check */
   VX_EDITOR_CLOSED,         /* item editor closed*/
   VX_DBL_LEFT,             /* left mouse double click */    
   VX_DROP,                 /* drop after dragging */
   VX_KEY_PRESS,            /* key */
   VX_HOVER,                /* normal hover */
   VX_HOVER_TIMEOUT,        /* item hovered timeout */
   VX_ENTER,
   VX_LEAVE
   } evxTreeEvent;

/* tree widget element codes */
typedef enum
   {
   VXTHP_BUTTON     = 0x0001,   /* hit on expand button associated with a node */
   VXTHP_ICON       = 0x0002,   /* Hit on node's icon */
   VXTHP_LABEL      = 0x0004,   /* Hit on node's label name */
   VXTHP_ITEM       = 0x0006,   /* V_ZDM_ICON|V_ZDM_LABEL */
   VXTHP_SPACE      = 0x0008,   /* Hit on design tree space */
   VXTHP_CHECKBOX   = 0x0010,   /* Hit on the checkbox */
   VXTHP_APD_ICON1  = 0x0020,   /* Hit on node's second icon */
   VXTHP_APD_ICON2  = 0x0040,   /* Hit on node's third icon */
   VXTHP_HEADER     = 0x0080,   /* Hit on the header of the tree */
   } evxTreeHitPos;

/* tree widget callback function parameters */
typedef struct
   {
   evxTreeEvent mode;
   const char* shell;
   int field;
   vxTreeItemId item; //current hover id
   int column;  // the column where the current item is located
   union uvxU
      {
      struct svxDRAG  /* drag multiple nodes */
         {
         vxTreeItemId pressedItem;
         vxTreeItemId *selection;
         int        num;
         vxTreeItemId droppedItem;
         int        dropPos;  // 0:item,1:above,2:below,3:viewport,
         } svxdrag;

      struct svxHIT
         {
         evxTreeHitPos     pos;    /* Clicking postion (ZeTreeHitPos)*/
         } svxhit;

      struct svxSELECTION /* multiple nodes selection */
         {
         vxTreeItemId *idList;
         int num;
         } svxselection;

      struct svxLABELEDIT
         {
         char* text;       //pointer to node's label string
         } svxlabel;
      } uvxu;

   svxKeyEvent key;   /* key*/

   int  state;          /* 1:state is on, 0:state is off 
                           or V_ZDM_EXPAND/V_ZDM_COLLAPSE
                        */

   int  itemIsDisabled; /* 1:node is disabled, 0:node is enabled */
   /* if success/ok/enable next procedure, assign it to 0, else to 1 */

   } svxTreeCbParam;


/*| database entity data
******************************************************/

/* entity unique identifier */
typedef struct svxUid
   {
   int Time;  /* base time */
   int Rand;  /* random number */
   int Count; /* entity count */
   } svxUid;

/* entity types */
typedef enum evxEntType
   {
   VX_ENT_BREP = -4,       /* any brep entity (i.e. shape,face,loop,edge,vertex) */
   VX_ENT_OP = -3,         /* any history operation */
   VX_ENT_POINT = -2,      /* any point entity (2D or 3D) */
   VX_ENT_WIRE = -1,       /* any wireframe entity (line, arc, curve) */
   VX_ENT_ALL = 0,         /* any entity type */
   VX_ENT_SHAPE = 42,      /* b-rep shape (i.e. faceset) */
   VX_ENT_FACE = 43,       /* b-rep face */
   VX_ENT_EDGE = 46,       /* b-rep edge */
   VX_ENT_FTR = 48,        /* feature operation */
   VX_ENT_NRB_CRV = 54,    /* NURBS curve */
   VX_ENT_LINE3 = 56,      /* 3D line */
   VX_ENT_LINE2 = 58,      /* 2D line */
   VX_ENT_CIRCLE3 = 60,    /* 3D circle */
   VX_ENT_CIRCLE2 = 61,    /* 2D circle */
   VX_ENT_ARC3 = 62,       /* 3D arc */
   VX_ENT_ARC2 = 63,       /* 2D arc */
   VX_ENT_POINT3 = 64,     /* 3D point */
   VX_ENT_POINT2 = 65,     /* 2D point */
   VX_ENT_LAYER = 67,      /* layer */
   VX_ENT_SKETCH = 70,     /* sketch */
   VX_ENT_REF_GEOM = 72,   /* reference geometry */
   VX_ENT_TEXT = 76,       /* text */
   VX_ENT_DIMEN = 77,      /* dimension */
   VX_ENT_ANNO = 83,       /* annotation text */
   VX_ENT_BLOCK = 86,      /* block */
   VX_ENT_DWG_VIEW = 87,   /* drawing view */
   VX_ENT_COMP = 88,       /* assembly component */
   VX_ENT_PART = 89,       /* part */
   VX_ENT_SHEET = 90,      /* drawing sheet */
   VX_ENT_PACKET = 91,     /* drawing packet */
   VX_ENT_DATUM = 95,      /* datum plane */
   VX_ENT_CRV_LIST = 102,  /* curve list */
   VX_ENT_PART_LINE = 106, /* parting line */
   VX_ENT_HATCH = 111,     /* cross-hatch */
   VX_ENT_LIGHT = 118,     /* light source */
   VX_ENT_INT_CRV = 119,   /* interpolated curve */
   VX_ENT_VAR = 120,       /* variable */
   VX_ENT_EQN_SET = 121,   /* equation set */
   VX_ENT_SYMBOL = 123,    /* 2D symbol */
   VX_ENT_TRACE = 133,     /* sketch trace */
   VX_ENT_DRAWING=169,     /* Drawing under the sheet*/
   VX_ENT_PMIENT = 190,    /* PMI entity */
   VX_ENT_BLOCK2D = 197,   /* 2D sketch block */
   VX_ENT_BLKREF = 198     /* block reference (the instance of 2d sketch block) */
   } evxEntType;

/* entity search types */
typedef enum evxEntFind
   {
   VX_FIND_POINT = 1,               /* point */
   VX_FIND_CURVE = 2,               /* curve */
   VX_FIND_EDGE = 3,                /* edge */
   VX_FIND_WIRE = 4,                /* point, edge or curve */
   VX_FIND_FACE = 5,                /* face */
   VX_FIND_SKETCH = 6,              /* sketch */
   VX_FIND_CRV_LIST = 7,            /* 3D curve list */
   VX_FIND_SKETCH_OR_CRV_LIST = 8,  /* sketch or curve list */
   VX_FIND_PROFILE = 9,             /* curve, edge, sketch or curvelist */
   VX_FIND_EDGE_OR_CRV = 10,        /* edge or curve */
   VX_FIND_3D_BLOCK = 14,           /* 3D point or STL block */
   VX_FIND_DIM = 15                 /* dimension */
   } evxEntFind;

/* entity search data */
typedef struct svxEntFind
   {
   /* input - basic parameters */
   evxEntFind Option;   /* type of entity to search for */
   svxPoint RefPnt;     /* find entity closest to this reference point */

   /* input - filter parameters */ 
   evxColor Color;      /* entity color (0 to ignore, V_COLOR_RGB to use RGB parameters) */
   svxColor Rgb;        /* RGB color if "Color = V_COLOR_RGB" */
   int CheckLayer;      /* 1 to check layer number (0 to ignore) */
   int Layer;           /* entity layer number (0 to 255) */
   int idFeature;       /* parent feature id (0 to ignore) */
   int isPlane;         /* 1 if face entity must be "planer" (0 to ignore) */
   int isHole;          /* 1 if face/edge must belong to a hole (0 to ignore) */
   int isLine;          /* 1 if curve or edge must be linear (0 to ignore) */

   /* output */
   int idEntity;        /* id of entity found by search (0 if not found) */    
   svxPoint Pnt;        /* projection of reference point onto entity */
   double Dist;         /* distance from reference point to entity */
   } svxEntFind;

/* root object type */
typedef enum evxRootType
   {
   VX_ROOT_NULL = 0,
   VX_ROOT_PART = 89,
   VX_ROOT_PART_NEW = -VX_ROOT_PART,
   VX_ROOT_SHEET = 90,
   VX_ROOT_DRAWING = 91,
   VX_ROOT_SKETCH = 70,
   VX_ROOT_EQUATION_SET = 121,
   VX_ROOT_CAM_PLAN = 200
   } evxRootType;

/* dimension type */
typedef enum evxDimType
{
   VX_DIM_LINEAR = 1,               /* Linear dimension */
   VX_DIM_BASELINE = 2,             /* Baseline dimension */
   VX_DIM_CONTINUOUS = 3,           /* Continuous dimension */
   VX_DIM_ORDINATE = 4,             /* Ordinate dimension */
   VX_DIM_LINEAR_OFFSET = 5,        /* Linear Offset dimension */
   VX_DIM_LINEAR_CHAMFER = 6,       /* Linear Chamfer dimension */
   VX_DIM_ANGULAR = 7,              /* Angular dimension */
   VX_DIM_RADIAL_DIAMETRIC = 8,     /* Radial/Diametric dimension */
   VX_DIM_ARC_LENGTH = 9,           /* Arc Length dimension */
   VX_DIM_SYMMETRY,                 /* Symmetry dimension*/
   VX_DIM_HLCALL,                   /* hole call out */
   VX_DIM_LABEL,                    /* leader label */
   VX_DIM_BALLOON,                  /* balloon */
   VX_DIM_DATUMF,                   /* datum feature */
   VX_DIM_DATUMT,                   /* datum target */
   VX_DIM_FCS,                      /* feature control symbol */
   VX_DIM_CLCM,                     /* centerline/center mark */
   VX_DIM_INTERSYM,                 /* intersection symbol */
   VX_DIM_LAST
} evxDimType;

/* symbol object type */
typedef enum evxSymbolType
   {
   VX_SYM_USER = 0,     /* user defined symbol */
   VX_SYM_SURF = 1,     /* surface finish */
   VX_SYM_WELD = 2,     /* weld */
   VX_SYM_IMAGE = 3,    /* image */
   VX_SYM_BEAD = 5,     /* caterpillar */
   VX_SYM_ENDTRT = 6    /* end treatment */
   } evxSymbolType;

typedef enum evxViewType
   {
   VX_VIEW_ALL = 0,     /* any view type */
   VX_VIEW_BASE = 1,    /* base view */
   VX_VIEW_PROJ = 2,    /* project view */
   VX_VIEW_SECT = 3,    /* section view */
   VX_VIEW_DET = 4      /* detail view */
   } evxViewType;

typedef enum evxTableType
   {
   VX_TABLE_ALL = 0,     /* any table type */
   VX_TABLE_USER = 1,    /* User table */
   VX_TABLE_BOM = 2,     /* BOM table */
   VX_TABLE_HOLE = 3,    /* Hole table */
   VX_TABLE_TRODE = 4,   /* Electrode table */
   VX_TABLE_WDSEG = 5,   /* Structural BOM table */
   VX_TABLE_WELD = 6     /* Weld table */
   } evxTableType;

/* maximum length of a non-persistent (i.e. evaluated) pick path (sync with VsPickPath in VxToolsPub.h) */
#define V_PP_LEN 200
/* entity pick path (sync with VsPickPath) */
typedef struct svxEntPath
   {
   int Count;     /* number of id's in the path */
   int Id[V_PP_LEN];/* list of id's that define the path to an entity */
                  /* Id[0] = -1; path begins with currently active root object (root path) */
                  /* Id[0] = -2; path begins with top-level parent file/object (target path) */
                  /* Id[1] = id of first parent entity in path */
                  /* Id[N] = id of child entity to act upon */
                  /* See cvxEntPathToRoot() to convert target path to root path */
   } svxEntPath;

/* entity pick (includes optional parent entity) */
typedef struct svxEntPick
	{
	int idEntity;	/* entity id */
	int idParent;	/* id of entity's parent object (i.e. sketch) or "0" if the
							entity is a direct child of the target object (i.e. part) */
	} svxEntPick;


/*| command or gui form template data
*******************************************/

/* command types */
typedef enum evxCmdType
   {
   VX_STANDARD_COMMAND = -1,  /* non-history command */
   VX_HIST_GENERAL = 0,       /* general-purpose history operation */
   VX_HIST_SOLID_CREATE = 11, /* "solid create" history operation */
   VX_HIST_SOLID_EDIT = 12,   /* "solid edit" history operation */
   VX_HIST_SURF_CREATE = 13,  /* "surface create" history operation */
   VX_HIST_SURF_EDIT = 14,    /* "surface edit" history operation */
   VX_HIST_CRV_CREATE = 15,   /* "curve create" history operation */
   VX_HIST_CRV_EDIT = 16,     /* "curve edit" history operation */
   VX_HIST_COMP = 17,         /* "component" history operation */
   VX_HIST_SKETCH = 18,       /* "sketch" history operation */
   VX_HIST_DATUM = 19,        /* "datum" history operation */
   VX_HIST_FILLET = 20,       /* "fillet/chamfer" history operation */
   VX_HIST_SHELL = 21,        /* "shell" history operation */
   VX_HIST_SHT_METAL = 22,    /* "sheet metal" history operation */
   VX_HIST_HOLE = 23,         /* "hole" history operation */
   VX_HIST_CRV_LIST = 24,     /* "curve list" history operation */
   VX_HIST_IMPORT = 25,       /* "import" history operation */
   VX_HIST_REF_GEOM = 26,     /* "reference geometry" history operation */
   VX_HIST_EQN = 27,          /* "equation" history operation */
   VX_HIST_HEAL = 28,         /* "heal" history operation */
   VX_HIST_ERASE = 29,        /* "erase" history operation */
   VX_HIST_BLANK = 30,        /* "blank/unblank" history operation */
   VX_HIST_ATTRIBUTE = 33,    /* "attributes" history operation */
   VX_HIST_RENDER = 34,       /* "rendering" history operation */
   VX_HIST_ANALYZE = 35,      /* "analyze" history operation */
   VX_HIST_MOVE = 37,         /* "copy/move/pattern" history operation */
   VX_HIST_TEXT = 38,         /* "text" history operation */
   VX_HIST_POINT = 39,        /* "point" history operation */
   VX_HIST_DRAFT = 40,        /* "draft" history operation */
   VX_HIST_ASSEMBLY = 41,     /* "assembly" history operation */
   VX_HIST_TAG = 42,          /* "tag" history operation */
   VX_HIST_MOLD = 43          /* "mold design" history operation */
   } evxCmdType;

/* command state type */
typedef enum evxCmdState
{
   VX_CMD_ENABLE = 0,
   VX_CMD_DISABLE = 1,
} evxCmdState;

/* numeric input field type */
typedef enum evxFldNumType
   {
   VX_INP_DOUBLE = 0,
   VX_INP_INTEGER = 1,
   VX_INP_DISTANCE = 2,
   VX_INP_ANGLE = 3
   } evxFldNumType;

/* gui form template field (i.e. widget) classes */
typedef enum evxFormFldClass
   {
   VX_FORM_FLD_NULL = -1,        /* null */
   VX_FORM_FLD_NUM = 0,          /* numeric input */
   VX_FORM_FLD_SLIDER = 1,       /* bounded numeric input (slider bar) */
   VX_FORM_FLD_TXT_IN = 2,       /* text input */
   VX_FORM_FLD_TXT_OUT = 3,      /* text output */
   VX_FORM_FLD_TXT_BTN = 4,      /* text input with an extra button */
   VX_FORM_FLD_TXT_LBL = 5,      /* toggle+label+text */
   VX_FORM_FLD_DATA = 6,         /* data input for handling a template */
   VX_FORM_FLD_LIST = 7,         /* scrolling list */
   VX_FORM_FLD_OPT = 8,          /* exclusive/non-exclusive group of options */
   VX_FORM_FLD_BTNS = 9,         /* menu button group */
   VX_FORM_FLD_GROUP = 10,       /* widget grouping entity */
   VX_FORM_FLD_LABEL = 11,       /* label only */
   VX_FORM_FLD_SEPARATOR = 12,   /* separator */
   VX_FORM_FLD_COLOR = 13,       /* color widget */
   VX_FORM_FLD_TREE = 14,        /* tree widget */
   VX_FORM_FLD_COMBO = 15,       /* combo widget */
   VX_FORM_FLD_TAB = 16,         /* tab widget */
   VX_FORM_FLD_PROGRESS = 17,    /* progress control widget */
   VX_FORM_FLD_TABLE = 18        /* table widget */
   } evxFormFldClass;

/* command template field classes */
typedef enum evxCmdFldClass
   {
   VX_CMD_FLD_NULL = 0,    /* null */
   VX_CMD_FLD_TEXT = 1,    /* text string */
   VX_CMD_FLD_NUM = 2,     /* number */
   VX_CMD_FLD_DIST = 3,    /* distance */
   VX_CMD_FLD_ANGLE = 4,   /* angle */
   VX_CMD_FLD_POINT = 5,   /* point */
   VX_CMD_FLD_ENTITY = 6,  /* entity */
   VX_CMD_FLD_OPTION = 7,  /* option */
   VX_CMD_FLD_CONT = 8,    /* continue */
   VX_CMD_FLD_FORM = 9,    /* gui form */
   VX_CMD_FLD_WIN = 10     /* pick window */
   } evxCmdFldClass;

/* template field data */
typedef struct svxTplFld
   {
   char Name[65];    /* field name (i.e. label) */
   int Id;           /* command field or gui widget id */
   int Class;        /* see evxFormFldClass or evxCmdFldClass */
   void *Callback;   /* pointer to field's callback function */
   } svxTplFld;

/* gui form action (see cvxFormFunc()) */
typedef enum evxFormAction
   {
   VX_FORM_INIT = -4,   /* initialize form */
   VX_FORM_CANCEL = -3, /* take down the form */
   VX_FORM_RESET = -2,  /* reset form values */
   VX_FORM_APPLY = -1,  /* apply form values and leave form up */
   VX_FORM_OKAY = 0,    /* accept form values and take form down */
   VX_FORM_DEFAULT = 1  /* default form values */
   } evxFormAction;

/*| command input data
*******************************************/

/* command license codes */
#define VX_CODE_GENERAL 0.0
#define VX_CODE_ADV_RENDERING 1.0
#define VX_CODE_ASSEMBLY 2.0
#define VX_CODE_ADV_ASSEMBLY 17179869184.0
#define VX_CODE_DRAFTING 4.0
#define VX_CODE_WIREFRAME 8589934592.0
#define VX_CODE_MODELING 512.0
#define VX_CODE_ADV_MODELING 34359738368.0
#define VX_CODE_ADV_SURF 256.0
#define VX_CODE_MOLD_DESIGN 1024.0
#define VX_CODE_HEALING 4294967296.0
#define VX_CODE_POST 2199023255552.0
#define VX_CODE_KEYSHOT 576460752303423488.0
#define VX_CODE_ELECTRODE 1152921504606846976.0
#define VX_CODE_PARTSOLUTIONS 32768.0
#define VX_CODE_MISUMI VX_CODE_GENERAL

/* numeric input data types */
typedef enum evxNumType
   {
   /* output type   VeNumType*/
   VX_NUM_UNDEFINED = -1,  /* undefined */
   VX_NUM_NULL = 0,        /* undefined */
   VX_NUM_TO_FACE = 1,     /* distance/angle to face */
   VX_NUM_FACE_BND = 2,    /* boundary face  */
   VX_NUM_TO_PNT = 3,      /* distance/angle to point */
   VX_NUM_THRU_PLUS = 4,   /* "through all" in positive direction */
   VX_NUM_THRU_MINUS = 5,  /* "through all" in negative direction */
   VX_NUM_CURVE_BND = 6,   /* boundary curve */
   VX_NUM_DATA = 7,        /* number defined by a VDATA object */
   /* input type   VeNumClass*/
   VX_UNDEF = 0,           /* undefined */
   VX_NUM = 1,             /* dimensionless number (e.g. curve degree) */
   VX_DST = 2,             /* distance (mm) */
   VX_ANG = 3,             /* angle (deg) */
   VX_MASS = 4,            /* mass (kg) */
   VX_DENSITY = 5,         /* density (kg/m3) */
   VX_AREA = 6,            /* area (m2) */
   VX_VOLUME = 7,          /* volume (m3) */
   VX_MASS_MOMENT = 8      /* mass moments (kg.mm2) */
   } evxNumType;

/* point input data types */
typedef enum evxPntType
   {
   VX_PNT3_ABS = 0,        /* defined by absolute coord. in object space */
   VX_PNT3_ENT = 1,        /* entity "critical" point (start, end, mid, center) */
   VX_PNT3_ON = 2,         /* point on entity defined in entity parameter space */
   VX_PNT3_TO = 3,         /* point on entity for "tangent to" or "extend to" */
   VX_PNT3_MID = 4,        /* curve midpoint */
   VX_PNT3_NUM = 5,        /* point object stores dimensionless numeric input */
   VX_PNT3_DST = 6,        /* point object stores linear numeric input */
   VX_PNT3_ANG = 7,        /* point object stores angular numeric input */
   VX_PNT3_TAN = 8,        /* point on entity for "tangent to" */
   VX_PNT3_FRAC = 9,       /* point on curve defined as fraction of curve length */
   VX_PNT3_REL = 10,       /* point defined relative to previous point */
   VX_PNT3_DATA = 11,      /* point defined by a VDATA object */
   VX_PNT3_ABS_COMP = 12   /* defined by abs coord in space of active component */
   } evxPntType;

/* command input data item */
typedef struct svxData
   {
   /* flags indicating the types of data defined by this structure */
   char isPoint;        /* 1 if "Pnt" is defined, else 0 */
   char isEntity;       /* 1 if "idEntity" is defined, else 0 */
   char isNumber;       /* 1 if "Num" is defined, else 0 */
   char isDirection;    /* 1 if "Dir" is defined, else 0 */
   char isText;         /* 1 if "Text" is defined, else 0 */
   char isPntOnCrv;     /* 1 if point on curve, else 0.
                        ** idEntity="curve id"; Pnt="point coordinates";
                        ** Param[0]="curve t param"; idParent is optional.
                        ** For cvxPartAlign, Param[0]="point id" in the control
                        ** point list. For example, Param[0]=2.0 means using the
                        ** the third control point.
                        */
   char isEndPnt;       /* if "isPntOnCrv=1", set this flag to 1 if 
                        ** Param[0] is to be used to identify the closest
                        ** endpoint on the specified curve (idEntity).
                        */
   char isPntOnFace;    /* 1 if point on face, else 0.
                        ** idEntity="face id"; Pnt="point coordinates";
                        ** Param[0]="u parameter"; Param[1]="v parameter".
                        */
   /* various parameters that may be defined (see the flags above) */
   int idEntity;        /* entity/object id (0 if undefined) */
   int idParent;        /* id of parent entity (0 if undefined) */
   evxNumType NumType;  /* numeric input type (input and output) */
   double Num;       	/* numeric value */
   evxPntType PntType;  /* point input type (output only) */
   svxPoint Pnt;        /* point coordinates */
   svxVector Dir;       /* unit direction vector */
   double Param[3];     /* pick parameters (i.e. t value or point id) */
   char Text[256];      /* text string (expression if "isNumber=1") */
   } svxData;

/*| assembly data
*****************************************/

/* component data */
typedef struct svxCompData
   {
   vxPath Dir;          /* pathname of directory that contains "File" */
                        /* set "Dir[0]=0" to search default pathnames */
   vxLongName File;      /* name of file that contains "Part" */
   vxLongName Part;      /* name of part to instance as a component */
   vxLongName Comp;      /* name to assign to new component in parent part */
                        /* set "Comp[0]=0" for default name */
   svxMatrix Frame;     /* insertion frame for new component */
   int Anchor;          /* 1 to anchor component, or 0 */
   int AutoRegen;       /* 1 to regen component when parent is regen'd, or 0 */
   int AutoDelete;      /* 1 to delete component when parent is deleted, or 0 */
   int Copy;            /* 1 to copy "Part" before instancing, or 0 */
   vxLongName CopyName;  /* name to assign to copied part when "Copy=1" */
                        /* if no name is given, the default name is output */
   int Overwrite;       /* 0 to automatically avoid overwriting existing part
                        ** 1 to prompt whether to overwrite existing part
                        ** -1 to automatically overwrite existing part 
                        */
   vxLongName DestFile;  /* name of destination file if copying part
                        ** If no name is given (DestFile[0]=0), the part
                        ** is copied to the file of the parent assembly.
                        */
   int Align;           /* 1 to align component with point or frame, or 0*/
   svxData AlignData;   /* align data for point and frame align.
                        ** use cvxDataGet to get the data.
                        */
   } svxCompData;

/* brief component info */
typedef struct svxCompName
   {
   vxLongName File;     /* name of file that contains "Part" */
   vxLongName Part;     /* name of part to instance as a component */
   } svxCompName;

/* geometry export */
typedef struct svxGeomExport
   {
   int numEnts;      /* number of geometry entities to export */
   int *idEnts;      /* id's of geometry entities to export */
   vxLongName File;  /* destination file (set File[0]=0 to use active file) */
   vxLongName Part;  /* name of destination part (new or existing) */
   svxMatrix Frame;  /* local coordinate frame */
   int Option;       /* 0 - add import operation to destination part.
                     ** 1 - add import operation and then "encapsulate" history.
                     ** 2 - add import operation and then "backup" history.
                     ** (Option 1 is recommended)
                     */
   int AddComponent; /* 1 to instance part as component; else 0 */
   int NoExportOp;   /* 1 to disable logging of "Export" operation; else 0 */
   int Unblank;      /* 1 to unblank exported entities in destination part */
   } svxGeomExport;

/* copy external part data */
typedef struct svxPartInstData
   {
   vxPath Dir;       /* pathname of directory that contains "File" */
                     /* set "Dir[0]=0" to search default pathnames */
   vxLongName File;  /* name of VX file that contains "Part" */
   vxLongName Part;  /* name of part that to be copied*/
   svxMatrix Frame;  /* insertion frame for sub-part,the insert point
                      is the original point of frame */
   int CopyWire;     /* 1 to copy wireframe, or 0 */
   int CopyDim;      /* 1 to copy dimension, or 0 */
   int Option;       /* 0 - sub-part with associative copy.
                     ** 1 - sub-part with history.
                     ** 2 - history copied into this part.
                     ** 3 - associative copy in this part.
                     */
   }svxPartInstData;

/*| hole feature data
*******************************************/

/* hole types */
typedef enum evxHoleType
   {
   VX_HOLE_SIMPLE = 0,
   VX_HOLE_TAPERED = 1,
   VX_HOLE_COUNTER_BORE = 2,
   VX_HOLE_COUNTER_SUNK = 3,
   VX_HOLE_SPOT_FACE = 4
   } evxHoleType;

/* hole end conditions */
typedef enum evxEndCondition
   {
   VX_BLIND = 0,
   VX_UNTIL_FACE = 1,
   VX_THROUGH_ALL = 2
   } evxEndCondition;

/* hole thread types */
typedef enum evxThreadType
   {
   VX_THREAD_NONE = 1,
   VX_THREAD_CUSTOM = 2,
   VX_THREAD_M = 3,
   VX_THREAD_UNC = 4,
   VX_THREAD_UNF = 5,
   VX_THREAD_ISO7 = 6,
   VX_THREAD_ISO228 = 7,
   VX_THREAD_NPT = 8,
   VX_THREAD_NPSM = 9,
   VX_THREAD_MC = 10,
   VX_THREAD_MF = 11,
   VX_THREAD_UN = 12,
   VX_THREAD_UNEF = 13,
   VX_THREAD_UNJ = 14,
   VX_THREAD_UNJC = 15,
   VX_THREAD_UNJF = 16,
   VX_THREAD_UNJEF = 17,
   VX_THREAD_ACME = 18,
   VX_THREAD_BSP = 19,
   VX_THREAD_BSTP = 20
   } evxThreadType;

typedef struct svxThreadData
   {
   evxThreadType Type;  /* thread type */
   double Diameter;     /* thread diameter (mm) */
   double Length;       /* thread length (mm) */
   double PerUnit;      /* threads per unit (0 if not used) */
   double Pitch;        /* thread pitch (mm) (0 if not used) */
   int EndChamfer;      /* 1=Yes; 0=No */
   } svxThreadData;

// Hole chamfer, added in 1900
typedef struct svxHoleChams
   {
   int fCham;              /* Flag if chamfer */
   int fChamSym;           /* Flag if it is symmetric chamfer */
   double chamDst;         /* Chamfer distance */
   double chamAngle;       /* Chamfer angle */
   }svxHoleChams;

/* hole data */
typedef struct svxHoleData
   {
   /* Geometry */
   evxHoleType Type;    /* hole type */
   int idInsFace;       /* id of insertion face */
   int Count;           /* number of holes */
   svxPoint *Points; 	/* list of "Count" hole insertion points */
                        /* (calling procedure must allocate/deallocate) */
   double Diameter1;    /* 1st diameter (mm) */
   double Depth1;       /* 1st depth (mm) */
   double Diameter2;    /* 2nd diameter (mm, countersunk holes) */
   double Depth2;       /* 2nd depth (mm, countersunk holes) */
   double TaperAngle;   /* taper angle (degrees) */
   double TipAngle;     /* tip angle (degrees) (0 is mapped to 180) */
   evxEndCondition End; /* hole end condition */
   int idUntilFace;     /* id of "until" face (0 if not required) */
   int UseDirection;    /* 1 to use "Direction" input; 0 for default */
   svxVector Direction; /* optional direction for hole centerline */
   int OpNoRemove;      /* bool operation type (0 means remove for default, 1 is none) */
   int NoProjPntLoc;    /* 1 not to project location point, 0 to project */

   /* Thread */
   evxThreadType ThreadType;  /* thread type */
   double ThreadDiameter;     /* thread diameter (mm) */
   double ThreadLength;       /* thread length (mm) */
   double ThreadsPerUnit;     /* threads per unit (0 if not used) */
   double ThreadPitch;        /* thread pitch (mm) (0 if not used) */
   int    fThrdClass;         /* T/F flag for thread class, added in 1900 version */
   int    thrdClass;          /* thread class 1B/2B/3B, added in 1900 version */

   svxHoleChams sCham;      /* Start chamfer */
   svxHoleChams mCham;      /* Middle chamfer for counter bore or spot face bore */
   svxHoleChams eCham;      /* End chamfer */
   int          fExtTap;    /* Flag if extend with taper direction for tapered or counter-sink hole */
   int          fExtStart;  /* Flag if extend the start of hole */
   /* Tolerance */
   int UseTol;          /* 1 to use tolerances; or 0 */
   double TolIn;        /* inner tolerance on Diameter 1 */
   double TolOut;       /* outer tolerance on Diameter 1 */

   int UseD2Tol;          /* T/F flag for ID2 Tol input choice, add in 1900 version */
   double D2InTol;      /* D2 tolerance lower limit, added in 1900 version */
   double D2OutTol;     /* D2 tolerance upper limit, added in 1900 version */

   /* Other */
   char Callout[128];   /* callout text string */
   int DoNotMachine;    /* 1 to ignore hole in CAM, or 0 */

   } svxHoleData;

/*| shape feature data
*******************************************/

/* boolean combination method */
typedef enum evxBoolType
   {
   VX_BOOL_NONE = 0,
   VX_BOOL_ADD = 1,
   VX_BOOL_REMOVE = 2,
   VX_BOOL_INTERSECT = 3
   } evxBoolType;

/* box */
typedef struct svxBoxData
   {
   evxBoolType Combine; /* combination method */
   int idPlane;         /* optional datum plane, or 0 */
   int useAxis;         /* flag to use axis, 0 default */
   svxAxis axis;        /* optional axis */
   svxPoint Center;     /* center of box volume */
   double X;            /* length along X */
   double Y;            /* length along Y */
   double Z;            /* length along Z */
   vxName ftrName;  /* feature name for "FtAllBox", 0 default */
   } svxBoxData;

/* cylinder */
typedef struct svxCylData
   {
   evxBoolType Combine; /* combination method */
   int idPlane;         /* optional datum plane (0 to use XY plane) */
   int useAxis;         /* flag to use axis, 0 default */
   svxAxis axis;        /* optional axis */
   svxPoint Center;     /* center of bottom face */
   double Radius;       /* radius (mm) in XY plane */
   double Length;       /* length (mm) along Z */
   vxName ftrName;  /* feature name for "FtAllCyl", 0 default */
   } svxCylData;

/* extruded shape */
typedef struct svxExtrudeData
   {
   evxBoolType Combine; /* combination method */
   int idProfile;       /* profile (sketch or curvelist) */
   double Start;        /* start distance (mm) */
   double End;          /* end distance (mm) */

   double Draft;        /* draft angle (deg) */
   int Blend;           /* corner blend option */
                        /* (0-variable, 1-constant, 2-round) */

   int UseDirection;    /* 1 to use optional extrusion direction, else 0 */	
   svxVector Direction; /* optional extrusion direction */

   double Twist;        /* twist angle (deg) */
   svxPoint Pivot;      /* pivot point if twist is non-zero */

   double Offset;       /* profile offset (mm) (0 if undefined) */
   double Thickness;    /* wall thickness (mm) (0 if undefined) */

   int iKeep;           /* keep profile (1-keep, 0-not keep) */

   int numBooleanEnts;  /* count of shapes in BooleanEnts array, use 0 if BooleanEnts is NULL*/
   int *BooleanEnts;    /* id list of shapes to combine with extrusion, use NULL if Combine is VX_BOOL_NONE or to
                           combine extrusion with all visible shapes */
   vxName ftrName;  /* feature name for "FtAllExt", 0 default */
   } svxExtrudeData;

/* revolved shape */
typedef struct svxRevolveData
   {
   evxBoolType Combine; /* combination method */
   int idProfile;       /* profile (sketch or curvelist) */
   double Start;        /* start angle (deg) */
   double End;          /* end angle (deg) */
   svxAxis Axis;        /* axis of revolution */

   double Offset;       /* profile offset (mm) (0 if undefined) */
   double Thickness;    /* wall thickness (mm) (0 if undefined) */
   } svxRevolveData;

/* sweep option */
typedef enum evxSweepOpt
   {
   VX_SWP_NATURAL = 0,     /* Default frame */
   VX_SWP_AT_PATH = 1,     /* At Path */
   VX_SWP_FRAME = 2,       /* Selected */
   VX_SWP_AT_PROFILE = 3,  /* At intersection */
   VX_SWP_ALONG_PATH = 4   /* Along Path */
   } evxSweepOpt;

/* swept shape */
typedef struct svxSweepData
   {
   evxBoolType Combine; /* combination method */
   int idProfile;       /* profile (sketch, curve, edge, curvelist) */
   int idPath;          /* id of curve that identifies the sweep path */
   svxPoint StartPnt;   /* point that identifies start of path curve */
   /* (defined relative to curve's coordinate frame) */

   int idParent;        /* id of path curve's parent sketch or curvelist */
   int UseParent;       /* 1 to use curve's parent as the sweep path, else 0 */

   evxSweepOpt Option;  /* sweep option */

   int idFrame;         /* id of sweep frame if Option=VX_SWP_FRAME, else 0 */

   int idXaxis;         /* id of X-axis point, curve or plane (0 if undefined or tangent to path) */

   int idZaxis;         /* id of Z-axis point, curve or plane (0 if undefined or minimum twist) */

   double Offset;       /* profile offset (mm) (0 if undefined) */
   double Thickness;    /* wall thickness (mm) (0 if undefined) */
   } svxSweepData;

/*| curve data
*******************************************/

/* 3D interpolated curve */
typedef struct svxCrvIntData
   {
   /* required inputs */
   int isClosed;        /* 1 if curve is closed, 0 if open */
   int Degree;          /* curve degree (2 - 6) */
   int Count;           /* number of points to interpolate */
   svxPoint *Points;    /* pointer to list of points to interpolate */
                        /* (set Z coordinate to zero for 2D curves) */

   /* optional inputs */
   int UseStartTan;     /* 1 to use start tangent direction, else 0 */
   svxVector StartTan;  /* normalized start tangent direction vector */
   double StartScale;   /* start tangent scale factor (0.1 - 10.0) */

   int UseEndTan;       /* 1 to use start tangent direction, else 0 */
   svxVector EndTan;    /* normalized end tangent direction vector */
   double EndScale;     /* end tangent scale factor (0.1 - 10.0) */
   } svxCrvIntData;

/* 3D ellipse & rectangle type */
typedef enum evxRectangleType
   {
   VX_RECT_2PT = 0,
   VX_RECT_WIDTH_HEIGHT = 1,
   VX_RECT_3PT = 2
   } evxRectangleType;

/* 3D ellipse & rectangle data */
typedef struct svxRectangleData
   {
   /* required inputs */
   evxRectangleType type;  /* type to make rectangle */
   int idPlane;            /* not required if "type" is VX_RECT_3PT */
   int firstPtType;        /* type of 1st input point (0 corner, 1 center) */
   svxPoint pts[3];        /* 3 input points depend on "type" */
   double width;           /* width if "type" is VX_RECT_WIDTH_HEIGHT */
   double height;          /* height if "type" is VX_RECT_WIDTH_HEIGHT */

   /* optional inputs */
   double angle;           /* rotation angle */
   int prjFirstPt;         /* 1 to project first point to plane, 0 not */
   int parallelogram;      /* 0 rectangle, 1 parallelogram */
   } svxRectangleData;

/*| general part data
*******************************************/

/* global settings */
typedef enum evxGlobal
   {
   VX_TOLERANCE = 1,    /* proximity tolerance (double) */
   VX_AUTO_MERGE = 2,   /* 1=auto-merge shells before boolean (int) */
   VX_EXACT_MATCH = 3,   /* 1=exact label matching enabled; 0=disabled (int) */
   VX_LOCALCOORDIDX = 4,   /* Local coordinate */
   } evxGlobal;

/* attribute data type */
typedef enum evxAttributeType
   {
   VX_ATTR_STRING = 0,  /* string */
   VX_ATTR_BOOL   = 1,  /* bool */
   VX_ATTR_INT    = 2,  /* integer */
   VX_ATTR_REAL   = 3,  /* real number */
   VX_ATTR_DATE   = 4,  /* date/time */
   } evxAttributeType;

/* standard attribute data */
typedef struct svxAttribute
   {
   char  label[64];     /* attribute label */
   char  data[256];     /* attribute data */
   double dValue;       /* value for bool, integer, real number, date type */
   char  type;          /* attribute type (evxAttributeType) */
   char  sub_type;      /* sub type for real number type (use input type of evxNumType) */
   char  unit;          /* unit type for non-constant real number (evxUnitType, evxAngUnitType...) */
   } svxAttribute;

/* part attribute structure */
typedef struct svxPartAttribute
   {
   int Version;
   int UserAttributeCount;
   svxAttribute UserAttribute[64];
   svxAttribute Name;
   svxAttribute Number;
   svxAttribute Partclass;
   svxAttribute Designer;
   svxAttribute Cost;
   svxAttribute Supplier;
   svxAttribute Description;
   svxAttribute Keyword;
   svxAttribute Manager;
   svxAttribute Material;
   svxAttribute Startdate;
   svxAttribute Enddate;
   svxAttribute Derived;
   svxAttribute No_section;   /* set "data" to "yes" or "no" */
   svxAttribute No_hatch;     /* set "data" to "yes" or "no" */
   svxAttribute Dimen[20];
   svxAttribute No_bom;       /* set "data" to "yes" or "no" */
   svxAttribute File_path;
   svxAttribute No_rootshow;  /* set "data" to "yes" or "no" */
   } svxPartAttribute;

/* 3D Block Type */
typedef enum evxBlockType
	{
	VX_BLOCK_UNDEFINED=0,	/* undefined 3D block type */
	VX_BLOCK_FACETS=1,		/* block of 3D triangular facets */
	VX_BLOCK_POINTS=2			/* block of 3D points (i.e. point cloud) */
	} evxBlockType;

/* part component/shape alignment */
typedef enum evxAlignType
   {
   VX_ALIGN_COINCIDENT = 0,
   VX_ALIGN_TANGENT = 1,
   VX_ALIGN_CONCENTRIC = 2,
   VX_ALIGN_PARALLEL = 3,
   VX_ALIGN_PERPENDICULAR = 4,
   VX_ALIGN_ANGLE = 5
   } evxAlignType;

typedef enum evxAlignInterference
   {
   VX_INTERFERENCE_NONE = 0,
   VX_INTERFERENCE_HIGHLIGHT = 1,
   VX_INTERFERENCE_STOP_AT = 2,
   VX_INTERFERENCE_ADD_ALIGN = 3
   } evxAlignInterference;

typedef struct svxAlign
   {
   svxData Entity1;     /* "isPntOnCrv" or "isPntOnFace" required */
   svxData Entity2;     /* "isPntOnCrv" or "isPntOnFace" required
                        ** point must be defined in active part space
                        ** (see cvxPntOnFace())
                        */
   evxAlignType Type;   /* alignment type */
   double Offset;       /* distance between alignment entities */
   double Angle;        /* angle between alignment entities */
   int isOpposite;      /* 1 to make entities opposite facing, else 0 */
   int ShowExistingAlign;
   evxAlignInterference Interference;
   } svxAlign;

typedef struct svxMassProp 
   {
   double Density;      /* Density (kg/mm^3) */
   double Area;         /* Area (mm^2) */
   double Volume;       /* Volume (mm^3) */
   double Mass;         /* Mass (kg) */
   svxPoint Center;     /* Centroid (i.e. Center of Gravity) */
   svxVector Axis[3];   /* Principal axes relative to global xyz frame */
   double Im[6];        /* Moments of inertia relative to xyz axes */
   double Ip[3];        /* Moments of inertia relative to principal axes */
   double Rad[3];       /* Radii of gyration relative to principal axes */
   } svxMassProp;

typedef struct svxPrfAreaProp
   {
   svxPoint2    org;     /* Origin for caculating the properties */
   svxPoint2    centroid;/* Centroid */

   double      area;     /* Area */

   double      Ix;       /* Inertia about x axis at org*/
   double      Iy;       /* Inertia about y axis at org*/
   double      Ir;       /* Inertia about z axis at org */
   double      Ixy;      /* Product of Inertia at org */

   double      Kx;       /* Radius of Gyration about x axis at org */
   double      Ky;       /* Radius of Gyration about y axis at org */
   double      Kr;       /* Radius of Gyration about z axis at org */

   double      Pmin;     /* Principal axis of which inertia is minimum */
   double      Pmax;     /* Principal axis of which inertia is maximum */
   double      Imin;     /* Inertia about Pmin */
   double      Imax;     /* Inertia about Pmax */
   } svxPrfAreaProp;

/*| file export data
*******************************************/

typedef enum evxImgColorMode
   {
   VX_EXPORT_IMG_COLOR_MODE_24BITS = 0,
   VX_EXPORT_IMG_COLOR_MODE_8BITS = 1,
   VX_EXPORT_IMG_COLOR_MODE_COlORIZED = 0,
   VX_EXPORT_IMG_COLOR_MODE_MONOCHROME = 2,
   VX_EXPORT_IMG_COLOR_MODE_GRAYSCALE = 3
   } evxImgColorMode;

typedef enum evxImgBkgndMode
   {
   VX_EXPORT_IMG_BKGND_MODE_CURRENT = 0,
   VX_EXPORT_IMG_BKGND_MODE_CUSTOM
   } evxImgBkgndMode;

typedef enum evxImgRangeMode
   {
   VX_EXPORT_IMG_RANGE_MODE_NORMAL = 0,
   VX_EXPORT_IMG_RANGE_MODE_EXTENTS,
   VX_EXPORT_IMG_RANGE_MODE_CUSTOM
   } evxImgRangeMode;

typedef enum evxImgType
   {
   VX_EXPORT_IMG_TYPE_BMP = 1,
   VX_EXPORT_IMG_TYPE_GIF,
   VX_EXPORT_IMG_TYPE_PNG,
   VX_EXPORT_IMG_TYPE_TIFF,
   VX_EXPORT_IMG_TYPE_JPEG
   } evxImgType;

typedef enum evxPdfType
   {
   VX_EXPORT_PDF_TYPE_RASTER = 1, /* load the raster image into pdf */
   VX_EXPORT_PDF_TYPE_VECTOR,     /* create the vector graphic data into pdf */
   VX_EXPORT_PDF_TYPE_OBJECT      /* export type is 3D */
   } evxPdfType;

typedef enum evxPdfRangeMode
   {
   VX_EXPORT_PDF_RANGE_MODE_NORMAL = 0, /* export are-display */
   VX_EXPORT_PDF_RANGE_MODE_EXTENTS,    
   VX_EXPORT_PDF_RANGE_MODE_CUSTOM      /* export are-windows */
   } evxPdfRangeMode;

typedef enum evxExportType
   {
   VX_EXPORT_TYPE_IMG = 1,
   VX_EXPORT_TYPE_PDF,
   VX_EXPORT_TYPE_GRP,
   VX_EXPORT_TYPE_DWG,
   VX_EXPORT_TYPE_IGES,
   VX_EXPORT_TYPE_STEP,
   } evxExportType;

typedef struct svxImgData
   {
   evxImgType Type;
   evxImgColorMode ColorMode;
   unsigned int Width;
   unsigned int Height;
   unsigned int Quality;
   evxImgBkgndMode BkgndMode;
   svxColor Background;
   evxImgRangeMode RangeMode;
   svxBndBox Rect;
   } svxImgData;

typedef struct svxPdfData
   {
   evxPdfType Type;   /*export pdf type*/
   int Color;         /*all black color or not, 1- MONOCHROME, 2-GRAYSCALE, 3-COLORIZED*/
   int Font;          /*embed font or not, can be ignored */
   int Dpi;           /*DPI of pdf*/
   float PaperWidth;  /*width of page*/
   float PaperHeight; /*height of the page*/
   evxPdfRangeMode RangeMode;  /*export mode, VX_EXPORT_PDF_RANGE_MODE_NORMAL-Rect can be ignored*/
   svxBndBox Rect;    /*export extent*/
   char Author[128];   /*author of pdf*/ 
   char Title[128];    /*title of pdf*/
   char Subject[128];  /*subject of pdf*/
   char Password[128]; /*password of pdf*/
   char Description[8192]; /*description of pdf*/
   } svxPdfData;         

typedef struct svxSTL
   {
   char        *name;     /* name                                             */
   int         nFacet;    /* number of facet                                  */
   svxTriangle *facets;   /* Array of STLfacet                                */
   double	   dTol;		  /* Tolerance to use when extracting facets          */
   } svxSTL;

/* version of DWG */
typedef enum evxDWGVer
   {
   VX_DWG_VERSION_2013 = 1,
   VX_DWG_VERSION_2010 = 2,
   VX_DWG_VERSION_2007,
   VX_DWG_VERSION_2004,
   VX_DWG_VERSION_2000,
   VX_DWG_VERSION_R14,
   VX_DWG_VERSION_R13,
   VX_DWG_VERSION_R11_R12,
   }evxDWGVer;

typedef struct svxDWGData
   {
   evxDWGVer Version;    /* version of DWG */ 
   char FileType;        /* export type of file. 0-DWG, 1- DFX(ASCLL), 2-DFX(Binary) 
                            make sure the file extension type is same with FileType */
   char ExportType;      /* 0-All objects, 1-Specified entities */
   int EntCnt;           /* the count of export entities if ExportType = 1 */
   int *EntList;         /* the specified entities id list if ExportType = 1 */
   }svxDWGData;

typedef struct svxIGESData
   {
   char Coordinate;      /* 0-World, 1- Local (0-default)*/
   char ExportType;      /* 0-All objects, 1-Specified entities */
   int EntCnt;           /* the count of export entities if ExportType = 1 */
   int *EntList;         /* the specified entities id list if ExportType = 1 */
   } svxIGESData;

typedef struct svxSTEPData
   {
   char AppProtocol;     /* 0-AP203, 1-AP214 (1-default) */
   char OutPut;          /* 0-PART, 1-Assembly (1-default) */
   char ExportType;      /* 0-All objects, 1-Specified entities */
   int EntCnt;           /* the count of export entities if ExportType = 1 */
   int *EntList;         /* the specified entities id list if ExportType = 1 */
   } svxSTEPData;

typedef enum evxImportType
   {
   VX_IMPORT_TYPE_PARASOLID = 1,  /* import .x_t file */
   VX_IMPORT_TYPE_DWG_DXF = 2,    /* import .dwg/.dxf file */
   VX_IMPORT_TYPE_STEP,           /* import .stp/.step file */
   VX_IMPORT_TYPE_SAT,            /* import .sat/.sab file */
   VX_IMPORT_TYPE_IGES,           /* import .igs/.iges file */
   VX_IMPORT_TYPE_CATIA4,         /* import .model/.exp... file */
   VX_IMPORT_TYPE_CATIA5,         /* import .CATPart/.CATProduct file */
   VX_IMPORT_TYPE_INVENTOR,       /* import .ipt/.iam file */
   VX_IMPORT_TYPE_JT,             /* import .jt file */
   VX_IMPORT_TYPE_PROE_CERO,      /* import .prt/.asm file */
   VX_IMPORT_TYPE_SOLIDEDGE,      /* import .par/.psm... file */
   VX_IMPORT_TYPE_SOLIDWORKS,     /* import .sldprt/.sldasm file */
   VX_IMPORT_TYPE_UG,             /* import .prt file */
   }evxImportType;

typedef struct svxImportData
   {
   evxImportType type; /* import type */
   vxPath filePath;    /* import file path */
   int importTo;       /* 0-current object, 1-new object, 2-new file */
   int importTarget;   /* 0-default, 1-part,2-sheet,3-sketch, 
                          if it doesn't need this param, use the default value.*/
   }svxImportData;

/*| CAM data
*******************************************/

#define ZW_CAM_GOUGE 1
#define ZW_CAM_NOTGOUGE 0

typedef enum evxCmOpType
   {
   /*Drill:0-19*/
   ZW_CAM_OPERATION_CENTER       =0,
   ZW_CAM_OPERATION_DRILL,
   ZW_CAM_OPERATION_PECK,
   ZW_CAM_OPERATION_CHIP,
   ZW_CAM_OPERATION_REAM,
   ZW_CAM_OPERATION_BORE,
   ZW_CAM_OPERATION_FINEBORE,
   ZW_CAM_OPERATION_COUNTERBORE,
   ZW_CAM_OPERATION_COUNTERSINK,
   ZW_CAM_OPERATION_TAP,
   /*2X:20-34*/
   ZW_CAM_OPERATION_PROFILECUT   =20,
   ZW_CAM_OPERATION_SPIRAL,
   ZW_CAM_OPERATION_ZIGZAG,
   ZW_CAM_OPERATION_BOX,
   ZW_CAM_OPERATION_CONTOUR,
   ZW_CAM_OPERATION_CHAMFER,
   ZW_CAM_OPERATION_COURNER_ROUND,
   ZW_CAM_OPERATION_HELICAL,
   ZW_CAM_OPERATION_RAMP,
   ZW_CAM_OPERATION_TOPFACE_CUT,
   ZW_CAM_OPERATION_NESTING,
   ZW_CAM_OPERATION_INTER_PATH_MOVE,
   /*Turning:35-49*/
   ZW_CAM_OPERATION_DRILL_TURNING=35,
   ZW_CAM_OPERATION_FACING,
   ZW_CAM_OPERATION_ROUGH,
   ZW_CAM_OPERATION_FINISH,
   ZW_CAM_OPERATION_GROOVE,
   ZW_CAM_OPERATION_THREADING,
   ZW_CAM_OPERATION_PARTOFF,
   /*3X:50-79*/
   ZW_CAM_OPERATION_SPIRAL_ROUGH =50,
   ZW_CAM_OPERATION_ZIGZAG_ROUGH,
   ZW_CAM_OPERATION_BOX_ROUGH,
   ZW_CAM_OPERATION_CONTOUR_ROUGH,
   ZW_CAM_OPERATION_SPIRAL_CAST  =55,
   ZW_CAM_OPERATION_ZIGZAG_CAST,
   ZW_CAM_OPERATION_BOX_CAST,
   ZW_CAM_OPERATION_CONTOUR_CAST,
   ZW_CAM_OPERATION_SPIRAL_FINISH=60,
   ZW_CAM_OPERATION_ZIGZAG_FINISH,
   ZW_CAM_OPERATION_BOX_FINISH,
   ZW_CAM_OPERATION_CONTOUR_FINISH,
   ZW_CAM_OPERATION_PROFILE_FINISH,
   ZW_CAM_OPERATION_SIDE,
   ZW_CAM_OPERATION_PEELING,
   ZW_CAM_OPERATION_ISO,
   ZW_CAM_OPERATION_3XISO,
   ZW_CAM_OPERATION_MAP,
   ZW_CAM_OPERATION_REST,
   ZW_CAM_OPERATION_PENCIL_TRACE,
   ZW_CAM_OPERATION_SCALLOP_REMOVAL,
   ZW_CAM_OPERATION_SURFACE_ENGRAVING,
   /*QM:80-109*/
   ZW_CAM_OPERATION_SMOOTHFLOW   =80,
   ZW_CAM_OPERATION_OFFSET2D,
   ZW_CAM_OPERATION_LACE_ROUGH,
   ZW_CAM_OPERATION_PLUNGE,
   ZW_CAM_OPERATION_PRE_DRILL,
   ZW_CAM_OPERATION_OFFSET3D,
   ZW_CAM_OPERATION_LACE_FINISH,
   ZW_CAM_OPERATION_ANGLE_LIMITING_FINISHING,
   ZW_CAM_OPERATION_DRIVE_CURVE,
   ZW_CAM_OPERATION_ZLEVEL,
   ZW_CAM_OPERATION_PENCIL,
   ZW_CAM_OPERATION_FLOW_3D,
   ZW_CAM_OPERATION_BULGE,
   ZW_CAM_OPERATION_ENGRAVE_2D,
   ZW_CAM_OPERATION_SMOOTHFLOW_HSM  =95,
   ZW_CAM_OPERATION_OFFSET2D_HSM,
   ZW_CAM_OPERATION_LACE_HSM,
   ZW_CAM_OPERATION_FLOW_HSM,
   /*5X:110-129*/
   ZW_CAM_OPERATION_PLANE           =110,
   ZW_CAM_OPERATION_SWARF,
   ZW_CAM_OPERATION_DRIVECURVE_5X,
   ZW_CAM_OPERATION_INTERACTIVE,
   ZW_CAM_OPERATION_FLOW_5X,
   ZW_CAM_OPERATION_SIDE_5X,
   ZW_CAM_OPERATION_GUIDE_SURFACE_ISO
   }evxCmOpType;

/*300-399*/
typedef enum evxCm2XOpAttr
   {
   ZW_CAM_2X_OPERATION_ATTR_NAME             =300,
   ZW_CAM_2X_OPERATION_ATTR_FRAME,            
   ZW_CAM_2X_OPERATION_ATTR_PATH_TOL,

   }evxCm2XOpAttr;

/*400-499*/
typedef enum evxCmQmOpAttr
   {
   ZW_CAM_QM_OPERATION_ATTR_NAME             =400,
   ZW_CAM_QM_OPERATION_ATTR_FRAME ,           
   ZW_CAM_QM_OPERATION_ATTR_PATH_TOL,         
   ZW_CAM_QM_OPERATION_ATTR_SURF_THICK,       
   ZW_CAM_QM_OPERATION_ATTR_Z_SURF_THICK,
   ZW_CAM_QM_OPERATION_ATTR_TOOL,
   ZW_CAM_QM_OPERATION_ATTR_MINTOOLHEIGHT,
   ZW_CAM_QM_OPERATION_ATTR_CLASS,
   ZW_CAM_QM_OPERATION_ATTR_LIMIT_TOPZ,
   ZW_CAM_QM_OPERATION_ATTR_LIMIT_BOTTOMZ,
   ZW_CAM_QM_OPERATION_ATTR_LIMIT_BOTTOM_CURVE,
   ZW_CAM_QM_OPERATION_ATTR_LIMIT_BOTTOM_CURVE_ZDISPLACEMENT,
   ZW_CAM_QM_OPERATION_ATTR_LIMIT_BOTTOM_CURVE_OFFSET
   }evxCmQmOpAttr;

typedef enum evxCmCompAtrr
   {
   ZW_CAM_COMP_ATTR_FACES,
   ZW_CAM_COMP_ATTR_CLASS,
   ZW_CAM_COMP_ATTR_CDCOMP
   }evxCmCompAtrr;

typedef enum evxCmCompClass
   {
   ZW_CAM_COMP_CLASS_PART,
   ZW_CAM_COMP_CLASS_CLAMP,
   ZW_CAM_COMP_CLASS_STOCK,
   ZW_CAM_COMP_CLASS_TABLE,
   ZW_CAM_COMP_CLASS_TOOLHOLDER,
   ZW_CAM_COMP_CLASS_ATTACHMENT
   }evxCmCompClass;

typedef enum evxCmAttrValueType
   {
   ZW_CAM_ATTR_VALUE_TYPE_INVALID = -1,
   ZW_CAM_ATTR_VALUE_TYPE_INTEGER = 0,
   ZW_CAM_ATTR_VALUE_TYPE_DOUBLE,
   ZW_CAM_ATTR_VALUE_TYPE_STRING
   }evxCmAttrValueType;

typedef struct svxCmAttrValue
   {
   evxCmAttrValueType type;
   int cnt;
   void * data;      /* user is response to free this memory */
   }svxCmAttrValue;

typedef enum evxCmFtrType
   {
   ZW_CAM_FEATURE_PROFILE =5,
   ZW_CAM_FEATURE_SURFACE = 7
   }evxCmFtrType;

typedef enum evxCmToolType
   {
   CM_TOOL_TYPE_MILL,            /* mill */
   CM_TOOL_TYPE_FLAME,           /* flame */
   CM_TOOL_TYPE_DRILL            /* drill */
   }evxCmToolType;

typedef enum evxCmToolSubType
   {
   CM_TOOL_SUBTYPE_BALLNOSE,     /* taperball */
   CM_TOOL_SUBTYPE_BULLNOSE,     /* taperbull */
   CM_TOOL_SUBTYPE_ENDNOSE       /* end */
   }evxCmToolSubType;

typedef enum evxCmToolAttr
   {
   CM_TOOL_NAME = 400,
   CM_TOOL_TYPE,
   CM_TOOL_SUBTYPE,
   CM_TOOL_ID,
   CM_TOOL_LEN,
   CM_TOOL_CUTTERDIAMETER,
   CM_TOOL_HOLDER,
   /* tool register data in active machine */
   CM_TOOLCHANGER_LOCATION,
   CM_TOOLCHANGER_DIAMETER,
   CM_TOOLCHANGER_HEIGHT
   }evxCmToolAttr;

typedef enum evxCmFolderAttr
   {
   CM_FOLDER_NAME,
   CM_FOLDER_SUB
   }evxCmFolderAttr;

typedef enum evxCmFtrAttr
   {
   CM_FEATURE_TYPE
   }evxCmFtrAttr;

typedef enum evxCmFtrProfileType
   {
   PART,
   CONTAIN
   }evxCmFtrProfileType;

typedef enum evxCmMachClass
   {
   TWO_AXIS_MC,
   THREE_AXIS_MC
   }evxCmMachClass;

typedef enum evxCmMachType
   {
   CM_MACH_TYPE_VERTICAL,
   CM_MACH_TYPE_HORIZNTAL
   }evxCmMachType;

typedef enum evxCmMachSubType
   {
   CM_MACH_SUBTYPE_ROTATING_HEAD,
   CM_MACH_SUBTYPE_ROTATING_TABLE
   }evxCmMachSubType;

typedef enum evxCmMachCutCompenation
   {
   CM_MACH_CUT_COMPENSATION_NONE,
   CM_MACH_CUT_COMPENSATION_LENGTH,
   CM_MACH_CUT_COMPENSATION_CENTER
   }evxCmMachCutCompenation;

typedef enum evxCmMachAttr
   {
   /* Definition Form */
   CM_MACH_NAME,
   CM_MACH_CLASS,
   CM_MACH_TYPE,
   CM_MACH_SUBTYPE,
   CM_MACH_POST_CONFIGURATION,
   CM_MACH_CUT_COMPENSATION,
   /* Others */
   }evxCmMachAttr;

typedef enum evxCmFrameAttr
   {
   /* Definition Form */
   CM_FRAME_NAME,
   CM_FRAME_CLEAR_Z,
   CM_FRAME_APPROACH_Z,
   CM_FRAME_RETRACT_Z
   }evxCmFrameAttr;

typedef enum evxCmObjType
   {
   CM_OPERATION,
   CM_OPFOLDER,
   CM_TOOL,
   CM_MACHINE,
   CM_CMCOMP,
   CM_FTR,
   CM_FRAME,
   CM_OUT
   }evxCmObjType;

typedef struct svxNcSetting
   {
   vxName   machine;
   char     filename[512];
   vxName   part_id;
   vxName   programmer;
   vxName   space;
   vxName   tool_changes;
   vxName   spd_fd;
   vxName   tool_num;
   vxName   coolant;
   char     prog_comment[256];
   } svxNcSetting;

typedef enum evxCmOutFldType
   {
   CM_OUTFLD_FLD,
   CM_OUTFLD_NC
   }evxCmOutFldType;

typedef enum evxCmOutType
   {
   CM_OUT_CL,
   CM_OUT_NC,
   CM_OUT_TOOL,
   CM_OUT_OP,
   CM_OUT_XML,
   CM_OUT_HTML
   }evxCmOutType;


 /* Sheet Format Attributes structure */
typedef struct svxDrawingAt
   {	
   vxLongName	templateName;/*template name*/
   vxLongName  configName;  /* configuration name for drawing format */
   char        paper[16];   /* paper size name */
   char        useTemplate; /* 1-use template,0-use custom */
   char        useConfig;   /* 1-use config,else 0 */
   double      width;       /* drawing width */
   double      height;      /* drawing height */  
   char		   useBorder;   /* 1: use border created by the parameters,else 0 */
   double	   margin[4];   /* margin[0] the margin of the top;
                               margin[1] the margin of the right;
                               margin[2] the margin of the bottom;
                               margin[3] the margin of the left;*/
   char		   bound;       /* 0: no bound line;1: have bound line*/                              
   char		   centerMark;  /* 0: have no centering marks;1: have centering marks*/                              
   char		   partition;   /* 0: do not divide the border, 1: divide the border */                              
   int		   partNum[2];  /* partNum[0]: the number of the partitions in the direction of horizontal;
                               partNum[1]: the number of the partitions in the direction of vertical;*/
   char		   trimmingMark;/* 0: no trimming mark; 1:have trimming mark;*/
   vxLongName  blockTemplateName[3];/* template name of drawing block, 
                              blockTemplateName[0]: template name of title block, 
                              blockTemplateName[1]: template name of additional list block, 
                              blockTemplateName[2]: template name of code list block*/
   } svxDrawingAt;

/* Dimension attributes structure */
typedef struct svxDimAttr
   {
   char   prefix[32];      /* dimension prefix */
   char   suffix[32];      /* dimension suffix */
   char   tolPlus[8];      /* tolerance plus */
   char   tolMinus[8];     /* tolerance minus */
   char   fontName[36];    /* font name */
   int    extLnClr;        /* extension lines color (evxColor) */
   int    dimLnClr;        /* dimension lines color (evxColor) */
   int    dimTxtClr;       /* dimension text color (evxColor) */
   int    valFmt;          /* dimension value text format
                           (1-no tol, 2-limit, 3-unequal tol, 4-equal tol, 5-tol zone, 6-fit tol) */
   int    zeroSupp;        /* zero suppression (1-none, 2-lead, 3-trail, 4-both) */
   int    decPlc;          /* dimension text decimal places */
   double chHgt;           /* text height */
   int    dimTxtOrient;    /* dimension text orientation(>=1) */
   int    LeaderAlign;     /* dimension line (or leader) justification(0-center, 1-top, 2-half, 3-bottom) */
   int    trmTyp1;         /* arrowhead1 type(0-12) */
   int    trmTyp2;         /* arrowhead2 type(0-12) */
   int    dimLineSize;     /* dimension line size(1-6) */
   int    extLineSize;     /* extension line size(1-6) */
   int    dimLineType;     /* dimension line type(0-5) */
   int    extLineType;     /* extension line type(0-5) */
   int    dimTxtBold;      /* dim. text bold flag (0-normal, 1-bold) */
   int    dimPrefixFlag;   /* radial, diameter, and chamfer prefix on/off(0-on, 1-off) */
   } svxDimAttr;

typedef enum
   {
/* The standard id of the part attribute item. */
   VX_AT_USER = 0,       /* user defined attributes */
   VX_AT_NAME = 1,       /* part name */
   VX_AT_NUMBER = 2,     /* part number */
   VX_AT_CLASS,          /* CAM class */
   VX_AT_DESIGNER,       /* designer */
   VX_AT_COST,           /* cost */
   VX_AT_SUPPLIER,       /* supplier */
   VX_AT_DESCRIPT,       /* description */
   VX_AT_KEYWORD,        /* keyword */
   VX_AT_MANAGER,        /* manager */
   VX_AT_MATERIAL,       /* material */
   VX_AT_START,          /* create time */
   VX_AT_END,            /* last modified time */
   VX_AT_DERIVED,        /* derived from */
   VX_AT_NO_SECTION,     /* do not section */
   VX_AT_NO_HATCH,       /* do not hatch */
   VX_AT_NO_BOM,         /* do not list in BOM */
   VX_AT_NO_ROOTSHOW,    /* do not list in root manager */
   VX_AT_DENSITY = 18,   /* density */
   VX_AT_FILEPATH = 20,  /* file path */
   VX_AT_SHEETPATH = 21, /* corresponded drawing sheet: FILE NAME,SHEET ROOT NAME */
   VX_AT_CAMPATH = 22,   /* corresponded CAM object: FILE NAME,CAM ROOT NAME */
   VX_AT_SHEETCODE = 23, /* sheet code */

   VX_AT_AREA = 25,      /* area */
   VX_AT_VOLUME = 26,    /* volume */
   VX_AT_MASS = 27,      /* mass property */
   VX_AT_LENGTH = 28,    /* length */
   VX_AT_WIDTH = 29,     /* width */
   VX_AT_HEIGHT = 30,    /* height */
   VX_AT_STOCK_SIZE = 43,     /* stock size */

/* The standard id of the sheet attribute item. */
   VX_AT_SHEET_SCALE = 200,   /* scale */
   VX_AT_SHEET_SEQUENCE = 201,/* sequence */
   VX_AT_SHEET_AMOUNT,        /* amount */
   VX_AT_SHEET_SIZE,          /* size */
   VX_AT_SHEET_PROJECTION,    /* projection */
   VX_AT_SHEET_FILEPATH,      /* filepath */
   } evxAtItemId;

/* image data */
typedef struct svxSymImg
   {
   /* required inputs */
   vxPath path;            /* image path or user define string */
   int firstPtType;        /* type of 1st input point (0 corner, 1 center) */
   svxPoint2 pts[2];       /* 2 input points depend on "type" */
   double width;           /* width */
   double height;          /* height */
   double angle;           /* rotation angle(degree) */
   double opaque;          /* opaque factor */
   int flip;               /* flip flag (0-none, 1-with, 2-with y ) */
   int locked;             /* 1-lock aspect ratio,0 not */
   } svxSymImg;

/*symbol type of surface roughness */
typedef enum
   {
   VX_SYMSURF_BAS = 0, /* basic */
   VX_SYMSURF_MRR,     /* material removal required */
   VX_SYMSURF_MRP,     /* material removal prohibited */
   VX_SYMSURF_JIS1,    /* JIS surface texture 1 */
   VX_SYMSURF_JIS2,    /* JIS surface texture 2 */
   VX_SYMSURF_JIS3,    /* JIS surface texture 3 */
   VX_SYMSURF_JIS4,    /* JIS surface texture 4 */
   VX_SYMSURF_JISNM    /* JIS no machining */
   } evxSymSurf;

/* surface finish data*/
typedef struct svxSymSurf
   {
   int          symTyp;             /* symbol type(evxSymSurf) */
   int          lay;                /* lay symbol */
   int          grinding;           /* grinding */
   char         maxR[65];           /* max roughness average (or =NULL) */
   char         minR[65];           /* min roughness average (or =NULL) */
   char         procMeth[65];       /* processing method/treatment (or =NULL) */
   char         sampR[65];          /* sampling length/roughness cutout (or =NULL) */
   char         machAllow[65];      /* machining allowance (or =NULL) */
   char         R[65];              /* Rz, roughness value (or =NULL) */
   char         otherR[65];         /* other roughness value/spacing (or =NULL) */
   char         optStr[65];         /* third row optional string (or =NULL) */
   }svxSymSurf;

typedef struct svxTextAt
   {   
   unsigned char    bold;             /* bold flag 0-normal, 1-bold */
   unsigned char    underline;        /* underline flag 0x01-ul,
                                         overline flag 0x02-ol,
                                         delete line flag 0x04-dl */
   unsigned char    italic;           /* italic flag 0-normal, 1-italic */
   unsigned char    word_wrap;        /* fit method 0-none, 1-word wrap, 2-auto size text */
   char     fontName[36];     /* font name */

   float    char_height;      /* text height */
   float    char_aspect;      /* width of text, char_aspect = width / height */
   float    multi_line_space; /* Vertical spacing between text */
   float    char_spacing;     /* Horizontal spacing between text */

   evxColor  color;
   unsigned char    size;             /* the  width of line */
   unsigned char    precision;        /* text precision */
   unsigned char    font_type;        /* font type (ex: proportional) */ 

   unsigned char    hor_align_type;   /* horizontal text alignment type */
   unsigned char    hor_align;        /* horizontal text alignment */
   unsigned char    vert_align_type;  /* vertical text alignment type */
   unsigned char    vert_align;       /* vertical text alignment */

   float    path;             /* path of text as angle 0.0-360.0 degrees  */
   float    text_angle;       /* slope of text */ 
   float    char_slant;       /* slant of text */
   float    text_expansion;   /* Multiplier for text expansion */

   float    first_line_ind;   /* first line indent */
   float    hanging_ind;      /* hanging indent */
   float    right_ind;        /* right indent */
   }svxTextAt;

typedef struct svxTextLoc
   {   
   unsigned char mirror;      /* mirror flag 0-normal , 1-mirror */
   float      text_radius;    /* radius of arc that the text follows */
   float      scale;          /* text scale factor */
   svxPoint2f origin_point;   /* text origin point */
   svxPoint2f align_point;    /* text alignment point */
   svxPoint2f mirror_axis[2]; /* two points defining mirror axis */
   svxPoint2f bbox[2];        /* two points defining text bounding box */
   }svxTextLoc;

typedef struct svxWeldAttr
   {
   svxAttribute spec;
   svxAttribute description;
   svxAttribute material;
   svxAttribute mass;
   svxAttribute length;
   }svxWeldAttr;

/* type of sketch constraint dimension */
typedef enum
   {
   VX_SK_CONSDIM_LINEAR,        /* type of linear dimension */
   VX_SK_CONSDIM_LINEAR_OFFSET, /* type of linear offset dimension */
   VX_SK_CONSDIM_ANGULAR,       /* type of angular dimension */
   VX_SK_CONSDIM_RADIAL,        /* type of radial/diametric dimension */
   VX_SK_CONSDIM_ARC_LENGTH,    /* type of arc length dimension */
   }evxConsDimType;

/* data of constraint dimension */
typedef struct svxConsDim
   { 
   svxPoint firstPnt;   /* first point */
   svxPoint secondPnt;  /* second point, NULL if it doesn't need */
   svxPoint textPnt;    /* text point */
   int type;            /* type of current dimension, defined by current command */
   int firstEnt;        /* entity id which the first point on it, NULL if it doesn't need*/
   int secondEnt;       /* entity id which the second point on it, NULL if it doesn't need*/
   }svxConsDim;

typedef struct
   {
   unsigned char color;   /* color (0 if bundle undefined, else 1 through (V_COLOR_LAST-1), ColorEnv[]) */
   unsigned char type;      /* line/marker type ( >= 0) */
   unsigned char size;      /* line/marker size ( >= 1) */
   char mode;      /* display mode */
                   /* -3, permanently blanked */
                   /* -2, used for assembly alignment */
                   /* -1, blanked */
                   /* 0, normal */
                   /* 1, highlight */
                   /* 2, lowlight */
                   /* 3, highlight and change line style */
                   /* 4, lowlight and change line style */
   } svxWireAt;
   
   /* Line attributes */
typedef struct svxLineAt
   {
   evxColor color;   /* line/marker color (1-18) */
   unsigned char type; /* line/marker type (0-15) */
   unsigned char size; /* line/marker size (1-6) */
   }svxLineAt;

/* Rectangle data */
typedef struct svxRectangle
   {
   long    left;
   long    top;
   long    right;
   long    bottom;
   }svxRectangle;

/* Interference check result data */
typedef struct svxIferChkResData
   {
   vxLongName    BaseCompName;   /* base component name for interference check */
   vxLongName    ChkCompName;    /* check component name for interference check */
   double        IferVolme;      /* volume of interference (mm^3) */
   }svxIferChkResData;

/*
*********************************************************************
*! 1. Custom Commands
*********************************************************************
*/

/* file: VxApiCmd.c */
/*| custom command definition and execution 
*/
extern void cvxCmdBuffer(const char *String, int Priority);
extern void cvxCmdCallback(const vxName Name, void *Function);
extern int cvxCmdEval(vxName Name, int idData);
extern int cvxCmdExec(int idData);
extern int cvxCmdExec2(int idData);
extern int cvxCmdFunc(const vxName Name, void *Function, double Code);
extern int cvxCmdChkLic(const vxName Name);
extern int cvxCmdFuncUnload(const vxName Name);
extern int cvxCmdInput(const vxName Name, int idDataIn, int *idDataOut);
extern int cvxCmdInteract(vxName Name);
extern void cvxCmdInqActive(vxName Name);
extern int cvxCmdMacro(char *Input, char **Output);
extern int cvxCmdMacro2(char *path);
extern void cvxCmdMarker(vxName Okay, vxName Cancel);
extern void cvxCmdSend(char *CommandString);
extern int cvxCmdTemplate(vxPath File);
extern int cvxCmdTemplateUnload(vxPath File);
extern int cvxCmdRegisterRcc(vxName File);
extern int cvxCmdUnRegisterRcc(vxName File);
extern void cvxCmdVariable(vxName Name, void *Variable);
extern int cvxCmdVariableUnload(const vxName Name);
extern int cvxCmdStateGet(vxName CmdName, evxCmdState *isEnable);
extern int cvxCmdStateSet(vxName CmdName, evxCmdState State);
extern void cvxEchoColorGet(evxColor *Color);
extern void cvxEchoColorSet(evxColor Color);
extern void cvxEchoColorAtSet(svxWireAt *at);
extern void cvxEchoColorAtGet(svxWireAt *at);
extern void cvxEchoCustomStart();
extern void cvxEchoCustomEnd();
extern void cvxEchoDraw();
extern void cvxEchoStart();
extern void cvxEchoEnd();
extern void cvxEchoEscDisable();
extern void cvxEchoEscRestore();

/* file: VxApiTpl.c */

/*| command template definition
*/
extern int cvxTplInqFnCust(vxName Name, int NumBytes, char *FnName);
extern int cvxTplInqFnEcho(vxName Name, int NumBytes, char *FnName);
extern int cvxTplInqFnInit(vxName Name, int NumBytes, char *FnName);
extern int cvxTplInqFnTerm(vxName Name, int NumBytes, char *FnName);

/* file: VxApiData.c */
/*| input data management for custom commands
*/
extern int cvxDataClear(int idData, int idField);
extern int cvxDataDel(int idData, int idField);
extern int cvxDataDelUnHi(int idData, int idField);
extern int cvxDataFree(int idData);
extern int cvxDataGet(int idData, int idField, svxData *Data);
extern int cvxDataGetColor(int idData, int idField, svxColor *Color);
extern int cvxDataGetEnt(int idData, int idField, int *idEnt, int *idParent);
extern int cvxDataGetEnts(int idData, int idField, int *Count, int **idEnts);
extern int cvxDataGetList(int idData, int idField, int *Count, svxData **Data);
extern int cvxDataGetNum(int idData, int idField, double *Number);
extern int cvxDataGetOpt(int idData, int idField);
extern int cvxDataGetPath(int idData, int idField, int *FirstEnt, svxEntPath*);
extern int cvxDataGetPnt(int idData, int idField, svxPoint*);
extern int cvxDataGetPnts(int idData, int idField, int *Count, svxPoint**);
extern int cvxDataGetPromote(int idData, int idField, int *Promote);
extern int cvxDataGetText(int idData, int idField, int NumBytes, char*);
extern int cvxDataGetMatrix(int idData, int idField, svxMatrix *Matrix);
extern int cvxDataGrpRem(int idData, int idField, int index);
extern int cvxDataInit(const vxName Template, int *idData);
extern int cvxDataSet(int idData, int idField, svxData *Data);
extern int cvxDataSetColor(int idData, int idField, svxColor *Color);
extern int cvxDataSetPath(int idData, int idField, svxEntPath*);
extern int cvxDataSetPromote(int idData, int idField, int Promote);
extern int cvxDataSetPntOnCrv(int idData, int idField, int idCrv, svxPoint*);
extern int cvxDataSetPntOnFace(int idData, int idField, int idComp, int idFace);
extern int cvxDataSetPntOnFace2(int idData, int idField, int idFace, svxPoint*);
extern int cvxDataToFeature(int idData, int idFeature, int ExcludeRegen);
extern void cvxDataZero(svxData *Data);
extern char* cvxDataName(int idData);
extern int cvxDataSetPntByPath(int idData, int idField, svxEntPath *, svxPoint*);


/*
*********************************************************************
*! 2. Custom GUI Forms
*********************************************************************
*/

/* file: VxApiForms.c */
/*| manage forms
*/
extern void cvxFormActiveField(int *idField);
extern void cvxFormCallback(vxName Name, void *Function);
extern int cvxFormCreate(char *Form, int Display);
extern int cvxFormFieldVisSet(const char *form, int field, int state);
extern void cvxFormFunc(vxName Name, void *Function, double Code);
extern void cvxFormKill(char *Form);
extern int cvxFormDataGet(char *Form, char **Data);
extern int cvxFormDataSet(char *Form, char *Data);
extern int cvxFormInqFld(vxName Name, int *Count, svxTplFld **Fields);
extern void cvxFormRefresh(char *Form);
extern void cvxFormShow(char *Form);
extern void cvxFormHide(char *Form);
extern int cvxFormState(char *Form);
extern void cvxFormTitleGet(char *Form, int NumBytes, char *Title);
extern void cvxFormTitleSet(char *Form, char *Title);
extern void cvxFormUpdate(int idField);
extern int cvxFormWait(char *Form);
extern void cvxFormWaitExit(int ReturnValue);

/*| manage fields within a form
*/
extern void cvxFieldCaretSet(char *Form, int idField, int Position);
extern void cvxFieldClear(char *Form, int idField);
extern void cvxFieldDisable(char *Form, int idField);
extern void cvxFieldEnable(char *Form, int idField);
extern int cvxFieldEnableGet(char *Form, int idField);
extern void cvxFieldFilterApply(char *Form, int idField, const char *FilterItem, int item);
extern void cvxFieldFocusSet(char *Form, int idField);
extern int cvxFieldInitFont(const char *Form, int idField, const char *Name,int hasZwFont);
extern void cvxFieldImageSet(char *Form, int idField, char *Image);
extern void cvxFieldNumItems(char *Form, int idField, int *NumItems);

/*| manage items within a field of a form
*/
extern void cvxItemAdd(char *Form, int idField, char *Text);
extern void cvxItemCbxSet(char *Form, int idField, int idItem, int isOn);
extern void cvxItemDel(char *Form, int idField, int idItem);
extern void cvxItemFind(char *Form, int idField, char *Text, int *idItem);
extern void cvxItemGet(char *Form, int idField, int idItem, char *Text);
extern void cvxItemModify(const char *Form, int idField, int idItem, const char *Text);
extern void cvxItemSelect(char *Form, int idField, int idItem);
extern void cvxItemSelectText(char *Form, int idField, char *Text);
extern void cvxItemSelected(char *Form, int idField, int *idItem);
extern void cvxItemSet(char *Form, int idField, int idItem, char *Text);
extern void cvxItemStateGet(char *Form, int idField, int idItem, int *isOn);
extern void cvxItemStateSet(char *Form, int idField, int idItem, int isOn);
extern int cvxItemGetCheckState(char *Form, int idField, int idItem, int *checkState);

/*| manage a popup menu
*/
extern int cvxPopupSetVisible(const char* name, int visible);
extern int cvxPopupShow(const char* name);


/*| manage a table on a form
*/
extern int cvxTableAtSet(char *Form, int idTable, svxTableAt *At);
extern int cvxTableComboSet(char *Form, int idTable, int Row, int Col, char *Options);
extern int cvxTableIndexComboSet(char *Form, int idTable, int Row, int Col, char *Options);
extern int cvxTableInputComboSet(char *Form, int idTable, int Row, int Col, char *Options);
extern int cvxTableSizeSet(char *Form, int idTable, int NumRow, int NumCol);
extern void cvxTableVisSet(char *Form, int idTable, int Row, int Col);
extern int cvxTableCellTextSet(char *Form, int idTable, int Row, int Col, char*text);
extern int cvxTableCellTextGet(char *Form, int idTable, int Row, int Col, char*text, int nBytes);
extern int cvxTableComboSet2(const char* Form, int idTable, int Row, int Col, char *Options, char *Images);
extern int cvxTableIndexComboSet2(const char* Form, int idTable, int Row, int Col, char *Options, char *Images);
extern int cvxTableInputComboSet2(const char* Form, int idTable, int Row, int Col, char *Options, char *Images);
extern int cvxTableComboSetOption(const char* Form, int idTable, int Row, int Col, int Index, char *IdxText);
extern int cvxTableComboGet(const char* Form, int idTable, int Row, int Col, int* Index, char* Text, int nBytes);
extern int cvxTableCellTextSet2(const char* Form, int idTable, int Row, int Col, char* Text, char* Image);
extern int cvxTableCheckSet(const char* Form, int idTable, int Row, int Col, int Check);
extern int cvxTableIsCheck(const char* Form, int idTable, int Row, int Col);
extern int cvxTableDateSet(const char* Form, int idTable, int Row, int Col, int Date, evxDateType DateType);
extern int cvxTableDateGet(const char* Form, int idTable, int Row, int Col, int* Date, evxDateType* DateType);
extern int cvxTableColorSet(const char* Form, int idTable, int Row, int Col, const svxColor* Color, int ClrIdx, unsigned char Type);
extern int cvxTableColorGet(const char* Form, int idTable, int Row, int Col, svxColor* Color, int* ClrIdx, unsigned char* Type);
extern int cvxTableNumSet(const char* Form, int idTable, int Row, int Col, const char* Num);
extern int cvxTableNumGet(const char* Form, int idTable, int Row, int Col, char* Num, int Bytes);
extern int cvxTableTypeGet(const char* Form, int idTable, int Row, int Col, evxTableCellType* Type);
extern int cvxTableCallBackParamGet(const char* Form, int idTable, svxTableCallBackParam* Param);
extern void cvxTableCallBackParamFree(svxTableCallBackParam *param);
extern void cvxTableCellEnableSet(const char* Form, int idTable, int Row, int Col, int State);
extern int cvxTableRowInsert(const char* Form, int idTable, int Row);
extern int cvxTableRowRemove(const char* Form, int idTable, int Row);
extern int cvxTableRowCnt(const char* Form, int idTable);
extern int cvxTableColInsert(const char* Form, int idTable, int Col);
extern int cvxTableColRemove(const char* Form, int idTable, int Col);
extern int cvxTableColCnt(const char* Form, int idTable);
extern int cvxTableGetSelRange(const char* Form, int idTable, int *RangeCnt, svxRectangle **SelRange);

/*| manage a tree widget on a form
*/
extern int cvxTreeItemsCount(const char*form, int field);
extern void cvxTreeSetColumnCount(const char* form, int field, int count);
extern int cvxTreeHeaderGetSize(const char* form, int field, int* wid, int* hi);
extern void cvxTreeSetHeaderText(const char* form, int field, int col, const char* text);
extern int cvxTreeSetHeaderTextAlignment(const char* form, int field, int col, evxAlignFlag align);
extern void cvxTreeSetColumnWidth(const char* form, int field, int col, int width);
extern int cvxTreeSetSelectionMode(const char* form, int field, int mode);
extern int cvxTreeSetSelectionBehavior(const char* form, int field, int behavior);
extern int cvxTreeAddItem(const char *form,int field,int parentId,int siblingId,const char *label,
           int imgIdx,int folderIdx,evxTreeItemType itemType,int dbIdx,int itemflag,int *outItemId,const char* cmdName);
extern int cvxTreeSetItemIconNameByColumn(const char *form, int field, int id, int col, const char* iconName);
extern int cvxTreeSetItemTextByColumn(const char* form, int field, int id, int col, const char* text);
extern void cvxTreeGetItemTextByColumn(const char* form, int field, int id, int col, char* text, int size);
extern int cvxTreeSelectItem(const char *form, int field, int id);
extern int cvxTreeUnselectAll(const char *form, int field);
extern int cvxTreeIsItemSelected(const char*form, int field, int id);
extern int cvxTreeGetDBIdByItem(const char *form, int field, int id, int*dbid);
extern int cvxTreeGetCbParam(const char*form, int field, int id, svxTreeCbParam* param);
extern int cvxTreeDeleteItem(const char*form, int field, int id);
extern int cvxTreeEditItemByLineEdit(const char *form, int field, int id, int col, const char *defaultLabel);
extern int cvxTreeGetChildItem(const char*form, int field, int id,int index, int*child);
extern int cvxTreeGetChildrenCount(const char*form, int field, int id,int *count);
extern int cvxTreeGetParentItem(const char*form, int field, int id, int *parent);
extern int cvxTreeGetDescendants(const char *sName, int field, int isAll, int parent, int *num, int **ids);
extern int cvxTreeGetSelectedItems(const char *form, int field, int *count, int **ids);

/*
*********************************************************************
*! 3. GUI/Display Management
*********************************************************************
*/

/* file: VxApiDisp.c */
/*| display management
*/
extern void cvxDispColorGet(evxDispAttrib Type, evxColor *Color);
extern void cvxDispColorSet(evxDispAttrib Type, evxColor Color);
extern void cvxDispDataUpdate(void);
extern int cvxDispGetLights(int *Count, svxLight **Lights);
extern int cvxDispSetLights(int Count, svxLight *Lights);
extern void cvxDispModeGet(evxDispMode *Mode);
extern void cvxDispModeSet(evxDispMode Mode);
extern void cvxDispRedraw(void);
extern void cvxDispRgbGet(evxDispAttrib Type, svxColor *Rgb);
extern void cvxDispRgbSet(evxDispAttrib Type, const svxColor *Rgb);
extern void cvxDispState(int Option, int State[]);
extern void cvxDispSwitch(evxDispItem Item, int isOn);
extern void cvxDispSwitchGet(evxDispItem Item, int *isOn);
extern void cvxDispZoomAll(int Redraw);
extern void cvxDispZoomAllIncludeDim(int Redraw);
extern void cvxDispZoomAt(const svxPoint point, const float factor);
extern int cvxDispWindowRectGet(const int includeFrame, const int isGlobal, int *x, int *y, int *h, int *w);
extern void cvxAuxFrameShow(int idData, int entField, int matField);
extern void cvxVisDispStatusGet(evxVisualDispItem Item, int *isOn);

/* file: VxApiLayer.c */
/*| layer management (active part or sheet)
*/
extern int cvxActiveLayerNumGet(int* Number);
extern int cvxActiveLayerNumSet(int Number);
extern int cvxLayerId(vxName name, int *id);
extern int cvxLayerInfoGet(int Number, svxLayerInfo* pLayerInfo);
extern int cvxLayerInfoSet(int Number, const svxLayerInfo* pLayerInfo);
extern int cvxLayerActivate(vxName Name);
extern int cvxLayerAssign(vxName Name, int numEnt, int *idEnts);
extern int cvxLayerAdd(vxName Name);
extern int cvxLayerAdd2(const svxLayerInfo* psLayerInfo, int *Number);
extern int cvxLayerDel(vxName Name);
extern int cvxLayerDel2(int Number);
extern int cvxLayerExists(vxName Name);
extern int cvxLayerExists2(int Number);
extern void cvxLayerInqActive(vxName Name);
extern int cvxLayerList(int *Count, vxName **Names);
extern int cvxLayerName(int Number, vxName Name);
extern int cvxLayerNum(vxName Name, int *Number);
extern int cvxLayerShowAll(void);
extern int cvxLayerStateGet(vxName Name, int *isVisible, int *isFrozen);
extern int cvxLayerStateSet(vxName Name, int isVisible, int isFrozen);
extern int cvxLayerSync(void);
extern int cvxLayerIdModify(int srcNumber,int tarNumber);

/* file: VxApiView.c */
/*| view management
*/
extern void cvxViewExtent(double Extent);
extern void cvxViewGet(svxMatrix *Frame, double *Extent);
extern void cvxViewOrigin(svxPoint *Origin);
extern void cvxViewSet(svxMatrix *Frame, double Extent);
extern void cvxViewStd(evxViewStd Type, double NullExtent);

/* file: VxApiGui.c */
/*| config file management
*/
extern int cvxUiSettingsCreate(char* Name, char* Base);
extern int cvxUiSettingsRename(char* Name, char* Dest);
extern int cvxUiSettingsRemove(char* Name);
extern int cvxUiSettingsImport(char* Path);
extern int cvxUiSettingsExport(char* Name, char* Path);
extern int cvxUiSettingsExists(char* Name);
extern int cvxUiSettingsApply(char* Name);
extern int cvxUiSettingsReset(char* Name);
extern int cvxUiSettingsCount(void);
extern int cvxUiSettingsFetch(int i, char* Name);

/*| cursor management
*/
extern void cvxCursorSet(evxCursorType Type);
extern void cvxCursorRestore(void);

/*| error management
*/
extern void cvxErrDisable(void);
extern void cvxErrEnable(void);

/*| message management
*/
extern void cvxMsgAreaClose(void);
extern void cvxMsgAreaOpen(void);
extern void cvxMsgAreaState(int);
extern void cvxMsgDisable(void);
extern void cvxMsgDisp(char *Text);
extern void cvxMsgEnable(void);
extern int cvxMsgFileLoad(vxPath File, int *idFile);
extern char *cvxMsgPtr(int idFile, int MessageNum);
extern void cvxPromptDisable(void);
extern void cvxPromptEnable(void);
extern void cvxTranStatusDisable(void);
extern void cvxTranStatusEnable(void);
extern void cvxProgress(int iprogress);
extern void cvxUserActionStatusGet(int *disableStatus);
extern void cvxUserActionStatusSet(int disable);
extern void cvxMsgDisp2(char *Text, int leval);
extern int cvxUserActionStatusSet2(evxDialogType type, int value);

/*| user input
*/
extern int cvxGetAngle(char *Prompt, double *Angle);
extern int cvxGetDistance(char *Prompt, double *Distance);
extern int cvxGetNumber(char *Prompt, double *Number);
extern int cvxGetEnt(char *Prompt,evxEntInpOpt,int EmptyOk,int*idEntity);
extern int cvxGetEnts(char*Prompt,evxEntInpOpt,int EmptyOk,int*Count,int**idEnts);
extern int cvxGetExternalPick(const char *Prompt,evxPntInpOpt Opt,int EmptyOk,svxPoint *Point,svxEntPath *EntPath,svxMatrix *Matrix);
extern void cvxGetFile(int Open,char*Prompt,char*Default,char*Filter,char*Path);
extern void cvxGetFileActiveField(int Open,const char*Prompt,const char*Default,const char*Filter);
extern void cvxGetFolder(char*Prompt, char*Default, char*Path);
extern void cvxGetFolderActiveField(const char*Prompt,const char*Default);
extern int cvxGetPoint(const char*Prompt,evxPntInpOpt,int EmptyOk,svxPoint*,int*idEnt);
extern int cvxGetPoints(char*Prompt,evxPntInpOpt,int EmptyOk,int*Count,svxPoint**Points);
extern int cvxGetResponse(int Option, char *Message);
extern int cvxGetString(const char*Prompt,const char*Default,int NumBytes,char*String);
extern int cvxMessageBox(int MsgType, int WndMode, const char* Title, const char* Message, const char* Btn1Str, const char* Btn2Str, int* Action);

/*| color utilities
*/
extern void cvxColorfToColor(const svxColorf* rgbf, svxColor* rgb);
extern void cvxColorToColorf(const svxColor* rgb, svxColorf* rgbf);
extern int cvxColorRGB(evxColor color, svxColor* rgb);
extern evxColor cvxColorNear(const svxColor* rgb);

/*| escape checking
*/
extern int cvxEscCheck(void);
extern void cvxEscDisable(int *EscapeState);
extern void cvxEscRestore(int EscapeState);
extern void cvxEscStart(void);
extern void cvxEscEnd(void);

/*| GUI framework
*/
extern int cvxGetViewRect(int includeFrame, int isGlobal, int* tlX, int* tlY, int* brX, int* brY);

/*
*********************************************************************
*! 4. File Management
*********************************************************************
*/

/* file: VxApiFile.c */
/*| file management
*/
extern int cvxFileActivate(vxPath Name);
extern void cvxFileClose(void);
extern void cvxFileClose2(vxPath Name, int Option);
extern void cvxFileDirectory(vxPath Directory);
extern int cvxFileExport(evxExportType Type, vxPath Path, void* Data);
extern void cvxFileInqActive(char* Name, int nBytes);
extern void cvxFileInqOpen(char* Name, int nBytes);
extern int cvxFileNew(vxPath Name);
extern int cvxFileNew2(vxPath Name, char *Description);
extern int cvxFileOpen(vxPath File);
extern int cvxFileSave(int Close);
extern int cvxFileSave2(int Close);
extern int cvxFileSave3(int Close, int DisPrompt, int Related);
extern int cvxFileSaveAll(void);
extern int cvxFilePreviewExtract(vxPath SrcFile, vxPath BmpFile);
extern int cvxFileSaveAs(vxPath File);
extern void cvxFileDescriptionSet(vxPath Name, char *Description);
extern int cvxFileDescriptionGet(vxPath Name, vxLongName Description);
extern void cvxFileTypeGet(int *MultiObject);
extern void cvxFileTypeSet(int MultiObject);
extern int cvxFileCustomDataSet(vxPath file, char* data, int dataLength);
extern int cvxFileCustomDataGet(vxPath file, char** data, int* dataLength);
extern int cvxFileCustomDataDel(vxPath file);
extern int cvxFileImportStl(const char *path, int *idOut);
extern int cvxFileVersionGet(vxPath file, int* version);
extern void cvxFileOpenList(int *count, vxLongName **fileList);
extern int cvxFileIsModified(vxPath Name, int* Modified);
extern int cvxFileInqAssociatedList(int *count, vxPath ** fileList);
extern int cvxFileImport(svxImportData *importData);
extern int cvxFileInqAsm(char* Name, int nBytes);
extern char cvxFileInqAssemMode(void);

/*| session management
*/
extern int cvxSessionClear(void);
extern int cvxSessionRestore(void);
extern int cvxSessionSave(void);

/* file: VxApiPath.c */
/*| path management
*/
extern int cvxPathAdd(vxPath SearchDirectory);
extern int cvxPathAddApiPath(vxName LibName, char *SubFolder);
extern int cvxPathApiLib(vxName LibName, vxPath LibDirectory);
extern int cvxPathCompose(vxPath Path, const char *Name);
extern void cvxPathDel(vxPath Path);
extern void cvxPathDelAll(void);
extern void cvxPathDelim(vxPath Path);
extern void cvxPathDir(vxPath FullPath, vxPath Directory);
extern void cvxPathFile(vxPath FullPath, vxLongName FileName);
extern void cvxPathFile2(char *FullPath, int MaxNameLen, char *FileName);
extern int cvxPathFind(vxName FileName, vxPath FullPath);
extern void cvxPathInstall(vxPath InstallDirectory);
extern void cvxPathSearchFirst(vxPath Directory);
extern void cvxPathTemp(vxPath Directory);
extern void cvxPathUserFolder(vxPath Directory);
extern int cvxPathIsDebug(const char *FolderName);
extern int cvxPathSet(vxPath path);
extern int cvxPathCheckActiveOnlyGet();
extern int cvxPathSearchGet();
extern void cvxPathCheckActiveOnlySet(int Value);
extern void cvxPathSearchSet(int Value);

/*
*********************************************************************
*! 5. Root Object Management
*********************************************************************
*/

/* file: VxApiRoot.c */
/*| root object management
*/
extern int cvxRootActivate(vxLongName Name);
extern int cvxRootActivate2(vxLongName File, vxLongName Name);
extern int cvxRootAdd(evxRootType Type, vxLongName Name,const char *Template);
extern int cvxRootAdd2(evxRootType, vxLongName Name, const char *Template, char *Desc);
extern int cvxRootAdd2Bg(evxRootType, vxLongName Name);
extern int cvxRootCopy(vxLongName,vxLongName,vxLongName,vxLongName,int Overwrite,int UpdateUid);
extern int cvxRootDel(vxLongName Name);
extern void cvxRootExit(void);
extern int cvxRootId(vxLongName Name, int *idRoot, evxRootType *Type);
extern void cvxRootInqActive(char *Name, int nBytes);
extern int cvxRootInqAsm(char *Name, int nBytes);
extern int cvxRootInqMod(int *Mod);
extern int cvxRootInqModSinceRegen(int *Mod);
extern int cvxRootList(vxLongName File, int *Count, vxLongName **Names);
extern evxPreviewMode cvxRootPreviewGet(void);
extern void cvxRootPreviewSet(evxPreviewMode Mode, vxLongName File, vxLongName Object);
extern int cvxRootRename(vxLongName Name,vxLongName NewName,int Update);
extern int cvxRootRename2(vxLongName Name,vxLongName NewName,char *Descript,int Update);
extern void cvxRootTemplateFile(vxLongName File);
extern int cvxRootVarGet(vxLongName Name, svxVariable *Variable);
extern int cvxRootVarSet(vxLongName Name,int Count,svxVariable*Var,int Working);
extern void cvxRootVisibility(vxLongName Name, int Visible);
extern int cvxRootCustomDataSet(vxLongName file, vxLongName objectName, char* data, int dataLength);
extern int cvxRootCustomDataGet(vxLongName file, vxLongName objectName, char** data, int* dataLength);
extern int cvxRootCustomDataDel(vxLongName file, vxLongName objectName);
extern int cvxRootInqSymbol(vxLongName file, int *iCount, vxLongName**names);
extern int cvxRootIsAsm(vxLongName File, vxLongName Part, int *isAsm);


/*
*********************************************************************
*! 6. Active Part/Assembly
*********************************************************************
*/

/* file: VxApiPart.c */
/*| datum plane creation
*/
extern int cvxPartDatum(svxMatrix *Frame, int *idDatum);
extern int cvxPartDatum2(svxMatrix *Frame, int iCode, int *idDatum);
extern int cvxPartNewDatum(svxMatrix *Frame, int *idDatum);
extern int cvxPartPlnCrv(int idCrv,svxPoint*Origin,svxPoint*Xaxis, int *idPln);
extern int cvxPartPlnCrv2(int idCrv,svxData*Origin,svxData*Xaxis,int Flip,int *idPln);
extern int cvxPartPlnSrf(int idFace, svxPoint*Origin, int *idPln);
extern int cvxPartPlnSrf2(int idFace,svxPoint*Origin,svxPoint*XZ,int Flip,int*idPln);

/*| point/curve creation
*/
extern int cvxPartArc3pt(svxPoint*Start,svxPoint*End,svxPoint*Mid,int*idEnt);
extern int cvxPartArcRad(svxMatrix *Pln,double R,double S,double E,int *idEnt);
extern int cvxPartCir3pt(svxPoint *P1, svxPoint *P2, svxPoint *P3 ,int *idEnt);
extern int cvxPartCirRad(svxMatrix *Plane, double Radius, int *idEnt);
extern int cvxPartCrvInt(svxCrvIntData *Crv, int *idEnt);
extern int cvxPartCrvList(int Count, int *Curves, int *idEnt);
extern int cvxPartCurve(svxCurve *Crv, int *idEnt);
extern int cvxPartLine2pt(svxPoint *Start, svxPoint *End, int *idEnt);
extern int cvxPartPnt(svxPoint *Point, int *idEnt);
extern int cvxPartPntBlock(vxLongName, int Count, svxPointf *Points, int *idEnt);
extern int cvxPartPnts(int Count, svxPoint *Points);
extern int cvxPartPntsOnCrv(int idCrv, svxPoint*, int Num, double Dist, evxExtendOpt, int *Count, int **idEnts);
extern int cvxPartEllipse(svxRectangleData *ellipseData, int *idEnt);
extern int cvxPartRectangle(svxRectangleData *rectData, int *iCnt, int **idEnt);
extern int cvxPartProjStl(int idStl, int idPln, double tol, double smth, int *idCurve);
extern int cvxPartCurveSilh(int idCurve, svxPoint *dir, int *iCnt, int **idList);
extern int cvxPartFittingCrv(int iCnt, svxPoint *pnts, double tol, double smth, int *idEnt);

/*| curve edit
*/
extern int cvxPartCrvSplitAtPnts(int idCurve, int CountPnts, svxPoint *Points, int *CountCrvs, int **NewCrvs);

/*| sketch creation
*/
extern int cvxPartSkIns(vxLongName File,vxLongName Name,svxMatrix*Plane,int*idSketch);
extern int cvxPartSkIns2(vxLongName File,vxLongName Name,int idPlane,int iAsso,int*idSketch);
extern int cvxPartSkNew(svxMatrix *Plane, int *idEnt);

/*| shape creation
*/
extern int cvxPartBox(svxBoxData *Box, int *idShape);
extern int cvxPartCyl(svxCylData *Cyl, int *idShape);
extern int cvxPartExtrude(svxExtrudeData *Ext, int *idShape);
extern int cvxPartFace(svxSurface*, int NumCurves, svxCurve *TrimCurves, int Code, int Sew, double Tol, int *idFace);
extern int cvxPartRefGeom(svxEntPath*, int Associative, int *idGeom);
extern int cvxPartRevolve(svxRevolveData *Rev, int *idShape);
extern int cvxPartSweep(svxSweepData *Swp, void *Options, int *idShape);
extern int cvxPartIns(svxPartInstData *Part, int *Count, int **ShapeList);

/*| shape intersection check
*/
extern int cvxPartIsectShpChk(int idBaseShape, int idOpShape);
extern int cvxPartIsectShpChk2(int idBaseShape, int idOpShape, int fClosedChk);

/*| face edit
*/
extern int cvxPartSetFaceAt(int Count, int *idFaces, const svxFaceAt *At);
extern int cvxPartSetFaceTx(int idFace, const svxTextureAt *Tx);

/*| facet block creation
*/
extern int cvxPartFacets(vxLongName Name, svxFacets *Facets, int *idBlock);

/*| facet block edit
*/
extern int cvxPartFacetsMod(int idBlock, svxFacets *Facets);
extern int cvxPartFacetsSetAt(int idBlock, const svxFaceAt *At);
extern int cvxPartFacetsSetTx(int idBlock, const svxTextureAt *Tx);

/*| feature creation
*/
extern int cvxPartChamAng(int Cnt,int*Edges,int idFace,double Sb,double Angle);
extern int cvxPartChamAsym(int Cnt,int*Edges,int idFace,double Sb1,double Sb2);
extern int cvxPartChamConst(int Count, int *Edges, double Sb);
extern int cvxPartFillet(int Count, int *Edges, double Radius);
extern int cvxPartHole(svxHoleData *Data, int *idOp);
extern int cvxPartThreadAt(int idFace, svxPoint *Pnt, svxThreadData *Thread);
extern int cvxPartFaceOffset(int idFace, double Dis, int Keep);
extern int cvxPartCurveOffset(int idCurve, double Dis, int *idOut);

/*| assembly component
*/
extern int cvxCompExtract(int idShape,svxMatrix*,vxLongName,int NewFile,evxResponse,int*idComp);
extern int cvxCompEditPart(int idComp);
extern int cvxCompFind(evxFaceTrim,svxAxis*,double Dist,int SkipBlank,int*Count,int**Comps);
extern int cvxCompInqPart(int idComp, char *File, int nFileBytes, char *Part, int nPartBytes);
extern int cvxCompIns(svxCompData *Comp, int *idComp);
extern int cvxLibPartIns(const svxCompData *Comp, const char *InstanceName, const char *ValStr, int fFileType, int fInsShape, int *idComp);
extern int cvxLibPartAdjust(int idComp, const vxLongName File, const vxLongName Part, const char *ValStr, const char *InstanceName);
extern int cvxCompIsectShpChk(svxEntPath *BaseShpPath, svxEntPath *OpShpPath, int *fInterfer);
extern int cvxCompIsectShpChk2(svxEntPath *BaseShpPath, svxEntPath *OpShpPath, int fClosedChk, int *fInterfer);
extern int cvxCompMerge(evxBoolType Combine,int idComp,int *idShape);
extern int cvxCompMerge2(evxBoolType Combine,int idComp,int MergeDim, int MergeWire, int *idShape);
extern int cvxCompRegenSet(int idComp, int Regen);
extern int cvxCompSetPart(int idComp, vxLongName File, vxLongName Part);
extern int cvxCompShift(int idComp);
extern int cvxCompSuppressGet(int idComp, int *suppress);
extern int cvxCompSuppress(int idComp, int suppress);
extern int cvxAssemCompVisAll(int *nSize, int ** ppath_mode);
extern int cvxAsmUpdateCompRef(const char *, const char *);
extern int cvxCompIferCheck(svxEntPath *BaseComp, svxEntPath *ChkComp, int SubAsmAsWhole, int IgnoreHidden, int *IferCnt, svxIferChkResData **IferResults);
extern int cvxCompInqPaths(svxEntPath *CompPath, int Depth, int IgnoreHidden, int *PathCnt, svxEntPath **lstPath);

/*| sub-part
*/
extern int cvxSubPartIns(vxLongName Name, int AutoRegen, int MergeDimen);
extern int cvxSubPartFlag(vxLongName File, vxLongName Part, int isSubPart);

/*| variables
*/
extern int cvxPartEqnSet(int Count, svxVariable *Variables);
extern int cvxPartVarAdd(svxVariable *Variable);
extern int cvxPartVarGet(svxVariable *Variable);
extern int cvxPartVarSet(int Count, svxVariable *Variables, int Working);
extern int cvxPartVarSubstitute(char *Str, int nBytes, int *Truncated);
extern int cvxPartVarSubstituteResize(char **Str);
extern int cvxEqGetExpByName(vxName name, char *expression, int expressionsize);


/*| display
*/
extern int cvxPartViewAdd(svxView *View, int *idView);
extern void cvxPartShowAll(void);
extern void cvxPartShowTarg(void);

/*| general edit
*/
extern int cvxPartBool(evxBoolType,int idBase,int Count,int *Shapes,int Keep);
extern int cvxPartBool2(evxBoolType, int, int*, int, int*, int*, int**);
extern int cvxPartBool3(evxBoolType, int, int*, int, int*, int, int*, int*, int**);
extern int cvxPartCopyDirDis(int idEntity,svxVector*Dir,double Dis,int*idCopy);
extern int cvxPartCopyPntToPnt(int idEntity,svxAxis*S,svxAxis*E,int*idCopy);
extern int cvxPartCopyTransform(int idEntity,const svxMatrix *Mat,int *idCopy);
extern int cvxPartCopyTransformList(const int Count, const int* Entities, const svxMatrix *Mat, int* EntCnt, int **idList);
extern int cvxPartErase(int Count, int *idEnts);
extern int cvxPartMovePntToPnt(int idEntity,svxAxis*S,svxAxis*E);
extern int cvxPartMoveTransform(int idEntity,const svxMatrix *Mat);
extern int cvxPartMoveTransformList(int Count,const int *Entities,const svxMatrix *Mat);
extern int cvxPartNameLastOp(vxName NewName);
extern int cvxPartScale(int Count,int*idEnts,double X,double Y,double Z,int idDatum);
extern int cvxPartScale2(int Count,int*idEnts,double X,double Y,double Z,int idDatum,svxPoint *CentPnt);
extern int cvxPartSew(double Tol, int *OpenEdges, double *MaxGap);
extern int cvxPartSewForce(double Tol);

/*| query
*/
extern int cvxPartInqActiveCfg(int *idCfg);
extern int cvxPartInqBlocks(int *Count, int **Blocks);
extern int cvxPartInqBlockPnts(int idBlock, int *Count, svxPointf **Points);
extern int cvxPartInqBlockType(int idBlock, evxBlockType *Type);
extern int cvxPartInqCfgList(int *Count, int **CfgList);
extern int cvxPartInqComps(vxLongName File, vxLongName Part, int *Count, int **Comps);
extern int cvxPartInqComps2(vxLongName File, vxLongName Part, int Option, int *Count, int **Comps);
extern int cvxPartInqCompsInfo(vxPath Path, vxLongName Name, int* Count, vxPath** Paths, vxLongName** Names);
extern int cvxPartInqCrvList(int idCrvList, int *Count, svxEntPick **Curves);
extern int cvxPartInqCurve(int idEntity, int Nurb, svxCurve *Crv);
extern int cvxPartInqCurves(int *Count, int **Curves);
extern int cvxPartInqCrvLists(int *Count, int **CrvLists);
extern int cvxPartInqDims(vxLongName File, vxLongName Part, int *Count, svxVariable**);
extern int cvxPartInqAnnoTexts(int *Count, int **Texts);
extern int cvxPartInqFtrEnts(int idFeature, evxEntType EntType, int *Count, int **Ents);
extern int cvxPartInqFtrRegenStat(int idFeature, char *stat);
extern int cvxPartInqEdgeCrv(int idEdge, int idFace, svxCurve *Crv);
extern int cvxPartInqEdgeFaces(int idEdge,int *Count,int **Faces);
extern int cvxPartInqEdgePreEdges(int idEdge, int *Count, int **PreEdges);
extern int cvxPartInqEntByLabel(int *lbl, int flag, int *entId);
extern int cvxPartInqEntFtr(int idEntity, int *idFeature);
extern int cvxPartInqEntLabel(int ent, int **lbl);
extern int cvxPartInqEntShape(int idEntity, int *idShape);
extern int cvxPartInqFaceAt(int idFace, svxFaceAt *At);
extern int cvxPartInqFaceSh(int idFace, svxShaderAt *Sh);
extern int cvxPartInqFaceTx(int idFace, svxTextureAt *Tx);
extern int cvxPartInqFaceTxFile(int idFace, vxPath Path);
extern int cvxPartInqFaceBox(int idFace, svxBndBox *Box);
extern int cvxPartInqFaceCrvs(int idFace,int *Count,svxCurve **TrimCurves);
extern int cvxPartInqFaceEdges(int idFace,int *Count,int **Edges);
extern int cvxPartInqFaceFacets(int idFace, int *Count, svxTriangle **Facets);
extern int cvxPartInqFaceFacets2(int idFace, svxFacets *Facets);
extern int cvxPartInqFaceLoops(int idFace,int Inner, int *Count,int **Loops);
extern int cvxPartInqFaceSrf(int idFace, svxSurface *Srf);
extern int cvxPartInqFaceShape(int idFace, int *idShape);
extern int cvxPartInqFacets(int idBlock, svxSTL *zStl);
extern int cvxPartInqFacets2(int idBlock, svxFacets *Facets);
extern int cvxPartInqFacetsTxFile(int idBlock, vxPath Path);
extern int cvxPartInqFtrData(int idFeature, int iNoEval, int *idData);
extern int cvxPartInqFtrList(int *Count, int **Features);
extern int cvxPartInqFtrTemplate(int idFeature, vxName Template);
extern int cvxPartInqFtrVersion(int idFeature, int *Version);
extern int cvxPartInqHoles(vxLongName File, vxLongName Part, int *Count, svxHoleData**);
extern int cvxPartInqLoopEdges(int idFace,int *Count,int **Edges);
extern int cvxPartInqLoopFace(int idLoop, int *idFace);
extern int cvxPartInqLoopPreEdges(int idLoop, int *Count, int **PreEdges);
extern int cvxPartInqPoints(int *count, int **pointIdx);
extern int cvxPartInqPreEdgeInfo(int idPreEdge, svxPreEdge *PreEdge);
extern int cvxPartInqPreEdgeStartPnt(int idPreEdge, int *idEdge, svxPoint2 *StartPoint);
extern int cvxPartInqShapeBox(int idShape, svxMatrix *Mat, svxBndBox *Box);
extern int cvxPartInqShapeComp(int idShape, int *idComp);
extern int cvxPartInqShapeEdges(int idShape,int *Count,int **Edges);
extern int cvxPartInqShapeFaces(int idShape,int *Count,int **Faces);
extern int cvxPartInqShapeFtrs(int idShape, int Option, int *Count, int **Features);
extern int cvxPartInqShapeMass(int idShape, double Density, svxMassProp*);
extern int cvxPartInqShapes(vxLongName File, vxLongName Part, int *Count, int **Shapes);
extern int cvxPartInqVars(vxLongName File, vxLongName Part, int *Count, svxVariable**);
extern int cvxPartInqView(int idView, svxView *View);
extern int cvxPartInqViews(int *Count, int **Views);
extern int cvxPartInqTagNames(int *Count,vxLongName** Names);
extern int cvxPartDelGrpFtr(int idFtr, int delFtrs);
extern int cvxPartFtrPlayTo(int idxFtrPlayTo, int state);

/*| history
*/
extern int cvxHistBackup(void);
extern void cvxHistDisable(void);
extern void cvxHistEnable(void);
extern int cvxHistRedefine(vxName Operation);
extern int cvxHistSkipTo(char *Expression, vxName Operation, int *idOp);
extern int cvxPartHistClip(int Cut, int Count, int *Operations);
extern int cvxPartHistDel(int Count, int *Operations);
extern int cvxPartHistDelUtil(int Count, int *Operations, int OpType);
extern int cvxPartHistEnd(void);
extern int cvxPartHistGroup(int idFirst, int idLast, vxName Name, int Close);
extern int cvxPartHistPaste(void);
extern int cvxPartHistRename(int idOp, vxName Name, char *Descript);
extern int cvxPartHistStart(vxName Name);
extern int cvxPartHistList(int Type, int *Cnts, int **HistList);

/*| other
*/
extern int cvxPartAlign(svxAlign *Align);
extern int cvxPartAnchor(int idEntity);
extern int cvxPartAtGet(vxLongName Name, svxPartAttribute *At);
extern int cvxPartAtMod(vxLongName Name, svxPartAttribute *At);
extern int cvxPartAtSet(vxLongName Name, svxPartAttribute *At);
extern int cvxPartBackup(void);
extern int cvxPartEncapsulate(int Option, int KeepDimen);
extern void cvxPartExit(void);
extern void cvxPartFreeHoles(int Count, svxHoleData **Holes);
extern int cvxPartGeomExport(svxGeomExport *Data);
extern int cvxPartMassProp(void);
extern void cvxPartPartialResults(int Disable);
extern int cvxPartRedefineIsActive(void);
extern int cvxPartRegenFacets(int Count, int *idFaces, int Assembly, int LOD, double Tol);
extern int cvxPartRegenIsActive(void);
extern int cvxPartVolumAndMassGet(vxLongName Name, double *dVolume, double *dMass );
extern int cvxPartCfgActivate(int idCfg);
extern int cvxPartPhyAttrCalc(vxLongName Name);
extern int cvxPartAtItemGet(evxAtItemId itemId, svxAttribute *At);
extern int cvxPartAtItemSet(evxAtItemId itemId, svxAttribute *At);
extern int cvxPartShapeStructGet(int idShape, svxWeldAttr *at);
extern int cvxPartSetCustomStr(int idComp, const char *CustomString, int *idStr);
extern int cvxPartGetCustomStr(int idComp, int *idStr, char **CustomString);
extern int cvxShellAtGet(int, svxPartAttribute *At);
extern int cvxShellAtItemGet(int, evxAtItemId itemId, svxAttribute *At);
extern int cvxShellAtItemSet(int, evxAtItemId itemId, svxAttribute *At);
extern int cvxInstPartAsShp(const svxCompData *);
extern int cvxInstShpGetMatrix(int idShape, svxMatrix *Matrix);

/*| dimension
*/
extern int cvxDimInqVar(int idDim, svxVariable* Var);
extern int cvxDimValMod(int idDim, char* exp);
extern int cvxDimValModNoUpdate(int idDim, char* exp);
extern int cvxDimModTxt(int idDim, int subDim, char *pStr);
extern int cvxDimInqText(int idDim, int subDim, char **pStr);
extern int cvxPartAddLineDim(int idEnt1, int idEnt2, svxPoint *TextPos, int idPlane, int *idDim);
extern int cvxPartAddCoorDim(svxPoint *LeaderPnt, int idParent, svxPoint *TextPnt, int idDatum, int idPlane, int *idDim);
extern int cvxDimInqPos(int idDim, svxPoint *pPos);
extern int cvxDimInqType(int idDim, evxDimType *type);

/*
*********************************************************************
*! 7. Active Sketch or Sheet
*********************************************************************
*/

/* file: VxApiSketch.c */
extern int cvxSkActivate(int idSketch, int UpdateGUI);
extern int cvxSkArc3pt(svxPoint2*Start,svxPoint2*End,svxPoint2*Mid,int*idEnt);
extern int cvxSkArcRad(svxPoint2*Center,double R,double S,double E,int*idEnt);
extern int cvxSkPntsOnCrv(int idCrv, svxPoint*, int Num, double Dist, evxExtendOpt, int *Count, int **idEnts);
extern int cvxSkCir3pt(svxPoint2 *P1,svxPoint2 *P2,svxPoint2 *P3,int *idEnt);
extern int cvxSkCirRad(svxPoint2 *Center,double Radius,int *idEnt);
extern int cvxSkCrvInt(svxCrvIntData *Crv, int *idEnt);
extern int cvxSkLine2pt(svxPoint2 *Start, svxPoint2 *End, int *idEnt);
extern int cvxSkScale(int Count, int*idEnts, double X, double Y, svxPoint2*Pnt);
extern int cvxSkInqCns(int *Count, int **idEnts);
extern int cvxSkInqDim(int *Count, int **idEnts);
extern int cvxSkInqGeom(int *Count, int **idEnts);
extern int cvxSkInqGeomX(int *Count, int **idEnts);
extern int cvxSkInqPnt(int *Count, int **idEnts);
extern int cvxSkInqRef(int *Count, int **idEnts);
extern int cvxSkInqText(int *Count, int **idEnts);
extern int cvxSkInqArea(int nCrv, int *idCrvs, svxPoint2 *innerPnt, svxPrfAreaProp *aboutCM, svxPrfAreaProp *aboutRf);
extern int cvxShtInqSymbol(evxSymbolType type, int *Count, int **idEnts);
extern int cvxSkInqDimRefFlag(int idDim,int* refFlag);
extern int cvxSkInqCrvConnect(int iCount, int *idCrvs, int *iUnMatch, int *iOverMatch);
extern int cvxSkEntWireType(int entId, int *type);
extern int cvxSkAddConsDim(evxConsDimType type, svxConsDim* pData);
extern int cvxSkAddPolyline(int cntPnt, svxPoint2* pnts, int addCns);

/*| variables
*/
extern int cvxSkVarAdd(svxVariable *Variable);
extern int cvxSkVarGet(svxVariable *Variable);
extern int cvxSkVarSet(int Count, svxVariable *Variables);
extern int cvxSkInqVars(int *Count, svxVariable **Variables);

/*| sketch blocks
*/
extern int cvxSkInqBlockReferences(int idSketch, int *Count, int **idEnts);
extern int cvxSkInqSketchBlocks(int *Count, int **idEnts, vxLongName **Names);
extern int cvxSkBlockActivate(int idBlock, int UpdateGUI);
extern int cvxSkBlockGetSketch(int idBlock, int *idSketch);
extern int cvxSkBlockRefGetDefinition(int idRef, int *idBlock);
extern int cvxSkBlockInqGeom(vxLongName blockName, int *Count, int **idEnts);
extern int cvxSkBlockInqPnt(vxLongName blockName, int *Count, int **idEnts);
extern int cvxSkBlockInqDim(vxLongName blockName, int *Count, int **idEnts);
extern int cvxSkBlockInqGeomX(vxLongName blockName, int *Count, int **idEnts);
extern int cvxSkBlockInqRefs(int idBlock, int*count, int**idEnts);

/*
*********************************************************************
*! 8. General Entity Operations
*********************************************************************
*/

/* file: VxApiEnt.c */
extern int cvxEntActivate(svxEntPath*NewPath, int UpdateGUI, int*idEntity, svxEntPath*OldPath);
extern int cvxEntBlank(int Show, int Count, int *idEnts);
extern int cvxEntBndBox(int idEntity, svxBndBox *Box);
extern int cvxEntByLabel(int *Label, int Exact, int *idEntity);
extern int cvxEntByName(const char *Name, int *idEntity);
extern int cvxEntByName2(vxLongName File, vxLongName Root, vxName Name, int *idEntity);
extern int cvxEntByUid(const svxUid *Uid, int *idEntity);
extern void cvxEntClassName(int ClassNumber, char *ClassName);
extern int cvxEntClassNum(int idEntity);
extern int cvxEntColorGet(int idEntity, evxColor *Color);
extern int cvxEntColorSet(evxColor Color, int Count, int *idEnts);
extern int cvxEntComp(int idEntity, int *idComp, char *File, int nFileBytes, char *Parent, int nParentBytes);
extern int cvxEntEndPnt(int idEntity, svxPoint *Start, svxPoint *End);
extern int cvxEntErase(svxEntPath *Ent);
extern int cvxEntExists(int idEntity, evxEntType EntType);
extern int cvxEntFind(evxEntFind,svxPoint*RefPnt,int*idEntity,svxPoint*Pnt,double *Dist);
extern int cvxEntFind2(svxEntFind *Data);
extern void cvxEntHighlight(int idEntity);
extern void cvxEntUnHighlight(int idEntity);
extern int cvxEntIsBlanked(int idEntity);
extern int cvxEntIsCurve(int idEntity);
extern int cvxEntIsElectrode(int idEntity);
extern int cvxEntLabelGet(int idEntity, int **Label);
extern int cvxEntLabelSet(int idEntity, int *Label);
extern int cvxEntLayer(int idEntity, vxName Layer);
extern int cvxEntLayerId(int idEntity, int* LayerId);
extern int cvxEntMatrix(int idEntity, svxMatrix *Matrix);
extern int cvxEntMatrix2(svxEntPath Entity, svxMatrix *Matrix);
extern int cvxEntMatrixSet(int idEntity, svxMatrix *Matrix);
extern int cvxEntTextInq(int idText, char **Text);
extern int cvxEntTextMod(int idText, char *Text);
extern int cvxEntTextNew(char* String, int Type, svxPoint2 Pnt[2], int*idEnt);
extern int cvxEntTextAtGet(int idText, svxTextAt*At);
extern int cvxEntTextAtSet(int idText, svxTextAt*At);
extern int cvxEntTextLocGet(int idText, svxTextLoc *loc);
extern int cvxEntPnt(int idEntity, svxPoint *Pnt);
extern int cvxEntParent(int idEntity, int *idParent);
extern int cvxEntPathToRoot(svxEntPath *EntPath);
extern int cvxEntName(int idEntity, char *Name, int nBytes);
extern int cvxEntNameSet(int idEntity, const char *Name);
extern int cvxEntNew(int StartOp, evxEntType Type);
extern void cvxEntNewList(int StartOp,evxEntType Type,int *Count,int **idEnts);
extern int cvxEntRefEnt(int idRefEnt, int *idEnt);
extern int cvxEntRgbGet(int idEntity, svxColor *Color);
extern int cvxEntRgbSet(svxColor Color, int Count, int *idEnts);
extern int cvxEntRgbSetNoAt(svxColor Color, int Count, int *idEnts);
extern int cvxEntTransSet(int iTrans, int Count, int *idEnts);
extern void cvxEntSketch(int idEntity, int *idSketch);
extern int cvxEntUidGet(int idEntity, svxUid* Uid);
extern int cvxEntUidSet(int idEntity, const svxUid* Uid);
extern void cvxEntUnhighlightAll(void);
extern int cvxEntInqSelected(int *Count, int **Ents);
extern int cvxEntNameByPath(const svxEntPath *Path, vxLongName Name);
extern int cvxEntGetInhSubIdx(int idx_ent, int klass, int sub_class, int *subIdx);
extern int cvxEntSetNameTag(int idx_ent, char *name);
extern int cvxEntAddPick(int idx_ent);
extern int cvxEntLineAtGet(int idLine, svxLineAt* At);
extern int cvxEntLineAtSet(svxLineAt* At, int Count, int* idEnts);
extern void cvxEntUnPickAll();
extern void cvxEntUnPickLast();
extern int cvxEntGetDistance(int ent1, int ent2, svxPoint *p1, svxPoint *p2, double *dist);

/*
*********************************************************************
*! 9. General 2D Sheet Operations
*********************************************************************
*/

/* file: VxApiDrawing.cpp */
extern int cvxDwgInqList(int idRoot, int *count, int **idDrawings);
extern int cvxDwgActivate(vxLongName drawingName);
extern int cvxDwgInqActive(char *drawingName, int nBytes);
extern int cvxDwgInqName(int idDrawing, char *drawingName, int nBytes);
extern int cvxDwgFind(vxLongName rootName, vxLongName drawingName, int *idDrawing);
extern int cvxDwgInqDims(int idDrawing, evxDimType *dimTypeList, int dimTypeCount, int *count, int **dims);
extern int cvxDwgInqTables(int idDrawing, evxTableType type, int *count, int **tables );
extern int cvxDwgInqViews(int idDrawing, evxViewType type, int *count, int **views);
extern int cvxDwgViewInqPart(int idView, svxCompName *part);
extern int cvxDwgInqPart(char* rootName, int *iCount, svxCompName **partList);
extern int cvxDwgAtGet(int idDrawing, svxDrawingAt *drawingAt);
extern int cvxDwgAtSet(int idDrawing, svxDrawingAt *drawingAt);
extern int cvxDwgTableExport(int idTable, vxPath path);
extern int cvxDwgTableLinkToExcel(int idTable, vxPath path);
extern int cvxDwgInqGeom(int idDrawing, int *count, int **idEnts);
extern int cvxDwgSymImgNew(svxSymImg*data, int embed, int locked, int *idSymImg);
extern int cvxDwgSymImgAtSet(int idSymImg, svxSymImg*data);
extern int cvxDwgSymImgAtGet(int idSymImg, svxSymImg*data);
extern int cvxDwgInqText(int idDrawing, int *count, int **idEnts);
extern int cvxDwgAtItemGet(evxAtItemId itemId, svxAttribute *At);
extern int cvxDwgTableCellSet(int idTable, int Row, int Col, char* Value);
extern int cvxDwgTableCellGet(int idTable, int Row, int Col, char* Value, int nBytes);
extern int cvxDwgTableAdjustSize(int idTable);
extern int cvxDwgDimAtGet(int idDim, svxDimAttr *dmAttr);
extern int cvxDwgDimAtSet(int idDim, svxDimAttr *dmAttr);
extern int cvxDwgInsertSymbol(vxLongName fileName, vxLongName symName, svxPoint2 *pnt, int*symId);
extern int cvxDwgViewInqOrgPnt(int idView, svxPoint2*pnt, int *idEnt);
extern int cvxDwgViewRotate(int idView, double angle);
extern int cvxDwgViewGeomInqList(int viewId, int option, int *count, int **indices);
extern int cvxDwgViewGeomGetShapeId(int geomId, char *fileName, int fileNameBytes, char *partName,
   int partNameBytes, svxEntPath *pickPath, int *geom3DId, int *shapeId);
extern int cvxDwgViewPnt2DTo3D(int pnt2DId, svxPoint *pnt3D);
extern int cvxDwgViewPnt3DTo2D(int viewId, svxPoint pnt3D, svxPoint2 *pnt2D);
extern int cvxDwgGetDimTextBox(int idDim, double *tbWid, double *tbHgt, double *chSlp, svxPoint2 *txtPt);
extern int cvxDwgSymSurfDataGet(int idSymSurf, svxSymSurf*data);
extern int cvxDwgSymSurfDataSet(int idSymSurf, svxSymSurf*data);



/*
*********************************************************************
*! 10. General Application
*********************************************************************
*/

/* file: VxApiAppl.c */
extern void cvxAutoRegenGet(int *Status);
extern void cvxAutoRegenSet(int Status);
extern void cvxConfigGetColor(const char *Name, evxColor *Color);
extern int cvxConfigGetInteger(const char *Name, int *Value);
extern int cvxConfigGetNumber(const char *Name, double *Value); 
extern int cvxConfigGetOption(const char *Name, const char *Options);
extern int cvxConfigGetRgb(const char *Name, svxColor *Rgb);
extern int cvxConfigGetString(const char *Name, char *Str, int nBytes);
extern int cvxConfigGetYesNo(const char *Name, int *YesNo);
extern int cvxConfigSetColor(const char *Name, evxColor Color);
extern int cvxConfigSetInteger(const char *Name, int Value);
extern int cvxConfigSetNumber(const char *Name, double Value);
extern int cvxConfigSetOption(const char *Name, const char *Option);
extern int cvxConfigSetRgb(const char *Name, svxColor *Rgb);
extern int cvxConfigSetString(const char *Name, char *Str);
extern int cvxConfigSetYesNo(const char *Name, int YesNo);
extern int cvxGlobalGet(evxGlobal, void *Data);
extern int cvxGlobalSet(evxGlobal, void *Data);
extern int cvxLabelMatch(int *Label1, int *Label2);
extern void cvxLangGet(vxName Language);
extern int cvxOpCount(void);
extern int cvxNameIsOkay(char* name, int size, int option);
extern void cvxNewCommand(void);
extern int cvxNoteGet(char **Note);
extern int cvxNoteSet(char *Note);
extern void cvxHostId(double *idNetwork, double *idDongle);
extern void cvxShowDisp(int SlideNumber);
extern void cvxShowOpen(vxPath File);
extern void cvxUndoBundleEnd(int RefreshDisplay, int Settings[], int isError);
extern void cvxUndoBundleStart(int DisableManager, int Settings[]);
extern int cvxUndoRedo(int Undo);
extern int cvxUndoRedoTo(int Undo, vxName Name);
extern int cvxUndoRedoMarker(vxName Name);
extern int cvxXnUndoCount();
extern void cvxXnBundle(int iCount, const char* xn_name);
extern void cvxFilterListGet(int *count, vxName *filterList);
extern void cvxUnitGet(evxUnitType *Type, vxName Name);
extern void cvxUnitTypeGet(char*,char*,char*);
extern void cvxUnitSet(evxUnitType Type);
extern void cvxUnitToSys(double *Distance);
extern void cvxUnitToUser(double *Distance);
extern int cvxVersion(char *vxStatus);
extern int cvxLicModulesGet(char szModules[1024]);
extern int cvxLicIdentifierGet(char szIdentifier[36]);
extern int cvxLicEvaluationGet(char szEvaluation[36]);
extern int cvxLicActivate(const char *pzEntitlement);
extern void cvxSetMSMMoldInstalled(int IsInstalled);
extern void cvxSetMSMPressInstalled(int IsInstalled);
extern int cvxConfigPathGet(vxPath path);
extern int cvxPluginRegPathGet(vxPath **pathList, int *pCnt, int is64);
extern void cvxColorFilter(int color);

/*
*********************************************************************
*! 11. Math and Geometry
*********************************************************************
*/

/* file: VxApiMath.c */
/*| transformation matrix
*/
extern int cvxMatEigen(const double Mat3x3[3][3], double *Values, svxVector *Vectors);
extern void cvxMatInit(svxMatrix *Mat);
extern void cvxMatInvert(svxMatrix *Mat, svxMatrix *InvMat);
extern void cvxMatMult(svxMatrix *Mat1, svxMatrix *Mat2, svxMatrix *Mat3);
extern void cvxMatPntVec(svxPoint *Origin, svxVector *zAxis, svxMatrix *Mat);
extern void cvxMatRotate(svxMatrix*,double Angle,svxAxis*Axis);
extern void cvxMatScale(svxMatrix*,svxPoint*,double sX, double sY, double sZ);
extern void cvxMatSetIdentity(svxMatrix *Mat);
extern void cvxMatTransform(svxMatrix *Mat, svxMatrix *Frame);
extern void cvxMatTranslate(svxMatrix *Mat, double dX, double dY, double dZ);
extern void cvxMatView(svxMatrix *Mat, evxViewStd Type);

/*| point
*/
extern double cvxPntDist(const svxPoint *Point1, const svxPoint *Point2);
extern int cvxPntIsCritical(svxPoint *pnt, int idCurve);
extern int cvxPntIsOn(svxPoint *Point, int idEntity); 
extern int cvxPntOnCrv(int idCurve, double Fraction, svxPoint *Point);
extern int cvxPntOnFace(int idComp, int idFace, double Param[2], svxPoint*);
extern int cvxPntProject(svxPoint *Pnt, int idEntity, svxPoint *ProjPnt);
extern void cvxPntTransform(svxMatrix *Mat, svxPoint *Point);
extern void cvxPntTransformList(svxMatrix *Mat, int Count, svxPoint *Points);
extern void cvxPntTranslate(svxPoint*, svxVector*, double Distance);

/*| axis
*/
extern void cvxAxisTransform(svxMatrix *Mat, svxAxis *Axis);

/*| vector
*/
extern void cvxVecAngle(svxVector *V1, svxVector *V2, double *Angle);
extern void cvxVecCross(svxVector *V1, svxVector *V2, svxVector *Cross);
extern void cvxVecInit(svxPoint *Point1, svxPoint *Point2, svxVector *Vector);
extern void cvxVecNormalize(svxVector *Vector);
extern void cvxVecPerp(svxVector *Vector, svxVector *Perp);
extern void cvxVecReverse(svxVector *Vector);
extern void cvxVecTransform(svxMatrix *Mat, svxVector *Vector);

/*| bounding box
*/
extern void cvxBndBoxAdd(svxBndBox *Box, svxPoint *Pnt);
extern void cvxBndBoxInit(svxBndBox *Box);
extern void cvxBndBoxInit2(svxBndBox *Box);
extern void cvxBndBoxPnts(svxBndBox *Box, svxPoint *Pnts);
extern double cvxBndBoxSize(svxBndBox *Box);

/*| curve
*/
extern int cvxCrvEval(int idCurve,double T,svxPoint*Point,svxVector*Normal);
extern int cvxCrvEval2(const int idCurve, const double t, const int level, svxEvalCurv* eval);
extern int cvxCrvLen(int idCurve, double *Len);
extern int cvxCrvParam(int idCurve, svxLimit *T);
extern int cvxCrvPntProj(int idCurve,svxPoint*Pnt,double *T,svxPoint*ProjPnt);
extern int cvxCrvExtremum(int idCurve,int *Count,svxPoint **Pnts,svxBndBox *BndBox);
extern void cvxCrvTransform(svxMatrix *Mat, svxCurve *Crv);
extern int cvxCrvGetInflections(int idCurve, int *Count, double **T, svxPoint **InfPnts);
extern int cvxCrvGetCusps(int idCurve, int *Count, double **T, svxPoint **Cusps);
extern int cvxCrvEvalCrvtr(const int idCurve, const double t, svxPoint *point, svxPoint *normal, double *curvature);


/*| surface
*/
extern int cvxSrfEval(svxSurface*,double U,double V,svxPoint*,svxVector*Normal,svxVector*Utan,svxVector*Vtan);
extern void cvxSrfTransform(svxMatrix *Mat, svxSurface *Srf);

/*| face
*/
extern int cvxFaceEval(int idFace,double U,double V,svxPoint*,svxVector*Normal);
extern int cvxFaceEval2(int idFace,double U,double V,svxPoint*,svxVector*Normal,svxVector*Utan,svxVector*Vtan);
extern int cvxFaceParam(int idFace, svxLimit *U, svxLimit *V);
extern int cvxFacePntLoc(int idFace, double U, double V, evxPntLocation *);
extern int cvxFacePntProj(int idFace, svxPoint*, double *U, double *V);
extern int cvxFaceReverse(int numFaces, int *idFaces);
extern int cvxFaceGetArea(const int faceId, const double tolerance, double *area);
extern int cvxFaceGetMaxCurvature(const int faceId, const svxPoint2 uvPoint, svxPoint *point, svxPoint *normal,
   svxPoint *direction, double *minCurvature, double *maxCurvature, double *curvature, svxPoint *curvatureDirection, svxPoint *curvatureCenter);


/*| intersection
*/
extern int cvxIsectCrvCrv(int Crv1,int Crv2,int Tan,int*Count,svxPoint**Points);
extern int cvxIsectCrvFace(int idCurve, int idFace, int Trim, int TangentOk, int *Count, svxPoint**);
extern int cvxIsectRayComp(int idComp,evxFaceTrim,svxAxis*,svxPoint*);
extern int cvxIsectRayCrv(svxAxis*,int Crv,int Tan,int*Count,svxPoint**Points);
extern int cvxIsectRayFace(int idFace,evxFaceTrim,svxAxis*,svxPoint*,svxPoint2*);
extern int cvxIsectRayPart(evxFaceTrim,svxAxis*,svxPoint*,int*idFace);
extern int cvxIsectRayPartVis(evxFaceTrim,svxAxis*,svxPoint*,int*idFace);
extern int cvxIsectRayPlane(svxAxis*,svxMatrix*,svxPoint*);
extern int cvxIsectRayShape(int idShape,evxFaceTrim,svxAxis*,svxPoint*,int*);

/*| plane/polygon
*/
extern int cvxPlnThruPnts(int PntCount, svxPoint *PntList, svxVector *Normal, double *D);
extern int cvxPolyToTri(int PntCount, svxPoint *PntList, int* VtxCount, int** VtxList);

/*| other
*/
extern double cvxRoundNumber(double Num, double Step, double Tol, int Dir);


/*
*********************************************************************
*! 12. Memory Management
*********************************************************************
*/

/* file: VxApiMem.c */
extern void cvxCurveFree(svxCurve *Crv);
extern void cvxFacetsFree(svxFacets *Facets);
extern int cvxMemAlloc(int NumBytes, void **MemPointer);
extern void cvxMemFree(void **MemPointer);
extern int cvxMemResize(int NumBytes, void **MemPointer);
extern void cvxMemZero(void *MemPointer, int NumBytes);
extern void cvxStlFree(svxSTL *Stl);
extern void cvxSurfaceFree(svxSurface *Srf);


/*
*********************************************************************
*! 13. CAM
*********************************************************************
*/

/* file: VxApiCmTool.c */
/*| tool
*/
extern int cvxCmToolInsert(evxCmToolType tool_type,evxCmToolSubType sub_type, int *idx_tool);
extern int cvxCmToolInsertAsTemplate(vxPath template_path, vxName file_name, vxName libriary_name, vxName tool_name, int *idx_tool);
extern int cvxCmToolDelete(int idx_tool);
extern int cvxCmToolGetAttr(int idx_tool, evxCmToolAttr attr, svxCmAttrValue *value);
extern int cvxCmToolSetAttr(int idx_tool, evxCmToolAttr attr, svxCmAttrValue *value);
extern int cvxCmHolderInsertAsTemplate(vxPath template_path, vxName file_name, vxName libriary_name, vxName holder_name, int *idx_holder);
extern int cvxCmSpeedInsertAsTemplate(vxPath template_path, vxName file_name, vxName libriary_name, vxName speed_name, int *idx_speed);

/* file: VxApiCmMachine.c */
/*| machine
*/
extern int cvxCmMachineInsert(evxCmMachClass machine_class, evxCmMachType machine_type,evxCmMachSubType sub_type, int *idx_machine);
extern int cvxCmMachineActive(int idx_machine);
extern int cvxCmMachineDelete(int idx_machine);
extern int cvxCmMachineGetAttr(int idx_machine, evxCmMachAttr attr, svxCmAttrValue *value);
extern int cvxCmMachineSetAttr(int idx_machine, evxCmMachAttr attr, svxCmAttrValue *value);
extern int cvxCmToolChangerInsert(int idx_toolchanger, int Location, int Diameter, int Height);
extern int cvxCmToolChangerDelete(int idx_toolchanger);

/* file: VxApiCmFrame.c */
/*| frame
*/
extern int cvxCmFrameInsert(vxName frame_name, svxPoint origin_point, svxPoint x_point, svxPoint y_point, int *idx_frame);
extern int cvxCmFrameDelete(int idx_frame);
extern int cvxCmFrameGetAttr(int idx_frame, evxCmFrameAttr attr, svxCmAttrValue *value);
extern int cvxCmFrameSetAttr(int idx_frame, evxCmFrameAttr attr, svxCmAttrValue *value);

/* file: VxApiCmOp.c */
/*| operation
*/
extern int cvxCmOpInsert(evxCmOpType opType,int *idx_Op);
extern int cvxCmOpDelete(int idx_Op);
extern int cvxCmOpDuplicate(int idx_Op,int *cpyOp);
extern int cvxCmOpAddComp(int idx_Op,int idx_CmComp);
extern int cvxCmOpRemoveComp(int idx_Op,int idx_CmComp);
extern int cvxCmOpAddFtr(int idx_Op,int idx_FtrGrp);
extern int cvxCmOpRemoveFtr(int idx_Op,int idx_ftrgrp);
extern int cvxCmOpAddTool(int idx_Op,int idx_Tool);
extern int cvxCmOpRemoveTool(int idx_Op);
extern int cvxCmOpGetAttr(int idx_Op,int cnt,int *attr,svxCmAttrValue *value);
extern int cvxCmOpSetAttr(int idx_Op,int cnt,int *attr,svxCmAttrValue *value);
extern int cvxCmOpSetAttrAsCfg(int idx_Op,vxPath cfg_location);
extern int cvxCmOpCalTPath(int idx_Op);
extern int cvxCmOpSetSpeed(int idx_op, vxName speedName);
extern int cvxCmOpSetAttr2(int idx_op, int field, int item, const char *value);
extern int cvxCmOpGetAttr2(int idx_op, int field, int item, int *is_on, char *value);

/* file: VxApiCmComp.c */
/*| cam component
*/
extern int cvxCmCompInsert(char *fileName, char *shapeName,int *idx_CmComp);
extern int cvxCmCompDelete(int idx_CmComp);
extern int cvxCmCompGetAttr(int idx_Cmcomp,int cnt,evxCmCompAtrr *attr,svxCmAttrValue **pValue);
extern int cvxCmCompSetAttr(int idx_Cmcomp,int cnt,evxCmCompAtrr *attr,svxCmAttrValue *value);

/* file: VxApiCmFtr.c */
/*| feature
*/
extern int cvxCmFtrInsert(evxCmFtrType ftrType,int idx_CmComp,int cnt,int *geom,int *idx_FtrGrp);
extern int cvxCmFtrDelete(int idx_ftrgrp);
extern int cvxCmFtrGetAttr(int idx_ftrgrp,int cnt,evxCmFtrAttr *attr,svxCmAttrValue *value);
extern int cvxCmFtrSetAttr(int idx_ftrgrp,int cnt,evxCmFtrAttr *attr,svxCmAttrValue *value);

/* file: VxApiCmInq.c */
/*| cam inquire
*/
extern int cvxCmInqAl(evxCmObjType type, int* cnt, int** idx_obj);
extern int cvxCmSolidVerify(void);
extern int cvxCmClashDetect(int partIdx, int stockIdx, int cnt, int *idx_op);
extern int cvxCmGougeDetect(int partIdx, int stockIdx, int cnt, int *idx_op);
extern int cvxCmConfigFileGetString(const char *form, int field, int casesensitive, char *dbstring, char *guistring, int *ifound);

/* file: VxApiCmForms.c */
/*| cam application
*/
extern int cvxCmSetClearance(double dClearZ,double dAppZ,double dRtZ,double dAutoClear,int iMiniClr);
extern int cvxCmGetClearance (double* dClearZ,	double* dAppZ, double* dRtZ, double* dAutoClear, int* iMiniClr);
extern int cvxCmSetSAndF(double FeedRough, double FeedFinish,double SpeedRough,double SpeedFinish,char* pName);
extern int cvxCmOutputManhours(int* idx_opedef, int cnt, double* manhours);
extern int cvxCmOutputInsNC(int idx_fld, int *idx_out, evxCmOutFldType fldType);
extern int cvxCmOutputDelNC(int idx_in, evxCmOutFldType fldType);
extern int cvxCmGetOutputNCSet(int idx_nc, svxNcSetting *ncSet);
extern int cvxCmSetOutputNCSet(int idx_nc, svxNcSetting *ncSet);
extern int cvxCmOutputInsOps(int idx_nc, int cnt, int *idx_op);
extern int cvxCmOutputDelOps(int idx_nc, int cnt, int *idx_op);
extern int cvxCmOutputNC(int idx_nc, evxCmOutType outType);
extern int cvxCmFolderGetAttr(int idx_folder, int cnt, int* attr, svxCmAttrValue* value);


/*
*********************************************************************
* END PUBLISHED API
*********************************************************************
*/

/* file: VxApiErr.c */

/*| internal API error reporting
*/
extern void cvxErrDisp(void);
extern void cvxErrInt(int Err, int IntegerNum);
extern void cvxErrMsg(int Err);
extern void cvxErrStr(int Err, const char *TextString);
extern void cvxErrStrInt(int Err, const char *TextString, int IntegerNum);
extern void cvxErrVal(int Err, double FloatValue);

/*| internal API support functions
*/
extern void ApiPntOptions(evxPntInpOpt,int,char*,int);
extern void ApiEntOptions(evxEntInpOpt,int,char*,int);
extern int VxApiErrID(void);
extern int VxApiExit(void);
extern int VxApiGlobal(int, void*);
extern int VxApiInit(int, void*);
extern void VxApiInitSym(void);
extern int VxApiMsgID(void);
extern void VxApiEventString(const char *String);
/* special-case API support functions (not declared in a header file) */
extern void VxApiNcvToCrv(void*, void*);
extern void VxApiNsfToSrf(void*, void*);

/* file: VxApiDoc.c */

/*| internal API documentation functions 
*/
extern void ApiDoc(void);
extern int cvxHelpDocLoad(vxPath file);



/*
*********************************************************************
**  END HEADER FILE
*********************************************************************
*/

#ifdef V_VXAPILOC_H
#include "api/inc/VxApiLoc.h"
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* VX_API_H */
