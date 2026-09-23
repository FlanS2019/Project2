#pragma once
#include "component.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
using namespace DirectX;

// 1コマぶんのUV情報
struct CellUV
{
    float u = 0.0f;
    float v = 0.0f;
    float uSize = 1.0f;
    float vSize = 1.0f;
};

class SpriteComponent : public Component
{
private:
    ID3D11Buffer* m_VertexBuffer = nullptr;
    ID3D11VertexShader* m_VertexShader = nullptr;
    ID3D11PixelShader* m_PixelShader = nullptr;
    ID3D11InputLayout* m_VertexLayout = nullptr;
    ID3D11ShaderResourceView* m_Texture = nullptr;

    float m_CellWidth = 100.0f;   // 1コマの幅
    float m_CellHeight = 100.0f;  // 1コマの高さ
    int   m_CellCount = 1;        // 横に並べるコマ数（桁数）

    std::vector<CellUV> m_Cells;  // 各コマのUV（m_CellCount個）

    XMFLOAT2 m_Offset = { 0,0 };

    void BuildVertexBuffer(); // 初回バッファ作成（CellCount変更時）

public:
    SpriteComponent(GameObject* obj) : Component(obj) {}

    // 1コマのサイズと、横に並べるコマ数（桁数）をまとめて設定
    void SetCell(float cellWidth, float cellHeight, int cellCount)
    {
        m_CellWidth = cellWidth;
        m_CellHeight = cellHeight;
        m_CellCount = cellCount;
        m_Cells.assign(cellCount, CellUV{});
        BuildVertexBuffer();
    }

    // 旧インターフェース互換（1コマ表示用）
    void SetSize(float w, float h)
    {
        SetCell(w, h, 1);
    }

    void LoadTexture(const wchar_t* path);

    // index番目のコマのUVを設定し、頂点バッファを更新
    void SetCellUV(int index, float u, float v, float usize, float vsize);

    // 旧インターフェース互換（1コマ目だけ更新）
    void SetUV(float u, float v, float usize, float vsize)
    {
        SetCellUV(0, u, v, usize, vsize);
    }

    void Init()   override;
    void Uninit() override;
    void Update(double) override {}
    void Draw()   override;

    void SetOffset(float x, float y)
    {
        m_Offset = { x,y };
    }
};