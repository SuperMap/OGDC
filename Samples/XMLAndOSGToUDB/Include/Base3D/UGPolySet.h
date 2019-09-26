// UGPolySet.h: interface for the PolySet class.
//
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.0           
//                                                           
//!  \file UGPolySet.h
//!  \brief 参数化对象建模模型结构。
//!  \details 文件详细信息。
//!  \author 陈玉成。
//!  \attention 
//!   Copyright (c) 1996-2010 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.0
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_UGPOLYSET_H__F8C91F88_9FBD_40F7_AB5D_17220764D627__INCLUDED_)
#define AFX_UGPOLYSET_H__F8C91F88_9FBD_40F7_AB5D_17220764D627__INCLUDED_
// {F8C91F88-9FBD-40F7-AB5D-17220764D627}

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Base3D/UGPolysDataPackage.h"
#include "Base3D/UGPlane.h"
#include "Base3D/UGModelNode.h"

namespace UGC
{
	class UGTINPoint;

	//! \brief 顶点索引对。
	typedef std::pair<UGuint, UGuint> UGIndexPair;
	typedef std::vector<UGIndexPair> UGIndexPairs;

	//! \brief 按法线为索引的三角面分包。
	typedef std::list<std::pair<UGVector3d, UGPolyList> > UGPolysPackage;

	//! \brief 多边形集合。
	//! \attention UGPolySet必须存储凸多边形。
	class BASE3D_API UGPolySet
	{
	public:
		//! \brief 构造函数。 
		UGPolySet(UGuint nDimension = 3);

		//! \brief 拷贝构造函数。
		UGPolySet(const UGPolySet& copy);

		UGPolySet& operator=(const UGPolySet& other);

		//! \brief 析构函数。
		virtual ~UGPolySet();

		//! \brief 深复制。
		// \brief 深度复制数据包。
		virtual UGPolySet* Copy() const;
		virtual UGPolySet& Copy(const UGPolySet& ps);

		//! \brief 浅复制。
		//! \remark 引用数据包指针。
		virtual UGPolySet* ShallowCopy() const;
		virtual UGPolySet& ShallowCopy(const UGPolySet& ps);

		//! \brief 复制参数。
		//! \remark 独立的数据包指针。
		virtual UGPolySet* CopyParam() const;
		virtual UGPolySet& CopyParam(const UGPolySet& ps);

		//! \brief 多面体凹凸类型(三维)。
		enum Type { unknown = -1, concave = 0, convex = 1 };
		//! \brief 获取多面体凹凸类型(三维)。
		Type GetType() const { return m_eType; }
		//! \brief 设置多面体凹凸类型(三维)。
		void SetType(Type type) { m_eType = type; }
		//! \brief 是否为凸多面体(三维)。
		UGbool IsConvex() const { return m_eType == convex; }

		//! \brief 名称。
		void SetName(const UGString& strName) { m_strName = strName; }
		const UGString& GetName() const { return m_strName; }

		//! \brief 重置。
		virtual void Reset();

		//! \brief 清空存储数据。
		virtual void Clear();

		//! \brief 多边形集合存储的模型所属维度。
		UGuint GetDimension() const { return m_nDimension; }
		void SetDimension(UGuint nDimension) { m_nDimension = nDimension; }

		//! \brief 切片数量。
		void SetFragments(UGuint nFragments) { m_nFragments = nFragments; }
		UGuint GetFragments() const { return m_nFragments; }

		//! \brief 计算LOD切换像素。
		static UGdouble CalculateSwitchPixels(
			UGdouble dBoundingSphereRadius,
			UGdouble dBottomRadius, 
			UGuint nFragments, UGuint nPixelCount = 5);

		//! \brief 参数化对象参数类。
		UGPrimitiveParam& GetPrimitiveParam() { return m_priParam; }
		const UGPrimitiveParam& GetPrimitiveParam() const { return m_priParam; }
		void SetPrimitiveParam(const UGPrimitiveParam& param) { m_priParam = param; }

		//! \brief 纹理贴图参数类。
		UGUVWMapParam& GetUVWMapParam() { return m_uvwParam; }
		const UGUVWMapParam& GetUVWMapParam() const { return m_uvwParam; }
		void SetUVWMapParam(const UGUVWMapParam& param) { m_uvwParam = param; }

		//! \brief 模型顶点类型。
		void SetCoordsType(UGCoordinatesType type) { m_eCoordsType = type; }
		UGCoordinatesType GetCoordsType() const { return m_eCoordsType; }

		//! \brief 生成纹理坐标。
		UGbool IsGenerateMappingCoords() const { return m_bGenerateMappingCoords; }
		UGbool GetGenerateMappingCoords() const { return m_bGenerateMappingCoords; }
		void SetGenerateMappingCoords(UGbool bGenerateMappingCoords) { m_bGenerateMappingCoords = bGenerateMappingCoords; }

		//! \brief 是否生成法线。
		UGbool IsGenerateNormal() const { return m_bGenerateNormal; }
		UGbool GetGenerateNormal() const { return m_bGenerateNormal; }
		void SetGenerateNormal(UGbool bGenerateNormal) { m_bGenerateNormal = bGenerateNormal; }

		//! \brief 是否生成颜色。
		UGbool IsGenerateColor() const { return m_bGenerateColor; }
		UGbool GetGenerateColor() const { return m_bGenerateColor; }
		void SetGenerateColor(UGbool bGenerateColor) { m_bGenerateColor = bGenerateColor; }

		//! \brief 是否将z值作为属性不参与运算。
		UGbool IsZValAsAttribute() const { return m_bZValAsAttribute; }
		void SetZValAsAttritute(UGbool bZValAsAttribute) { m_bZValAsAttribute = bZValAsAttribute; }

		//! \brief 是否有w字段。
		UGbool HasW() const { return m_bHasW; }
		void SetW(UGbool bHasW) { m_bHasW = bHasW; }

	public:
		//! \brief Spherical_LonLat -> Spherical_Cartesian。
		UGbool LonLatToCartesian();

		//! \brief Spherical_Cartesian -> Spherical_LonLat。
		UGbool CartesianToLonLat();

		//! \brief Spherical_ENU -> Spherical_LonLat。
		UGbool ENUToLonLat();

		//! \brief Spherical_LonLat -> Spherical_ENU。
		UGbool LonLatToENU(
			const UGVector3d& vPosition, 
			const UGVector3d& vScale = UGVector3d::UNIT_SCALE, 
			const UGVector3d& vRotate = UGVector3d::ZERO);

		//! \brief Spherical_ENU -> Spherical_Cartesian。
		UGbool ENUToCartesian();

		//! \brief Spherical_Cartesian -> Spherical_ENU。
		UGbool CartesianToENU(
			const UGVector3d& vPosition, 
			const UGVector3d& vScale = UGVector3d::UNIT_SCALE, 
			const UGVector3d& vRotate = UGVector3d::ZERO);

		//! \brief Spherical_LonLat -> Pseudo_Cartesian。
		UGbool LonLatToPseudoCartesian(UGdouble dFactor = GLOBAL_RADIUS);

		//! \brief Pseudo_Cartesian -> Spherical_LonLat。
		UGbool PseudoCartesianToLonLat(UGdouble dFactor = GLOBAL_RADIUS);

		//! \brief Spherical_ENU -> Pseudo_Cartesian。
		UGbool ENUToPseudoCartesian(UGdouble dFactor = GLOBAL_RADIUS);

		//! \brief Pseudo_Cartesian -> Spherical_ENU。
		UGbool PseudoCartesianToENU(
			const UGVector3d& vPosition, 
			const UGVector3d& vScale = UGVector3d::UNIT_SCALE, 
			const UGVector3d& vRotate = UGVector3d::ZERO,
			UGdouble dFactor = GLOBAL_RADIUS);

		//! \brief Ellipsoidal_ENU -> Spherical_LonLat。
		UGbool EllipsoidalENUToLonLat();

		//! \brief Spherical_LonLat -> Ellipsoidal_ENU。
		UGbool LonLatToEllipsoidalENU(
			const UGVector3d& vPosition, 
			const UGVector3d& vScale = UGVector3d::UNIT_SCALE, 
			const UGVector3d& vRotate = UGVector3d::ZERO);

		//! \brief Local_Cartesian -> Global_Cartesian。
		UGbool CartesianToGlobalCartesian();

		//! \brief Global_Cartesian -> Local_Cartesian。
		UGbool GlobalCartesianToCartesian(
			const UGVector3d& vPosition, 
			const UGVector3d& vScale = UGVector3d::UNIT_SCALE, 
			const UGVector3d& vRotate = UGVector3d::ZERO);

	public:
		//! \brief 添加面。
		void BeginSurface();
		void EndSurface();

		//! \brief 添加顶点。
		void AddVertex(const UGPolyVertex& v);
		void AddVertices(const UGPolyVertices& arrPts);

		//! \brief 是否错误。
		UGbool IsError() const;
		void RollBack();

		//! \brief 添加面。
		void BeginFacet();
		void EndFacet();

		//! \brief 添加面内点索引。
		void AddFacetId(UGPolyIndex id);
		void AddFacet(const UGPolyIndices& arrIds);
		void AddFacets(const UGPolyFacets& arrFacets);

		//! \brief 获取纹理坐标
		UGUVWs& GetTexCoords(const UGPosition2& pos);

		//! \brief 获取浮点型拓展位。
		std::vector<UGdouble>& GetRealExt(const UGPosition2& pos);

		//! \brief 获取整数型拓展位。
		std::vector<UGuint>& GetIntegerExt(const UGPosition2& pos);

	public:
		//! \brief 获取数据包中的面索引包。
		const UGPolyFacets& GetFacets() const;

		const UGPolysDataPackage* GetPolysDataPackage() const;

		UGbool IsOrient() const;
		void SetOrient(UGbool bOrient);

		//! \brief 获取数据包中的顶点包。
		const UGPolyVertices& GetPts() const;
		UGPolyVertex& GetPtOfMutability(UGPolyIndex id);
		const UGPolyVertex& GetPt(UGPolyIndex id) const;

		//! \brief 获取顶点在Polys的位置信息。
		const UGPolyVertexPositions& GetPtPositions() const;

		//! \brief 获取数据包中顶点数量。
		UGuint GetNumPts() const;

		//! \brief 获取数据包中索引总数量。
		UGuint GetNumFacetsIds() const;

		//! \brief 获取数据包中多面体点总数量。
		UGuint GetNumPolysPts() const;

		//! \brief 获取数据包中的多面体数据。
		const UGPolys& GetPolys() const;
		UGPolys& GetPolysOfMutability();

		//! \brief 获取纹理坐标
		const UGUVWs& GetTexCoords(const UGPosition2& pos) const;

		//! \brief 获取浮点型拓展字段
		const std::vector<UGdouble>& GetRealExt(const UGPosition2& pos) const;

		//! \brief 获取整数型拓展字段
		const std::vector<UGuint>& GetIntegerExt(const UGPosition2& pos) const;

		//! \brief 获取所存储的多边形数量。
		UGuint GetNumPolys() const;

		//! \brief 所存储的多边形数量是否为空。
		UGbool IsEmpty() const;

		//! \brief 是否拓扑校正。
		UGbool IsSanitized() const { return m_bSanitized; }

		//! \brief 设置拓扑校正状态(二维)。
		void SetSantized(UGbool bSantized) { m_bSanitized = bSantized; }

		//! \brief 是否三角化。
		UGbool IsTriangulated() const { return m_bTriangulated; }

		//! \brief 设置三角化状态。
		void SetTriangulated(UGbool bTriangulated) { m_bTriangulated = bTriangulated; }

	public:
		//! \brief 追加默认多边形。
		void AppendPoly();

		//! \brief 追加多边形
		//! \param poly [in] 多边形。
		void AppendPoly(const UGPoly& poly);

		//! \brief 追加顶点。
		//! \param x [in] 顶点坐标X分量。
		//! \param y [in] 顶点坐标Y分量。
		//! \param z [in] 顶点坐标Z分量。
		//! \param uvws [in] 纹理坐标。
		//! \param iAttribute [in] 顶点属性。
		void AppendVertex(
			const UGdouble& x, 
			const UGdouble& y, 
			const UGdouble& z,
			const UGUVWs& uvws = UGUVWs(),
			const UGAttribute& iAttribute = UGAttribute());

		//! \brief 追加顶点。
		//! \param vertex [in] 顶点坐标。
		//! \param uvws [in] 纹理坐标。
		//! \param iAttribute [in] 顶点属性。
		void AppendVertex(
			const UGVector3d& vertex, 
			const UGUVWs& uvws = UGUVWs(), 
			const UGAttribute& iAttribute = UGAttribute());

		//! \brief 追加顶点。
		//! \param pt [in] 顶点结构。
		void AppendVertex(const UGPolyPoint& pt);

		//! \brief 插入顶点。
		//! \param x [in] 顶点坐标X分量。
		//! \param y [in] 顶点坐标Y分量。
		//! \param z [in] 顶点坐标Z分量。
		//! \param uvws [in] 纹理坐标。
		//! \param iAttribute [in] 顶点属性。
		void InsertVertex(
			const UGdouble& x, 
			const UGdouble& y, 
			const UGdouble& z,
			const UGUVWs& uvws = UGUVWs(), 
			const UGAttribute& iAttribute = UGAttribute());

		//! \brief 插入顶点。
		//! \param vertex [in] 顶点坐标。
		//! \param uvw [in] 纹理坐标。
		//! \param iAttribute [in] 顶点属性。
		void InsertVertex(
			const UGVector3d& vertex, 
			const UGUVWs& uvws = UGUVWs(), 
			const UGAttribute& iAttribute = UGAttribute());

		//! \brief 插入顶点。
		//! \param pt [in] 顶点结构。
		void InsertVertex(const UGPolyPoint& pt);

		//! \brief 追加多边形集合。
		void Append(const UGPolySet& ps);

		//! \brief 追加多个多边形。
		void AppendPolys(const UGPolys& polys);

	public:
		//! \brief 平移。
		//! \param vecTranslation [in] 平移向量。
		void Translate(const UGVector3d& vOffset);
		void Translate(const UGdouble& x, const UGdouble& y, const UGdouble& z);

		//! \brief 旋转。
		//! \param vecNormal [in] 目标平面法向量。
		//! \attention 参数必须为单位向量。
		void Rotate(const UGVector3d& vNormal);
		void Rotate(const UGVector3d& vToNormal, const UGVector3d& vFromNormal);

		//! \brief 缩放。
		//! \param vecScale [in] 缩放参数。
		void Scale(const UGVector3d& vScale);
		void Scale(const UGdouble& x, const UGdouble& y, const UGdouble& z);

		//! \brief 镜像。
		//! \param normal [in] 镜像参照向量。
		void Mirror(const UGPlane& plane);

		//! \brief 翻转。
		void Reverse(UGbool bSplitFirst = TRUE);

		//! \brief 位置量。
		void SetPosition(const UGVector3d& vPos) { m_vPosition = vPos; }
		void SetPosition(UGdouble dX, UGdouble dY, UGdouble dZ) { m_vPosition = UGVector3d(dX, dY, dZ); }
		const UGVector3d& GetPosition() const { return m_vPosition; }
		UGVector3d& GetPosition() { return m_vPosition; }

		//! \brief 缩放量。
		void SetScale(const UGVector3d& vScale) { m_vScale = vScale; }
		void SetScale(UGdouble dX, UGdouble dY, UGdouble dZ) { m_vScale = UGVector3d(dX, dY, dZ); }
		const UGVector3d& GetScale() const { return m_vScale; }
		UGVector3d& GetScale() { return m_vScale; }

		//! \brief 旋转量。
		void SetRotate(const UGVector3d& vRotate) { m_vRotate = vRotate; }
		void SetRotate(UGdouble dX, UGdouble dY, UGdouble dZ) { m_vRotate = UGVector3d(dX, dY, dZ); }
		const UGVector3d& GetRotate() const { return m_vRotate; }
		UGVector3d& GetRotate() { return m_vRotate; }

		//! \brief 设置多边形集合矩阵
		//! \param matTransform [in] 模型矩阵
		void Transform(const UGMatrix3d& matTransform);
		void Transform(const UGMatrix4d& matTransform);

		//! \brief 获取已经三角化的模型顶点和索引。
		//! \param pVertexDataPackage [out] 模型顶点数组。
		//! \param arrIndexPackage [out] 模型索引。
		//! \param bSimple [in] 是否精简顶点数组。
		//! \attention 传入参数pVertexDataPackage不能为NULL。
		void GetTriMesh(UGVertexDataPackage* pVertexDataPackage, UGArray<UGIndexPackage*>& arrIndexPackage) const;
		void GetTriMesh(UGVertexDataPackageExact* pVertexDataPackage, UGArray<UGIndexPackage*>& arrIndexPackage) const;

		void GetTriMesh(std::vector<UGTINPoint>& arrPts, std::vector<UGuint>& arrIds) const;

		UGbool FromTriMesh(const UGVertexDataPackage* pVertexDataPackage, const UGArray<UGIndexPackage*>& arrIndexPackage);
		UGbool FromTriMesh(const UGVertexDataPackageExact* pVertexDataPackage, const UGArray<UGIndexPackage*>& arrIndexPackage);

		UGbool FromTriMesh(const std::vector<UGTINPoint>& arrPts, const std::vector<UGuint>& arrIds, UGbool bHasUV = FALSE, UGbool bIgnoreInvalid = FALSE);

		//! \brief 曲度细分。
		//! \param dGranularity [in] 细分粒度，经纬度按度数，平面坐标按米。
		//! \return -1代表错误，0代表未修改，1代表已修改。
		UGOperationResult ComputeLineSubDivision(UGdouble dGranularity = DTOR);
		UGOperationResult ComputeRegionSubDivision(UGdouble dGranularity = DTOR);
		UGOperationResult ComputeTINSubDivision(UGdouble dGranularity = DTOR);

		//! \brief 简化曲度。
		//! \param dGranularity [in] 简化粒度，按度数。
		//! \return -1代表错误，0代表未修改，1代表已修改。
		UGOperationResult ReduceLineSubDivision(UGdouble dGranularity = DTOR);
		UGOperationResult ReduceRegionSubDivision(UGdouble dGranularity = DTOR);
		UGOperationResult ReduceTINSubDivision(UGdouble dGranularity = DTOR);

		//! \brief 对齐网格。
		void QuantizeVertices();
		
		//! \brief 三角化多面体。
		UGbool Tessellate();

		//! \brief 三角化空间中的平面多边形。
		UGbool TessellateFace();

		//! \brief 带约束边的剖分(二维)。
		//! \param arrIndex [out] 三角剖分后的索引值。
		//! \param arrVertice [in / out] 顶点数组，在约束边相交的情况下会都增加点。
		//! \param idsEdge [in] 构成约束边的顶点索引。
		static UGbool FreeDelaunay(
			std::vector<UGuint>& arrIndex,
			UGPoly& arrVertice, 
			const std::vector<UGuint>& idsEdge);
		static UGbool FreeDelaunay(
			std::vector<UGuint>& arrIndex,
			UGPoly& arrVertice, 
			const UGIndexPairs& idsEdgePairs = UGIndexPairs());

		//! \brief 根据模型顶点和索引构建多边形集合。
		//! \param pVertexDataPackage [in] 模型顶点数组。
		//! \param arrIndexPackage [in] 模型索引。
		//! \attention 返回的指针需要由用户释放，否则会造成内存泄露。
		static UGPolySet* Make(const UGVertexDataPackage* pVertexDataPackage, const UGArray<UGIndexPackage*>& arrIndexPackage);
		static UGPolySet* Make(const UGVertexDataPackageExact* pVertexDataPackage, const UGArray<UGIndexPackage*>& arrIndexPackage);

		//! \brief 由边界和内部控制点构建闭合体。
		//! \param vecBorderPt [in] 外边界点，逆时针顺序。
		//! \param vecInnerPt [in] 内部点。
		//! \param dZ [in] 构成闭合体的z值高度。
		//! \return 返回快速建模模型结构指针。
		static UGPolySet* Make(const UGdouble& dZ, UGPoly& vecBorderPt, const UGPoly& vecInnerPt = UGPoly());

		//! \brief 由边界和内部控制点构建三角网。
		//! \param vecBorderPt [in] 外边界，逆时针顺序。
		//! \param vecInnerPt [in] 内部控制点。
		//! \return 返回快速建模模型结构指针。
		static UGPolySet* Make(const UGPoly& vecBorderPt, const UGPoly& vecInnerPt = UGPoly());

		//! \brief 由TIN顶点和索引构建三角网。
		static UGPolySet* Make(const std::vector<UGTINPoint>& arrPts, const std::vector<UGuint>& arrIds, UGbool bHasUV = FALSE, UGbool bIgnoreInvalid = FALSE);

		//! \brief 场景模型UGModelNode与快速建模模型的相互转换。
		//! \remark 参数bHighPrecision表示是否采用高精度，TRUE表示采用double双精度，FALSE表示采用float单精度。
		static UGModelNode* CreateModelNodeFromPolySet(const UGPolySet* pPolySet, UGbool bHighPrecision = TRUE);
		static UGModelGeode* CreateGeodeFromPolySet(const UGPolySet* pPolySet, UGbool bHighPrecision = TRUE);
		static UGModelSkeleton* CreateSkeletonFromPolySet(const UGPolySet* pPolySet, UGbool bHighPrecision = TRUE);

		static UGPolySet* CreatePolySetFromModelNode(/*const */UGModelNode* pModelNode);
		static UGPolySet* CreatePolySetFromPatch(UGModelPagedPatch* pPatch);
		static UGPolySet* CreatePolySetFromGeode(UGModelGeode* pGeode);
		static UGPolySet* CreatePolySetFromSkeleton(/*const */UGModelSkeleton* pSkeleton, const UGMatrix4d& mGeodeTrans = UGMatrix4d());
		static UGPolySet* CreatePolySetFromROGeometry(/*const */UGRenderOperationGeometry* pGeometry, const UGMatrix4d& mGeodeTrans = UGMatrix4d());

		UGbool FromSkeleton(UGModelSkeleton* pSkeleton);
		UGbool ToSkeleton(UGModelSkeleton* pSkeleton) const;

		//! \brief 从点云中创建三角网。
		static UGPolySet* CreatePolySetFromPtCloud(UGPoly& pts);

	public:
		//! \brief 线性拉伸。
		//! \remark 又称 拉伸、挤出，将XY平面上的二维多边形沿着指定方向拉伸生成三维模型。
		virtual UGbool LinearExtrude(const UGLinearExtrudeParam& param, UGPolySetObjs& arrObjs);

		//! \brief 旋转拉伸。
		//! \remark 又称 转体、车削，将XY平面上的二维多边形沿着指定角度旋转生成三维模型。
		virtual UGbool RotateExtrude(const UGRotateExtrudeParam& param, UGPolySetObjs& arrObjs);

		//! \brief 管状拉伸。
		//! \remark 又称 放样，将XY平面上的二维多边形沿着指定线路放样生成三维模型。
		virtual UGbool PipeExtrude(const UGPoly& spine, const UGPipeExtrudeParam& param, UGPolySetObjs& arrObjs);

		//! \brief 放样。
		//! \param pCrossSections [in] 放样横截面。
		//! \param vecLoftPath [in] 放样路径点。
		//! \param bCreateTexCoordight [in] 是否生成纹理坐标
		//! \param uChamferFactor [in] 平滑参数，取值范围[0, 100]。
		//! \param bIsSym [in] FALSE为椭圆弧平滑，TRUE为圆弧平滑。
		virtual UGbool Loft(
			const std::vector<UGVector3d>& vecLoftPath, 
			UGbool bCreateTexCoord = false,
			UGuint uChamferFactor = 10, 
			UGbool bIsSym = FALSE);

		//! \brief 3D曲面建模。
		//! \param bParametric [in] 是否参数化建模。
		//! \return 3D曲面模型。
		virtual UGbool MakeSurfaceMesh(const UGSurfaceMeshParam& param);

		//! \brief 融合。
		//! \param arrPolySets [in] 被融合对象。
		//! \param bAddPolys [in] 是否封上三角网。
		virtual UGbool Blend(
			const UGPolySets& arrPolySets,
			UGbool bAddPolys = TRUE);

		//! \brief 平面投影。
		//! \remark 在XY平面的投影。
		virtual UGbool Project(UGbool bCut = FALSE, const UGPlane& pPlane = UGPlane::PlaneXY);

		//! \brief 2.5维出图。
		virtual UGbool Project2Dot5Map(const UGPlane& plane);

		//! \brief 平面投影体。
		//! \param vecDirection [in] 投影方向/投射视点位置。
		//! \param bPerspective [in] 是否采用透视投影。
		//! \param projectType [in] 投影后的结果类型。
		//! \return 返回阴影体计算是否成功。
		virtual UGbool ShadowVolume(UGVector3d vecDirection, UGShadowVolumeType shadowType, UGProjectResultType projectType = shadowVolume);
		
		//! \brief 拓展多边形(二维)。
		//! \remark 仅针对二维多边形。
		//! \param dOffset [in] 拓展距离。
		//! \param type [in] 外拓边衔接类型。
		virtual UGbool PolygonOffsetting(UGdouble dOffset, UGJoinType type = Miter);

		//! \brief 模型轮廓。
		//! \param dZ [in] 构成闭合体的z值高度。
		//! \param dTolarence [in] 高度误差范围
		//! \param bHasTextureCoord [in]是否保留边界纹理坐标。
		virtual UGbool Envelop(UGdouble dZ, UGdouble dTolarence = 0.0, UGbool bHasTextureCoord = FALSE);

		//! \brief 多边形布尔运算(二维)。
		UGbool BooleanOperation2D(const UGPolySet& ps, UGBooleanOperationType opType);

	public:
		//! \brief 修改模型。
		//! \param pOperationPolys [in] 修改参照数据。
		//! \param params [in] 修改参数。
		//! \param pClipedTris [in] 是否保留被修改掉的三角网。
		//! \param dTol [in] 容差，影响精度和性能。
		//! \return failure代表操作失败，nothing代表全部保留，success代表更新。
		UGOperationResult Operate(
			const void* pOperationPolys,
			const UGOperationParam* pOperationParam,
			UGPolySet* pReserve = NULL,
			UGdouble dTol = EP);

		//! \brief 影响模型。
		UGOperationResult Effect(
			void* pEffectPolys,
			const UGOperationParam* pEffectParam,
			UGPolySet* pReverse = NULL,
			UGdouble dTol = EP);

	private:
		//! \brief 裁剪三角网。
		//! \param arrClipPolys [in] 裁剪面集。
		//! \param params [in] 裁剪参数。
		//! \param pClipedTris [in] 是否保留被裁剪掉的三角网。
		//! \param dTol [in] 容差，影响精度和性能。
		//! \return failure代表操作失败，nothing代表全部保留，success代表部分保留或者全被裁剪掉。
		UGOperationResult OperateSurface(
			const UGClipPolys& arrClipPolys,
			const UGClipOperationParam& param,
			UGPolySet* pClipedTris = NULL,
			UGdouble dTol = EP);

		//! \brief 剔除三角网。
		//! \param arrCullPolys [in] 剔除面集。
		//! \param params [in] 剔除参数。
		//! \param pCulledTris [in] 是否保留被剔除掉的三角网。
		//! \param dTol [in] 容差，影响精度和性能。
		//! \return failure代表操作失败，nothing代表全部保留，success代表剔除成功。
		UGOperationResult OperateSurface(
			const UGPolySets& arrCullPolys,
			const UGCullOperationParam& param,
			UGPolySet* pCulledTris = NULL,
			UGdouble dTol = EP);

	public:
		//! \brief 裁剪。
		//! \param arrClipPolys [in] 裁剪面。
		//! \param eKeepType [in] 保留类型。
		//! \param eConstraintType [in] 约束类型。
		//! \param pClipedTris [in] 是否保留被裁剪掉的三角形。
		//! \param dTol [in] 容差，影响精度和速度。
		//! \return failure代表操作失败，nothing代表未处理，success代表更新。
		UGOperationResult Clip(
			const UGPolySets& arrClipPolys, 
			UGKeepType eKeepType = KeepInside, 
			UGConstraintType eConstraintType = SoftConstraint,
			UGPolySet* pClipedTris = NULL,
			UGdouble dTol = EP);

		//! \brief 压平。
		//! \param arrFlattenPolys [in] 压平面。
		//! \param eKeepType [in] 保留类型。
		//! \param eConstraintType [in] 约束类型。
		//! \param dFlattenHigh [in] 压平高度。
		//! \param dTol [in] 容差，影响精度和速度。
		//! \return failure代表操作失败，nothing代表未处理，success代表更新。
		UGOperationResult Flatten(
			const UGPolySets& arrFlattenPolys,
			UGKeepType eKeepType = KeepInside, 
			UGConstraintType eConstraintType = SoftConstraint,
			UGdouble dFlattenHigh = 0.0,
			UGdouble dTol = EP);

		//! \brief 替换。
		//! \param arrReplacePolys [in] 替换面。
		//! \param eConstraintType [in] 约束类型。
		//! \param pReplacedTris [in] 是否保留被替换掉的三角形。
		//! \param dTol [in] 容差，影响精度和速度。
		//! \return failure代表操作失败，nothing代表未处理，success代表更新。
		UGOperationResult Replace(
			const UGPolySets& arrReplacePolys,
			UGConstraintType eConstraintType = SoftConstraint,
			UGPolySet* pReplacedTris = NULL,
			UGdouble dTol = EP);

		//! \brief 镶嵌。
		//! \param arrMosaicPolys [in] 镶嵌面。硬约束。
		//! \param arrRefOffsettings [in] 参考护坡面。生成带护坡的镶嵌面时必须指定。
		//! \param dSlopeRadius [in] 坡度缓冲距离，可选项，优化性能。
		//! \param arrOffsettings [in] 护坡面。软约束。可选项，优化性能。
		//! \param dTol [in] 容差，影响精度和速度。
		//! \return failure代表操作失败，nothing代表未处理，success代表更新。
		UGOperationResult Mosaic(
			const UGPolySets& arrMosaicPolys,
			const UGPolySets& arrRefOffsettings = UGPolySets(),
			UGdouble dSlopeRadius = 0.0,
			const UGPolySets& arrOffsettings = UGPolySets(),
			UGdouble dTol = EP);

		//! \brief 插值高度。
		//! \param arrInterPolys [in] 待插值数据。
		//! \return failure代表操作失败，nothing代表未处理，success代表更新。
		UGOperationResult InterpHigh(
			const UGPolySets& arrInterpPolys,
			const UGMatchOperationParam& param) const;

	private:
		//! \brief 分离范围内网的三角形。
		static UGbool SplitPolysInBounds(UGPolyList& in, UGPolyList& out, const UGPolys& polys, const UGRect2D& bound);

		//! \brief 拟合硬约束面。
		UGbool FitSurface(
			const UGPolySets& arrHardPolys);

		//! \brief 拟合软硬约束面。
		UGbool FitSurface(
			const UGPolySets& arrHardPolys,
			const UGPolySets& arrSoftPolys);

		 //! \brief 插值高度。
		UGOperationResult InterpHigh(
			std::vector<UGdouble>& vecZValues,
			const std::vector<UGPoint2D>& vecXYValues,
			const UGMatchOperationParam& param,
			const UGRect2D& rcXYBounds = UGRect2D()) const;

	public:
		//! \brief 提取三角网的边界点。
		//! \remark 将不规则三角网的边界点构成首尾闭合的边界区域。
		//! \param bHasTextureCoord [in]是否保留边界纹理坐标。
		//! \attention 返回的闭合边界，岛洞之间可能没有逻辑包含关系，可以使用保护性分解。
		UGPolySet* ExtractBorderFromTIN(UGbool bHasTextureCoord = FALSE) const;

		//! \brief 从点集中提取边界点和内部点。
		static UGbool ExtractPtAttribute(UGPoly& pts, std::vector<UGuint>& borderPtIds, std::vector<UGuint>& innerPtIds);

		//! \brief 提取回路。
		//! \brief edgeIndex [in] 线段端点索引号对应表。
		//! \brief pVert [in] 顶点数组。
		static UGPolySet* ExtractLoops(
			const std::vector<UGuint>& indices, 
			const UGPolyPoint* pPt, 
			UGuint nPtCount, 
			UGbool bHasTextureCoord = FALSE);
		static UGPolySet* ExtractLoops(
			const std::vector<std::vector<UGuint> >& edgeIdsLoops, 
			const UGPolyPoint* pPt, 
			UGuint nPtCount, 
			UGbool bHasTextureCoord = FALSE);

		//! \brief 根据圆的半径和片段数量生成圆。
		//! \param pCircle [out] 生成圆的点表。
		//! \param dRadius [in] 圆的半径。
		//! \param nFragments [in] 片段数量。
		//! \attention 传入参数pCircle不能为NULL，需要用户申请数量为nFragments大小的数组。
		static void GenerateCircle(UGVector3d* pCircle, const UGdouble& dRadius, UGuint nFragments);

		//! \brief 根据椭圆轴半径和片段数量生成椭圆。
		//! \param pEllipse [out] 生成椭圆的点表。
		//! \param dPriAxis [in] 椭圆在X轴的半径。
		//! \param dThiAxis [in] 椭圆在Y轴的半径。
		//! \param nFragments [in] 片段数量。
		//! \attention 传入参数pEllipse不能为NULL，需要用户申请数量为nFragments大小的数组。
		static void GenerateEllipse(UGVector3d* pEllipse, const UGdouble& dPriAxis, const UGdouble& dThiAxis, UGuint nFragments);

		//! \brief 计算一定顺序的三点坐标的法线。
		//! \param p0 [in] 坐标点0。
		//! \param p1 [in] 坐标点1。
		//! \param p2 [in] 坐标点2。
		//! \attention 传入点顺序按顺时针或者逆时针。
		static UGVector3d CalculateNormal(const UGVector3d& p0, const UGVector3d& p1, const UGVector3d& p2);

		//! \brief 计算多边形法线(二维和三维)。
		//! \brief 适用于二维和三维多边形。
		//! \param bZValAsAttribute [in] 顶点Z值是否作为属性。
		UGVector3d GetPolyNormal() const;
		static UGVector3d GetPolyNormal(const UGPoly& poly, UGbool bZValAsAttribute = FALSE);

		//! \brief 计算多边形中心点(二维和三维)。
		//! \remark 适用于二维和三维多边形。
		UGVector3d GetPolyCenter() const;
		static UGVector3d GetPolyCenter(const UGPoly& poly);

		//! \brief 计算三维多面体中心点。
		const UGVector3d& GetPolyhedronCenter() const;

		//! \brief 计算多面体的容积。
		//! \brief 模型是闭合体才有意义。
		UGdouble GetPolyhedronVolume() const;

		//! \brief 折线拐角抛光平滑处理。
		//! \param vecCurved [out] 抛光平滑后的点。
		//! \param vecLine [in] 折线路径。
		//! \param uFactor [in] 平滑参数，取值范围[0, 100]。
		//! \param bIsSym [in] FALSE为椭圆弧平滑，TRUE为圆弧平滑。
		static void Chamfer(std::vector<UGVector3d>& vecCurved, 
			const std::vector<UGVector3d>& vecLine, 
			UGuint uFactor = 10, 
			UGbool bIsSym = FALSE,
			UGuint nSlice = 5);

		//! \brief 是否为带岛或者带洞的多边形。
		UGbool IsComplexPolygon() const { return m_nDimension == 2 && GetNumPolys() > 1; }

		//! \brief 返回多边形包围盒。
		static UGBoundingBox GetBoundingBox(const UGPoly& polygon);

		//! \brief 返回多面体包围盒。
		const UGBoundingBox& GetBoundingBox() const;

		//! \brief 获取多面体包围球。
		UGBoundingSphere GetBoundingSphere() const;

		//! \brief 返回范围。
		UGRect2D GetBounds() const;
		static UGRect2D GetBounds(const UGPoly& poly);
		static UGRect2D GetBounds(const UGPolys& polys);

		//! \brief 返回最大和最小Z值。
		void GetHigh(UGdouble& dMax, UGdouble& dMin) const;

		//! \brief 判断两个包围盒是否相交。
		static UGbool IsIntersected(const UGBoundingBox& box1, const UGBoundingBox& box2);

		//! \brief 导入文件。
		UGbool ImportOFF(const UGString& strFileName);
		UGbool ImportDXF(const std::string& path);

		//! \brief 导出文件。
		UGbool ExportOFF(const UGString& strFileName) const;
		UGbool ExportDXF(const std::string& path) const;

		//! \brief 输出UGPolySet数据。
		UGbool Save(const std::string& strFilePath) const;

		//! \brief 判断多边形凸凹性(二维和三维)。
		//! \remark 适用于二三维多边形。
		//! \param poly [in] 多边形。
		//! \param bZValAsAttribute [in] 是否将z值作为属性不参与运算。
		//! \return TRUE为凸多边形，FALSE为凹多边形。
		static UGbool IsConvex(const UGPoly& poly, UGbool bZValAsAttribute = FALSE);

		//! \brief 计算多边形凸包。
		//! \param poly [in] 多边形。
		//! \param bZValAsAttribute [in] 是否将z值作为属性不参与运算。
		static UGPoly CalculateConvexHull(const UGPoly& polygon2d, UGbool bZValAsAttribute = FALSE);

		//! \brief 判断多边形的顺序(二维和三维)。
		//! \param poly [in] 多边形。
		//! \param bZValAsAttribute [in] 是否将z值作为属性不参与运算。
		//! \return 逆时针返回TRUE，顺时针返回FALSE。
		static UGbool IsCCWContour(const UGPoly& poly, UGbool bZValAsAttribute = FALSE);

		//! \brief 返回一个圆的数量切片。
		//! \param dRadius [in] 圆的半径。
		//! \param nResolution [in] 分辨率。
		//! \param nFragmentSize [in] 片段大小，单位毫米。
		//! \param nFragmentAngle [in] 片段角度，单位度。
		static UGuint GetFragments(const UGdouble& dRadius, 
			UGuint nResolution = 0, UGuint nFragmentSize = 2, UGuint nFragmentAngle = 12);

		//! \brief 空间中的三个点是否能构成三角形。
		static UGbool IsTriangle(const UGVector3d& p0, const UGVector3d& p1, const UGVector3d& p2);

		//! \brief 判断三个点是否共线。
		static UGbool IsColinear(const UGVector3d& p0, const UGVector3d& p1, const UGVector3d& p2);

		//! \brief 判断多边形是否共面。
		static UGbool IsCoplanar(UGPlane& plane, const UGPolys& polys);

		//! \brief 获取多边形的面积(二维)。
		static UGdouble GetPolyArea(const UGPoly& poly);
		//! \brief 获取多边形的面积(三维)。
		UGdouble GetPolyArea() const;

		//! \brief 计算两条线段的交点(二维)。
		//! \param resultPt [out] 交点坐标。
		//! \param line0Pt0 [in] 线段0的顶点0坐标。
		//! \param line0Pt1 [in] 线段0的顶点1坐标。
		//! \param line1Pt0 [in] 线段1的顶点0坐标。
		//! \param line1Pt1 [in] 线段1的顶点1坐标。
		//! \param bCross [in] 仅计算相互跨越的交点。
		//! \return 返回两条线段是否相交。
		//! \remark 当两条线段相交时，返回的交点坐标才有意义。
		static UGbool IntersectLineSegs(
			UGVector3d& resultPt,
			const UGVector3d& line0Pt0,
			const UGVector3d& line0Pt1,
			const UGVector3d& line1Pt0,
			const UGVector3d& line1Pt1,
			UGbool bCross = FALSE);

		//! \brief 判断点是否在线段上(三维)。
		static UGbool PointInLine(const UGVector3d& pt, const UGVector3d& linePt0, const UGVector3d& linePt1);

		//! \brief 判断点是否在多边形内部(二维)。
		//! \param pt [in] 待判断点。
		//! \param poly [in] 简单多边形。
		//! \param bBorderPt [in] 点在多边形边界上属于内还是属于外, 为true时，属于内，为false时，属于外。
		//! \return 点在多边形内返回true，否则返回false。
		static UGbool PointInPolygon(const UGVector3d& pt, const UGPoly& poly, UGbool bBorderPt = TRUE);
		//! \brief 判断点是否在多边形内部(二维)。
		//! \param pt [in] 待判断点。
		//! \param poly [in] 简单多边形或者复杂多边形(一个大岛中包含N个小洞和小岛)。
		//! \param bBorderPt [in] 点在多边形边界上属于内还是属于外, 为true时，属于内，为false时，属于外。
		//! \return 点在多边形内返回true，否则返回false。
		static UGbool PointInPolygonWithHoles(const UGVector3d& pt, const UGPolys& polys, UGbool bBorderPt = TRUE);

		//! \brief 线段是否在多边形(二维)。
		static UGbool LineInPolygon(const UGVector3d& startPt, const UGVector3d& endPt, const UGPoly& poly, UGbool bBorderPt = TRUE);
		static UGbool LineInPolygonWithHoles(const UGVector3d& startPt, const UGVector3d& endPt, const UGPolys& polys, UGbool bBorderPt = TRUE);

		//! \brief 点是否在多边形边上(二维)。
		static UGbool PointInPolygonBorder(const UGVector3d& pt, const UGPoly& poly);
		static UGbool PointInPolygonBorderWithHoles(const UGVector3d& pt, const UGPolys& polys);

		//! \brief 线段是否在多边形边上(二维)。
		static UGbool LineInPolygonBorder(const UGVector3d& startPt, const UGVector3d& endPt, const UGPoly& poly);
		static UGbool LineInPolygonBorderWithHoles(const UGVector3d& startPt, const UGVector3d& endPt, const UGPolys& polys);

		//! \brief 线段是否与多边形相离(二维)。
		static UGbool LineDisjointPolygon(const UGVector3d& startPt, const UGVector3d& endPt, const UGPoly& poly);

		//! \brief 判断多边形Poly2是否包含多边形Poly1(二维)。
		//! \param poly1 [in] 简单多边形1。
		//! \param poly2 [in] 简单多边形2。
		//! \param bFully [in] 是否严格包含。
		//! \remark 参数为TRUE时，多边形1的点必须都在多边形2内部，在边界上返回FALSE。
		static UGbool Poly2ContainsPoly1(const UGPoly& poly1, const UGPoly& poly2, UGbool bFully = FALSE);
		//! \brief 判断多边形Poly2是否包含多边形Poly1(二维)。
		//! \param poly1 [in] 多边形1。
		//! \param poly2 [in] 简单或复杂多边形2(复杂多边形指一个大岛中包含N个小洞和小岛)。
		//! \param bFully [in] 是否严格包含。
		//! \remark 参数为TRUE时，多边形1的点必须都在多边形2内部，在边界上返回FALSE。
		static UGbool Poly2WithHolesContainsPoly1(const UGPoly& poly1, const UGPolys& polys2, UGbool bFully = FALSE);

		//! \brief 两条线段是否相互跨越(二维)。
		static UGbool LineCrossLine(const UGVector3d& line0Pt0, const UGVector3d& line0Pt1, const UGVector3d& line1Pt0,const UGVector3d& line1Pt1);

		//! \brief 两条线段是否平行(三维)。
		static UGbool LineParallelLine(const UGVector3d& line0Pt0, const UGVector3d& line0Pt1, const UGVector3d& line1Pt0,const UGVector3d& line1Pt1);

		//! \brief 线段是否跨越多边形(二维)。
		static UGbool LineCorssPoly(const UGVector3d& startPt, const UGVector3d& endPt, const UGPoly& poly, UGbool bBorderPt = TRUE);
		static UGbool LineCorssPolyWithHoles(const UGVector3d& startPt, const UGVector3d& endPt, const UGPolys& polys, UGbool bBorderPt = TRUE);

		//! \brief 多边形是否相互跨越(二维)。
		static UGbool Poly2CrossPoly1(const UGPoly& poly1, const UGPoly& poly2, UGbool bBorderPt = TRUE);
		static UGbool Poly2WithHolesCrossPoly1(const UGPoly& poly1, const UGPolys& polys2, UGbool bBorderPt = TRUE);

		//! \brief 多边形是否相互接触(二维)。
		static UGbool Poly2TouchPoly1(const UGPoly& poly1, const UGPoly& poly2);
		static UGbool Poly2WithHolesTouchPoly1(const UGPoly& poly1, const UGPolys& polys2);

		//! \brief 保护性分解，分离岛洞。
		//! \attention 返回的数组包含的指针需要用户释放，否则会造成内存泄漏。
		//! \param arrOutPolySet [out] 返回分解后的结果。
		void ProtectedDecompose(UGPolySets& arrOutPolySet) const;

		//! \brief 计算空间点到空间直线的距离。
		//! \param linePt0 [in] 直线上的点Pt0。
		//! \param linePt1 [in] 直线上的点Pt1。
		//! \param pt [in] 空间上一点。
		//! \return 距离。
		static UGdouble Distance(const UGVector3d& linePt0, const UGVector3d& linePt1, const UGVector3d& pt);
		static UGdouble Distance2(const UGVector3d& linePt0, const UGVector3d& linePt1, const UGVector3d& pt);

		//! \brief 参数化对象构建。
		virtual void CreateGeometry();

		//! \brief 插值计算三角形内一点的纹理坐标值。
		//! \remark 已知三个顶点和顶点上的纹理坐标以及顶点内的一个顶点坐标，插值计算纹理坐标。
		static UGPolyPoint Interp(const UGPolyPoint& pt0, const UGPolyPoint& pt1, const UGPolyPoint& pt2, const UGVector3d& v);

		//! \brief 创建投影后的体数据。
		//! \param pTopPolySet [in] 投影上表面。
		//! \param pBotPolySet [in] 投影下表面。
		//! \param pDesPolySet [in/out] 投影后体数据。
		//! \return 返回是否创建成功。
		static UGbool CreateProjectVolume(UGPolySet* pTopPolySet, UGPolySet* pBotPolySet, UGPolySet* pDesPolySet);

		//! \brief 生成纹理坐标。
		UGbool GenerateTexCoord();

	private:
		//! \brief 按照面法线将面分包。
		//! \param package [out] 三角面分包数据。
		//! \param dAngle [in] 面法线容差度数，取值范围[0, 180]。
		UGbool SplitPolysPackage(UGPolysPackage& package, UGdouble dAngle = 0.0) const;

		//! \brief 按照拓扑连接性将面分包。
		UGbool SplitPolysPackage(std::list<UGPolyList>& package) const;

	public:
		//! \brief 去除无效三角面，前提必须是已经三角化。
		UGOperationResult RemoveInvalidPoly();

		//! \brief 移除重复的面。
		//! \return 返回-1代表操作失败，0代表没有重复的面，1代表已经语移除成功。
		UGOperationResult RemoveDuplicatePoly();

		//! \brief 校正拓扑。
		//! \return 返回-1代表操作失败，0代表没有可校正的面，1代表已经校正成功。
		UGOperationResult CorrectTopology();

	private:
		//! \brief 矩形。
		void CreateRectGeometry();

		//! \brief 圆形。
		void CreateCircleGeometry();

		//! \brief 椭圆形。
		void CreateEllipseGeometry();
		
		//! \brief 立方体。
		void CreateBoxGeometry();

		//! \brief 圆柱体。
		void CreateCylinderGeometry();

		//! \brief 椭球体。
		void CreateEllipsoidGeometry();

		//! \brief 金字塔。
		void CreatePyramidGeometry();

		//! \brief 球体。
		void CreateSphereGeometry();

	private:
		//! \brief 盒子方式投影生成纹理坐标。
		UGbool GenerateBoxMapping();
		UGbool GenerateBoxMappingWithRealMapSize();
		UGbool GenerateBoxMappingApplyInCATIA();

		//! \brief 球形方式投影生成纹理坐标。
		UGbool GenerateSphericalMapping();

		//! \brief 圆柱方式投影生成纹理坐标。
		UGbool GenerateCylindricalMapping();

		//! \brief 收缩环绕方式投影生成纹理坐标。
		UGbool GenerateShrinkWrapMapping();

		//! \brief 按面方式投影生成纹理坐标。
		UGbool GenerateFaceMapping();

		//! \brief 平面方式投影生成纹理坐标。
		UGbool GeneratePlanarMapping();

		//! \brief 将顶点坐标直接作为纹理坐标。
		UGbool GenerateXYZ2UVWMapping();

	private:
		//! \brief 添加边。
		void AddSlice(const UGPolySet* pPolygon2d,
			const UGdouble& dRotate1, const UGdouble& dRotate2, 
			const UGdouble& dHeight1, const UGdouble& dHeight2, 
			const UGVector3d& vScale1, const UGVector3d& vScale2, UGbool bClosedRing);

		//! \brief 获取侧面拉伸点信息
		UGPolyPoint GetSlicePoint(const UGPolyPoint& srcPoint, UGdouble dHeight, const UGMatrix4d& matTran, UGbool bBotom = FALSE);

		//! \brief 扩充环。
		void FillRing(const UGPoly& outline2d,
			std::vector<UGVector3d>& ring, 
			const UGdouble& dAngel,
			UGbool bFlip) const;

		//! \brief 放置多边形。
		//! \param vControlPoint [in] 放置点坐标。
		//! \param vLineTanget [in] 放置点处切线。
		void PlacePolygon(const UGVector3d& vControlPoint, const UGVector3d& vLineTanget);

		//! \brief 投影多边形。
		//! \param vLoftTanget [in] 放样点切线向量。
		//! \param vProjPoint [in] 投影点坐标。
		//! \param vProjNormal [in] 投影点所在投影平面法线。
		void ProjectPolygon(const UGVector3d& vLoftTanget, const UGVector3d& vProjPoint, const UGVector3d& vProjNormal);

		//! \brief 获取在XY平面上的投影结果（未求并）。
		//! \param vecDirection [in] 投影方向/投射视点位置。
		//! \param bPerspective [in] 是否采用透视投影。
		//! \return 返回投影后数据。
		UGPolySet* GetProjectPlaneXY(UGVector3d vecDirection, UGShadowVolumeType shadowType) const;

		//! \brief 提升投影后顶点精度。
		//! \param destPolys [in] 需要修改的投影数据。
		//! \param srcPolys [in] 原始参照的投影数据。
		//! \remark 目前只在Project函数应用。
		void ImproveProjectData(UGPolys& destPolys, const UGPolys& srcPolys) const;

		//! \brief 生成侧面纹理坐标
		//! \param vecShapes [in] 拉伸体截面几何体数组。
		//! \param param [in] 图片纹理参数。
		UGbool GenerateSliceTexCoord(UGPolySets& arrSides);
		
		//! \brief 计算矢量面各顶点纹理坐标。
		//! \param poly [in] 拉伸体底面几何体。
		//! \param arrTexCoordsOffset [out] 各顶点纹理坐标
		//! \param dSubLength [in] 矢量面总长度
		void ComputeTexOffsetAndVertexLength(UGPoly& poly, UGArray<UGdouble>& arrTexCoordsOffset, UGdouble& dSubLength);

		//! \brief 获取对象公共方法。
		static void Copy(UGPolySet& des, const UGPolySet& src, UGbool bShallowCopy = FALSE);
		static void CopyParam(UGPolySet& des, const UGPolySet& src);

	protected:
		//! \brief 多边形集合存储的模型所属维度。
		UGuint m_nDimension;

		//! \brief 切片数量。
		UGuint m_nFragments;

		//! \brief 是否拓扑校正(二维)。
		mutable UGbool m_bSanitized;

		//! \brief 是否三角化。
		mutable UGbool m_bTriangulated;

		//! \brief 是否有w属性。
		UGbool m_bHasW;

		//! \brief 模型顶点类型。
		UGCoordinatesType m_eCoordsType;

		//! \brief 参数化对象参数类。
		UGPrimitiveParam m_priParam;

		//! \brief 纹理贴图参数类。
		UGUVWMapParam m_uvwParam;

		//! \brief 多面体凹凸类型(三维)。
		Type m_eType;

		//! \brief 位置插值。
		UGVector3d m_vPosition;
		//! \brief 旋转插值。
		UGVector3d m_vRotate;
		//! \brief 缩放插值。
		UGVector3d m_vScale;

		//! \brief 名称。
		UGString m_strName;
		
		//! \brief 是否生成法线。
		UGbool m_bGenerateNormal;

		//! \brief 是否生成纹理坐标。
		UGbool m_bGenerateMappingCoords;

		//! \brief 是否生成颜色。
		UGbool m_bGenerateColor;

		//! \brief 是否将z值作为属性不参与运算。
		UGbool m_bZValAsAttribute;

		//! \brief 数据包的智能指针。
		UGPolysDataPackageRefPtr m_pPolysDataPackage;
	};
}
#endif // !defined(AFX_UGPOLYSET_H__F8C91F88_9FBD_40F7_AB5D_17220764D627__INCLUDED_)

