//
// Copyright 2011 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "SmartHandle.h"
#include "GeometryEnums.h"
#include "RenderEnums.h"

namespace MaxSDK { namespace Graphics {

/** IndexBufferHandle is a memory buffer that contain index data. Index data, or 
indices, are integer offsets into vertex buffers and are used to render primitives. 
*/
class IndexBufferHandle : public SmartHandle
{
public:
	GraphicsDriverAPI IndexBufferHandle();
	GraphicsDriverAPI IndexBufferHandle(const IndexBufferHandle& from);
	GraphicsDriverAPI IndexBufferHandle& operator = (const IndexBufferHandle& from);
	GraphicsDriverAPI virtual ~IndexBufferHandle();

public:
	/** Initialize the index buffer. 
	\param type The type of the newly created buffer. 
	\return true if successfully initialized, false otherwise.
	*/
	GraphicsDriverAPI bool Initialize(IndexType type);

	/** Get the type of the index buffer.
	\return The type of the index buffer as an IndexType 
	*/
	GraphicsDriverAPI IndexType GetType() const;

	/** Get the number of indices of this buffer.
	\return The number of indices
	*/
	GraphicsDriverAPI size_t GetNumberOfIndices() const;

	/** Set the number of indices of this buffer.
	\param numberOfIndices The size of the buffer
	*/
	GraphicsDriverAPI void SetNumberOfIndices(size_t numberOfIndices);
	
	/** This function is used for reading/writing data from/into the index buffer.

	\param index the start index that the lock operation starts
	\param numberOfIndices the number of indices that this function will lock.
	\param accessType indicate how to operate on the buffer.
	\return the buffer for developers to read/write index data. The size 
	of the buffer is numberOfIndices * indexStride.
	*/
	GraphicsDriverAPI unsigned char* Lock(size_t index, size_t numberOfIndices, AccessType accessType);

	/** Unlock the buffer to update. This function must be called after Lock. 
	Once this function is called, the buffer that returned by Lock become invalid 
	and should not be used any more.
	*/
	GraphicsDriverAPI void Unlock();
};

} } // namespaces
