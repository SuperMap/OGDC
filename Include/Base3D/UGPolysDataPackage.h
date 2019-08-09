// UGPolySet.h: interface for the UGPolysDataPackage class.
//
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.0           
//                                                           
//!  \file UGPolysDataPackage.h
//!  \brief 参数化对象建模模型顶点包结构。
//!  \details 文件详细信息。
//!  \author 陈玉成。
//!  \attention 
//!   Copyright (c) 1996-2010 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.0
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_UGPOLYSDATAPACKAGE_H__6DE128A0_500E_4791_9255_4F0CA58E98C3__INCLUDED_)
#define AFX_UGPOLYSDATAPACKAGE_H__6DE128A0_500E_4791_9255_4F0CA58E98C3__INCLUDED_
// [Guid("6DE128A0-500E-4791-9255-4F0CA58E98C3")]

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Base3D/UGGrid.h"
#include "Base3D/UGBoundingBox.h"
#include "Toolkit/UGReferenced.h"
#include "Toolkit/UGRefPtr.h"
#include "Toolkit/UGSingleton.h"
#include "Base3D/UGPolysDefine.h"

namespace UGC
{
	//! \brief 纹理的维度与纹理坐标。
	//! \remark 索引介于[0, 3]。
	typedef std::pair<UGuint, UGVector3d> UGTexDimCoord;
	//! \brief 纹理所在的通道与纹理信息。
	//! \remark 索引介于[0, SMSCN_MAX_TEXTURE_COORD_SETS - 1]。
	typedef std::pair<UGuint, UGTexDimCoord> UGTexChannelCoord;
	//! \brief 单通道纹理坐标结构。
	typedef UGTexChannelCoord UGUVW;
	//! \brief 多通道纹理坐标结构。
	//! \remark 大小介于[0, SMSCN_MAX_TEXTURE_COORD_SETS]。
	typedef std::vector<UGUVW> UGUVWs;
	typedef UGUVWs UGTexMapCoords;

	//! \brief 前置声明结构。
	struct UGPolyPoint;

	//! \brief 多边形 或 一条边。
	//	typedef std::vector<UGVector3d> UGPoly;
	typedef std::vector<UGPolyPoint> UGPoly;
	//! \brief 多边形数组 或 边数组。
	typedef std::vector<UGPoly> UGPolys;

	//! \brief 多边形列表。
	typedef std::list<UGPoly> UGPolyList;
	typedef std::list<UGPolyPoint> UGPolyPointList;

	//! \brief 索引数组。
	typedef UGuint UGPolyIndex;
	typedef std::vector<UGPolyIndex> UGPolyIndices;
	typedef std::vector<UGPolyIndices> UGPolyFacets;

	//! \brief 顶点数组。
	typedef UGVector3d UGPolyVertex;
	typedef std::vector<UGPolyVertex> UGPolyVertices;

	//! \brief 三维点在Polys结构中的位置信息。
	typedef std::vector<UGPosition2> UGPolyVertexPosition;
	typedef std::vector<UGPolyVertexPosition> UGPolyVertexPositions;

	//! \brief 数据包的智能指针。
	class UGPolysDataPackage;
	typedef UGRefPtr<UGPolysDataPackage> UGPolysDataPackageRefPtr;

	//! \brief 数据包。
	//! \remark 数据包中存储多面体数据和顶点数据包。
	class BASE3D_API UGPolysDataPackage : public UGReferenced
	{
		//! \brief 构造函数、析构函数。
	public:
		UGPolysDataPackage();
		UGPolysDataPackage(const UGPolysDataPackage& package);
		UGPolysDataPackage& operator=(const UGPolysDataPackage& package);

	protected:
		virtual ~UGPolysDataPackage(); 

		//! \brief 公共方法。
	public:
		//! \brief 清空数据包。
		void Clear();

		//! \brief 数据包是否为空。
		UGbool IsEmpty() const;

		//! \brief 获取包围盒。
		const UGBoundingBox& GetBoundingBox() const;

		//! \brief 获取质心点。
		const UGVector3d& GetBaryCenter() const;

		//! \brief 多面体数据。
	public:
		//! \brief 获取多面体数据(只读)。
		const UGPolys& GetPolys() const;

		//! \brief 获取多面体数据(导致顶点数据无效)。
		UGPolys& GetPolysOfMutability();

		//! \brief 获取所存储的多面体层数。
		UGuint GetNumPolys() const;

		//! \brief 多面体数据中点总数。
		UGuint GetNumPolysPts() const;

		//! \brief 设置/获取多面体数据的无效状态。
		void SetPolysPackageDirty(UGbool bDirty);
		UGbool IsPolysPackageDirty() const;


		//! \brief 顶点数据。
	public:
		//! \brief 获取面索引数据(只读)。
		const UGPolyFacets& GetFacets() const;

		//! \brief 获取顶点数据(只读)。
		const UGPolyVertices& GetPts() const;

		//! \brief 获取顶点在Polys的位置信息(只读)。
		const UGPolyVertexPositions& GetPtPositions() const;

		//! \brief 获取顶点(只读)。
		const UGPolyVertex& GetPt(UGPolyIndex id) const;

		//! \brief 获取顶点(导致多面体数据无效)。
		UGPolyVertex& GetPtOfMutability(UGPolyIndex id);

		//! \brief 面索引数量。
		UGuint GetNumFacetIds(UGuint iFacetId) const;

		//! \brief 顶点数量。
		UGuint GetNumPts() const;

		//! \brief 数据包索引总数。
		UGuint GetNumFacetsIds() const;
		
		//! \brief 设置/获取顶点数据的无效状态。
		void SetDataPackageDirty(UGbool bDirty);
		UGbool IsDataPackageDirty() const;


		//! \brief 多面体数据处理。
	public:
		//! \brief 追加多边形。
		void AppendPoly();

		//! \brief 追加多边形
		//! \param poly [in] 多边形。
		void AppendPoly(const UGPoly& poly);

		//! \brief 追加顶点。
		//! \param pt [in] 顶点。
		void AppendVertex(const UGPolyPoint& pt);

		//! \brief 插入顶点。
		//! \param pt [in] 顶点。
		void InsertVertex(const UGPolyPoint& pt);

		//! \brief 追加多个多边形。
		void AppendPolys(const UGPolys& polys);

		//! \brief 获取纹理坐标
		const UGUVWs& GetTexCoords(const UGPosition2& pos) const;

		//! \brief 获取浮点型拓展位。
		const std::vector<UGdouble>& GetRealExt(const UGPosition2& pos) const;

		//! \brief 获取整数型拓展位。
		const std::vector<UGuint>& GetIntegerExt(const UGPosition2& pos) const;


		//! \brief 顶点数据处理。
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

		//! \brief 获取点。
		UGPolyPoint& GetPt(const UGPosition2& pos);

		//! \brief 矩阵变换。
		//! \param m4 [in] 变换矩阵。
		void MatrixTransform(const UGMatrix4d& m4);

		//! \brief 顶点索引模式。
		UGbool IsOrient() const { return m_bOrientDataPackage; }
		void SetOrient(UGbool bOrient) { m_bOrientDataPackage = bOrient; }

		//! \brief 添加数据。
		void AppendSurface(const UGPolyVertices& arrPts, const UGPolyFacets& arrFacets, const UGPolys& arrPolys = UGPolys());

	public:
		//! \brief 追加数据。
		void Append(const UGPolysDataPackage* pPackage);


	private:
		//! \brief 计算多面体数据。
		void ComputePolysPackage() const;

		//! \brief 计算顶点数据。
		void ComputeDataPackage() const;

		//! \brief 计算数据包元数据。
		void ComputePackageMeta() const;

		//! \brief 清空多面体数据。
		void ClearPolysPackage();

		//! \brief 清空顶点数据。
		void ClearDataPackage();

	private:
		//! \brief 多面体数据包是否无效。
		mutable UGbool m_bPolysPackageIsDirty;

		//! \brief 顶点数据包是否无效。
		mutable UGbool m_bDataPackageIsDirty;

		//! \brief 顶点包是否已经校正。
		UGbool m_bOrientDataPackage;

		//! \brief 多面体数据包。
		mutable UGPolys m_polys;

		//! \brief 顶点数据包。
		mutable UGPolyFacets m_facets;
		mutable UGPolyVertices m_pts;
		mutable UGPolyVertexPositions m_pvs;

		//! \brief 数据包包围盒。
		mutable UGBoundingBox m_boundingbox;

		//! \brief 质心。
		mutable UGVector3d m_vBaryCenter;
	};

	//! \brief 顶点结构。
	//! \remark 包含顶点坐标、纹理坐标等信息。
	struct UGPolyPoint
	{
		//! \brief 顶点数组。
		UGPolyVertex vertex;

		//! \brief 纹理坐标。
		UGUVWs uvws;

		//! \brief 顶点属性。
		UGAttribute iAttribute;

		//! \brief 浮点型拓展位，如拓展(x, y, z), w0, w1, w2 ...
		std::vector<UGdouble> arrRealExts;

		//! \brief 整数型拓展位，如拓展顶点颜色c0, c1, c2 ...
		std::vector<UGuint> arrIntExts;

		//! \brief 构造函数。
		UGPolyPoint() {}

		UGPolyPoint(const UGdouble& x, const UGdouble& y, const UGdouble& z, const UGUVWs& uvws = UGUVWs(), const UGAttribute& iAttribute = UGAttribute())
		{
			vertex = UGVector3d(x, y, z);
			this->uvws = uvws;
			this->iAttribute = iAttribute;
		}

		UGPolyPoint(const UGVector3d& vertex, const UGUVWs& uvws = UGUVWs(), const UGAttribute& iAttribute = UGAttribute())
		{
			this->vertex = vertex;
			this->uvws = uvws;
			this->iAttribute = iAttribute;
		}

		UGPolyPoint(const UGPolyPoint& pt)
		{
			vertex	= pt.vertex;
			uvws		= pt.uvws;
			iAttribute = pt.iAttribute;
			arrRealExts	= pt.arrRealExts;
			arrIntExts	= pt.arrIntExts;
		}

		//! \brief 析构函数。
		~UGPolyPoint() {}

		//! \brief 判等函数。
		UGbool operator==(const UGPolyPoint& pt) const
		{
			return (vertex == pt.vertex) && (uvws == pt.uvws) && (iAttribute == pt.iAttribute);
		}

		//! \brief 判等函数。
		UGbool operator!=(const UGPolyPoint& pt) const
		{
			return (vertex != pt.vertex) || (uvws != pt.uvws) || (iAttribute != pt.iAttribute);
		}

		//! \brief 排序比较函数。
		UGbool operator < (const UGPolyPoint& pt) const
		{
			return UGReindexerTemplateCompareVec3D<UGdouble>(vertex.x, vertex.y, vertex.z) <
				UGReindexerTemplateCompareVec3D<UGdouble>(pt.vertex.x, pt.vertex.y, pt.vertex.z);
		}

		//! \brief 赋值函数。
		UGPolyPoint& operator=(const UGPolyPoint& pt)
		{
			vertex	= pt.vertex;
			uvws		= pt.uvws;
			iAttribute = pt.iAttribute;
			arrRealExts	= pt.arrRealExts;
			arrIntExts	= pt.arrIntExts;
			return *this;
		}

		const UGVector3d& GetVector3d() const { return vertex; }
		UGVector3d& GetVector3d() { return vertex; }
		UGVector3d GetVector2d() const { return UGVector3d(vertex.x, vertex.y, 0); }
		UGPoint2D GetPoint2D() const { return UGPoint2D(vertex.x, vertex.y); }
		UGPoint3D GetPoint3D() const { return UGPoint3D(vertex.x, vertex.y, vertex.z); }
	};

	// 曲面构建参数类。
	struct UGSurfaceMeshParam : public UGOperationParam
	{
		//! \brief 离散采样点阵。
		std::vector<UGPolyPoint> arrDotMatrix;

		//! \brief 是否以UV坐标作为极坐标参数。
		UGbool bPolarParametric;

		UGSurfaceMeshParam() :
			bPolarParametric(TRUE) {}

		virtual UGBuilderOperation GetType() const { return builder; }
	};

	//! \brief 数据流。
	template<class DATA_TYPE>
	class UGStreamBuffer : public UGReferenced
	{
		//! \brief 构造函数、析构函数。
	public:
		UGStreamBuffer() {}
		virtual ~UGStreamBuffer() {}

	public:
		//! \brief 数据类型。
		typedef DATA_TYPE Type;

	public:
		//! \brief 数据缓存。
		const Type& GetBuffer() const { return m_buffer; }
		Type& GetBuffer() { return m_buffer; }
		void SetBuffer(const Type& buffer) { m_buffer = buffer; }

	protected:
		//! \brief 数据缓存。
		Type m_buffer;
	};

	template<typename MEMBER_TYPE>
	class UGStreamArrayBuffer : public UGStreamBuffer<std::vector<MEMBER_TYPE> >
	{
	public:
		UGuint GetSize() const { return this->m_buffer.size(); }
		void SetSize(UGuint nSize) { this->m_buffer.resize(nSize); } 
	};


	template<typename MEMBER_TYPE>
	class UGSingletonInstanceRef
	{
	private:
		MEMBER_TYPE* m_pValue;
		UGuint m_nRefCount;

	public:
		explicit UGSingletonInstanceRef(MEMBER_TYPE* pValue) : m_pValue(pValue), m_nRefCount(0) {}
		~UGSingletonInstanceRef()
		{
			if (m_pValue != NULL)
			{
				delete m_pValue; m_pValue = NULL;
			}
		}

	public:
		MEMBER_TYPE* GetValue() const { return m_pValue; }

		void AddRef() { ++m_nRefCount; }
		void DecRef() { --m_nRefCount; }
		UGuint GetRefCount() const { return m_nRefCount; }
	};

	template<typename Key, class Value>
	class UGSingletonInstanceCollection
	{
	public:
		typedef UGSingletonInstanceRef<Value> Instance;
		typedef std::map<Key, Instance*> Collection;
		typedef typename Collection::iterator CollectionIter;

		UGSingletonInstanceCollection() : m_mutex(TRUE) {}
		~UGSingletonInstanceCollection()
		{
			UGAutoLock locker(m_mutex);
			for (CollectionIter iter = m_mapInstances.begin(); iter != m_mapInstances.end(); ++iter)
			{
				Instance* pInstance = iter->second;
				if (pInstance != NULL)
				{
					delete pInstance; pInstance = NULL;
				}
			}
			m_mapInstances.clear();
		}

		Value* GetValue(const Key& key)
		{
			Value* pValue = NULL;
			Instance* pInstance = NULL;
			UGAutoLock locker(m_mutex);
			CollectionIter iter = m_mapInstances.find(key);	
			if (iter != m_mapInstances.end() && iter->second != NULL)
			{	
				pInstance = iter->second;
				pValue = pInstance->GetValue();
			}
			else
			{
				pValue = new Value();
				pInstance = new Instance(pValue);
				m_mapInstances[key] = pInstance;
			}
			pInstance->AddRef();
			return pValue;
		}

		void RemoveValue(const Key& key)
		{
			UGAutoLock locker(m_mutex);
			CollectionIter iter = m_mapInstances.find(key);	
			if (iter != m_mapInstances.end() && iter->second != NULL)
			{
				Instance* pInstance = iter->second;
				pInstance->DecRef();
				if (pInstance->GetRefCount() == 0)
				{
					delete pInstance; pInstance = NULL;
					m_mapInstances.erase(key);
				}
			}
		}

	private:
		Collection m_mapInstances;
		UGMutex m_mutex;
	};
}

#endif // !defined(AFX_UGPOLYSDATAPACKAGE_H__6DE128A0_500E_4791_9255_4F0CA58E98C3__INCLUDED_)