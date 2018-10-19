/*---------------------------------------------------------------------------
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.0           
//                                                           
//!  \file UGGeoModel(Pro).h
//!  \brief Geo + Model 对象
//!  \details 
//!  \author zq
//!  \attention
//!   Copyright (c) 1996-2010 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.0
---------------------------------------------------------------------------*/

#ifndef __UGGEOMODELPRO_H__
#define __UGGEOMODELPRO_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000	
#include "Geometry3D/UGGeometry3D.h"
#include "Geometry3D/UGGeoLine3D.h"
#include "Base3D/UGModelNode.h"

namespace UGC
{

class UGOSGGroup;
class UGNodeAnimationInfo;

class GEOMETRY3D_API UGGeoModelPro : public UGGeometry3D
{
public:
	//! \brief 构造函数
	UGGeoModelPro();
	UGGeoModelPro(UGModelNode* pNode, UGbool bDelegate=FALSE);

	//! \brief 析构函数
	virtual ~UGGeoModelPro();

	//! \brief 得到几何对象的类型，本接口返回UGGeometry::GeoModel
	virtual UGGeometry::Type GetType() const;

public:
	//! \brief 清空地理数据
	virtual void Clear();

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

	//! \brief 模型获取基底面
	virtual UGbool ConvertToRegion( UGGeoRegion &geoRegion, UGint nSegmentCount  = 0 ) const ;

	//! \brief 模型获取基底面
	virtual UGbool ConvertToRegion(EmSpatialRefType prjCoordType, UGGeoRegion &geoRegion, UGint nSegmentCount  = 0 ) const ;

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

	//! \brief 存储到Stream中
	virtual UGbool SaveGeoData(UGStream& stream, 
		UGDataCodec::CodecType eCodecType,UGbool bSDBPlus = FALSE) const;

	//! \brief 从Stream中生成Geometry
	virtual UGbool LoadGeoData( UGStream& stream , 
		UGDataCodec::CodecType eCodecType = UGDataCodec::encNONE,UGbool bSDBPlus = FALSE );

	virtual UGbool GetSnapPoints(UGArray<UGPoint2D>& aryPnts);

	//! \brief 根据句柄获得相应控制点的值
	//! \param nHandle 控制点句柄
	//! \param point 句柄对应的控制点的坐标值
	//! \return 返回辅助点的类型
	virtual UGint GetHandle( UGint nHandle, UGPoint2D& point);

	//! \brief 获取GeoModel的版本号
	UGint GetVersion(){ return m_nVersion;};

	//! \brief 设置三维几何对象的包围盒。
	//! \param box [in] 新包围盒的值。
	virtual void SetBoundingBox(const UGBoundingBox& box);

	//! \brief 得到三维几何对象的包围盒。
	//! \return 返回包围盒的值。
	virtual const UGBoundingBox& GetBoundingBox()	const;

	//! \brief 得到模型体积。
	virtual UGdouble GetVolume();

	void SetSpherePlaced(UGbool bSpherePlaced);

	UGbool GetSpherePlaced();

	//! \brief 修改:模型的放置点位置(m_pntPos)-->模型的实际中心点
	//! \brief 在 SaveGeoData 中调用
	void TransformPntPosition() const;

	//! \brief GeoModel构造
	//! \attention 构造成功返回true。
	UGbool FromGeoModel(const UGGeometry* pGeoSrc);

	//! \brief 设置EM
	void SetEM(UGModelEM* pEM);

	//! \brief 根据传入的Geometry设置位置等信息，ModelNode不变
	UGbool MakeShell(const UGGeometry* pGeometry);

	//! \brief 模型合并
	static UGGeoModelPro* Compose(UGArray<UGGeoModelPro*> arrGeoModel);

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

	void SetModelNode(UGModelNode* pGroup,UGbool bOwned = FALSE);
	UGModelNode* GetModelNode();

	//! \brief 根据m_pntScale m_pntRotate m_pntPos构造矩阵；
	//! \brief 该 矩阵+ModelNode=UGGeoModelPro
	UGMatrix4d GetLocalMatrix() const;

//! \brief 设置对象的缩放大小，这个函数只对模型和规则几何体意义
	//! \param fScaleX [in] X方向缩放比率。
	//! \param fScaleY [in] Y方向缩放比率。
	//! \param fScaleZ [in] Z方向缩放比率。
	virtual void SetScale(UGdouble fScaleX,UGdouble fScaleY,UGdouble fScaleZ);	

	//! \brief 设置对象的缩放大小，这个函数只对模型和规则几何体意义 
	//! \param fScaleXYZ [in] 物体在三个方向上进行同样大小的缩放 
	virtual void SetScale(UGdouble fScaleXYZ);

	//! \brief 设置对象的旋转量，这个函数只对模型和规则几何体意义 
	//! \param fRotateX [in] X方向的旋转角度。
	//! \param fRotateY [in] Y方向的旋转角度。
	//! \param fRotateZ [in] Z方向的旋转角度。
	virtual void SetRotation(UGdouble fRotateX,UGdouble fRotateY,UGdouble fRotateZ);	

	//! \brief 从加载UGModelsEntitiesManager填充vector
	UGbool LoadEntities(UGModelEM* pEM, UGint nLOD = -1, 
		UGModelEntity::METype mtype = UGModelEntity::etUnknown);

	//! \brief 计算对象的二维Bounds使用
	UGBoundingBox GetGeoBoundingBox() const;

	//! \brief 获取对象海拔高度，缓存八叉树切分使用
	void GetAltitude( UGdouble& dbMaxZ, UGdouble& dbMinZ);

	//! \brief 投影转换：外部设置是否只转换插入点
	void SetPJ(UGbool bInsertPntOnly);

	//! \brief 设置模型文件的路径,主要用在KML中保持模型文件路径
	UGString GetFilePathName() const;

	//! \brief 设置模型文件的路径,主要用在KML中保持模型文件路径
	void SetFilePathName(UGString strFilePath);

	//! \brief 创建一个节点动画对象
	void CreateNodeAnimation();

	//! \brief 获取节点动画对象
	UGNodeAnimationInfo* GetNodeAnimation();

	//! \brief 平移的基准点：球面上换算笛卡尔坐标使用
	//! \brief 模型对象平移，单位：米或弧度
	//! \brief 仅支持球面对象
	UGbool Translate(UGdouble dbX, UGdouble dbY, UGdouble dbZ, UGPoint3D pntBase);

	//! \brief pntBase [in] 旋转前统一基准点
	//! \brief dXAngle [in]	x轴旋转角度
	//! \brief dYAngle [in]	y轴旋转角度
	//! \brief dZAngle [in]	z轴旋转角度
	//! \brief rotatePoint [in] 旋转点
	//! \brief 仅支持球面对象
	UGbool Rotate(UGPoint3D rotatePoint, \
		UGdouble dXAngle, UGdouble dYAngle, UGdouble dZAngle,UGPoint3D pntBase);

	//! \brief 获取W位的最值
	void GetMinMaxW(UGdouble& dMin, UGdouble& dMax);
private:
	//! \brief 处理Patch
	void TranslatePosition(UGModelPagedPatch* pPatch, UGMatrix4d& matSrc, UGMatrix4d& matLocalBase);

private:
	//! \GeoModel版本号
	UGint m_nVersion;
	UGModelNode* m_pModelNode;
	UGbool m_bNodeOwned;

	//! \brief 是否放置在球面上，影响GeoModelPro计算Bounds
	UGbool m_bSpherePlaced;

	UGbool m_bPJInsertPntOnly;

	//! \brief 记录模型文件路径，主要用于KML文件保存
	UGString m_strModelFilePath;

	//! \brief 节点动画信息
	UGNodeAnimationInfo* m_pNodeAnimation;

	//! \brief 
	UGModelEM *m_pEM;
};


}

#endif
