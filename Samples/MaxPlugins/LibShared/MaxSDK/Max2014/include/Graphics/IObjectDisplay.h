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
#include "MaterialRequiredStreams.h"
#include "RenderItemHandleArray.h"
#include "MaxContext.h"
#include "UpdateDisplayContext.h"

#include "../BaseInterface.h"

/** Interface id of IObjectDisplay.
*/
#define IOBJECT_DISPLAY_INTERFACE_ID Interface_ID(0x39820fa0, 0x6311340d)

/** Interface id of IAddRenderItemsHelper.
*/
#define IADD_RENDERITEMS_HELPER_INTERFACE_ID Interface_ID(0x71305cc8, 0x2316315d)

namespace MaxSDK { namespace Graphics {

class IObjectDisplayImpl;

/** This is the base interface for all objects that want to be displayed in viewport. 
A plug-in which desires to be displayed in the viewport must expose this interface 
to 3ds Max via the GetInterface function. Max will then obtain graphics data via 
this interfaces. The possible plug-in types that need this interface are: Modifiers, 
Objects and Controllers. 

Internally plugin should maintain a set of render items and manage the life time 
of all those items. Plugin needs to decide the type of the most fit render items 
and fill in correct graphics data into them.
*/
class IObjectDisplay : public BaseInterface
{
public:
	GraphicsDriverAPI IObjectDisplay();
	GraphicsDriverAPI virtual ~IObjectDisplay();

	/** Check if this plugin contains legacy display code and want legacy code 
	to be called as well.
	\return true if contains legacy display code, false otherwise.
	\remarks default return value is false.
	*/
	virtual bool RequiresSupportForLegacyDisplayMode() const = 0;

	/** Manually update all render items of this object. Plugin should build 
	render items in this function to satisfy the input category, flags and required streams.
	\param maxContext Contains all viewports in which this object will be displayed.
	\param displayContext The display context, which contains all the requirements for 
			plugin to generate render items.
	\return true if update successfully, false otherwise.
	*/
	virtual bool UpdateDisplay(
		const MaxContext& maxContext, 
		const UpdateDisplayContext& displayContext) = 0;

	/** Add render items of this plugin to the given node.
	\param maxContext Contains all viewports in which this object will be displayed.
	\param hTargetNode the render node handle that reference this plugin object.
	\param targetRenderItemContainer the target render item container to which this 
	plugin object will be added.
	\return true if added successfully, false otherwise.
	*/
	GraphicsDriverAPI virtual bool AddRenderItems(
		const MaxContext& maxContext, 
		RenderNodeHandle& hTargetNode, 
		IRenderItemContainer& targetRenderItemContainer);

	/** Retrieves the render items to be displayed.
	\return the render items to be displayed.
	*/
	virtual const RenderItemHandleArray& GetRenderItems() const = 0;

	/** This is the default implementation to add render items to the given node.
	\param maxContext Contains all viewports in which the object will be displayed.
	\param hTargetNode the render node handle that reference the plugin object. 
	\param targetRenderItemContainer the target render item container to which this 
			plugin object will be added.
	\param sourceRenderItemContainer the source render item container. 
	\return true if update successfully, false otherwise.
	*/
	GraphicsDriverAPI static bool DefaultAddRenderItems(
		const MaxContext& maxContext, 
		RenderNodeHandle& hTargetNode, 
		IRenderItemContainer& targetRenderItemContainer, 
		const IRenderItemContainer& sourceRenderItemContainer);

	// from BaseInterface
	GraphicsDriverAPI virtual Interface_ID GetID();

	GraphicsDriverAPI virtual BaseInterface* GetInterface(Interface_ID id);

	// internally used only
	GraphicsDriverAPI void SetFrameStamp(size_t framestamp);

	GraphicsDriverAPI size_t GetFrameStamp() const;

private:
	IObjectDisplayImpl* mpImpl;
};

/** Internal used only. This is the utility interface for any GeomObject to add 
	their render items to targetRenderItemContainer after calling BaseObject::UpdateDisplay()
*/
class IAddRenderItemsHelper : public BaseInterface
{
public:
	GraphicsDriverAPI IAddRenderItemsHelper();
	GraphicsDriverAPI virtual ~IAddRenderItemsHelper();

	virtual bool AddRenderItems(
		const MaxSDK::Graphics::MaxContext& maxContext, 
		MaxSDK::Graphics::RenderNodeHandle& hTargetNode, 
		MaxSDK::Graphics::IRenderItemContainer& targetRenderItemContainer) = 0;

	GraphicsDriverAPI virtual void DeleteInterface();
};

} } // end namespace
