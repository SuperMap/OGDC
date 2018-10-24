#pragma once
#include "IFCEngine/ifcengine.h"

#define		ABSORBEDDOSEUNIT					101
#define		AREAUNIT							102
#define		DOSEEQUIVALENTUNIT					103
#define		ELECTRICCAPACITANCEUNIT				104
#define		ELECTRICCHARGEUNIT					105
#define		ELECTRICCONDUCTANCEUNIT				106
#define		ELECTRICCURRENTUNIT					107
#define		ELECTRICRESISTANCEUNIT				108
#define		ELECTRICVOLTAGEUNIT					109
#define		ENERGYUNIT							110
#define		FORCEUNIT							111
#define		FREQUENCYUNIT						112
#define		ILLUMINANCEUNIT						113
#define		INDUCTANCEUNIT						114
#define		LENGTHUNIT							115
#define		LUMINOUSFLUXUNIT					116
#define		LUMINOUSINTENSITYUNIT				117
#define		MAGNETICFLUXDENSITYUNIT				118
#define		MAGNETICFLUXUNIT					119
#define		MASSUNIT							120
#define		PLANEANGLEUNIT						121
#define		POWERUNIT							122
#define		PRESSUREUNIT						123
#define		RADIOACTIVITYUNIT					124
#define		SOLIDANGLEUNIT						125
#define		THERMODYNAMICTEMPERATUREUNIT		126
#define		TIMEUNIT							127
#define		VOLUMEUNIT							128
#define		USERDEFINED							129

#define		STRUCT_TYPE_PROPERTY				203
#define		STRUCT_TYPE_PROPERTY_SET			204

struct IFC__SIUNIT {
	int_t							type;
	wchar_t							* unitType;

	wchar_t							* prefix;
	wchar_t							* name;

	IFC__SIUNIT						* next;
};

struct IFC_COLOR {
	float							R;
	float							G;
	float							B;
	float							A;
};

struct IFC_MATERIAL {
	bool							active;

	IFC_COLOR					ambient;
	IFC_COLOR					diffuse;
	IFC_COLOR					specular;
	IFC_COLOR					emissive;

	double							transparency;
	double							shininess;

	//void							* MTRL;

	IFC_MATERIAL					* next;
	IFC_MATERIAL					* prev;
};

struct IFC_MATERIAL_META_INFO {
	bool							isPoint;
	bool							isEdge;
	bool							isShell;

	int_t TST_REMOVE;

	int_t							expressID;
	IFC_MATERIAL					* material;

	IFC_MATERIAL_META_INFO		* child;
	IFC_MATERIAL_META_INFO		* next;
};

struct IFC_MATERIALS {
	//	int64_t							indexArrayOffset;
	//	int64_t							indexArrayPrimitives;

	//	int64_t							indexOffsetForFaces;

	int64_t							__indexOffsetForFaces;
	int64_t							__indexArrayOffset;

	int64_t							__noPrimitivesForFaces;
	int64_t							__indexBufferSize;

	IFC_MATERIAL					* material;

	IFC_MATERIALS					* next;
};

struct IFC__PROPERTY {
	int_t							structType;
	int_t							ifcInstance;

	wchar_t							* name;
	wchar_t							* description;

	wchar_t							* nominalValue;
	wchar_t							* lengthValue;
	wchar_t							* areaValue;
	wchar_t							* volumeValue;
	wchar_t							* countValue;
	wchar_t							* weigthValue;
	wchar_t							* timeValue;
	wchar_t							* unit;

	//HTREEITEM						hTreeItem;

	wchar_t							* nameBuffer;

	IFC__PROPERTY				* next;
};

struct IFC__PROPERTY__SET {
	int_t							structType;
	int_t							ifcInstance;

	wchar_t							* name;
	wchar_t							* description;

	IFC__PROPERTY					* properties;

	//HTREEITEM						hTreeItem;

	wchar_t							* nameBuffer;

	IFC__PROPERTY__SET				* next;
};

struct IFC__OBJECT
{
	int_t							ifcInstance;
	int_t							ifcEntity;

	wchar_t							* entityName;
	bool							hide;
	int_t							segmentationParts;
	IFC__OBJECT						* next;

	IFC_MATERIALS					* materials;

	//UGVector3d					vecMin;
	//UGVector3d					vecMax;

	int_t							noVertices;
	float							* vertices;

	int_t							vertexOffset;

	int_t							noPrimitivesForPoints;
	int32_t							* indicesForPoints;
	int_t							indexOffsetForPoints;

	int_t							noPrimitivesForLines;
	int32_t							* indicesForLines;
	int_t							indexOffsetForLines;

	int_t							noPrimitivesForFaces;
	int32_t							* indicesForFaces;
	int_t							indexOffsetForFaces;

	int_t							noPrimitivesForWireFrame;
	int32_t							* indicesForLinesWireFrame;
	int_t							indexOffsetForWireFrame;
};