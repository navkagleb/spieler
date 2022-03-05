#pragma once

#include <string>

#include "renderer/bindable.h"

namespace Spieler
{

    class Resource : public Bindable
    {
    protected:
        static ComPtr<ID3D12Resource> CreateUploadBuffer(const D3D12_RESOURCE_DESC& resourceDesc);
        static ComPtr<ID3D12Resource> CreateDefaultBuffer(const D3D12_RESOURCE_DESC& resourceDesc);

        template <typename T>
        static void CopyDataToDefaultBuffer(ComPtr<ID3D12Resource>& buffer, const ComPtr<ID3D12Resource>& uploadBuffer, const T* data, std::uint32_t count);

    public:
        void SetName(const std::wstring& name);

    protected:
        ComPtr<ID3D12Resource> m_Resource;
    };

} // namespace Spieler

#include "resource.inl"