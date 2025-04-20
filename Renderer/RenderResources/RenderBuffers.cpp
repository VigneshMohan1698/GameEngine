#include "RenderBuffers.hpp"

GpuBuffer::GpuBuffer()
{
	m_resource = nullptr;
	m_gpuVirtualAddress = 0;
	m_sizeInBytes = 0;
}

GpuBuffer::~GpuBuffer()
{
	//Do nothing underlying compptr takes care of RAII
}

GpuBuffer::GpuBuffer(ResourcePtr bufferResource)
{
	m_resource = bufferResource;
	m_gpuVirtualAddress = bufferResource->GetGPUVirtualAddress();
	m_sizeInBytes = bufferResource->GetDesc().Width;
}

void GpuBuffer::SetDebugName(const std::wstring& name)
{
    m_debugName = name;
    if (m_resource)
    {
        m_resource->SetName(name.c_str());
    }
}

ID3D12Resource* GpuBuffer::GetResource()
{
	return m_resource.Get();
}

void GpuBuffer::ResetResource()
{
	m_resource.Reset();
}
