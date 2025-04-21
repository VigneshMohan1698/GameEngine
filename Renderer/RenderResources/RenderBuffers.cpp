#include "RenderBuffers.hpp"
#include <Engine/Core/ErrorWarningAssert.hpp>
#include <Engine/Renderer/RendererD12.hpp>
#include <Engine/EngineData.hpp>

extern RendererD12* g_theRenderer;
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

void GpuBuffer::AllocateResource(UINT bufferSize)
{
	auto uploadHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto device = g_theRenderer->GetDevice();

	auto bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);
	if (FAILED(device->CreateCommittedResource(
		&uploadHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&bufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_resource))))
	{
		ERROR_AND_DIE("Failed while creating committed resource");
	}

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
void ShaderHandle::ResetResources()
{
	m_gpuBuffer->ResetResource();
}


uint8_t* ShaderHandle::MapCpuWriteOnly()
{
	uint8_t* mappedData = nullptr;
	CD3DX12_RANGE readRange(0, 0);
	if (FAILED(m_gpuBuffer->GetResource()->Map(0, &readRange, reinterpret_cast<void**>(&mappedData))))
	{
		ERROR_AND_DIE("Failed while creating committed resource");
	}
	return mappedData;
}



//template<typename DataType>
//ConstantBufferHandle<DataType>::ConstantBufferHandle(int totalInstances)
//{
//	m_totalInstances = totalInstances;
//	m_alignedInstanceSize = Align(sizeof(DataType), D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT);
//	UINT bufferSize = totalInstances * m_alignedInstanceSize;
//	m_gpuBuffer->AllocateResource(bufferSize);
//	m_mappedGPUdata = MapCpuWriteOnly();
//}

//template<typename DataType>
//Dx12VirtualGPUAddr ConstantBufferHandle<DataType>::GpuVirtualAddress(UINT instanceIndex)
//{
//	return m_gpuBuffer->m_resource->GetGPUVirtualAddress() + instanceIndex * m_alignedInstanceSize;
//}
//
//template<typename DataType>
//void ConstantBufferHandle<DataType>::CopyCputoGpu(UINT instanceIndex)
//{
//	memcpy(m_mappedGPUdata + instanceIndex * m_alignedInstanceSize, &m_data, sizeof(DataType));
//}

//template<typename DataType>
//DataType* ConstantBufferHandle<DataType>::operator->()
//{
//	return &m_data;
//}

template<typename DataType>
void ConstantBufferHandle<DataType>::SetDebugName(const std::wstring& name)
{
	m_gpuBuffer->SetDebugName(name);
}


