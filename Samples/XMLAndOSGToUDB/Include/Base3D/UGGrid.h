// UGGrid.h: interface for the UGGrid template class.
//
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.0           
//                                                           
//!  \file UGGrid.h
//!  \brief 网格对齐。
//!  \details 文件详细信息。
//!  \author 陈玉成。
//!  \attention 
//!   Copyright (c) 1996-2010 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.0
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_UGGRID_H__D2134939_45C0_42E5_86FA_876ED928B4FF__INCLUDED_)
#define AFX_UGGRID_H__D2134939_45C0_42E5_86FA_876ED928B4FF__INCLUDED_
//[Guid("D2134939-45C0-42E5-86FA-876ED928B4FF")]

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdint.h>
#include <stdlib.h>
#include <utility>
#include "Base3D/UGReindexer.h"

namespace UGC
{
//	const double GRID_COARSE = 0.001;
//	const double GRID_FINE   = 0.000001;
/* Using decimals that are exactly convertible to binary floating point 
(and then converted exactly to a GMPQ Rational that uses a small amount 
of bytes aka "limbs" in CGAL's engine) provides at least a 5% speedup 
for ctest -R CGAL. We choose 1/1024 and 1/(1024*1024) In python: print 
'%.64f' % float(fractions.Fraction(1,1024)) */
const UGdouble GRID_COARSE = 0.0009765625;
const UGdouble GRID_FINE   = 0.00000095367431640625;

template <typename Key, typename ID, class Value>
class UGGridTemplate
{
public:
	typedef std::pair<ID, Value> Container;
	typedef std::map<Key, Container> GridContainer;

	UGGridTemplate(UGdouble dResolution) { m_dResolution = dResolution; }

	//! \brief 对齐顶点。
	//! \remark Aligns Value to the grid or to existing point if one close enough exists. 
	//! \attention Will automatically increase the index as new unique vertices are added.
	//! \return Returns the ID stored if a point already existing or an uninitialized new ID if not.
	virtual ID Align(Value& val) = 0;

	//! \brief 是否存在。
	virtual UGbool Has(const Value& val, ID* pID = NULL) const = 0;

	//! \brief 判等。
	virtual UGbool Equal(Value val1, Value val2) = 0;

	//! \brief 清空。
	void Clear() { m_gcDB.clear(); }


protected:
	//! \brief 分辨率。
	UGdouble m_dResolution;

	//! \brief 数据容器。
	GridContainer m_gcDB;
};

//! \brief 二维网格对齐。
template <typename ID, class Value>
class UGGridTemplate2d : public UGGridTemplate<UGReindexerTemplateCompareVec2N<UGlong>, ID, Value>
{
public:
	typedef UGReindexerTemplateCompareVec2N<UGlong> Key;
	typedef UGGridTemplate<Key, ID, Value> UGGridBase;

	UGGridTemplate2d(UGdouble dResolution) : UGGridBase(dResolution) {}

	virtual ID Align(Value& val)
	{
		Key iKey(0, 0);
		CreateGridKey(val, iKey);

		typename UGGridBase::GridContainer::const_iterator iter = this->m_gcDB.find(iKey);
		if (iter == this->m_gcDB.end())
		{
			UGlong iX = iKey.x;
			UGlong iY = iKey.y;
			UGint iDist = 10; // > max possible distance
			for (UGlong jX = iX - 1; jX <= iX + 1; jX++)
			{
				for (UGlong jY = iY - 1; jY <= iY + 1; jY++)
				{
					Key jKey(jX, jY);
					typename UGGridBase::GridContainer::const_iterator tmpiter = this->m_gcDB.find(jKey);
					if (tmpiter == this->m_gcDB.end())
					{
						continue;
					}

					UGint jDist = abs(UGint(iX - jX)) + abs(UGint(iY - jY));
					if (jDist < iDist)
					{
						iDist = jDist;
						iter = tmpiter;
					}
				}
			}
		}

		ID id;
		if (iter == this->m_gcDB.end())
		{ // Not found: insert using key
			id = this->m_gcDB.size();
			this->m_gcDB.insert(std::make_pair(iKey, std::make_pair(id, val)));
		}
		else
		{
			// If found return existing data
			id = iter->second.first;
			val = iter->second.second;
		}

		return id;
	}

	virtual UGbool Has(const Value& val, ID* pID = NULL) const
	{
		Key iKey(0, 0);
		CreateGridKey(val, iKey);

		typename UGGridBase::GridContainer::const_iterator iter = this->m_gcDB.find(iKey);
		if (iter != this->m_gcDB.end())
		{
			if (pID != NULL)
			{
				*pID = iter->second.first;
			}
			return TRUE;
		}

		UGlong iX = iKey.x;
		UGlong iY = iKey.y;
		for (UGlong jX = iX - 1; jX <= iX + 1; jX++)
		{
			for (UGlong jY = iY - 1; jY <= iY + 1; jY++)
			{
				Key jKey(jX, jY);
				iter = this->m_gcDB.find(jKey);
				if (iter != this->m_gcDB.end())
				{
					if (pID != NULL)
					{
						*pID = iter->second.first;
					}
					return TRUE;
				}
			}
		}
		return FALSE;
	}

	virtual UGbool Equal(Value val1, Value val2)
	{
		Align(val1);
		Align(val2);
		return (fabs(val1.x - val2.x) < this->m_dResolution && fabs(val1.y - val2.y) < this->m_dResolution);
	}

private:
	void CreateGridKey(const Value& val, Key& key) const
	{
		key.x = UGlong(val.x / this->m_dResolution);
		key.y = UGlong(val.y / this->m_dResolution);
	}
};

//! \brief 三维网格对齐。
template <typename ID, class Value>
class UGGridTemplate3d : public UGGridTemplate<UGReindexerTemplateCompareVec3N<UGlong>, ID, Value>
{
public:
	typedef UGReindexerTemplateCompareVec3N<UGlong> Key;
	typedef UGGridTemplate<Key, ID, Value> UGGridBase;

	UGGridTemplate3d(UGdouble dResolution) : UGGridBase(dResolution) {}

	virtual ID Align(Value& val)
	{
		Key iKey(0, 0, 0);
		CreateGridKey(val, iKey);

		typename UGGridBase::GridContainer::const_iterator iter = this->m_gcDB.find(iKey);
		if (iter == this->m_gcDB.end()) 
		{
			UGlong iX = iKey.x;
			UGlong iY = iKey.y;
			UGlong iZ = iKey.z;
			UGfloat iDist = 10.0f; // > max possible distance
			for (UGlong jX = iX - 1; jX <= iX + 1; jX++)
			{
				for (UGlong jY = iY - 1; jY <= iY + 1; jY++)
				{
					for (UGlong jZ = iZ - 1; jZ <= iZ + 1; jZ++)
					{
						Key jKey(jX, jY, jZ);
						typename UGGridBase::GridContainer::const_iterator tmpiter = this->m_gcDB.find(jKey);
						if (tmpiter == this->m_gcDB.end())
						{
							continue;
						}

						UGfloat jDist = sqrt((iX - jX) * (iX - jX) + (iY - jY) * (iY - jY) + (iZ - jZ) * (iZ - jZ));
						if (jDist < iDist)
						{
							iDist = jDist;
							iter = tmpiter;
						}
					}
				}
			}
		}

		ID id;
		if (iter == this->m_gcDB.end())
		{ // Not found: insert using key
			id = this->m_gcDB.size();
			this->m_gcDB.insert(std::make_pair(iKey, std::make_pair(id, val)));
		}
		else
		{
			// If found return existing data
			id = iter->second.first;
			val = iter->second.second;
		}

		return id;
	}

	virtual UGbool Has(const Value& val, ID* pID = NULL) const
	{
		Key iKey(0, 0, 0);
		CreateGridKey(val, iKey);

		typename UGGridBase::GridContainer::const_iterator iter = this->m_gcDB.find(iKey);
		if (iter != this->m_gcDB.end())
		{
			if (pID != NULL)
			{
				*pID = iter->second.first;
			}
			return TRUE;
		}

		UGlong iX = iKey.x;
		UGlong iY = iKey.y;
		UGlong iZ = iKey.z;
		for (UGlong jX = iX - 1; jX <= iX + 1; jX++)
		{
			for (UGlong jY = iY - 1; jY <= iY + 1; jY++)
			{
				for (UGlong jZ = iZ - 1; jZ <= iZ + 1; jZ++)
				{
					Key jKey(jX, jY, jZ);
					iter = this->m_gcDB.find(jKey);
					if (iter != this->m_gcDB.end())
					{
						if (pID != NULL)
						{
							*pID = iter->second.first;
						}
						return TRUE;
					}
				}
			}
		}

		return FALSE;
	}

	virtual UGbool Equal(Value val1, Value val2)
	{
		Align(val1);
		Align(val2);
		return (fabs(val1.x - val2.x) < this->m_dResolution && 
			fabs(val1.y - val2.y) < this->m_dResolution && 
			fabs(val1.z - val2.z) < this->m_dResolution);
	}

private:
	void CreateGridKey(const Value& val, Key& key) const
	{
		key.x = UGlong(val.x / this->m_dResolution);
		key.y = UGlong(val.y / this->m_dResolution);
		key.z = UGlong(val.z / this->m_dResolution);
	}
};

typedef UGGridTemplate3d<UGint, UGVector3d> UGGrid3d;

typedef UGGridTemplate2d<UGint, UGPoint2D> UGGridP2d;
typedef UGGridTemplate3d<UGint, UGPoint3D> UGGridP3d;
}
#endif // !defined(AFX_UGPOLYSET_H__F8C91F88_9FBD_40F7_AB5D_17220764D627__INCLUDED_)

