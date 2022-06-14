#pragma once

#include "spieler/renderer/input_layout.hpp"
#include "spieler/renderer/types.hpp"

namespace spieler::renderer
{

    class Device;

    class RootSignature;
    class Shader;
    struct BlendState;
    struct RasterizerState;
    struct DepthStencilState;

    struct PipelineStateConfig
    {
        const RootSignature* RootSignature{ nullptr };
        const Shader* VertexShader{ nullptr };
        const Shader* PixelShader{ nullptr };
        const Shader* GeometryShader{ nullptr };
        const BlendState* BlendState{ nullptr };
        const RasterizerState* RasterizerState{ nullptr };
        const DepthStencilState* DepthStecilState{ nullptr };
        const InputLayout* InputLayout{ nullptr };

        PrimitiveTopologyType PrimitiveTopologyType{ PrimitiveTopologyType::Undefined };
        GraphicsFormat RTVFormat{ GraphicsFormat::R8G8B8A8UnsignedNorm };
        GraphicsFormat DSVFormat{ GraphicsFormat::D24UnsignedNormS8UnsignedInt };
    };

    class PipelineState
    {
    public:
        bool Init(const PipelineStateConfig& props);

    public:
        explicit operator ID3D12PipelineState* () const { return m_PipelineState.Get(); }

    private:
        ComPtr<ID3D12PipelineState> m_PipelineState;
    };

    struct ComputePipelineStateConfig
    {
        const RootSignature* RootSignature{ nullptr };
        const Shader* ComputeShader{ nullptr };
    };

    class ComputePipelineState
    {
    public:
        ComputePipelineState() = default;
        ComputePipelineState(Device& device, const ComputePipelineStateConfig& config);

    private:
        bool Init(Device& device, const ComputePipelineStateConfig& config);

    public:
        explicit operator ID3D12PipelineState* () const { return m_PipelineState.Get(); }

    private:
        ComPtr<ID3D12PipelineState> m_PipelineState;
    };

} // namespace spieler::renderer