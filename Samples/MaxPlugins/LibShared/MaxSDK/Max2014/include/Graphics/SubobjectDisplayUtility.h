//
// Copyright 2012 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "IDisplayCallback.h"
#include "MaxContext.h"
#include "RenderItemHandle.h"
#include "RenderNodeHandle.h"
#include "RenderItemHandleArray.h"
#include "BaseMaterialHandle.h"
#include "IObjectDisplay.h"

class BaseObject;

namespace MaxSDK{ namespace Graphics {

class ChildObjectDisplayCallbackImp;

/** This is the utility display callback class for any compound object that would 
	like to customize the display of its child objects so that each child object could
	decide on its own how its child render items would be rendered when 
	IDisplayCallback::DoDisplay() is invoked.
\sa class IDisplayCallback, class ImmediateRenderItemHandle
*/
class ChildObjectDisplayCallback : public MaxSDK::Graphics::IDisplayCallback 
{
public:
	GraphicsDriverAPI ChildObjectDisplayCallback();
	GraphicsDriverAPI virtual ~ChildObjectDisplayCallback();

	/** Get the number of child render items.
	\return the number of child render items.
	*/
	GraphicsDriverAPI size_t GetNumberOfRenderItems() const;

	/** Get the i-th child render item.
	\param i the index of the render item to be retrieved.
	\return the i-th child render item.
	*/
	GraphicsDriverAPI RenderItemHandle GetRenderItem(size_t i) const;

	/** Get the material of the i-th child render item.
	\param i the index of the child render item.
	\return the material of i-th child render item.
	*/
	GraphicsDriverAPI BaseMaterialHandle GetDefaultMaterial(size_t i) const;

	/** Add a child render item. 
		\param pRenderItem the render item to add.
		\param pMaterial the material that is to be assigned to this render item.
	*/
	GraphicsDriverAPI void AddRenderItem(const RenderItemHandle& pRenderItem,
		const BaseMaterialHandle& pMaterial);

	/** Remove the child render item with the specified index.
	\param i the index of the render item to be removed.
	*/
	GraphicsDriverAPI void RemoveRenderItem(size_t i);

	/** Remove all child render items.
	*/
	GraphicsDriverAPI void ClearAllRenderItems();

protected:
	ChildObjectDisplayCallbackImp* mpImp;
};

typedef RefPtr<ChildObjectDisplayCallback> ChildObjectDisplayCallbackPtr;

/** This is the utility AddRenderItemsHelper class for any compound object that would 
	like to customize the display of its child-objects.
*/
class CompoundObjectAddRenderItemsHelper : public IAddRenderItemsHelper
{
public:
	/** Default constructor.
	\param pCompoundObject the compound object that will add its render item
	*/
	GraphicsDriverAPI CompoundObjectAddRenderItemsHelper(
		BaseObject* pCompoundObject);
	GraphicsDriverAPI ~CompoundObjectAddRenderItemsHelper();
	
	/** Call this function if you'd customize the display of the specified child-object
		so as to add all of its render items.
		\param maxContext Contains all viewports in which the object will be displayed.
		\param hTargetNode the render node handle that reference the plugin object. 
		\param targetRenderItemContainer the target render item container to which this 
				plugin object will be added.
		\param pChildObject the specified child object
		\param objectData object data related with pChildObject
		\remark objectData would be passed to CreateChildObjectDisplayCallback() when 
			the ChildObjectDisplayCallback instance associated with pChildObject will be
			created.
	*/
	GraphicsDriverAPI void AddChildObjectRenderItems(
		const MaxContext& maxContext, 
		RenderNodeHandle& hTargetNode,
		IRenderItemContainer& targetRenderItemContainer,
		BaseObject* pChildObject,
		LONG_PTR objectData);

protected:
	/** Implement this virtual function to create a concrete ChildObjectDisplayCallback
		instance related with the specified child object. 
		\param pChildObject the specified child-object
		\param objectData object data passed through when calling AddChildObjectRenderItems()
		\remark Developer should implement the concrete ChildObjectDisplayCallback class to
		customize the display of all child render items in IDisplayCallback::DoDisplay()
	*/
	virtual ChildObjectDisplayCallback* CreateChildObjectDisplayCallback(
		BaseObject* pChildObject,
		LONG_PTR objectData) = 0;

	BaseObject* mpParentObject;
};



}}