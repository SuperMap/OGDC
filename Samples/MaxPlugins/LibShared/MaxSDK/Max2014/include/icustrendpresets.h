/////////////////////////////////////////////////////////////////////////
//
//
//	Render Preset Interface
//
//	Created 5/14/2003	Tom Burke
//

#pragma once

// includes
#include "maxheap.h"
#include "sfx.h"
#include "itargetedio.h"
#include "irenderpresets.h"

//////////////////////////////////////////////////////////////
// Class ICustomRenderPresets
/*! 
\sa Class Renderer
\par Description:
	Render Presets compatibility interface:\n\n
	used by renderers to support renderer specific presets.
	If the renderer does not support this interface, the entire
	renderer will be saved.
*/
//////////////////////////////////////////////////////////////
class ICustomRenderPresets: public MaxHeapOperators
{
public:
	virtual int  RenderPresetsFileVersion() = 0;
	virtual BOOL RenderPresetsIsCompatible( int version ) = 0;

	virtual const MCHAR* RenderPresetsMapIndexToCategory( int catIndex ) = 0;
	virtual int     RenderPresetsMapCategoryToIndex( const MCHAR* category ) = 0;
#pragma warning( push )
#pragma warning( disable : 4481 )
	//! \deprecated Deprecated method in terms of implementation as of 3ds Max 2013 - re-implement with const MCHAR* method signature
	virtual int     RenderPresetsMapCategoryToIndex( MCHAR* category ) sealed { 
		return RenderPresetsMapCategoryToIndex(const_cast<const MCHAR*>(category)); }
#pragma warning( pop )

	virtual int RenderPresetsPreSave( ITargetedIO * root, BitArray saveCategories ) = 0;
	virtual int RenderPresetsPostSave( ITargetedIO * root, BitArray loadCategories ) = 0;
	virtual int RenderPresetsPreLoad( ITargetedIO * root, BitArray saveCategories ) = 0;
	virtual int RenderPresetsPostLoad( ITargetedIO * root, BitArray loadCategories ) = 0;
};

