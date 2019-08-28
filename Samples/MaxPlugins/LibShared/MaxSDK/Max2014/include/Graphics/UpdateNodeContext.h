//
// Copyright 2012 Autodesk, Inc.  All rights reserved.
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
#include "../maxtypes.h"

class ViewExp;

namespace MaxSDK { namespace Graphics {

class UpdateNodeContextImpl;

/** This class contains node dependent information for calling IObjectDisplay2::UpdatePerNodeItems(),
	and IObjectDisplay2::UpdatePerViewItems()
*/
class UpdateNodeContext : public MaxHeapOperators
{
public:
	GraphicsDriverAPI UpdateNodeContext();
	GraphicsDriverAPI virtual ~UpdateNodeContext();

	/** Get the current node to update render items.
	*/
	GraphicsDriverAPI RenderNodeHandle& GetRenderNode();

	/** Set the current node to update render items.
	*/
	GraphicsDriverAPI void SetRenderNode(RenderNodeHandle& renderNode);

protected:
	UpdateNodeContextImpl* mpImpl;
};

} } // end namespaces
