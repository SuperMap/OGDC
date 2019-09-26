// UGGeoTexture.h: interface for the UGGeoTexture class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UGGEOMODELENTITY_H__20EA7583_BE8A_40A7_884A_E1CDB7161DE1__INCLUDED_)
#define AFX_UGGEOMODELENTITY_H__20EA7583_BE8A_40A7_884A_E1CDB7161DE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Geometry3D/UGGeometry3D.h"
#include "Base3D/UGTextureData.h"
#include "Base3D/UGModelNode.h"

namespace UGC {
//! \brief 模型实体对象
class GEOMETRY3D_API UGGeoModelEntity : public UGGeometry3D  
{
public:
	//! \brief 构造函数
	UGGeoModelEntity();
	//! \brief 析构函数
	~UGGeoModelEntity();

public:
	//! \brief 实体类型
	enum METype
	{
		Unknown		= 0,
		//! \brief 骨架
		Skeleton	= 1,
		//! \brief 纹理
		Texture		= 2,
		//! \brief 材质
		Material	= 3
	};
	
	//! \brief 获取实体类型
	virtual UGGeoModelEntity::METype GetEntityType() const = 0;
	
	//! \brief 获取32位hash值
	UGint GeoHashCode();
	
	//! \brief 获取64位hash值
	UGlong GeoHashCode64();
	
	//! \brief 获取实体对象的名字
	UGString GetEntityName() const;

	//! \brief 托管给实体管理器对象pEM
	virtual void Deposit2EM(UGModelEM* pEM) = 0;

protected:
	//! \brief 实体对象的名字
	UGString m_strEntityName;

	//! \brief 是否需要重新计算Hash值
	UGbool m_bDirty;

	//! \brief 32位Hash值
	UGint m_nHashCode;

	//! \brief 64位Hash值
	UGlong m_lHashCode64;

	//! \brief 实体对象是否是自己独占的
	UGbool m_bEntityOwned;
};

class GEOMETRY3D_API UGGeoModelEntityTexture : public UGGeoModelEntity  
{
public:
	//! \brief 构造函数
	UGGeoModelEntityTexture();
	UGGeoModelEntityTexture(UGModelTexture* pModelTexture);
	//! \brief 析构函数
	~UGGeoModelEntityTexture();

public:
	//! \brief 获取实体类型
	virtual UGGeoModelEntity::METype GetEntityType() const;

	//! \brief 设置实体对象
	void SetEntity(UGModelTexture* pEntity);

	//! \brief 获取实体对象
	UGModelTexture* GetEntity();

	//! \brief 托管给管理器
	virtual void Deposit2EM(UGModelEM* pEM);

	//! \brief 清空
	virtual void Clear();

	//! \brief 几何对象是否有效
	//! \return 有效返回true,无效返回false
	virtual UGbool IsValid() const;

	//! \brief 得到类型, 固定为 GeoRect
	virtual Type GetType() const;

	//! \brief 存储到Stream中
	virtual UGbool SaveGeoData(UGStream& stream, 
		UGDataCodec::CodecType eCodecType,UGbool bSDBPlus = FALSE) const;

	//! \brief 从Stream中生成Geometry
	virtual UGbool LoadGeoData(UGStream& stream , 
		UGDataCodec::CodecType eCodecType = UGDataCodec::encNONE, UGbool bSDBPlus = FALSE );

	//! \brief 根据geometry类型，再调用各自的make方法去实现。
	//! \param pGeometry [in]。
	//! \attention 构造成功返回true。
	virtual UGbool MakeWithGeometry(const UGGeometry* pGeometry);

protected:
	UGModelTexture* m_pModelTexture;
};

class GEOMETRY3D_API UGGeoModelEntitySkeleton : public UGGeoModelEntity  
{
public:
	//! \brief 构造函数
	UGGeoModelEntitySkeleton();
	UGGeoModelEntitySkeleton(UGModelSkeleton* pModelSkeleton);
	//! \brief 析构函数
	~UGGeoModelEntitySkeleton();

public:
	//! \brief 获取实体类型
	virtual UGGeoModelEntity::METype GetEntityType() const;

	//! \brief 设置实体对象
	void SetEntity(UGModelSkeleton* pEntity);

	//! \brief 获取实体对象
	UGModelSkeleton* GetEntity();

	//! \brief 托管给管理器
	virtual void Deposit2EM(UGModelEM* pEM);

	//! \brief 清空
	virtual void Clear();

	//! \brief 几何对象是否有效
	//! \return 有效返回true,无效返回false
	virtual UGbool IsValid() const;

	//! \brief 得到类型, 固定为 GeoRect
	virtual Type GetType() const;

	//! \brief 存储到Stream中
	virtual UGbool SaveGeoData(UGStream& stream, 
		UGDataCodec::CodecType eCodecType,UGbool bSDBPlus = FALSE) const;

	//! \brief 从Stream中生成Geometry
	virtual UGbool LoadGeoData(UGStream& stream , 
		UGDataCodec::CodecType eCodecType = UGDataCodec::encNONE, UGbool bSDBPlus = FALSE );

	//! \brief 根据geometry类型，再调用各自的make方法去实现。
	//! \param pGeometry [in]。
	//! \attention 构造成功返回true。
	virtual UGbool MakeWithGeometry(const UGGeometry* pGeometry);

protected:
	UGModelSkeleton* m_pModelSkeleton;
};

class GEOMETRY3D_API UGGeoModelEntityMaterial3D : public UGGeoModelEntity  
{
public:
	//! \brief 构造函数
	UGGeoModelEntityMaterial3D();
	UGGeoModelEntityMaterial3D(UGModelMaterial* pModelMaterial3D);
	//! \brief 析构函数
	~UGGeoModelEntityMaterial3D();

public:
	//! \brief 获取实体类型
	virtual UGGeoModelEntity::METype GetEntityType() const;

	//! \brief 设置实体对象
	void SetEntity(UGModelMaterial* pEntity);

	//! \brief 获取实体对象
	UGModelMaterial* GetEntity();

	//! \brief 托管给管理器
	virtual void Deposit2EM(UGModelEM* pEM);
	//! \brief 清空
	virtual void Clear();

	//! \brief 几何对象是否有效
	//! \return 有效返回true,无效返回false
	virtual UGbool IsValid() const;

	//! \brief 得到类型, 固定为 GeoRect
	virtual Type GetType() const;

	//! \brief 存储到Stream中
	virtual UGbool SaveGeoData(UGStream& stream, 
		UGDataCodec::CodecType eCodecType,UGbool bSDBPlus = FALSE) const;

	//! \brief 从Stream中生成Geometry
	virtual UGbool LoadGeoData(UGStream& stream , 
		UGDataCodec::CodecType eCodecType = UGDataCodec::encNONE, UGbool bSDBPlus = FALSE );

	//! \brief 根据geometry类型，再调用各自的make方法去实现。
	//! \param pGeometry [in]。
	//! \attention 构造成功返回true。
	virtual UGbool MakeWithGeometry(const UGGeometry* pGeometry);

protected:
	UGModelMaterial* m_pModelMaterial3D;
};
}

#endif // !defined(AFX_UGGEOTEXTURE_H__20EA7583_BE8A_40A7_884A_E1CDB7161DE1__INCLUDED_)

