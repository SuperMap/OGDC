//
// Copyright 2010 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "RenderEnums.h"
#include "BaseRasterHandle.h"
#include "../Containers/Array.h"

namespace MaxSDK { namespace Graphics {

/** Application can use this class either as a render target or 
    convert it to texture and assign to model.
*/
class TargetHandle : public BaseRasterHandle
{
public:
	GraphicsDriverAPI TargetHandle();
	GraphicsDriverAPI TargetHandle(const TargetHandle& from);
	GraphicsDriverAPI TargetHandle& operator = (const TargetHandle& from);
	GraphicsDriverAPI virtual ~TargetHandle();

	/** Initialize the instance of target.
	The target format is fixed to A8R8G8B8
	A TargetHandle should be initialized before it's used
	\param[in] width the width of target.
	\param[in] height the height of target.
	\param[in] format member of the TextureFormat enumerated type, describing the pixel format of the target.
	*/
	GraphicsDriverAPI virtual bool Initialize(size_t width, size_t height);
};

typedef MaxSDK::Array<TargetHandle> TargetHandleArray;

/** This class is similar as TargetHandle. But instead of binding to a texture,
    application can use this class to present content to a specific window.
*/
class PresentableTargetHandle : public TargetHandle
{
public:
	GraphicsDriverAPI PresentableTargetHandle();
	GraphicsDriverAPI PresentableTargetHandle(PresentableTargetHandle& from);
	GraphicsDriverAPI PresentableTargetHandle& operator = (const PresentableTargetHandle& from);
	GraphicsDriverAPI virtual ~PresentableTargetHandle();

	/** Initialize the instance of target.
	The target format is fixed to A8R8G8B8
	A TextureHandle should be initialized before it's used
	\param[in] width the width of target.
	\param[in] height the height of target.
	\param[in] hWnd handle of the window
	*/
	GraphicsDriverAPI virtual bool Initialize(size_t width, size_t height, HWND hWnd);

	/** Present the content to specific window
	This method must called out of ImmediateFragment::Begin/End() block. 
	Typically, after ImmediateFragment::End().
	*/
	GraphicsDriverAPI virtual void Present();
};

}}