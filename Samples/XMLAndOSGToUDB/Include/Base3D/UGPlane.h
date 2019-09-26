// UGPlane.h: interface for the UGPlane class.
//
//////////////////////////////////////////////////////////////////////
/* 
	\file UGPlane.h
	\brief UGC三维平面类头文件。
	\author 马柳青
	\attention
	Copyright(c) 2000-2010  SuperMap GIS Technologies,Inc.<br>
	All Rights Reserved
	<b>更新说明：。</b>
	<em>1、第一条说明。</em>
	<em>2、第二条说明。</em>
*/

#if !defined(AFX_UGPLANE_H__FB442292_3E4D_49F8_A98D_245EE80AE3B2__INCLUDED_)
#define AFX_UGPLANE_H__FB442292_3E4D_49F8_A98D_245EE80AE3B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Base3D/UGPrerequisites3D.h"
#include "Base3D/UGVector3d.h"

namespace UGC 
{
//! \brief 三维平面类。
//! \remarks 	
class BASE3D_API UGPlane  
{
public:
	//! \brief 默认构造函数。
	UGPlane();	
	//! \brief 构造函数，由方向量和面距原点的距离构建面。
	//! \detail 可以通过一般式推导 Na * X + Nb * Y + Nc * Z + D / N.Length = 0，其中 vec = (Na, Nb, Nc)， distance = D / N.Length
	//! \param vec 面的法向量[in]。
	//! \param fDistance 面距离原点的距离[in]。
	UGPlane(const UGVector3d& vec,UGdouble fDistance);

	//! \brief 根据三个点构造平面
	UGPlane(const UGVector3d& rkPoint0, const UGVector3d& rkPoint1, const UGVector3d& rkPoint2);

	//! \brief 点法式
	//! \detail A * (X - X0) + B * (Y - Y0) + C * (Z - Z0) = 0
	//! \param vNormal [in] 面的法向量。
	//! \param vPt [in] 面上一点。
	UGPlane(const UGVector3d& vNormal, const UGVector3d& vPt);

	//! \brief 截距式
	//! \detail X / a + Y / b + Z / c = 1
	//! \param dCutA [in] 面与X轴截距。
	//! \param dCutB [in] 面与Y轴截距。
	//! \param dCutC [in] 面与Z轴截距。
	UGPlane(const UGdouble& dCutA, const UGdouble& dCutB, const UGdouble& dCutC);

	//! \brief 一般式
	//! \detail A * X + B * Y + C * Z + D = 0
	//! \param dA [in] 一般式参数A。
	//! \param dB [in] 一般式参数B。
	//! \param dC [in] 一般式参数C。
	//! \param dD [in] 一般式参数D。
	UGPlane(const UGdouble& dA, const UGdouble& dB, const UGdouble& dC, const UGdouble& dD);

	//! \brief 析构函数，什么也没干。
	virtual ~UGPlane();

	//! \brief XY平面，平面法线(0, 0, 1)
	static const UGPlane PlaneXY;
	//! \brief YZ平面，平面法线(1, 0, 0)
	static const UGPlane PlaneYZ;
	//! \brief XZ平面，平面法线(0, 1, 0)
	static const UGPlane PlaneXZ;

	//! \brief 判断平面是否有效
	UGbool IsValid() const { return m_vNormal != UGVector3d::ZERO; }

	//! \brief 平面沿着法线移动
	//! \remark 参数为正，将沿着平面法线正向移动；反之，沿负向移动。
	void Translate(const UGdouble& dOffset);

	//! \brief 计算由空间一点到面的平移向量
	UGVector3d CalcOffsetVector(const UGVector3d& vPnt) const;

	//! \brief 计算由空间一点到面上的垂直投影点
	UGVector3d CalcPrjPnt(const UGVector3d& vPt) const;

	//! \brief 判断空间点是否在该平面上
	UGbool PtInPlane(const UGVector3d& vPt) const;

	//! \brief 判断平面与线段是否相交
	UGbool Intersect(const UGPoint3D& pntStart, const UGPoint3D& pntEnd, UGPoint3D& pntIntersect);

	//! \brief 判断平面与直线是否相交
	UGbool Intersect(const UGVector3d& vTangent, const UGPoint3D& pntThru, UGPoint3D& pntIntersect);

	//! \brief 矩阵变换。
	void Transform(const UGMatrix4d& mTrans);

	//! \brief 获取一般式4参数
	//! \return 返回的4参数是否有效
	UGbool GetParam4(UGdouble& dA, UGdouble& dB, UGdouble& dC, UGdouble& dD) const;

	//! \判断两个平面是否相等
	UGbool operator==(const UGPlane& other) const;
	UGbool operator!=(const UGPlane& other) const;

	UGPlane & operator=(const UGPlane& other);

	//! \brief 对面进行标准化。
	void Normalize();

	//! \brief 获取指定的点到该平面的距离值。
	//! \param vPoint 指定的三维空间中的点位置[in]。
	//! \return 返回计算出来的距离。
	UGdouble GetDistance(const UGVector3d& vPoint) const;

	//! \brief 获取面的法向量。
	const UGVector3d& GetNormal() const;

	//! \brief 设置面的法向量。
	void SetNormal(const UGVector3d& val);

	//! \brief 获取面距离原点的距离。
	UGdouble GetPlaneDistance() const;

	//! \brief 设置面距离原点的距离。
	void SetPlaneDistance(UGdouble dist);

public:
	//! \brief 根据三个点构造平面。
	void Redefine(const UGVector3d& rkPoint0, const UGVector3d& rkPoint1, const UGVector3d& rkPoint2);

	//! \brief 点法式。
	void Redefine(const UGVector3d& vNormal, const UGVector3d& vPnt);

	//! \brief 一般式。
	void Redefine(const UGdouble& dA, const UGdouble& dB, const UGdouble& dC, const UGdouble& dD);

public:
	//! \从XML字符串读取参数设置
	//! \param strXML XML字符串  [in]
	//! \return 返回是否成功
	UGbool FromXML(UGString& strXML);

	//! \保存参数设置到XML字符串
	//! \param strXML XML字符串  [in]
	//! \return 返回是否成功
	UGbool ToXML(UGString& strXML);

protected:
	//! \brief 成员面的法向量
	UGVector3d m_vNormal;

	//! \brief 成员面距离原点的距离
	UGdouble m_fDistance;
};
}
#endif // !defined(AFX_UGPLANE_H__FB442292_3E4D_49F8_A98D_245EE80AE3B2__INCLUDED_)

