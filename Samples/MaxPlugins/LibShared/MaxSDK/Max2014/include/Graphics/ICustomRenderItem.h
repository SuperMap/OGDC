//
// Copyright 2012 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once
#include "../Graphics/IRefObject.h"
#include "../baseinterface.h"
#include "DrawContext.h"

namespace MaxSDK { namespace Graphics {

/** This class is used to custom behavior of a render item. It can be set via CustormRenderItemHandle::SetImplementation.
Main behavior of a render item is defined into 2 functions: Realize and Display.
Realize() is called once per frame. It is used to prepare render data for Display() function. To improve performance, it's
recommended to build and store geometry data in this function. User should build vertex streams at least meet material requirement 
in the pipeline context.
Display() might be called more than once. User should follow material requirement in the pipeline context to draw the geometry.
In some cases, the render item may looks different between viewports(For example, billboard). So users may build geometry dynamically 
in Display() function. The vertex stream should always follow material requirement in the pipeline context.
*/
class ICustomRenderItem : public ARefObject, public BaseInterfaceServer
{
public:
	/** Realize might be called once per frame. Sometimes if the render item is culled by Max, it wouldn't call realized() anymore.
	Realize() is designed to build necessary for Display() function. Most of the requirement and pipeline states are stored in the
	PiplineContext. So user should at least build vertex buffers to meet material requirement.
	Since this function may be called every frame, it's better to have some cache mechanism to avoid unnecessary rebuild geometry.
	\param[in] drawContext context of the pipeline.
	*/
	virtual void Realize(DrawContext& drawContext) = 0;

	/** Display function might be called more than once per frame. Users can get virtual device or legacy graphics window from the
	pipeline context. It's user's job to activate a material(either from pipeline context or using a custom material), loop all passes,
	set vertex format, set vertex stream, index and call VirtualDevice::Draw()/DrawInstanced() to render geometries. Always make sure
	the vertex format should be same as pipeline context's material requirement(if exists). And the vertex stream layout should meet the
	format
	\param[in] drawContext context of the pipeline.
	*/
	virtual void Display(DrawContext& drawContext) = 0;
	
};

typedef RefPtr<ICustomRenderItem> ICustomRenderItemPtr;

} } // end namespace
