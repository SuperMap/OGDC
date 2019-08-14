//
// Copyright 2010 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "SmartHandle.h"
#include "RenderEnums.h"
#include <wtypes.h>

namespace MaxSDK { namespace Graphics {


/** This is our base graphics raster resource class. 
The graphics raster resource includes TextureHandle and Render Target.
TextureHandle is a texture class for material.
*/
class BaseRasterHandle : public SmartHandle
{
public:
	GraphicsDriverAPI BaseRasterHandle();
	GraphicsDriverAPI BaseRasterHandle(const BaseRasterHandle& from);
	GraphicsDriverAPI BaseRasterHandle& operator = (const BaseRasterHandle& from);
	GraphicsDriverAPI virtual ~BaseRasterHandle();

	/** Returns the width of raster resource.
	\return the raster width.
	*/
	GraphicsDriverAPI size_t GetWidth() const;

	/** Returns the height of raster resource.
	\return the raster height.
	*/
	GraphicsDriverAPI size_t GetHeight() const;
};

}}