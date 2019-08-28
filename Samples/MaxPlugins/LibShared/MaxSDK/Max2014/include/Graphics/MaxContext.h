//
// Copyright 2010 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.
//
//

#pragma once

#include "Types.h"
#include "GraphicsExport.h"
#include "../MaxHeap.h"

class ViewExp;

namespace MaxSDK { namespace Graphics {

class MaxContextImpl;

/** MaxContext contains pointers to all the viewports the given object 
will be displayed.

This is setup by system and is passed to plugin via IObjectDisplay::AddRenderItem 
function. It is an optional information for plugin developers, plugins can 
add/customize render items at that time according to this information.

Instances of MaxContext are very temporary objects and should only be used 
inside IObjectDisplay::AddRenderItem. Plugins should not store this object.
*/
class MaxContext : public MaxHeapOperators
{
public:
	GraphicsDriverAPI MaxContext();
	GraphicsDriverAPI virtual ~MaxContext();

	/** Get the number of viewport pointers of this context.
	\return the number of viewport pointers of this context.
	*/
	GraphicsDriverAPI size_t GetNumberOfViewports() const;

	/** Get the i-th viewport pointer of this context.
	\param i the index in this context.
	\return the i-th viewport pointer of this context.
	*/
	GraphicsDriverAPI ViewExp* GetViewport(size_t i) const;

protected:
	MaxContextImpl* mpImpl;
};

} } // namespaces
