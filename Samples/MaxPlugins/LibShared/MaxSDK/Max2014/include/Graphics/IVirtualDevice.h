//
// Copyright 2012 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which 
// otherwise accompanies this software in either electronic or hard copy form.   
//
//

#pragma once

#include "../maxtypes.h"
#include "../baseinterface.h"
#include "../MaxHeap.h"
#include "../Point2.h"
#include "../Box2.h"

#include "GraphicsExport.h"
#include "RenderEnums.h"
#include "VertexBufferHandle.h"
#include "IndexBufferHandle.h"
#include "TargetHandle.h"

#include "RenderStates.h"

class AColor;

namespace MaxSDK { namespace Graphics {

	/**	This class provides low level access of GPU device. It wraps variable 3D API into an unified interface.
		So no matter Direct3D9, Direct3D11 or OpenGL the real device is, most of the time developers don't 
		need to aware of this information.
		Similar as device interface in Direct3D, this interface provides rendering functions such as Draw/DrawInstanced,
		apply resource functions like SetVertexStreams, SetIndexBuffer etc. You can also change render states
		via Get/SetBlendState, Get/SetRasterizerState, Get/SetDepthStencilState.
		For example, to change Z-Bias of next draw call, use RasterizerState::SetDepthBias or 
		RasterizerState::SetSlopeScaledDepthBias.

		Typical usage of rendering geometry data may looks like this:
		\code
		IVirtualDevice& vd = drawContext.GetVirtualDevice();

		vd.SetStreamFormat(material.GetStreamFormat());
		vd.SetVertexStreams(vertexBuffers);
		vd.SetIndexBuffer(&vertexIndex);

		material.Activate(drawContext);
		material.ActivatePass(drawContext, 0); //Assuming the effect only has 1 pass
		vd.Draw(PrimitiveTriangleList, 0, primitiveCount);
		streamOutEffect.PassesFinished(drawContext);
		material.Terminate(drawContext);
		\endcode

		Typical usage of stream-out data may looks like this:
		\code
		//Turn off Z-Buffer
		vd.SetDepthStencilState(mDepthOffState);
		streamOutEffect.Activate(drawContext);
		streamOutEffect.ActivatePass(drawContext, 0);
		vd.SetStreamFormat(streamOutEffect.GetStreamFormat());
		vd.SetIndexBuffer(nullptr);
		vd.SetVertexStreams(mRawDataBuffers);
		vd.SetOutputStreams(&mStreamOutBuffers);
		vd.Draw(PrimitivePointList, 0, (int)vertexCount);
		vd.SetOutputStreams(nullptr);
		streamOutEffect.PassesFinished(drawContext);
		streamOutEffect.Terminate();
		//Restore Z-Buffer state
		vd.SetDepthStencilState(mBackupState);
		\endcode
	*/
	class IVirtualDevice : public MaxHeapOperators
	{
	protected:
		virtual ~IVirtualDevice(){}
	public:
		/** Return the thread id of the virtual device.
		\return Return 0 for DirectX9, the real thread id for DirectX11
		*/
		virtual unsigned int ThreadID() = 0;
		
		/** Gets the status of the device.
		Note: When IsValid is false, all device related resource may invalid.
		Some times plugins may dispose after Nitrous. At this time, any calls of
		device resources may cause crash. So plugins should check this flag to make
		sure the device is valid before releasing the resources.
		\return true if the device is valid. false if not.
		*/
		virtual bool IsValid() const = 0;

		/** Begins the frame draws. Should pair with EndFrame()
		brackets sets of drawing calls, only 1 level deep allowed.
		*/
		virtual bool BeginFrame() = 0;

		/** Ends frame draws. Should pare with BeginFrame()
		*/
		virtual bool EndFrame() = 0;

		/** Begins the scene draws
		brackets sets of drawing calls. Only 1 level deep allowed.
		*/
		virtual bool BeginScene() = 0;

		/** Ends scene draws.
		*/
		virtual bool EndScene() = 0;

		
		/** Gets the current rasterizer state.
		\return the rasterizer state handle
		*/
		virtual const RasterizerState& GetRasterizerState() = 0;


		/** Sets the current rasterizer state.
		\param[in] new rasterizer state
		*/
		virtual void SetRasterizerState(const RasterizerState& newState) = 0;

		
		/** Gets the current depth-stencil state.
		\return the depth stencil state handle
		*/
		virtual const DepthStencilState& GetDepthStencilState() = 0;

		
		/** Sets the current depth stencil state.
		\param[in] new depth stencil state handle
		*/
		virtual void SetDepthStencilState(const DepthStencilState& newState) = 0;
		
		/** Gets the current blend state.
		\return current blend state handle
		*/
		virtual const BlendState& GetBlendState() = 0;

		/** Sets the current depth stencil state.
		\param[in] newState new blend state handle
		*/
		virtual void SetBlendState(const BlendState& newState) = 0;


		/** Gets the pixel coordinate viewport for the drawing.
		\return viewport's {0,0,w,h}, or {0,0,0,0} if no target defined.
		*/
		virtual const Box2 Viewport() = 0;

		/** Sets the pixel coordinate viewport for the drawing
		\param[in] vp the desired viewport size, or nullptr which means "use the full screen"
		*/
		virtual void Viewport(const Box2* vp) = 0;
				
		/** Gets the depth range
		\return the depth range, normally 0.0 to 1.0. 
		*/
		virtual const Point2 DepthRange() const = 0;
		
		/** Sets the depth range 
		\param[in] depthRange the depth range, depthRange.x means near distance, depthRange.y means far distance.
		Normally the values are in 0.0 to 1.0.
		*/
		virtual void DepthRange(const Point2& depthRange) = 0;

		/** Gets scissor box in pixels
		\return the scissor box. Default value is current render target size.
		*/
		virtual const Box2 ScissorBox() = 0;

		/** Sets scissor box in pixels
		\param[in] box the scissor box, nullptr means no scissor clip.
		*/
		virtual void ScissorBox(const Box2* box) = 0;

		/** Gets the current vertex buffer stream
		\return current number of active streams
		*/
		virtual size_t VertexStreamCount() const = 0;
		
		/** Gets the current vertex buffer.
		\param[in] nStream stream index of the vertex buffer
		\return vertex buffer handle at nStream
		*/
		virtual VertexBufferHandle GetVertexStream(int nStream) const= 0;

		/** Sets vertex buffer streams
		\param[in] bufferArray the vertex buffer array
		*/
		virtual void SetVertexStreams(VertexBufferHandleArray& bufferArray) = 0;

		/* Set stream format of the vertex buffer
		\param[in] stream format of the vertex buffer
		*/
		virtual void SetStreamFormat(MaxSDK::Graphics::MaterialRequiredStreams& format) = 0;

		/** Gets the number of current output vertex buffer stream
		\return number of current output vertex buffer stream
		*/
		virtual size_t OutputStreamCount() const = 0;
		
		/** Gets the current output vertex buffer.
		Note: this function only works for DX11 device.
		\param[in] nStream index of the stream
		\return vertex buffer handle at output nStream. For dx9 device(feature level < LEVEL_4_0,
		an invalid handle will be returned. (Use VertexBufferHandle::IsValid() to check the validity)
		*/
		virtual VertexBufferHandle GetOutputStream(int nStream) const= 0;
		
		/** Sets the current output vertex buffer stream
		Note: the max number of output stream is 0 for DX9, 4 for DX11 device.
		\param[in] bufferArray sets output streams. nullptr to clear output streams.
		*/
		virtual void SetOutputStreams(VertexBufferHandleArray* bufferArray) = 0;

		/** Set render target at index 0.
		\param[in] target the pointer of render target handle.
		\param[in] zBuffer the pointer of z buffer handle. The size of zBuffer must equal or larger 
		than target. Can be nullptr. 
		*/
		virtual void SetTarget(TargetHandle* target, TargetHandle* zBuffer) = 0;

		/** Set multiple render targets
		\param[in] targetArray render targets to be set. All targets must have same size.
		\param[in] zBuffer the pointer of z buffer handle. The size of zBuffer must equal or larger 
		than render targets. Can be nullptr. 
		*/
		virtual void SetTargets(TargetHandleArray& targetArray, TargetHandle* zBuffer) = 0;

		/** Gets current render target
		\param[in] index index of the target. For DX9, maximum number of the index is 4, for DX11, it's 8. 
		\return the target handle. The handle might be invalid if corresponding index doesn't exist.
		*/
		virtual TargetHandle GetTarget(int index) = 0;

		/** Gets current render target count
		\return current target count
		*/
		virtual size_t TargetCount() = 0;

		/** Gets current z buffer
		\return current z buffer. The handle might be invalid if z buffer doesn't exist.
		*/
		virtual TargetHandle GetZBuffer() = 0;
		
		/** Gets the current index buffer.
		\return current index buffer handle
		*/
		virtual IndexBufferHandle GetIndexBuffer() const = 0;
		
		/** Sets the current index buffer.
		\param[in] indexBuffer index buffer handle. Passing nullptr to clear current index buffer.
		*/
		virtual void SetIndexBuffer(IndexBufferHandle* indexBuffer) = 0;

		/** Draws part of a set of primitives.
		\param[in] type primitive type
		\param[in] start start vertex index in current vertex buffer
		\param[in] primCount number of primitives to draw.
		*/
		virtual void Draw(PrimitiveType type, int start, int primCount) = 0;

		/** Draws part of a set of primitives with instance
		Note: you need call VertexBufferHandle::SetInstanceCount() to set instance count of the instance data stream.
		\param[in] type primitive type
		\param[in] instanceCount number of instance
		\param[in] start start vertex index in current vertex buffer
		\param[in] primCount number of primitives to draw.
		*/
		virtual void DrawInstanced(PrimitiveType type, int instanceCount, int start, int primCount) = 0;

		/** Full clear: target( nTargets ) to given color, z to given z, stencil to given stencil.
			The target (or MRT targets), instead of the viewport, is cleared.
			The Clear operation is not affected by the scissor box.
		\param[in] flags component to clear. 
		\param[in] color color after clear. Available when ClearBackbuffer or ClearAll is set.
		\param[in] z z value after clear. Available when ClearDepthBuffer or ClearAll is set.
		\param[in] stencil stencil value after clear. Available when ClearStencilBuffer or ClearAll is set.
		*/
		virtual void Clear(BufferClearFlag flags, const AColor& color, float z, int stencil) = 0;

		/** Partial clear: sub-region of target/z/stencil is cleared.
			The region is clipped to the bounds of the viewport rectangle, 
			and the Clear operation is not affected by the scissor box.
		\param[in] rect region to clear
		\param[in] flags component to clear. 
		\param[in] color color after clear. Available when ClearBackbuffer or ClearAll is set.
		\param[in] z, z value after clear. Available when ClearDepthBuffer or ClearAll is set.
		\param[in] stencil stencil value after clear. Available when ClearStencilBuffer or ClearAll is set.
		*/
		virtual void ClearRegion(
			const Box2& rect,
			BufferClearFlag flags, 
			const AColor& color,  
			float z,  
			int stencil
			) = 0;
	};

} }
