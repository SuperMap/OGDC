//
// Copyright 2010 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "DataBridgeExport.h"
#include "Types.h"
#include "../MaxHeap.h"
#include "GraphicsExport.h"

class Matrix3;

namespace MaxSDK { namespace Graphics {

class Matrix44 : public MaxHeapOperators
{
public:
	inline Matrix44();
	inline Matrix44(const Matrix44& from);
	inline Matrix44& operator = (const Matrix44& from);
	inline ~Matrix44();

	DataBridgeAPI Matrix44& MakeTranspose();
	DataBridgeAPI Matrix44& MakeIdentity();
	DataBridgeAPI float MakeInverse();
	DataBridgeAPI Matrix44& MakeMultiply(const Matrix44& m2);

	DataBridgeAPI static Matrix44& Multiply(
		Matrix44& mOut, 
		const Matrix44& m1, 
		const Matrix44& m2);

	// warning C4201: nonstandard extension used : nameless struct/union
#pragma warning(disable:4201)
	union {
		struct {
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[4][4];
	};
#pragma warning(default:4201)
};

inline Matrix44::Matrix44()
{
	// do nothing.
}

inline Matrix44::Matrix44(const Matrix44& from)
{
	memcpy(m, from.m, sizeof(m));
}

inline Matrix44& Matrix44::operator = (const Matrix44& from)
{
	if (&from == this)
	{
		return *this;
	}

	memcpy(m, from.m, sizeof(m));

	return *this;
}

inline Matrix44::~Matrix44()
{
	// do nothing.
}

/** Convert Matrix3 to Matrix44
\param outMatrix the Matrix44 output
\param inMatrix the source Matrix3
*/
GraphicsDriverAPI void MaxWorldMatrixToMatrix44(
	Matrix44& outMatrix, 
	const Matrix3& inMatrix);

/** Convert Matrix44 to Matrix3
\param outMatrix the Matrix3 output
\param inMatrix the source Matrix44
*/
GraphicsDriverAPI void Matrix44ToMaxWorldMatrix(
	Matrix3& outMatrix,
	const Matrix44& inMatrix);

} } // namespaces
