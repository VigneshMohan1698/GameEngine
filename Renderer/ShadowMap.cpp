#include "Engine/Renderer/ShadowMap.hpp"
#include "Engine/Renderer/RendererD12.hpp"

ShadowMap::ShadowMap(RendererD12* renderer,ID3D12Device* device)
{
	m_device = device;
	IntVec2 shadowMapRes = ShadowMapResolution;
	m_width = shadowMapRes.x;
	m_height = shadowMapRes.y;
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;
	m_viewport.Width = (float)shadowMapRes.x;
	m_viewport.Height = (float)shadowMapRes.y;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	m_scissorRect.left = 0;
	m_scissorRect.top = 0;
	m_scissorRect.right = (int)shadowMapRes.x;
	m_scissorRect.bottom = (int)shadowMapRes.y;

	//BuildDepthResource();
	//BuildShaderResource();
	m_renderer = renderer;
	BuildBuffer();
	Vec3 ibasis = Vec3(0.0f, 0.0f, 1.0f);
	Vec3 jbasis = Vec3(-1.0f, 0.0f, 0.0f);
	Vec3 kbasis = Vec3(0.0f, 1.0f, 0.0f);
	m_shadowCamera.SetViewToRenderTransform(ibasis, jbasis, kbasis);
	m_shadowCamera.SetPerspectiveView(1.0f, 90.0f, 0.1f, 100.0f);
}
ShadowMap::~ShadowMap()
{
	m_shadowBuffer.ResetResource();
}
void ShadowMap::BuildBuffer()
{
	auto device = m_renderer->m_Rdevice;
	D3D12_RESOURCE_DESC depthStencilDesc;
	depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthStencilDesc.Alignment = 0;
	depthStencilDesc.Width = GetDimensions().x;
	depthStencilDesc.Height = GetDimensions().y;
	depthStencilDesc.DepthOrArraySize = 1;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

	D3D12_CLEAR_VALUE depthClear;
	depthClear.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthClear.DepthStencil.Depth = 1;
	depthClear.DepthStencil.Stencil = 0;

	CD3DX12_HEAP_PROPERTIES resource = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	m_renderer->ThrowIfFailed(device->CreateCommittedResource(&resource, D3D12_HEAP_FLAG_NONE, &depthStencilDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthClear,
		IID_PPV_ARGS(&m_shadowBuffer.m_resource)
	), "Failed while getting shadow buffer target");

	//--------------------------------DSV------------------------------
	m_shadowDepthStencilHandle.m_gpuBuffer  = &m_shadowBuffer;
	D3D12_DEPTH_STENCIL_VIEW_DESC depthDesc = {};
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	depthDesc.Flags = D3D12_DSV_FLAG_NONE;
	m_shadowDepthStencilHandle.m_usageState = D3D12_RESOURCE_STATE_DEPTH_WRITE;
	UINT dsvheapIndex = UINT_MAX;
	dsvheapIndex = m_renderer->AllocateDepthDescriptor(&m_shadowDepthStencilHandle.m_cpuDescriptorHandle, dsvheapIndex);
	device->CreateDepthStencilView(m_shadowBuffer.GetResource(), &depthDesc, m_shadowDepthStencilHandle.m_cpuDescriptorHandle);
	
	//--------------------------------SRV------------------------------
	m_shadowShaderResourceHandle.m_gpuBuffer = &m_shadowBuffer;
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	m_shadowShaderResourceHandle.m_srvHeapIndex = m_renderer->AllocateDescriptor(&m_shadowShaderResourceHandle.m_cpuDescriptorHandle, m_shadowShaderResourceHandle.m_srvHeapIndex);
	device->CreateShaderResourceView(m_shadowBuffer.GetResource(), &srvDesc, m_shadowShaderResourceHandle.m_cpuDescriptorHandle);
	//m_shadowBuffer.m_UsageState = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	m_shadowShaderResourceHandle.m_gpuReadDescriptorHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_renderer->m_descriptorHeap->GetGPUDescriptorHandleForHeapStart(), m_shadowShaderResourceHandle.m_srvHeapIndex, m_renderer->m_descriptorSize);

	////------------------TRANSITION RESOURCE TO BE USED AS DEPTH BUFFER---------------
	//CD3DX12_RESOURCE_BARRIER depthTransitionBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_shadowBuffer.GetResource(),
	//	D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
	//m_renderer->m_RcommandList->ResourceBarrier(1, &depthTransitionBarrier);


	//D3D12_RESOURCE_DESC depthStencilDesc;
	//depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	//depthStencilDesc.Alignment = 0;
	//depthStencilDesc.Width = GetDimensions().x;
	//depthStencilDesc.Height = GetDimensions().y;
	//depthStencilDesc.DepthOrArraySize = 1;
	//depthStencilDesc.MipLevels = 1;
	//depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	//depthStencilDesc.SampleDesc.Count = 1;
	//depthStencilDesc.SampleDesc.Quality = 0;
	//depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	//depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

	//D3D12_CLEAR_VALUE depthClear;
	//depthClear.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	//depthClear.DepthStencil.Depth = 1;
	//depthClear.DepthStencil.Stencil = 0;

	//CD3DX12_HEAP_PROPERTIES resource = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	//m_renderer->ThrowIfFailed(m_renderer->m_Rdevice->CreateCommittedResource(&resource, D3D12_HEAP_FLAG_NONE, &depthStencilDesc,
	//	D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
	//	&depthClear,
	//	IID_PPV_ARGS(&m_shadowBuffer.resource)
	//), "Failed while getting render targets");

	////------------------------UAV-----------------------
	///*D3D12_CPU_DESCRIPTOR_HANDLE& handleUAV = m_shadowBuffer.cpuDescriptorHandle;
	//m_shadowBuffer.uavHeapIndex = m_renderer->AllocateDescriptor(&handleUAV, UINT_MAX);

	//D3D12_UNORDERED_ACCESS_VIEW_DESC UAVDesc = {};
	//UAVDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
	//m_renderer->m_Rdevice->CreateUnorderedAccessView(m_shadowBuffer.GetResource(), nullptr, &UAVDesc, handleUAV);
	//m_shadowBuffer.gpuWriteDescriptorHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_renderer->m_descriptorHeap->GetGPUDescriptorHandleForHeapStart(), m_shadowBuffer.uavHeapIndex, m_renderer->m_descriptorSize);
	//m_shadowBuffer.m_UsageState = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;*/

	////--------------------------------SRV------------------------------
	//D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	//srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	//srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	//srvDesc.Texture2D.MipLevels = 1;

	//UINT heapIndex = UINT_MAX;
	//heapIndex = m_renderer->AllocateDescriptor(&m_shadowBuffer.cpuDescriptorHandle, heapIndex);
	//m_renderer->m_Rdevice->CreateShaderResourceView(m_shadowBuffer.resource.Get(), &srvDesc, m_shadowBuffer.cpuDescriptorHandle);
	//m_shadowBuffer.m_UsageState = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	//m_shadowBuffer.gpuReadDescriptorHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_renderer->m_descriptorHeap->GetGPUDescriptorHandleForHeapStart(), heapIndex, m_renderer->m_descriptorSize);

}
void ShadowMap::ClearShadows() 
{
	CD3DX12_RESOURCE_BARRIER depthTransitionBarrier = CD3DX12_RESOURCE_BARRIER::Transition(m_shadowBuffer.GetResource(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_DEPTH_WRITE );
	m_renderer->m_RcommandList.Get()->ResourceBarrier(1, &depthTransitionBarrier);

	D3D12_CLEAR_FLAGS flags = D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL;
	m_renderer->m_RcommandList.Get()->ClearDepthStencilView(m_shadowDepthStencilHandle.m_cpuDescriptorHandle, flags, 1, 0, 0, nullptr);
}
GpuBuffer* ShadowMap::GetShaderResourceBuffer()
{
	return &m_shadowBuffer;
}
void ShadowMap::UpdateCameraPosition(Vec3 position, EulerAngles orientation)
{
	m_shadowCamera.SetTransform(position, orientation);
}

void ShadowMap::UpdateCameraFov(float fov)
{
	m_shadowCamera.SetFov(fov);
	m_shadowCamera.RecalculateCameraMatrix();
}

void ShadowMap::UpdateCameraSettingsAndRecalculateProjection(float fov, float znear, float zfar)
{
	m_shadowCamera.SetFov(fov);
	m_shadowCamera.SetZnearAndFar(znear, zfar);
	m_shadowCamera.RecalculateCameraMatrix();
}

void ShadowMap::UpdateCameraNearAndFar(float znear, float zfar)
{
	m_shadowCamera.SetZnearAndFar(znear, zfar);
	m_shadowCamera.RecalculateCameraMatrix();
}
