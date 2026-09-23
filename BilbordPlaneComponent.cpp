#include "bilbordPlaneComponent.h"

#include "renderer.h"
#include "DirectXTex.h"
#include "gameObject.h"
#include "transform.h"
#include "camera.h"
#include "manager.h"


void BilbordPlaneComponent::Init()
{
    VERTEX_3D vertex[4];
    vertex[0].Position = XMFLOAT3(-m_Width / 2.0f, m_Height, 0.0f);
    vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
    vertex[0].Diffuse = XMFLOAT4(1, 1, 1, 1);
    vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

    vertex[1].Position = XMFLOAT3( m_Width / 2.0f, m_Height, 0.0f);
    vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
    vertex[1].Diffuse = XMFLOAT4(1, 1, 1, 1);
    vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

    vertex[2].Position = XMFLOAT3(-m_Width / 2.0f, 0.0f, 0.0f);
    vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
    vertex[2].Diffuse = XMFLOAT4(1, 1, 1, 1);
    vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

    vertex[3].Position = XMFLOAT3( m_Width / 2.0f, 0.0f, 0.0f);
    vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
    vertex[3].Diffuse = XMFLOAT4(1, 1, 1, 1);
    vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

    D3D11_BUFFER_DESC bd{};
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bd.ByteWidth = sizeof(VERTEX_3D) * 4;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
   


    D3D11_SUBRESOURCE_DATA sd{};
    sd.pSysMem = vertex;
    Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

    Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "unlitTextureVS.cso");
    Renderer::CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");
}

void BilbordPlaneComponent::SetPosition(Vector3 pos)
{
    TransformComponent* transform =
        m_GameObject->GetComponent<TransformComponent>();

    if (transform)
    {
        transform->SetPosition(pos);
    }
}

Vector3 BilbordPlaneComponent::GetPosition()
{
    TransformComponent* transform =
        m_GameObject->GetComponent<TransformComponent>();

    if (transform)
    {
        return transform->GetPosition();
    }

    return Vector3(0, 0, 0);
}


void BilbordPlaneComponent::LoadTexture(const wchar_t* path)
{
    TexMetadata metadata{};
    ScratchImage image;
    LoadFromWICFile(path, WIC_FLAGS_NONE, &metadata, image);
    CreateShaderResourceView(Renderer::GetDevice(),
        image.GetImages(), image.GetImageCount(), metadata, &m_Texture);
    assert(m_Texture);
}

void BilbordPlaneComponent::UpdateVertexBuffer()
{

    VERTEX_3D vertex[4];
    // 左上、右上、左下、右下の順（カリング対策で法線が-Zを向くように設定し、Xは中央揃え）
    vertex[0].Position = XMFLOAT3(-m_Width / 2.0f, m_Height, 0.0f);
    vertex[1].Position = XMFLOAT3( m_Width / 2.0f, m_Height, 0.0f);
    vertex[2].Position = XMFLOAT3(-m_Width / 2.0f, 0.0f, 0.0f);
    vertex[3].Position = XMFLOAT3( m_Width / 2.0f, 0.0f, 0.0f);

    vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
    vertex[0].Diffuse = XMFLOAT4(1, 1, 1, 1);
    vertex[0].TexCoord = XMFLOAT2(m_U, m_V);

    vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
    vertex[1].Diffuse = XMFLOAT4(1, 1, 1, 1);
    vertex[1].TexCoord = XMFLOAT2(
        m_U + m_USize,
        m_V);

    vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
    vertex[2].Diffuse = XMFLOAT4(1, 1, 1, 1);
    vertex[2].TexCoord = XMFLOAT2(m_U, m_V + m_VSize);

    vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
    vertex[3].Diffuse = XMFLOAT4(1, 1, 1, 1);
    vertex[3].TexCoord = XMFLOAT2(m_U + m_USize, m_V + m_VSize);

    D3D11_MAPPED_SUBRESOURCE msr;
    Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
    memcpy(msr.pData, vertex, sizeof(vertex));
    Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

}

void BilbordPlaneComponent::Uninit()
{
    if (m_Texture) { m_Texture->Release();      m_Texture = nullptr; }
    if (m_VertexBuffer) { m_VertexBuffer->Release(); m_VertexBuffer = nullptr; }
    if (m_VertexLayout) { m_VertexLayout->Release(); m_VertexLayout = nullptr; }
    if (m_VertexShader) { m_VertexShader->Release(); m_VertexShader = nullptr; }
    if (m_PixelShader) { m_PixelShader->Release();  m_PixelShader = nullptr; }
}

void BilbordPlaneComponent::Draw()
{
    Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);
    Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
    Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);


    // カメラの情報を取得
    Camera* camera = Manager::GetGameObject<Camera>();
    Vector3 camPos = camera->GetComponent<TransformComponent>()->GetPosition();

    // TransformComponentからスケールと位置を取得してビルボード用のワールド行列を作成
    TransformComponent* transform = m_GameObject->GetComponent<TransformComponent>();
    if (transform)
    {
        Vector3 pos = transform->GetPosition();
        Vector3 scale = transform->GetScale();
        
        // カメラの方向を向くようにY軸の回転角度を計算（円柱ビルボード）
        // 法線が-Zを向いているため、カメラ方向(+Z側)を向かせるにはさらに PI 回す
        float angle = atan2f(camPos.x - pos.x, camPos.z - pos.z) + XM_PI;
        
        XMMATRIX scaleM = XMMatrixScaling(scale.x, scale.y, scale.z);
        XMMATRIX rotM = XMMatrixRotationY(angle);
        XMMATRIX transM = XMMatrixTranslation(pos.x, pos.y, pos.z);
        
        // スケール * Y軸回転 * 平行移動 の順で乗算
        XMMATRIX billboardWorld = scaleM * rotM * transM;
        Renderer::SetWorldMatrix(billboardWorld);
    }

    MATERIAL material{};
    material.Diffuse = { 0.2,0.2,1,1 };
    material.TextureEnable = true;
    Renderer::SetMaterial(material);

    Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

    UINT stride = sizeof(VERTEX_3D), offset = 0;
    Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
    Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    Renderer::GetDeviceContext()->Draw(4, 0);
}