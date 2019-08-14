//////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.0           
//                                                           
//!  \file headerfile.h
//!  \brief Class 三维模型类。
//!  \details 完成对三维模型类的封装。
//!  \author 。
//!  \attention 
//!   Copyright (c) 1996-2010 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.0
//////////////////////////////////////////////////////////////////////////


#if !defined(AFX_UGGEOMODEL_H__9824004B_0E6A_4340_908D_6FE34882E0AB__INCLUDED_)
#define AFX_UGGEOMODEL_H__9824004B_0E6A_4340_908D_6FE34882E0AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Geometry3D/UGGeometry3D.h"
#include "Base3D/UGVector3d.h"
#include "Base3D/UGMatrix4d.h"
#include "Base3D/UGMaterial.h"
#include "Base3D/UGMesh.h"
#include "Toolkit/UGTextureInfo.h"
#include "Graphics3D/UGModelManager.h"
#include "Geometry3D/UGGeoLine3D.h"

namespace UGC
{

class UGMaterial3D;
//! \brief 三维模型类。
class GEOMETRY3D_API UGGeoModel : public UGGeometry3D
{
public:
	//! \brief 构造函数
	UGGeoModel();

	//! \brief 析构函数
	virtual ~UGGeoModel();
	
	//! \brief  拷贝一个模型的数据构建该模型
	UGbool Make( const UGGeoModel& model);

	//! \brief 得到几何对象的类型，本接口返回UGGeometry::GeoModel
	virtual UGGeometry::Type GetType() const;

	//! \brief 得到模型文件的路径,主要用在KML中保持模型文件路径
	UGString GetSGMName() const;

	//! \brief 设置模型文件的路径,主要用在KML中保持模型文件路径
	void SetSGMName(UGString value);

	//! \设置模型数据
	void SetModelDataPtr(const UGModelDataPtr& ModelDataPtr);
	
	//! \设置模型数据
	void SetModelDataInfo(ModelDataInfo* pModelDataInfo);
	
	//! \是否是外挂模型文件的模型
	UGbool IsFileAttached();

	//! \设置外挂的模型文件
	void SetAttachedFilePath(UGString strAttachedFilePath);

	//! \是否是外挂模型文件的模型
	UGString GetAttachedFilePath();

	// 先按点的实现来
public:
	
	//! \brief 得到维度, 点为0维, 线/曲线等为1维, 面/曲面等为2维, GeoCompound/GeoText等为-1
	virtual UGint GetDimension() const;
	
	//! \brief 得到内点（在Geometry内部，靠近中央的点）
	//! \remarks 为点数最多的子对象 的中间点(中间个数的点, 而不是中间距离的点)
	virtual UGPoint2D GetInnerPoint() const;

	//! \brief 得到内点，本函数返回模型的位置Position
	virtual UGPoint3D GetInnerPoint3D() const;

	//! \brief 得到内点，本函数返回模型的位置Position，点线面物体返回内点
	virtual UGPoint3D GetPosition() const;
	
	//! \brief 点击测试, 用于对象点选
	//! \param pntHitTest 用来测试选中的点
	//! \param dTolerance 容限
	//! \return 选中返回true, 否则返回false
	virtual UGbool HitTest( const UGPoint2D &pntHitTest, UGdouble dTolerance) const;
	
	//! \brief 几何对象是否有效
	//! \return 有效返回true,无效返回false
	virtual UGbool IsValid() const;

	//! \brief 根据geometry类型，再调用各自的make方法去实现。
	//! \param pGeometry [in]。
	//! \return 。
	//! \remarks 。
	//! \attention 构造成功返回true。
	virtual UGbool MakeWithGeometry(const UGGeometry* pGeometry);	

	//! \brief 获取数据存储时的大小
	//! \return 返回存储大小
	//! \remarks 暂未考虑编码情况，与SaveGeoData()一起维护
	//! \attention 字符串的长度要+4
	virtual UGint GetDataSize() const;

	//! \brief 射线与GeoModel相交。
	//! \param vecRayStart 射线起点[in]。
	//! \param vecRayEnd 射线的终点[in]。
	//! \return vecPos 返回相交的点坐标
	void PickingRayWithModel(UGVector3d vecRayStart,UGVector3d vecRayEnd, UGVector3d& vecPos);
	
	//! \brief 获取模型智能指针。
	UGModelDataPtr& GetModelDataPtr();

	//! \brief 获取模型数据指针。
	ModelDataInfo* GetModelDataInfo();

	//! \brief 模型获取基底面
	virtual UGbool ConvertToRegion( UGGeoRegion &geoRegion, UGint nSegmentCount  = 0 ) const ;

	//! \brief 模型获取基底面
	virtual UGbool ConvertToRegion(EmSpatialRefType prjCoordType, UGGeoRegion &geoRegion, UGint nSegmentCount  = 0 ) const ;

	//! \brief 三维对象缩放
	//! \param dRatioX [in] X方向缩放比率
	//! \param dRatioY [in] Y方向缩放比率
	//! \param dRatioZ [in] Z方向缩放比率
	virtual void Inflate3D( UGdouble dRatioX, UGdouble dRatioY, UGdouble dRatioZ);
public:
	//Projection..
	
	//! \brief 根据投影转化器的设置和转化方向要求, 进行投影转化
	//! \param pPJTranslator 投影转化器
	//! \param bForward 转化方向, true代表从UGRefTranslator的源(Src)转化到目的(Des), false则相反
	virtual void PJConvert( UGRefTranslator *pPJTranslator, UGbool bForward = true);
	
	//! \brief 从经纬坐标系统转化为投影坐标系统
	//! \param pCoordSys 投影坐标系统
	virtual void PJForward( UGPrjCoordSys *pCoordSys );
	
	//! \brief 从投影坐标系统转化为经纬坐标
	//! \param pCoordSys 投影坐标系统
	virtual void PJInverse( UGPrjCoordSys *pCoordSys );

	//! \brief 得到模型的几何数据。
	UGArray<UGMesh*>& GetMesh() const;

	//! \brief 获取模型的材质数据。
	UGArray<UGMaterial*>& GetMaterial();

	//! \brief 获取模型的材质数据。
	UGArray<UGMaterial3D*>& GetMaterial3D();

	//! \brief 模型的在局部坐标矩阵
	UGMatrix4d GetLocalMatrix();

	//! \brief 该矩阵可以将模型的顶点转到世界坐标中
	//! \brief bDegree为TRUE是模型在球面场景下转换，FALSE是在平面场景下转换，默认在球面场景下转换 add by gengjing 14/10/10
	UGMatrix4d GetConvertToWorldCartesianMatrix(UGbool bDegree = TRUE);

	//! \brief 存储到Stream中
	virtual UGbool SaveGeoData(UGStream& stream, 
		UGDataCodec::CodecType eCodecType,UGbool bSDBPlus = FALSE) const;

	//! \brief 存储到Stream中
	virtual UGbool SaveVecData(UGStream& stream, 
		UGDataCodec::CodecType eCodecType,UGbool bSDBPlus = FALSE, UGfloat fCacheVersion = 1.4) const;

	//! \brief 存储额外的附加数据,不和Geometry存储在一起的数据
	//! \brief 模型里边存储纹理和模型分开
	//! \return 成功返回TRUE,失败返回FLASE
	virtual UGbool SaveExtData(UGArray<UGTextureInfo>& arryTextureInfo);

	//! \brief 通过几何对象中保存的纹理路径初始化纹理信息
	//! \return 成功返回TRUE,失败返回FLASE
	virtual UGbool InitialTextureInfo(UGArray<UGTextureInfo>& arrayTextureInfos);

	//! \brief 加载额外纹理数据到几何对象中
	//! \return 成功返回TRUE,失败返回FLASE
	virtual UGbool LoadExtData(const UGArray<UGTextureInfo>& arrayTextureInfos);

	//! \brief 从Stream中生成Geometry
	virtual UGbool LoadGeoData( UGStream& stream , 
		UGDataCodec::CodecType eCodecType = UGDataCodec::encNONE,UGbool bSDBPlus = FALSE );

	//! \brief 从Stream中生成Geometry
	virtual UGbool LoadVecData( UGStream& stream ,
		UGDataCodec::CodecType eCodecType = UGDataCodec::encNONE,UGbool bSDBPlus = FALSE, UGfloat fCacheVersion = 1.4f);

	virtual UGbool GetSnapPoints(UGArray<UGPoint2D>& aryPnts);

	//! \brief 根据句柄获得相应控制点的值
	//! \param nHandle 控制点句柄
	//! \param point 句柄对应的控制点的坐标值
	//! \return 返回辅助点的类型
	virtual UGint GetHandle( UGint nHandle, UGPoint2D& point);

	//! \brief 获取GeoModel的版本号
	UGdouble GetVersion(){ return m_dVersion;};

	//! \brief 设置三维几何对象的包围盒。
	//! \param box [in] 新包围盒的值。
	virtual void SetBoundingBox(const UGBoundingBox& box);

	//! \brief 释放数据
	void Release(void);

	//! \brief 根据模型的位置，归一模型顶点，并重新计算修改为新的位置
	//! \param pntPos [in] 模型的位置。
	void UnitizeBaseBottom(const UGPoint3D& pntPos);

	//! \brief 得到模型体积。
	virtual UGdouble GetVolume();

protected:
	//! \brief 真正计算Bounds
	//! \return 返回得到的Bounds
	virtual UGRect2D ComputeBounds() const;	

	//!\brief 得到Boundingbox
	virtual UGBoundingBox ComputeBoundingBox() const;

public:
	// 模型表面距离
	//! \param pGeoLine 二维线（坐标系和模型应该一致）
	//! \param bDegree 线、模型数据是否都是经纬度坐标系，默认经纬度
	//! \param pSurfaceLine 依模型表面的线
	//! \return 表面距离
	UGdouble SurfaceDistance(UGGeoLine *pGeoLine, UGbool bDegree = TRUE, UGGeoLine3D *pSurfaceLine = NULL);

	// 模型表面积
	//! \param pGeoLine 二维面（坐标系和模型应该一致）
	//! \param bDegree 面、模型数据是否都是经纬度坐标系，默认经纬度
	//! \param pSurfaceLine 依模型表面的线
	//! \return 表面积
	UGdouble SurfaceArea(UGGeoRegion *pGeoRegion, UGbool bDegree = TRUE, UGGeoLine3D *pSurfaceLine = NULL);

	// 模型体积
	//! \param pGeoRegion 二维面（坐标系和模型应该一致）
	//! \param dBaseValue 底面高程
	//! \param bDegree 面、模型数据是否都是经纬度坐标系，默认经纬度
	//! \param pSurfaceLine 依模型表面的线
	//! \return 体积
	UGdouble SurfaceVolume(UGGeoRegion *pGeoRegion, UGdouble dBaseValue, UGbool bDegree = TRUE, UGGeoLine3D *pSurfaceLine = NULL);

	// 模型填挖方
	//! \param pGeoRegion 二维面（坐标系和模型应该一致）
	//! \param dBaseValue 底面高程
	//! \param dFillVolume 填方体积
	//! \param dDigVolume 挖方体积
	//! \param dFillArea 填方面积
	//! \param dDigArea 挖方面积
	//! \param bDegree 面、模型数据是否都是经纬度坐标系，默认经纬度
	//! \param pSurfaceLine 依模型表面的线
	void SurfaceFillDig(UGGeoRegion *pGeoRegion, UGdouble dBaseValue, 
		UGdouble &dFillVolume, UGdouble &dDigVolume, UGdouble &dFillArea, UGdouble &dDigArea,
		UGbool bDegree = TRUE, UGGeoLine3D *pSurfaceLine = NULL);
private:

	//计算三角网上表面距离、面积和体积提供
	struct ATriangle
	{
		UGint nMeshIndex;
		UGint nTriangleIndex;
		UGint nVertexIndex1;
		UGint nVertexIndex2;
		UGint nVertexIndex3;
	};

	struct ATriangleEx
	{
		ATriangle aTriangle;
		UGbool bVertex1InRegion;
		UGbool bVertex2InRegion;
		UGbool bVertex3InRegion;
	};

	UGdouble SurfaceDistanceGrid(UGGeoLine *pGeoLine, UGbool bDegree = TRUE, UGGeoLine3D *pSurfaceLine = NULL);
	UGdouble SurfaceAreaAccurate(UGGeoRegion *pGeoRegion, UGbool bDegree = TRUE, UGGeoLine3D *pSurfaceLine = NULL);
	UGdouble SurfaceVolumeAccurate(UGGeoRegion *pGeoRegion, UGdouble dBaseValue, UGbool bDegree = TRUE, UGGeoLine3D *pSurfaceLine = NULL);
	void SurfaceFillDigAccurate(UGGeoRegion *pGeoRegion, UGdouble dBaseValue, 
		UGdouble &dFillVolume, UGdouble &dDigVolume, UGdouble &dFillArea, UGdouble &dDigArea,
		UGbool bDegree = TRUE, UGGeoLine3D *pSurfaceLine = NULL);
	UGbool SurfaceClip(UGArray<UGMesh*> &pArrMeshes, UGGeoRegion geoRegionClip, UGArray< UGArray<UGushort> >& arrIndexClip, UGArray< UGArray<UGushort> >& arrIndexFill, UGMatrix4d matrixToWorldCartesian, UGdouble dBaseValue, UGbool bDegree = TRUE, UGbool bVolumeComputer = TRUE);//在二维面上判断包含
	UGbool JoinSurfaceLines(UGArray<UGVector3d>& intersects,UGGeoLine3D *&pSurfaceLine,UGVector3d minVector, UGVector3d maxVector,UGMatrix4d matrixToWorldCartesian, UGbool bDegree = TRUE);
	UGbool JoinSurfaceLines(UGGeoLine *pGeoLine, UGGeoLine3D *&pSurfaceLine, UGVector3d minVector, UGVector3d maxVector, UGArray<UGMesh*> &pArrMeshes, UGArray< UGArray<UGushort> >& arrIndexClip, UGMatrix4d matrixToWorldCartesian, UGbool bDegree = TRUE);

private:

	//! \breif 模型数据,用于管理
	UGModelDataPtr m_ModelDataPtr;

	//! \brief 模型本身的数据
	ModelDataInfo* m_pModelDataInfo;

	//! \GeoModel是否是数据的所有的
	UGbool m_bOwner;

	//! \GeoModel版本号
	UGdouble m_dVersion;

	//! \是否是外挂模型文件的模型
	UGbool m_bFileAttached;

	//! \模型外挂的模型文件
	UGString m_strAttachedFile;
};
}
#endif // !defined(AFX_UGGEOMODEL_H__9824004B_0E6A_4340_908D_6FE34882E0AB__INCLUDED_)

