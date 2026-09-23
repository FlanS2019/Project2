#include "planeComponent.h"
#include "renderer.h"
#include "DirectXTex.h"
#include "gameObject.h"
#include "transform.h"


void PlaneComponent::Init()
{
    VERTEX_3D vertex[21][21];
    float hw = m_Width / 2.0f;
    float hh = m_Height / 2.0f;

    for(int x = 0; x < 21; x++)
    {
        for(int y = 0; y < 21; y++)
        {
            vertex[x][y].Position = XMFLOAT3(-hw + (m_Width / 20.0f) * x, -hh + (m_Height / 20.0f) * y, 0.0f);
            vertex[x][y].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
            vertex[x][y].Diffuse = XMFLOAT4(1, 1, 1, 1);
            vertex[x][y].TexCoord = XMFLOAT2(static_cast<float>(x) / 20.0f, static_cast<float>(y) / 20.0f);
        }
    }
   /* vertex[1].Position = XMFLOAT3(hw, -hh, 0.0f);
    vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
    vertex[1].Diffuse = XMFLOAT4(1, 1, 1, 1);
    vertex[1].TexCoord = XMFLOAT2(1.0f, 1.0f);

    vertex[2].Position = XMFLOAT3(-hw, hh, 0.0f);
    vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
    vertex[2].Diffuse = XMFLOAT4(1, 1, 1, 1);
    vertex[2].TexCoord = XMFLOAT2(0.0f, 0.0f);

    vertex[3].Position = XMFLOAT3(hw, hh, 0.0f);
    vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
    vertex[3].Diffuse = XMFLOAT4(1, 1, 1, 1);
    vertex[3].TexCoord = XMFLOAT2(1.0f, 0.0f);*/

    D3D11_BUFFER_DESC bd{};
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bd.ByteWidth = sizeof(VERTEX_3D) * 21 * 21;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA sd{};
    sd.pSysMem = vertex;
    Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

    {
        unsigned int index[(22 * 2) * 20 - 2];
        int i = 0;
        for (int y = 0; y < 20; y++)
        {
            for (int x = 0; x < 21; x++)
            {
                index[i++] = x + y * 21;
                index[i++] = x + (y + 1) * 21;
            }
            if (y != 19)
            {
                index[i++] = (y + 1) * 21 + 21 - 1;
                index[i++] = (y + 1) * 21;
            }
		}

    }



    Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "unlitTextureVS.cso");
    Renderer::CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");
}

void PlaneComponent::LoadTexture(const wchar_t* path)
{
    TexMetadata metadata{};
    ScratchImage image;
    LoadFromWICFile(path, WIC_FLAGS_NONE, &metadata, image);
    CreateShaderResourceView(Renderer::GetDevice(),
        image.GetImages(), image.GetImageCount(), metadata, &m_Texture);
    assert(m_Texture);
}

void PlaneComponent::UpdateVertexBuffer()
{

    VERTEX_3D vertex[4];
    float hw = m_Width / 2.0f;
    float hh = m_Height / 2.0f;
    vertex[0].Position = XMFLOAT3(-hw, -hh, 0.0f);
    vertex[1].Position = XMFLOAT3(hw, -hh, 0.0f);
    vertex[2].Position = XMFLOAT3(-hw, hh, 0.0f);
    vertex[3].Position = XMFLOAT3(hw, hh, 0.0f);

    vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
    vertex[0].Diffuse = XMFLOAT4(1, 1, 1, 1);
    vertex[0].TexCoord = XMFLOAT2(0.0f, 1.0f);

    vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
    vertex[1].Diffuse = XMFLOAT4(1, 1, 1, 1);
    vertex[1].TexCoord = XMFLOAT2(1.0f, 1.0f);

    vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
    vertex[2].Diffuse = XMFLOAT4(1, 1, 1, 1);
    vertex[2].TexCoord = XMFLOAT2(0.0f, 0.0f);

    vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
    vertex[3].Diffuse = XMFLOAT4(1, 1, 1, 1);
    vertex[3].TexCoord = XMFLOAT2(1.0f, 0.0f);
    D3D11_MAPPED_SUBRESOURCE msr;
    Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
    memcpy(msr.pData, vertex, sizeof(vertex));
    Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

}

void PlaneComponent::Uninit()
{
    if (m_Texture) { m_Texture->Release();      m_Texture = nullptr; }
    if (m_VertexBuffer) { m_VertexBuffer->Release(); m_VertexBuffer = nullptr; }
    if (m_VertexLayout) { m_VertexLayout->Release(); m_VertexLayout = nullptr; }
    if (m_VertexShader) { m_VertexShader->Release(); m_VertexShader = nullptr; }
    if (m_PixelShader) { m_PixelShader->Release();  m_PixelShader = nullptr; }
}

void PlaneComponent::Draw()
{
    Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);
    Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
    Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

    // TransformComponentからワールド行列を取得
    TransformComponent* transform = m_GameObject->GetComponent<TransformComponent>();
    if (transform)
    {
        Renderer::SetWorldMatrix(transform->GetWorldMatrix());
    }

    Renderer::SetMaterial(m_Material);

    Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

    UINT stride = sizeof(VERTEX_3D), offset = 0;
    Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
    Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    Renderer::GetDeviceContext()->Draw(21 * 21, 0);
}


