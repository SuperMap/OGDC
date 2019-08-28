//
// Copyright 2011 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "Types.h"
#include "GraphicsExport.h"

namespace MaxSDK { namespace Graphics {
	
#define  MAX_VERTEX_STREAMS 16
#define  MAX_OUTPUT_STREAMS 4

	/** Supported primitive types. 
	*/
	enum PrimitiveType
	{
		/** A list of independent points.
		*/
		PrimitivePointList,

		/** A list of unconnected two-point line segments, such that the number 
		of lines is the the total number of points divided by two.
		*/
		PrimitiveLineList,

		/** A polyline, where each point is connected to the previous vertex, 
		yielding a single polyline with (vertex_count - 1) two-point line segments.
		*/
		PrimitiveLineStrip,

		/** A list of unconnected triangles, yielding (vertex_count / 3) triangles.
		*/
		PrimitiveTriangleList,

		/** 
		A strip of connected triangles, such that each vertex after the first 
		three vertices defines a new triangle, yielding (vertex_count - 1) connected triangles.

		\remarks Strips of even a small number of triangles provides better performance 
		and reduced memory consumption relative to the same geometry represented with a 
		triangle list. There is substantial research and software devoted to generating 
		optimal triangle strips.  See http://en.wikipedia.org/wiki/Triangle_strip for details.
		*/
		PrimitiveTriangleStrip,

		PrimitiveTypeCount, 
	};

	/** This flag describes the way to clear the current buffers.
	*/
	enum BufferClearFlag
	{
		/** clear the target
		*/
		ClearBackbuffer = 0x1, 
		/** clear the zbuffer
		*/
		ClearDepthBuffer = 0x2, 
		/** clear the stencil
		*/
		ClearStencilBuffer = 0x4, 
		/** clear the zbuffer, target & stencil
		*/
		ClearAll = 0x7,
	};

	/** blend selector. selects a blend source/destination
	*/
	enum BlendSelector
	{
		/** use blend factor in blend state
		*/
		BlendSelectorBlendFactor,
		/** use inverse source alpha
		*/
		BlendSelectorBothInvSourceAlpha,
		/** use source aplha
		*/
		BlendSelectorBothSourceAlpha,
		/** use the destination alpha
		*/
		BlendSelectorDestinationAlpha,
		/** use the destination color
		*/
		BlendSelectorDestinationColor,
		/** use inversed blend factor
		*/
		BlendSelectorInvBlendFactor,
		/** use the inverse destination alpha
		*/
		BlendSelectorInvDestinationAlpha,
		/** use the inverse destination color
		*/
		BlendSelectorInvDestinationColor,
		/** use inverse source alpha
		*/
		BlendSelectorInvSourceAlpha,
		/** use inverse source color
		*/
		BlendSelectorInvSourceColor,
		/** always 1,1,1
		*/
		BlendSelectorOne,
		/** use the source alpha
		*/
		BlendSelectorSourceAlpha,
		/** use the source alpha and saturate it
		*/
		BlendSelectorSourceAlphaSat,
		/** use the source color
		*/
		BlendSelectorSourceColor,
		/** always 0
		*/
		BlendSelectorZero

	};


	/** cull type.
	*/
	enum CullMode
	{
		/** cull backfaces of CW polygons
		*/
		CullModeCW, 
		/** cull backfaces of CCW polygons
		*/
		CullModeCCW,
		/** don't cull backfaces 
		*/
		CullModeNone
	};

	/// <description>
	/// fill mode for polygons
	/// </description>
	enum FillMode
	{
		/** draw as a solid object
		*/
		FillSolid, 
		/** draw as a wireframe object
		*/
		FillWireFrame
	} ;

	/** compare functions
	*/
	enum CompareFunction 
	{
		/** always true
		*/
		CompareFunctionAlways,
		/** true if equal
		*/
		CompareFunctionEqual,
		/** true if greater
		*/
		CompareFunctionGreater, 
		/** true if greater or equal
		*/
		CompareFunctionGreaterEqual,
		/** true if less
		*/
		CompareFunctionLess,
		/** true if less or equal
		*/
		CompareFunctionLessEqual,
		/** never true
		*/
		CompareFunctionNever,
		/** true if not equal
		*/
		CompareFunctionNotEqual
	};

	enum ColorWriteEnable
	{
		/** None, all off.
		*/
		ColorWriteEnableNone = 0,
		/** Write alpha.
			Same as d3d.ColorWriteEnable.Alpha,
		*/
		ColorWriteEnableAlpha = 0x8, 
		/** Write blue.
			Same as d3d.ColorWriteEnable.Blue,
		*/
		ColorWriteEnableBlue = 0x4, 
		/** Write green.
			Same as d3d.ColorWriteEnable.Green,
		*/
		ColorWriteEnableGreen = 0x2, 
		/** Write red.
			Same as d3d.ColorWriteEnable.Red,
		*/
		ColorWriteEnableRed = 0x1, 
		/** Write RGB but no alpha.
			Same as d3d.ColorWriteEnable.RedGreenBlue,
		*/
		ColorWriteEnableRedGreenBlue = 0x7, 
		/** Write all RGBA.
			Same as d3d.ColorWriteEnable.RedGreenBlueAlpha,
		*/
		ColorWriteEnableRedGreenBlueAlpha = 0xf, 

		WRITEENABLE_FORCE_DWORD      = 0x7fffffff, /* force 32-bit size enum */
	};

	/** blend operations
	*/
	enum BlendOperation 
	{
		/** add the source and destination together
		*/
		BlendOperationAdd,
		/** max of the source and destination
		*/
		BlendOperationMax,
		/** min of the source and destination
		*/
		BlendOperationMin, 
		/** dest gets dest minus source
		*/
		BlendOperationSubtract,
		/** dest gets source minus dest
		*/
		BlendOperationReverseSubtract,
	};

	
	/** Stencil buffer operation.
	*/
	enum StencilOperationType
	{
		/** Decrement the stencil buffer entry, wrapping to the maximum value if the new value is less than zero. 
			Same as d3d.StencilOperation.Decrement,
		*/
		StencilOperationTypeDecrement = 8, 
		/** Decrement the stencil buffer entry, clamping to zero.
			Same as d3d.StencilOperation.DecrementSaturation,
		*/
		StencilOperationTypeDecrementSaturation = 5, 
		
		/** Increment the stencil buffer entry, wrapping to zero if the new value exceeds the maximum value. 
			Same as d3d.StencilOperation.Increment,
		*/
		StencilOperationTypeIncrement = 7, 
		
		/** Increment the stencil buffer entry, clamping to the maximum value.
			Same as d3d.StencilOperation.IncrementSaturation,
		*/
		StencilOperationTypeIncrementSaturation = 4, 
		
		/** Invert the bits in the stencil-buffer entry. 
			Same as d3d.StencilOperation.Invert
		*/
		StencilOperationTypeInvert = 6, 
		
		/** Do not update the entry in the stencil buffer. This is the default value. 
			Same as d3d.StencilOperation.Keep,
		*/
		StencilOperationTypeKeep = 1, 
		
		/** Replace the stencil-buffer entry with a reference value.
			Same as d3d.StencilOperation.Replace,
		*/
		StencilOperationTypeReplace = 3, 

		/** Set the stencil-buffer entry to 0. 
			Same as d3d.StencilOperation.Zero,
		*/
		StencilOperationTypeZeroStencil = 2, 

		STENCILOP_FORCE_DWORD      = 0x7fffffff, /* force 32-bit size enum */

	};

	/** AccessType Types for accessing buffers.
	*/
	enum AccessType
	{
		/** Data is not accessed yet.
		*/
		AccessTypeNone = 0x0,
		/** Data will only be read.
		*/
		ReadAcess = 0x1,
		/** Data will only be written.
		*/
		WriteAcess = 0x2,
		/* Data will be read and written.
		*/
		ReadWriteAcess = ReadAcess | WriteAcess,  //0x3
		/** To be used by dynamic buffer for write-discard access
		*/
		WriteDiscardAcess
	};

	/** TextureFormat indicates which type the texture is.
	*/
	enum TextureFormat
	{
		/** Unknown texture format
		*/
		TextureFormatUnknown, 

		/** 8 bit int for each of RGB and A, with byte ordering ARGB
		*/
		TextureFormatA8R8G8B8,  

		/** 16 bit int for each of RGB and A, with byte ordering ABGR
		*/
		TextureFormatA16B16G16R16,

		/** 32 bit float for each of RGB and A, with byte ordering ABGR
		*/
		TextureFormatA32B32G32R32F,

		TextureFormatCount, 
	};

	
	/** This enum specifies the feature levels device supported.
	*/
	enum GraphicFeatureLevel
	{
		/** Unknown feature level
		*/
		Level_Unknown = 0,
		/** NULL feature level
		*/
		Level_NULL = 5,
		/** Feature Level 2_0
		*/
		Level2_0   = 20,
		/** Feature Level 2_1
		*/
		Level2_1   = 21,
		/** Feature Level 3_0
		*/
		Level3_0   = 30,
		/** Feature Level 3_1
		*/
		Level3_1   = 31,
		/** Feature Level 4_0
		*/
		Level4_0   = 40,
		/** Feature Level 4_1
			Notes: Feature Level4_1 corresponds to the NV DX10+ device, which
			supports some of DX10.1 features. 
		*/
		Level4_1   = 41,
		/** Feature Level 4_5
			Notes: Feature Level4_5 corresponds to DX10.1 device, which
			supports all of DX10.1 features. The OS must be Vista SP1 or above.
		*/
		Level4_5   = 45,
		/** Feature Level 5_0
			Notes: Feature Level5_0 corresponds to DX11 device, which
			supports all of DX11 features. The OS must be "Vista SP2 + Platform update" or above.
		*/
		Level5_0   = 50,

		/** Feature Level count
		*/
		Level_Count = 9
	};
	
	/** Get the number of bytes for each pixel by a given texture format.
	\param format The given texture format.
	\returns The number of bytes for each pixel by that given texture format.
	*/
	GraphicsDriverAPI size_t GetTextureFormatPixelSize(TextureFormat format);


	/** Texture type enums
	*/
	enum TextureType
	{
		/**	unknown texture type
		*/
		TextureTypeUnknown = 0,
		
		/** 1d bitmap texture
		*/
		TextureTypeImage1D = 1,

		/** 1d bitmap texture array
		*/
		TextureTypeImage1DArray = 2,
		
		/** bitmap texture
		*/
		TextureTypeImage2D = 3,

		/** bitmap texture
		*/
		TextureTypeImage2DArray = 4,
		
		/** cube map texture
		*/
		TextureTypeCubeMap = 5,
		
		/** volume texture
		*/
		TextureTypeVolume = 6,

		TextureTypeCount
	}; 

	/** Resource usage, used for texture 
	*/
	enum ResourceUsage
	{
		/** Normal usage, means it's a video memory resource
			Immutable or default depending on access
		*/
		ResourceUsageNormal = 0,

		/**	If the resource contents are to be changed often, ResourceUsageDynamic is recommended to achieve fast update.
			Used with the correct map flag( WriteDiscard or WriteNoOverwrite), maximum performance to change the
			contents will be achieved.
			Meaning of WriteNoOverwrite:
			WriteNoOverwrite signifies that the application promises not to write to data that the GPU is using. In exchange, 
			the GPU allows the application to write to other parts of the same buffer. The application must ensure that it does 
			not write over any data in use by the GPU.        
			Common Usage of WriteDiscard with WriteNoOverwrite as map flags:
			WriteDiscard and WriteNoOverwrite are normally used in conjunction with dynamic index/vertex buffers, 
			although they can also be used with dynamic textures. A common use of these two flags involves filling 
			dynamic index/vertex buffers with geometry that can be seen from the camera's current position. The first 
			time that data is entered into the buffer on a given frame, Map is called with WriteDiscard; doing so invalidates 
			the previous contents of the buffer. The buffer is then filled with all available data. Subsequent writes to the 
			buffer within the same frame should use WriteNoOverwrite. This will enable the CPU to access a resource that is
			potentially being used by the GPU as long as the restrictions described previously are respected.
		*/
		ResourceUsageDynamic = 1,

		/** Staging usage, means it's a system memory resource
			on each devices. For cpu readback
		*/
		ResourceUsageStaging = 2,

		/** Usage mask.	For anding off the high bits
		*/
		ResourceUsageMask = 0xf,       

		/** Usage for stream output. For vb & ibs, may be used as a texture
		*/
		ResourceUsageStream = 0x10,

		/** Usage for render target, it means the resource created from video memory and
			can be used as render-target
			for vb & ibs & textures, may be used as a render target/stream output buffer
		*/
		ResourceUsageTarget = 0x20,
	};
} } // end namespace

