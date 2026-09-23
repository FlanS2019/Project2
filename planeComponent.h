#pragma once
#include "component.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include "renderer.h"
using namespace DirectX;

class PlaneComponent : public Component
{
private:
    ID3D11Buffer* m_VertexBuffer = nullptr;
    ID3D11VertexShader* m_VertexShader = nullptr;
    ID3D11PixelShader* m_PixelShader = nullptr;
    ID3D11InputLayout* m_VertexLayout = nullptr;
    ID3D11ShaderResourceView* m_Texture = nullptr;

    float m_Width = 100.0f;
    float m_Height = 100.0f;
	float m_TexCoordScale = 1.0f; // テクスチャ座標のスケール

    MATERIAL m_Material{};

public:
    PlaneComponent(GameObject* obj) : Component(obj)
    {
        m_Material.Diffuse = { 1,1,1,1 };
        m_Material.Ambient = { 1,1,1,1 };
        m_Material.TextureEnable = true;
    }

    void SetMaterial(const MATERIAL& material)
    {
        m_Material = material;
    }

    MATERIAL& GetMaterial()
    {
        return m_Material;
    }

    void SetDiffuse(const XMFLOAT4& color)
    {
        m_Material.Diffuse = color;
    }

    void SetTextureEnable(bool enable)
    {
        m_Material.TextureEnable = enable;
    }


    void SetSize(float w, float h) {
        m_Width = w;
        m_Height = h;
        UpdateVertexBuffer();
    }

    void SetTexCoordScale(float scale) {
        m_TexCoordScale = scale;
        UpdateVertexBuffer();
    }

    void LoadTexture(const wchar_t* path);
    void UpdateVertexBuffer();
    void Init()   override;
    void Uninit() override;
    void Update(double) override {}
    void Draw()   override;

};