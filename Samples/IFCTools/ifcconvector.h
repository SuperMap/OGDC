#ifndef IFCCONVECTOR_H
#define IFCCONVECTOR_H

#include <QObject>
#include "ifcobj.h"

#include "Stream/ugdefs.h"
#include "OGDC/OgdcRecordset.h"
#include "Base3D/UGModelNodeEntities.h"


using namespace UGC;

class IFCConvector : public QObject
{
	Q_OBJECT

public:
	IFCConvector(QObject *parent = 0);
	~IFCConvector();

	// 加载IFC File
	UGbool ParseFile(std::wstring filename);

	// 导出数据集
	UGbool ExportUDB(UGString filename, UGString udbName);

	// 设置插入位置
	void setPosition(UGPoint3D & point);
	
	// 设置放置在平面还是球面
	void setIsPlaceSphere(UGbool bPlaceSphere);

private:
	void Init();
	void Clear();
	//--------------------------模型对象------------------------------------------	
	// IFC模型对象转换为Geo对象
	void IFCObjToGeoModel(OgdcRecordset* pRecordset);
	
	void ClearUpIfcModel();
	void DeleteIfcObject(IFC__OBJECT* ifcObject);

	// 获取IFC所有的对象树状结构
	IFC__OBJECT** GetChildrenRecursively(int_t ifcModel,int_t ifcParentEntity,IFC__OBJECT** firstFreeIfcObject,
		int_t segmentationParts,bool  hide);
	// 查询IFC模型对象
	IFC__OBJECT** QueryIfcObjects(int_t	ifcModel,IFC__OBJECT** firstFreeIfcObject,
		wchar_t* entityName,int_t segmentationParts,bool hide);
	// 创建IFC模型对象
	IFC__OBJECT* CreateIfcObject(int_t ifcEntity,int_t ifcInstance, wchar_t* entityName,
		int_t segmentationParts,bool hide);

	void GenerateGeometryNestedCall(int_t ifcModel);
	void GenerateWireFrameGeometry(int_t ifcModel, IFC__OBJECT* ifcObject);

	//-----------------------------单位-------------------------------------------
	void GetUnits(int_t ifcModel);
	void ClearUnits();
	IFC__SIUNIT* GetUnits(int_t ifcModel, int_t ifcProjectInstance);
	wchar_t	* GetUnit(IFC__SIUNIT * units, wchar_t * unitType);

	void UnitAddUnitType(IFC__SIUNIT * unit, wchar_t * unitType);
	void UnitAddPrefix(IFC__SIUNIT * unit, wchar_t * prefix);
	void UnitAddName(IFC__SIUNIT * unit, wchar_t * name);
	//----------------------------------------------------------------------------

	//-------------------材质------------------------------------------------------
	IFC_MATERIALS* IFCObjectMaterial(int_t ifcModel, int_t ifcInstance);
	UGModelMaterial* IFCMaterialToUGMaterial(UGString strMaterilName,IFC_MATERIALS * pIFCMaterials);
	void InitMaterials(int_t ifcObject);
	IFC_MATERIAL*  CreateMaterial();
	IFC_MATERIALS* CreateMaterials(IFC_MATERIAL * material);
	IFC_MATERIALS* CreateMaterials(IFC_MATERIAL	* material,int_t ifcModel,int_t ifcInstance);
	IFC_MATERIAL_META_INFO* CreateMaterialMetaInfo(int_t ifcInstance);
	void DeleteMaterial(IFC_MATERIAL * material);
	void DeleteMaterialMetaInfo(IFC_MATERIAL_META_INFO	* materialMetaInfo);
	void ClearMaterials();

	void MinimizeMaterialItems(
		IFC_MATERIAL_META_INFO	* materialMetaInfo,
		IFC_MATERIAL		   ** ppMaterial,
		bool					* pUnique,
		bool					* pDefaultColorIsUsed
		);

	void GetRGB_productDefinitionShape(
		int_t						model,
		int_t						ifcObjectInstance,
		IFC_MATERIAL_META_INFO	    ** ppMaterialMetaInfo
		);

	void GetRGB_shapeRepresentation(
		int_t						model,
		int_t						ifcShapeRepresentationInstance,
		IFC_MATERIAL_META_INFO	** ppMaterialMetaInfo
		);

	void GetRGB_styledItem(
#ifdef _DEBUG
		int_t			model,
#endif
		int_t			ifcStyledItemInstance,
		IFC_MATERIAL	* material
		);

	void GetRGB_presentationStyleAssignment(
#ifdef _DEBUG
		int_t			model,
#endif
		int_t			presentationStyleAssignmentInstance,
		IFC_MATERIAL	* material
		);

	void GetRGB_surfaceStyle(
#ifdef _DEBUG
		int_t			model,
#endif
		int_t			surfaceStyleInstance,
		IFC_MATERIAL	* material
		);

	void GetRGB_colourRGB(
		int_t			SurfaceColourInstance,
		IFC_COLOR	* color
		);

	void GetRGB_relAssociatesMaterial(
		int_t						model,
		int_t						ifcRelAssociatesMaterialInstance,
		IFC_MATERIAL				* material
		);

	void GetRGB_ifcMaterial(
		int_t						model,
		int_t						ifcMaterialInstance,
		IFC_MATERIAL				* material
		);

	void GetRGB_ifcMaterialDefinitionRepresentation(
		int_t						model,
		int_t						ifcMaterialDefinitionRepresentationInstance,
		IFC_MATERIAL				* material
		);

	void GetRGB_ifcStyledRepresentation(
		int_t						model,
		int_t						ifcStyledRepresentationInstance,
		IFC_MATERIAL				* material
		);

	void GetRGB_ifcMaterialLayerSetUsage(
		int_t						model,
		int_t						ifcMaterialLayerSetUsage,
		IFC_MATERIAL				* material
		);

	void GetRGB_ifcMaterialLayerSet(
		int_t						model,
		int_t						ifcMaterialLayerSet,
		IFC_MATERIAL				* material
		);

	void GetRGB_ifcMaterialLayer(
		int_t						model,
		int_t						ifcMaterialLayer,
		IFC_MATERIAL				* material
		);

	void WalkThroughGeometry__transformation(
		int64_t						owlModel,
		int64_t						owlInstance,
		IFC_MATERIALS			*** ppMaterials,
		IFC_MATERIAL_META_INFO	** ppMaterialMetaInfo
		);

	void WalkThroughGeometry__collection(
		int64_t						owlModel,
		int64_t						owlInstance,
		IFC_MATERIALS			*** ppMaterials,
		IFC_MATERIAL_META_INFO	** ppMaterialMetaInfo
		);

	void WalkThroughGeometry__object(
		int64_t						owlModel,
		int64_t						owlInstance,
		IFC_MATERIALS			*** ppMaterials,
		IFC_MATERIAL_META_INFO	** ppMaterialMetaInfo
		);

	void SearchDeeper(
		int_t						model,
		int_t						geometryInstance,
		IFC_MATERIAL_META_INFO	** ppMaterialMetaInfo,
		IFC_MATERIAL				* material
		);
	//-------------------------------------------------------------------------------

	//-------------------------属性---------------------------------------------------
	void IFCPropToFiled(OgdcRecordset* pRecordset, IFC__PROPERTY__SET* pIFCPropSet);
	void ClearPropertys();
	void DeletePropertySet(IFC__PROPERTY__SET* ifcPorpSet);
	void CreateObjProList(int_t ifcInstance, int_t entity , int_t & index, OgdcRecordset* pRecordset = NULL);
	//void CreateIFCPropertyList(IFC__PROPERTY__SET* ifcPropertySet);
	void CreateIfcInstanceProperties(
		int_t					ifcModel,
		IFC__PROPERTY__SET	** propertySets,
		int_t					ifcObjectInstance,
		IFC__SIUNIT			* units
		);
	void CreateRelDefinesByProperties(
		int_t ifcModel, 
		IFC__PROPERTY__SET ** propertySets, 
		int_t ifcRelDefinesByProperties, 
		IFC__SIUNIT * units);
	void CreateTypeObjectInstance(
		int_t ifcModel,
		IFC__PROPERTY__SET ** propertySets,
		int_t ifcTypeObjectInstance,
		IFC__SIUNIT * units);

	void CreateIfcElementQuantity(
		int_t ifcModel,
		IFC__PROPERTY__SET ** propertySets,
		int_t ifcPropertySetInstance,
		IFC__SIUNIT * units);

	IFC__PROPERTY__SET	* CreateIfcPropertySet(
		int_t ifcInstance,
		wchar_t * name,
		wchar_t * description);
	void CreateIfcPropertySet(
		int_t ifcModel, 
		IFC__PROPERTY__SET ** propertySets, 
		int_t ifcPropertySetInstance, 
		IFC__SIUNIT * units);
	void CreateIfcPropertySingleValue(
		int_t ifcPropertySingleValue, 
		IFC__PROPERTY * ifcProperty, 
		IFC__SIUNIT * units);
	IFC__PROPERTY	* CreateIfcProperty(
		int_t ifcInstance,
		wchar_t * name,
		wchar_t * description);

	void CreateIfcQuantityLength(
		int_t ifcQuantityLength,
		IFC__PROPERTY * ifcProperty,
		IFC__SIUNIT * units);
	void CreateIfcQuantityArea(
		int_t ifcQuantityArea, 
		IFC__PROPERTY * ifcProperty, 
		IFC__SIUNIT * units);

	void CreateIfcQuantityVolume(
		int_t ifcQuantityVolume, 
		IFC__PROPERTY * ifcProperty, 
		IFC__SIUNIT * units);
	void CreateIfcQuantityCount(
		int_t ifcQuantityCount,
		IFC__PROPERTY * ifcProperty);
	void CreateIfcQuantityWeigth(
		int_t ifcQuantityWeigth, 
		IFC__PROPERTY * ifcProperty, 
		IFC__SIUNIT * units);
	void	CreateIfcQuantityTime(
		int_t ifcQuantityTime, 
		IFC__PROPERTY * ifcProperty, 
		IFC__SIUNIT * units);
	OgdcString NestedPropertyValue(
		int_t ifcEntityInstance, 
		wchar_t * propertyName, 
		int_t propertyType, 
		int_t * ifcAggregate);
	//----------------------------------------------------------------------------

	//UGModelMaterial* IFCMaterialToUGMaterial(UGString strMaterilName,IFC_MATERIALS * pIFCMaterials);

private:

	wchar_t* m_ifcSchemaName_IFC2x3;
	wchar_t* m_ifcSchemaName_IFC4;
	wchar_t* m_ifcSchemaName_IFC4x1;

	int_t m_ifcSpace_TYPE;
	int_t m_ifcDistributionElement_TYPE;
	int_t m_ifcElectricalElement_TYPE;
	int_t m_ifcElementAssembly_TYPE;
	int_t m_ifcElementComponent_TYPE;
	int_t m_ifcEquipmentElement_TYPE;
	int_t m_ifcFeatureElement_TYPE;
	int_t m_ifcFeatureElementSubtraction_TYPE;
	int_t m_ifcFurnishingElement_TYPE;
	int_t m_ifcReinforcingElement_TYPE;
	int_t m_ifcTransportElement_TYPE;
	int_t m_ifcVirtualElement_TYPE;
	bool m_firstItemWithGeometryPassed;

	int_t		   m_ifcModel;
	IFC__OBJECT *  m_ifcObjects;
	IFC__OBJECT	** m_firstFreeIfcObject;
	std::map<int_t, IFC__OBJECT*> m_instanceIFCObject;

	// 材质
	IFC_MATERIAL	* m_firstMaterial;
	IFC_MATERIAL    * m_lastMaterial; 
	IFC_MATERIAL    * m_defaultMaterial;

	int64_t	m_rdfClassTransformation;
	int64_t	m_rdfClassCollection;
	int64_t	m_owlDataTypePropertyExpressID;
	int64_t	m_owlObjectTypePropertyMatrix;
	int64_t	m_owlObjectTypePropertyObject;
	int64_t	m_owlObjectTypePropertyObjects;

	// 单位集
	IFC__SIUNIT* m_units;
	// 属性集
	std::set<UGString> m_propList;
	std::set<UGString> m_objPropList;
	std::map<int_t, IFC__PROPERTY__SET*> m_ifcProperty;

	std::map<UGString, UGString> m_propForeign;

	// 插入点
	UGPoint3D m_Pos;
	UGbool m_bIsPlaceSphere;
	
};

#endif // IFCCONVECTOR_H
