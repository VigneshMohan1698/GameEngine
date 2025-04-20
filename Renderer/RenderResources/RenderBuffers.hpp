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
    void SetDebugName(const std::wstring& name);
    ID3D12Resource* GetResource();
    void ResetResource();
public:
    ResourcePtr                            m_resource;
    Dx12VirtualGPUAddr                     m_gpuVirtualAddress;
    UINT64                                 m_sizeInBytes;
    std::wstring                           m_debugName;
};

struct VertexBufferView
{
    Dx12VirtualGPUAddr                     m_bufferLocation;
    UINT                                   m_sizeInBytes;
    UINT                                   m_strideInBytes;
};

struct IndexBufferView
{
    Dx12VirtualGPUAddr                     m_bufferLocation;
    UINT                                   m_sizeInBytes;
    DXGI_FORMAT                            m_format; // e.g., DXGI_FORMAT_R16_UINT, DXGI_FORMAT_R32_UINT
};

struct ShaderHandle
{
    Dx12ResourceStates  m_usageState = D3D12_RESOURCE_STATE_COMMON;
    GpuBuffer*          m_gpuBuffer = nullptr; 

    ~ShaderHandle()     {  m_gpuBuffer = nullptr; }
};

struct VertexBufferHandle   : public ShaderHandle
{

};

struct IndexBufferHandle    : public ShaderHandle
{

};

struct ConstantBufferHandle :  public ShaderHandle
{

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
