#pragma once
#include "Engine/EngineData.hpp"
#include "ThirdParty/D3D12DXR/WICTextureLoader.h"
#include "ThirdParty/D3D12DXR/d3dx12.h"

typedef Microsoft::WRL::ComPtr<ID3D12Resource> ResourcePtr;

typedef D3D12_GPU_VIRTUAL_ADDRESS Dx12VirtualGPUAddr;
typedef D3D12_CPU_DESCRIPTOR_HANDLE Dx12CpuDescriptorHandle;
typedef D3D12_GPU_DESCRIPTOR_HANDLE Dx12GpuDescriptorHandle;
typedef D3D12_RESOURCE_STATES Dx12ResourceStates;

struct GpuBuffer
{
    GpuBuffer();
    ~GpuBuffer();
    GpuBuffer(ResourcePtr bufferResource);
    void AllocateResource(UINT bufferSize);
    void SetDebugName(const std::wstring& name);
    ID3D12Resource* GetResource();
    void ResetResource();
public:
    ResourcePtr                            m_resource;
    Dx12VirtualGPUAddr                     m_gpuVirtualAddress;
    UINT64                                 m_sizeInBytes;
    std::wstring                           m_debugName;
};

struct ShaderHandle
{
public:
    /*~ShaderHandle();*/

    uint8_t*            MapCpuWriteOnly();
    void                ResetResources();
public:
    Dx12ResourceStates  m_usageState = D3D12_RESOURCE_STATE_COMMON;
    GpuBuffer*          m_gpuBuffer = nullptr; 
    uint8_t*            m_mappedGPUdata = nullptr;
   
};

template <typename VertexType>
struct VertexBufferHandle : public ShaderHandle
{
    
};

struct IndexBufferHandle  : public ShaderHandle
{
    
};

template <typename DataType>
struct ConstantBufferHandle :  public ShaderHandle
{
public:
    ConstantBufferHandle() = default;
    ConstantBufferHandle(int totalInstances) {
        m_totalInstances = totalInstances;
        m_alignedInstanceSize = Align(sizeof(DataType), D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT);
        UINT bufferSize = totalInstances * m_alignedInstanceSize;
        m_gpuBuffer = new GpuBuffer();
        m_gpuBuffer->AllocateResource(bufferSize);
        m_mappedGPUdata = MapCpuWriteOnly();
    }
public:
    void                     CopyCputoGpu(UINT instanceIndex) {
        memcpy(m_mappedGPUdata + instanceIndex * m_alignedInstanceSize, &m_data, sizeof(DataType));
    };
    void                     SetDebugName(const std::wstring& name);
    Dx12VirtualGPUAddr       GpuVirtualAddress(UINT instanceIndex = 0) { 
    return m_gpuBuffer->m_resource->GetGPUVirtualAddress() + instanceIndex * m_alignedInstanceSize; 
    }
public:
    DataType*                operator->() { return &m_data;}
public:
    UINT                     m_alignedInstanceSize = 0;
    UINT                     m_totalInstances = 0;
    alignas(16) DataType     m_data;

};

struct ShaderResourceHandle :  public ShaderHandle
{
    UINT                     m_srvHeapIndex  = UINT_MAX;
    Dx12CpuDescriptorHandle  m_cpuDescriptorHandle;
    Dx12GpuDescriptorHandle  m_gpuReadDescriptorHandle;
};

struct DepthStencilHandle   :  public ShaderHandle
{
    D3D12_DEPTH_STENCIL_DESC m_depthDesc;
    Dx12CpuDescriptorHandle  m_cpuDescriptorHandle;
};

