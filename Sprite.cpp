#include "sprite.h"
#include "renderer.h"
#include "DirectXTex.h"
#include "gameObject.h"

void SpriteComponent::Init()
{
    Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "unlitTextureVS.cso");
    Renderer::CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");

    if (m_Cells.empty())
    {
        m_Cells.assign(m_CellCount, CellUV{});
    }
    BuildVertexBuffer();
}

void SpriteComponent::LoadTexture(const wchar_t* path)
{
    TexMetadata metadata{};
    ScratchImage image;
    LoadFromWICFile(path, WIC_FLAGS_NONE, &metadata, image);
    CreateShaderResourceView(Renderer::GetDevice(),
        image.GetImages(), image.GetImageCount(), metadata, &m_Texture);
    assert(m_Texture);
}

// コマ数ぶんの矩形(4頂点×コマ数)を作り直す。
// 頂点バッファが既にあれば一度解放してから作り直す。
void SpriteComponent::BuildVertexBuffer()
{
    if (m_VertexBuffer)
    {
        m_VertexBuffer->Release();
        m_VertexBuffer = nullptr;
    }

    const int vertexCount = m_CellCount * 4;
    std::vector<VERTEX_3D> vertex(vertexCount);

    for (int i = 0; i < m_CellCount; i++)
    {
        const float left = i * m_CellWidth;
        const float right = left + m_CellWidth;
        const float top = 0.0f;
        const float bottom = m_CellHeight;

        const CellUV uv = (i < (int)m_Cells.size()) ? m_Cells[i] : CellUV{};

        VERTEX_3D* v = &vertex[i * 4];
        v[0].Position = XMFLOAT3(left, top, 0.0f);
        v[1].Position = XMFLOAT3(right, top, 0.0f);
        v[2].Position = XMFLOAT3(left, bottom, 0.0f);
        v[3].Position = XMFLOAT3(right, bottom, 0.0f);

        for (int k = 0; k < 4; k++)
        {
            v[k].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
            v[k].Diffuse = XMFLOAT4(1, 1, 1, 1);
        }

        v[0].TexCoord = XMFLOAT2(uv.u, uv.v);
        v[1].TexCoord = XMFLOAT2(uv.u + uv.uSize, uv.v);
        v[2].TexCoord = XMFLOAT2(uv.u, uv.v + uv.vSize);
        v[3].TexCoord = XMFLOAT2(uv.u + uv.uSize, uv.v + uv.vSize);
    }

    D3D11_BUFFER_DESC bd{};
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bd.ByteWidth = sizeof(VERTEX_3D) * vertexCount;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA sd{};
    sd.pSysMem = vertex.data();
    Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
}

void SpriteComponent::SetCellUV(int index, float u, float v, float usize, float vsize)
{
    if (index < 0 || index >= (int)m_Cells.size()) return;

    m_Cells[index] = { u, v, usize, vsize };

    if (!m_VertexBuffer) return;

    // 該当コマの4頂点だけ書き換えるため、一旦全頂点を読み直して該当部分を更新する。
    // D3D11_MAP_WRITE_DISCARDは中身を保証しないので、CPU側に保持しているm_Cellsから毎回全頂点を組み立て直す。
    const int vertexCount = m_CellCount * 4;
    std::vector<VERTEX_3D> vertex(vertexCount);

    for (int i = 0; i < m_CellCount; i++)
    {
        const float left = i * m_CellWidth;
        const float right = left + m_CellWidth;
        const float top = 0.0f;
        const float bottom = m_CellHeight;

        const CellUV cell = m_Cells[i];

        VERTEX_3D* vv = &vertex[i * 4];
        vv[0].Position = XMFLOAT3(left, top, 0.0f);
        vv[1].Position = XMFLOAT3(right, top, 0.0f);
        vv[2].Position = XMFLOAT3(left, bottom, 0.0f);
        vv[3].Position = XMFLOAT3(right, bottom, 0.0f);

        for (int k = 0; k < 4; k++)
        {
            vv[k].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
            vv[k].Diffuse = XMFLOAT4(1, 1, 1, 1);
        }

        vv[0].TexCoord = XMFLOAT2(cell.u, cell.v);
        vv[1].TexCoord = XMFLOAT2(cell.u + cell.uSize, cell.v);
        vv[2].TexCoord = XMFLOAT2(cell.u, cell.v + cell.vSize);
        vv[3].TexCoord = XMFLOAT2(cell.u + cell.uSize, cell.v + cell.vSize);
    }

    D3D11_MAPPED_SUBRESOURCE msr;
    Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
    memcpy(msr.pData, vertex.data(), sizeof(VERTEX_3D) * vertexCount);
    Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);
}

void SpriteComponent::Uninit()
{
    if (m_Texture) { m_Texture->Release();      m_Texture = nullptr; }
    if (m_VertexBuffer) { m_VertexBuffer->Release(); m_VertexBuffer = nullptr; }
    if (m_VertexLayout) { m_VertexLayout->Release(); m_VertexLayout = nullptr; }
    if (m_VertexShader) { m_VertexShader->Release(); m_VertexShader = nullptr; }
    if (m_PixelShader) { m_PixelShader->Release();  m_PixelShader = nullptr; }
}

void SpriteComponent::Draw()
{
    if (!m_VertexBuffer) return;

    Renderer::SetDepthEnable(false);

    Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);
    Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
    Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

    Renderer::SetWorldViewProjection2D();

    XMMATRIX world = XMMatrixTranslation(
        m_GameObject->GetPosition().x + m_Offset.x,
        m_GameObject->GetPosition().y + m_Offset.y,
        0.0f
    );
    Renderer::SetWorldMatrix(world);

    MATERIAL material{};
    material.Diffuse = { 1,1,1,1 };
    material.TextureEnable = true;
    Renderer::SetMaterial(material);

    Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

    UINT stride = sizeof(VERTEX_3D), offset = 0;
    Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
    Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    // コマごとに4頂点ずつ、TRIANGLESTRIPとして個別に描画する
    // (1回のDrawでつなげるとコマの境界に余計な三角形ができてしまうため)
    for (int i = 0; i < m_CellCount; i++)
    {
        Renderer::GetDeviceContext()->Draw(4, i * 4);
    }

    Renderer::SetDepthEnable(true);
}