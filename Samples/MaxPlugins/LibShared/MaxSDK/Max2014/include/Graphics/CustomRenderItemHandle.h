//
// Copyright 2012 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once
#include "../Graphics/RenderItemHandle.h"
#include "ICustomRenderItem.h"

namespace MaxSDK { namespace Graphics {

/** CustomRenderItemHandle is a wrapper of ICustomRenderItem. It is used to add user-custom implementation
into Max graphic system without direct derive from RenderItemHandle.
*/
class CustomRenderItemHandle : public RenderItemHandle
{
public:
	GraphicsDriverAPI CustomRenderItemHandle();
	GraphicsDriverAPI CustomRenderItemHandle(const CustomRenderItemHandle& from);
	GraphicsDriverAPI CustomRenderItemHandle& operator = (const CustomRenderItemHandle& from);
	GraphicsDriverAPI virtual ~CustomRenderItemHandle();

	/**Initialize this handle. The handle is empty after initialization. It doesn't have
	default implementation.
	*/
	GraphicsDriverAPI void Initialize();

	/**Set a ICustomRenderItem object to this handle.
	\param[in] impl User created class which derives from ICustomRenderItem interface.
	*/
	GraphicsDriverAPI void SetCustomImplementation(ICustomRenderItem* impl);

	/**Get the ICustomRenderItem object in this handle.
	\return the ICustomRenderItem object. nullptr if no implementation.
	*/
	GraphicsDriverAPI ICustomRenderItem* GetCustomeImplementation() const;
};

} } // end namespace
