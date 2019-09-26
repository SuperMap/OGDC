// UGReindexer.h: interface for the UGReindexer class.
//
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.0           
//                                                           
//!  \file UGReindexer.h
//!  \brief 重建索引。
//!  \details 文件详细信息。
//!  \author 陈玉成。
//!  \attention 
//!   Copyright (c) 1996-2010 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.0
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_UGREINDEXER_H__F49F1AC3_903C_4A44_9973_5C0374739A79__INCLUDED_)
#define AFX_UGREINDEXER_H__F49F1AC3_903C_4A44_9973_5C0374739A79__INCLUDED_
//[Guid("F49F1AC3-903C-4A44-9973-5C0374739A79")]

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if (!defined(_AIX) && !defined(__linux__))
#include <unordered_map>
#else
#include <tr1/unordered_map>
#endif

#include <map>
#include "Base3D/UGVector3d.h"

namespace UGC
{
//! \brief 带映射表加速的重建索引模板类。
template<typename T, class CompareFunc>
class UGReindexerTemplate
{
protected:
	//! \remark 采用map结构能够加快查找速度，适合大数据
	std::map<T, UGuint, CompareFunc> m_mapTypeIndex;
	std::vector<T> m_vec;

public:
	//! \brief 查看点。如果点不存在则记录。
	//! \return 返回顶点索引。
	UGuint Lookup(const T& value)
	{
		// C++标准规定：引用模板类型内部的type必须显式告知编译器是type类型，而不是默认情况认为模板内部的成员为变量variable。
		typename std::map<T, UGuint, CompareFunc>::const_iterator iter = m_mapTypeIndex.find(value);
		if (iter != m_mapTypeIndex.end()) 
			return iter->second;
		else
		{
			m_mapTypeIndex.insert(std::make_pair(value, m_mapTypeIndex.size()));
			return m_mapTypeIndex.size() - 1;
		}
	}

	//! \brief 获取点数组大小。
	UGuint GetSize() const
	{
		return m_mapTypeIndex.size();
	}

	//! \brief 获取点数组。
	const T* GetArray(UGbool bForceUpdateArray = FALSE)
	{
		if (bForceUpdateArray || m_vec.size() != m_mapTypeIndex.size() )
		{
			m_vec.resize(m_mapTypeIndex.size());
			typename std::map<T, UGuint, CompareFunc>::const_iterator iter = m_mapTypeIndex.begin();
			while (iter != m_mapTypeIndex.end())
			{
				m_vec[iter->second] = iter->first;
				iter++;
			}
		}
		
		return m_vec.size() > 0 ? &m_vec[0] : NULL;
	}

	//! \brief 查找元素。
	UGbool Find(const T& value) const
	{
		typename std::map<T, UGuint, CompareFunc>::const_iterator iter = m_mapTypeIndex.find(value);
		if (iter != m_mapTypeIndex.end()) 
			return TRUE;
		else
			return FALSE;
	}

	//! \brief 清空元素。
	void Clear()
	{
		m_mapTypeIndex.clear();
		m_vec.clear();
	}
};

//! \brief 重建索引对儿类。
class UGReindexrPairs
{
public:
	UGbool Lookup(UGuint index0, UGuint index1)
	{
		if (Find(index0, index1)) return FALSE;
		m_mapIndexPair0.insert(std::make_pair(index0, index1));
		m_mapIndexPair1.insert(std::make_pair(index1, index0));
		return TRUE;
	}

	UGbool Find(UGuint index0, UGuint index1) const
	{
		std::multimap<UGuint, UGuint>::const_iterator iter = m_mapIndexPair0.find(index0);
		if (iter != m_mapIndexPair0.end())
		{
			for (UGuint i = 0; i < m_mapIndexPair0.count(index0); i++)
			{
				if ((*iter).second == index1)
				{
					return TRUE;
				}
				++iter;
			}
		}
		
		std::multimap<UGuint, UGuint>::const_iterator beg, end;
		beg = m_mapIndexPair1.lower_bound(index1);
		end = m_mapIndexPair1.upper_bound(index1);
		for (iter = beg; iter != end; ++iter)
		{
			if ((*iter).second == index0)
			{
				return TRUE;
			}
		}

		return FALSE;
	}

	UGuint GetSize() const
	{
		return m_mapIndexPair0.size();
	}

	const std::vector<std::pair<UGuint, UGuint> >& GetArray()
	{
		std::multimap<UGuint, UGuint>::const_iterator iter = m_mapIndexPair0.begin();
		while (iter != m_mapIndexPair0.end())
		{
			m_vecIndexPair.push_back(std::make_pair(iter->first, iter->second));
			iter++;
		}
		return m_vecIndexPair;
	}

	void Clear()
	{
		m_mapIndexPair0.clear();
		m_mapIndexPair1.clear();
		m_vecIndexPair.clear();
	}

private:
	std::multimap<UGuint, UGuint> m_mapIndexPair0;
	std::multimap<UGuint, UGuint> m_mapIndexPair1;
	std::vector<std::pair<UGuint, UGuint> > m_vecIndexPair;
};

// C++ Standard(2003): A local type, a type with no linkage, an unnamed type or a type compounded 
//from any of these types shall not be used as a template-argument for a template type-parameter.
//////////////////////////////////////////////////////////////////////////
template<typename T>
struct UGReindexerTemplateCompareVec
{
	virtual T operator [] (UGuint i) const = 0;
	virtual T& operator [] (UGuint i) = 0;
	virtual void Sort() = 0;
};

// Vec1
template<typename T>
struct UGReindexerTemplateCompareVec1 : public UGReindexerTemplateCompareVec<T>
{
	typedef UGReindexerTemplateCompareVec1<T> Vec1;
	UGReindexerTemplateCompareVec1() {}
	UGReindexerTemplateCompareVec1(const T& _x) : x(_x) {}
	virtual UGbool operator < (const Vec1& other) const = 0;
	virtual UGbool operator == (const Vec1& other) const = 0;
	virtual T operator [] (UGuint i) const
	{
		UGASSERT(i < 1);
		return *(&x + i);
	}
	virtual T& operator [] (UGuint i)
	{
		UGASSERT(i < 1);
		return *(&x + i);
	}
	virtual void Sort() {}
	T x;
};

template<typename T>
struct UGReindexerTemplateCompareVec1N : public UGReindexerTemplateCompareVec1<T>
{
	UGReindexerTemplateCompareVec1N() : UGReindexerTemplateCompareVec1<T>() {}
	UGReindexerTemplateCompareVec1N(const T& _x) : UGReindexerTemplateCompareVec1<T>(_x) {}
	virtual UGbool operator < (const UGReindexerTemplateCompareVec1<T>& other) const
	{
		return UGReindexerTemplateCompareVec1<T>::x < other.x;
	}
	virtual UGbool operator == (const UGReindexerTemplateCompareVec1<T>& other) const
	{
		return UGReindexerTemplateCompareVec1<T>::x == other.x;
	}
};

template<typename T>
struct UGReindexerTemplateCompareVec1D : public UGReindexerTemplateCompareVec1<T>
{
	UGReindexerTemplateCompareVec1D() : UGReindexerTemplateCompareVec1<T>() {}
	UGReindexerTemplateCompareVec1D(const T& x) : UGReindexerTemplateCompareVec1<T>(x) {}
	virtual UGbool operator < (const UGReindexerTemplateCompareVec1<T>& other) const
	{
		if (!UGEQUAL(UGReindexerTemplateCompareVec1<T>::x, other.x)) return UGReindexerTemplateCompareVec1<T>::x < other.x;
		return FALSE;
	}
	virtual UGbool operator == (const UGReindexerTemplateCompareVec1<T>& other) const
	{
		return UGEQUAL(UGReindexerTemplateCompareVec1<T>::x, other.x);
	}
};

// 模板类继承情况，子类看不到父类的成员变量，需要使用父类名称为作用域或者使用this指针明确指向父类成员
//////////////////////////////////////////////////////////////////////////
// Vec2
template<typename T>
struct UGReindexerTemplateCompareVec2 : public UGReindexerTemplateCompareVec<T>
{
	typedef UGReindexerTemplateCompareVec2<T> Vec2;
	UGReindexerTemplateCompareVec2() {}
	UGReindexerTemplateCompareVec2(const T& _x, const T& _y) : x(_x), y(_y) {}
	virtual UGbool operator < (const Vec2& other) const = 0;
	virtual UGbool operator == (const Vec2& other) const = 0;
	virtual T operator [] (UGuint i) const
	{
		UGASSERT(i < 2);
		return *(&x + i);
	}
	virtual T& operator [] (UGuint i)
	{
		UGASSERT(i < 2);
		return *(&x + i);
	}
	virtual void Sort()
	{
		if (x > y) std::swap(x, y);
	}
	T x, y;
};

template<typename T>
struct UGReindexerTemplateCompareVec2N : public UGReindexerTemplateCompareVec2<T>
{
	UGReindexerTemplateCompareVec2N() : UGReindexerTemplateCompareVec2<T>() {}
	UGReindexerTemplateCompareVec2N(const T& x, const T& y) : UGReindexerTemplateCompareVec2<T>(x, y) {}
	virtual UGbool operator < (const UGReindexerTemplateCompareVec2<T>& other) const
	{
		if (this->x < other.x) return TRUE;
		else if (this->x > other.x) return FALSE;
		else return (this->y < other.y);
	}
	virtual UGbool operator == (const UGReindexerTemplateCompareVec2<T>& other) const
	{
		return (this->x == other.x) && (this->y == other.y);
	}
};

template<typename T>
struct UGReindexerTemplateCompareVec2D : public UGReindexerTemplateCompareVec2<T>
{
	UGReindexerTemplateCompareVec2D() : UGReindexerTemplateCompareVec2<T>() {}
	UGReindexerTemplateCompareVec2D(const T& x, const T& y) : UGReindexerTemplateCompareVec2<T>(x, y) {}
	virtual UGbool operator < (const UGReindexerTemplateCompareVec2<T>& other) const
	{
		if (!UGEQUAL(this->x, other.x)) return this->x < other.x;
		if (!UGEQUAL(this->y, other.y)) return this->y < other.y;
		return FALSE;
	}
	virtual UGbool operator == (const UGReindexerTemplateCompareVec2<T>& other) const
	{
		return UGEQUAL(this->x, other.x) && UGEQUAL(this->y, other.y);
	}
};

//////////////////////////////////////////////////////////////////////////
// Vec3
template<typename T>
struct UGReindexerTemplateCompareVec3 : public UGReindexerTemplateCompareVec<T>
{
	typedef UGReindexerTemplateCompareVec3<T> Vec3;
	UGReindexerTemplateCompareVec3() {}
	UGReindexerTemplateCompareVec3(const T& _x, const T& _y, const T& _z) : x(_x), y(_y), z(_z) {}
	virtual UGbool operator < (const Vec3& other) const = 0;
	virtual UGbool operator == (const Vec3& other) const = 0;
	virtual T operator [] (UGuint i) const
	{
		UGASSERT(i < 3);
		return *(&x + i);
	}
	virtual T& operator [] (UGuint i)
	{
		UGASSERT(i < 3);
		return *(&x + i);
	}
	virtual void Sort()
	{
		if (x > y) std::swap(x, y);
		if (x > z) std::swap(x, z);
		if (y > z) std::swap(y, z);
	}
	T x, y, z;
};

template<typename T>
struct UGReindexerTemplateCompareVec3N : public UGReindexerTemplateCompareVec3<T>
{
	UGReindexerTemplateCompareVec3N() : UGReindexerTemplateCompareVec3<T>() {}
	UGReindexerTemplateCompareVec3N(const T& x, const T& y, const T& z) : UGReindexerTemplateCompareVec3<T>(x, y, z) {}
	virtual UGbool operator < (const UGReindexerTemplateCompareVec3<T>& other) const
	{
		if (this->x < other.x) return TRUE;
		else if (this->x > other.x) return FALSE;
		else if (this->y < other.y) return TRUE;
		else if (this->y > other.y) return FALSE;
		else return (this->z < other.z);
	}
	virtual UGbool operator == (const UGReindexerTemplateCompareVec3<T>& other) const
	{
		return (this->x == other.x) && (this->y == other.y) && (this->z == other.z);
	}
};

template<typename T>
struct UGReindexerTemplateCompareVec3D : public UGReindexerTemplateCompareVec3<T>
{
	UGReindexerTemplateCompareVec3D() : UGReindexerTemplateCompareVec3<T>() {}
	UGReindexerTemplateCompareVec3D(const T& x, const T& y, const T& z) : UGReindexerTemplateCompareVec3<T>(x, y, z) {}
	virtual UGbool operator < (const UGReindexerTemplateCompareVec3<T>& other) const
	{
		if (!UGEQUAL(this->x, other.x)) return this->x < other.x;
		if (!UGEQUAL(this->y, other.y)) return this->y < other.y;
		if (!UGEQUAL(this->z, other.z)) return this->z < other.z;
		return FALSE;
	}
	virtual UGbool operator == (const UGReindexerTemplateCompareVec3<T>& other) const
	{
		return UGEQUAL(this->x, other.x) && UGEQUAL(this->y, other.y) && UGEQUAL(this->z, other.z);
	}
};

//////////////////////////////////////////////////////////////////////////
// Vec4
template<typename T>
struct UGReindexerTemplateCompareVec4 : public UGReindexerTemplateCompareVec<T>
{
	typedef UGReindexerTemplateCompareVec4<T> Vec4;
	UGReindexerTemplateCompareVec4() {}
	UGReindexerTemplateCompareVec4(const T& _x, const T& _y, const T& _z, const T& _w) : x(_x), y(_y), z(_z), w(_w) {}
	virtual UGbool operator < (const Vec4& other) const = 0;
	virtual UGbool operator == (const Vec4& other) const = 0;
	virtual T operator [] (UGuint i) const
	{
		UGASSERT(i < 4);
		return *(&x + i);
	}
	virtual T& operator [] (UGuint i)
	{
		UGASSERT(i < 4);
		return *(&x + i);
	}
	virtual void Sort()
	{
		if (x > y) std::swap(x, y);
		if (x > z) std::swap(x, z);
		if (x > w) std::swap(x, w);
		if (y > z) std::swap(y, z);
		if (y > w) std::swap(y, w);
		if (z > w) std::swap(z, w);
	}
	T x, y, z, w;
};

template<typename T>
struct UGReindexerTemplateCompareVec4N : public UGReindexerTemplateCompareVec4<T>
{
	UGReindexerTemplateCompareVec4N() : UGReindexerTemplateCompareVec4<T>() {}
	UGReindexerTemplateCompareVec4N(const T& x, const T& y, const T& z, const T& w) : UGReindexerTemplateCompareVec4<T>(x, y, z, w) {}
	virtual UGbool operator < (const UGReindexerTemplateCompareVec4<T>& other) const
	{
		if (this->x < other.x) return TRUE;
		else if (this->x > other.x) return FALSE;
		else if (this->y < other.y) return TRUE;
		else if (this->y > other.y) return FALSE;
		else if (this->z < other.z) return TRUE;
		else if (this->z > other.z) return FALSE;
		else return (this->w < other.w);
	}
	virtual UGbool operator == (const UGReindexerTemplateCompareVec4<T>& other) const
	{
		return (this->x == other.x) && (this->y == other.y) && (this->z == other.z) && (this->w == other.w);
	}
};

template<typename T>
struct UGReindexerTemplateCompareVec4D : public UGReindexerTemplateCompareVec4<T>
{
	UGReindexerTemplateCompareVec4D() : UGReindexerTemplateCompareVec4<T>() {}
	UGReindexerTemplateCompareVec4D(const T& x, const T& y, const T& z, const T& w) : UGReindexerTemplateCompareVec4<T>(x, y, z, w) {}
	virtual UGbool operator < (const UGReindexerTemplateCompareVec4<T>& other) const
	{
		if (!UGEQUAL(this->x, other.x)) return this->x < other.x;
		if (!UGEQUAL(this->y, other.y)) return this->y < other.y;
		if (!UGEQUAL(this->z, other.z)) return this->z < other.z;
		if (!UGEQUAL(this->w, other.w)) return this->w < other.w;
		return FALSE;
	}
	virtual UGbool operator == (const UGReindexerTemplateCompareVec4<T>& other) const
	{
		return UGEQUAL(this->x, other.x) && UGEQUAL(this->y, other.y) && UGEQUAL(this->z, other.z) && UGEQUAL(this->w, other.w);
	}
};

//////////////////////////////////////////////////////////////////////////
// Compare Function
template<typename CLASS_TYPE, typename MEMBER_TYPE>
struct UGReindexerTemplateCompareFunction
{
	virtual UGbool operator()(const CLASS_TYPE& v1, const CLASS_TYPE& v2) const = 0;
};

template<typename CLASS_TYPE>
struct UGReindexerTemplateCompareFunction1N : public UGReindexerTemplateCompareFunction<CLASS_TYPE, CLASS_TYPE>
{
	virtual UGbool operator()(const CLASS_TYPE& v1, const CLASS_TYPE& v2) const
	{
		return UGReindexerTemplateCompareVec1N<CLASS_TYPE>(v1) < UGReindexerTemplateCompareVec1N<CLASS_TYPE>(v2);
	}
};

template<typename CLASS_TYPE>
struct UGReindexerTemplateCompareFunction1D : public UGReindexerTemplateCompareFunction<CLASS_TYPE, CLASS_TYPE>
{
	virtual UGbool operator()(const CLASS_TYPE& v1, const CLASS_TYPE& v2) const
	{
		return UGReindexerTemplateCompareVec1D<CLASS_TYPE>(v1) < UGReindexerTemplateCompareVec1D<CLASS_TYPE>(v2);
	}
};

template<typename CLASS_TYPE, typename MEMBER_TYPE>
struct UGReindexerTemplateCompareFunction2N : public UGReindexerTemplateCompareFunction<CLASS_TYPE, MEMBER_TYPE>
{
	virtual UGbool operator()(const CLASS_TYPE& v1, const CLASS_TYPE& v2) const
	{
		return UGReindexerTemplateCompareVec2N<MEMBER_TYPE>(v1.x, v1.y) < UGReindexerTemplateCompareVec2N<MEMBER_TYPE>(v2.x, v2.y);
	}
};

template<typename CLASS_TYPE, typename MEMBER_TYPE>
struct UGReindexerTemplateCompareFunction2D : public UGReindexerTemplateCompareFunction<CLASS_TYPE, MEMBER_TYPE>
{
	virtual UGbool operator()(const CLASS_TYPE& v1, const CLASS_TYPE& v2) const
	{
		return UGReindexerTemplateCompareVec2D<MEMBER_TYPE>(v1.x, v1.y) < UGReindexerTemplateCompareVec2D<MEMBER_TYPE>(v2.x, v2.y);
	}
};

template<typename CLASS_TYPE, typename MEMBER_TYPE>
struct UGReindexerTemplateCompareFunction3N : public UGReindexerTemplateCompareFunction<CLASS_TYPE, MEMBER_TYPE>
{
	virtual UGbool operator()(const CLASS_TYPE& v1, const CLASS_TYPE& v2) const
	{
		return UGReindexerTemplateCompareVec3N<MEMBER_TYPE>(v1.x, v1.y, v1.z) < UGReindexerTemplateCompareVec3N<MEMBER_TYPE>(v2.x, v2.y, v2.z);
	}
};

template<typename CLASS_TYPE, typename MEMBER_TYPE>
struct UGReindexerTemplateCompareFunction3D : public UGReindexerTemplateCompareFunction<CLASS_TYPE, MEMBER_TYPE>
{
	virtual UGbool operator()(const CLASS_TYPE& v1, const CLASS_TYPE& v2) const
	{
		return UGReindexerTemplateCompareVec3D<MEMBER_TYPE>(v1.x, v1.y, v1.z) < UGReindexerTemplateCompareVec3D<MEMBER_TYPE>(v2.x, v2.y, v2.z);
	}
};

template<typename CLASS_TYPE, typename MEMBER_TYPE>
struct UGReindexerTemplateCompareFunction4N : public UGReindexerTemplateCompareFunction<CLASS_TYPE, MEMBER_TYPE>
{
	virtual UGbool operator()(const CLASS_TYPE& v1, const CLASS_TYPE& v2) const
	{
		return UGReindexerTemplateCompareVec4N<MEMBER_TYPE>(v1.x, v1.y, v1.z, v1.w) < UGReindexerTemplateCompareVec4N<MEMBER_TYPE>(v2.x, v2.y, v2.z, v2.w);
	}
};

template<typename CLASS_TYPE, typename MEMBER_TYPE>
struct UGReindexerTemplateCompareFunction4D : public UGReindexerTemplateCompareFunction<CLASS_TYPE, MEMBER_TYPE>
{
	virtual UGbool operator()(const CLASS_TYPE& v1, const CLASS_TYPE& v2) const
	{
		return UGReindexerTemplateCompareVec4D<MEMBER_TYPE>(v1.x, v1.y, v1.z, v1.w) < UGReindexerTemplateCompareVec4D<MEMBER_TYPE>(v2.x, v2.y, v2.z, v2.w);
	}
};

//////////////////////////////////////////////////////////////////////////
// Class Type
template<typename CLASS_TYPE>
class UGReindexerTemplate1N : public UGReindexerTemplate<CLASS_TYPE, UGReindexerTemplateCompareFunction1N<CLASS_TYPE> >
{
};

template<typename CLASS_TYPE>
class UGReindexerTemplate1D : public UGReindexerTemplate<CLASS_TYPE, UGReindexerTemplateCompareFunction1D<CLASS_TYPE> >
{
};

template<typename CLASS_TYPE, typename MEMBER_TYPE>
class UGReindexerTemplate2N : public UGReindexerTemplate<CLASS_TYPE, UGReindexerTemplateCompareFunction2N<CLASS_TYPE, MEMBER_TYPE> >
{
};

template<typename CLASS_TYPE, typename MEMBER_TYPE>
class UGReindexerTemplate2D : public UGReindexerTemplate<CLASS_TYPE, UGReindexerTemplateCompareFunction2D<CLASS_TYPE, MEMBER_TYPE> >
{
};

template<typename CLASS_TYPE, typename MEMBER_TYPE>
class UGReindexerTemplate3N : public UGReindexerTemplate<CLASS_TYPE, UGReindexerTemplateCompareFunction3N<CLASS_TYPE, MEMBER_TYPE> >
{
};

template<typename CLASS_TYPE, typename MEMBER_TYPE>
class UGReindexerTemplate3D : public UGReindexerTemplate<CLASS_TYPE, UGReindexerTemplateCompareFunction3D<CLASS_TYPE, MEMBER_TYPE> >
{
};

template<typename CLASS_TYPE, typename MEMBER_TYPE>
class UGReindexerTemplate4N : public UGReindexerTemplate<CLASS_TYPE, UGReindexerTemplateCompareFunction4N<CLASS_TYPE, MEMBER_TYPE> >
{
};

template<typename CLASS_TYPE, typename MEMBER_TYPE>
class UGReindexerTemplate4D : public UGReindexerTemplate<CLASS_TYPE, UGReindexerTemplateCompareFunction4D<CLASS_TYPE, MEMBER_TYPE> >
{
};

//////////////////////////////////////////////////////////////////////////
// The Sample Application

//! \brief 重建索引器
typedef UGReindexerTemplate1D<UGdouble> UGReindexer1d;
typedef UGReindexerTemplate2D<UGVector3d, UGdouble> UGReindexer2d;
typedef UGReindexerTemplate3D<UGVector3d, UGdouble> UGReindexer3d;
typedef UGReindexerTemplate4D<UGVector4d, UGdouble> UGReindexer4d;

typedef UGReindexerTemplate2D<UGPoint2D, UGdouble> UGReindexerP2d;
typedef UGReindexerTemplate3D<UGPoint3D, UGdouble> UGReindexerP3d;

//! \brief 位置
typedef UGReindexerTemplateCompareVec2N<UGuint> UGPosition2;
typedef UGReindexerTemplateCompareVec3N<UGuint> UGPosition3;

//! \brief 范围
template<typename T>
struct UGRange : public UGReindexerTemplateCompareVec2<T>
{
	UGRange() : UGReindexerTemplateCompareVec2<T>() {}
	UGRange(const T& x, const T& y) : UGReindexerTemplateCompareVec2<T>(x, y) {}
	virtual UGbool operator < (const UGReindexerTemplateCompareVec2<T>& other) const
	{
		return *this < other;
	}
	virtual UGbool operator == (const UGReindexerTemplateCompareVec2<T>& other) const
	{
		return *this == other;
	}

	const T& GetUpper() const { return this->y; }
	const T& GetLower() const { return this->x; }

	T GetRange() const { return GetUpper() - GetLower(); }

	UGbool InRange(const T& value) const { return this->x <= value && value <= this->y; }

	T Normalize(const T& value) const
	{
		const T& lower = GetLower();
		const T& upper = GetUpper();
		if (lower == upper)
		{
			return 0.5;
		}
		else
		{
			return (value - lower) / (upper - lower);
		}
	}
};
}
#endif // !defined(AFX_UGREINDEXER_H__F49F1AC3_903C_4A44_9973_5C0374739A79__INCLUDED_)