#pragma once
#include "component.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include "Vector3.h"
using namespace DirectX;

class BilbordPlaneComponent : public Component
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


    float m_U = 0.0f;
    float m_V = 0.0f;
    float m_USize = 1.0f;
    float m_VSize = 1.0f;
public:
    BilbordPlaneComponent(GameObject* obj) : Component(obj) {}

    void SetSize(float w, float h) {
        m_Width = w;
        m_Height = h;
        UpdateVertexBuffer();
    }

    // 座標設定追加
    void SetPosition(Vector3 pos);


    // 座標取得
    Vector3 GetPosition();


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

    void SetUV(float u, float v, float usize, float vsize)
    {
        m_U = u;
        m_V = v;
        m_USize = usize;
        m_VSize = vsize;

        UpdateVertexBuffer();
    }
};