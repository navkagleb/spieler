#pragma once

#include "spieler/renderer/resource.hpp"
#include "spieler/renderer/types.hpp"
#include "spieler/renderer/descriptor_manager.hpp"
#include "spieler/renderer/upload_buffer.hpp"
#include "spieler/renderer/resource_view.hpp"

namespace spieler::renderer
{

    class Device;
    class Context;

    struct Image
    {
        std::unique_ptr<uint8_t[]> Data;
    };

    class Texture2DResource : public Resource
    {
    public:
        GraphicsFormat GetFormat() const { return m_Format; }

    public:
        bool InitAsDepthStencil(Device& device, const Texture2DConfig& config, const DepthStencilClearValue& depthStencilClearValue);
        bool LoadFromDDSFile(Device& device, Context& context, const std::wstring& filename, UploadBuffer& uploadBuffer);
        void SetResource(ComPtr<ID3D12Resource>&& resource);

    private:
        GraphicsFormat m_Format{ GraphicsFormat::UNKNOWN };
        Image m_Image;
    };

    template <typename T>
    concept TextureResourceView = std::_Is_any_of_v<T, RenderTargetView, DepthStencilView, ShaderResourceView>;

    class Texture2D
    {
    private:
        using ResourceViewInternal = std::variant<RenderTargetView, DepthStencilView, ShaderResourceView>;
        using ViewContainer = std::unordered_map<uint64_t, ResourceViewInternal>;

    public:
        Texture2D() = default;

    public:
        // Resource
        Texture2DResource& GetResource() { return m_Resource; }
        const Texture2DResource& GetResource() const { return m_Resource; }

        // View
        template <TextureResourceView ResourceView> const ResourceView& GetView() const;
        template <TextureResourceView ResourceView> void SetView(Device& device);
        template <TextureResourceView ResourceView> bool IsViewExist() const;

        void Reset();

    private:
        template <TextureResourceView ResourceView>
        static uint64_t GetHashCode();

    private:
        Texture2DResource m_Resource;
        ViewContainer m_Views;
    };

    template <TextureResourceView ResourceView>
    const ResourceView& Texture2D::GetView() const
    {
        return std::get<ResourceView>(m_Views.at(GetHashCode<ResourceView>()));
    }

    template <TextureResourceView ResourceView>
    void Texture2D::SetView(Device& device)
    {
        SPIELER_ASSERT(m_Resource.GetResource());

        m_Views[GetHashCode<ResourceView>()] = ResourceView{ device, m_Resource };
    }

    template <TextureResourceView ResourceView>
    bool Texture2D::IsViewExist() const
    {
        return m_Views.contains(GetHashCode<ResourceView>());
    }

    template <TextureResourceView ResourceView>
    static uint64_t Texture2D::GetHashCode()
    {
        return static_cast<uint64_t>(typeid(ResourceView).hash_code());
    }

} // namespace spieler::renderer