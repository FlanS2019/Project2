#include "wave.h"
#include "main.h"
#include "renderer.h"
#include "transform.h"
#include "DirectXTex.h"

constexpr int   kDiv = 20;                 // 分割数（多いほど波が滑らか）
constexpr int   kVertexNum = (kDiv + 1) * (kDiv + 1);
constexpr int   kIndexNum = kDiv * kDiv * 6;

constexpr float kWidth = 8.0f;                // 波エリアの幅（X）
constexpr float kDepth = 8.0f;                // 波エリアの奥行き（Z）

constexpr float kAmplitude = 0.15f;               // 波の高さ
constexpr float kWaveLengthX = 1.5f;                // X方向の波の細かさ
constexpr float kWaveLengthZ = 2.0f;                // Z方向の波の細かさ
constexpr float kSpeed = 1.5f;                // 波が流れる速さ

void Wave::Init()
{
	m_Layer = 1;

	UINT index[kIndexNum];
	int idx = 0;
	for (int z = 0; z < kDiv; z++)
	{
		for (int x = 0; x < kDiv; x++)
		{
			int i0 = z * (kDiv + 1) + x;
			int i1 = i0 + 1;
			int i2 = i0 + (kDiv + 1);
			int i3 = i2 + 1;

			index[idx++] = i0;
			index[idx++] = i1;
			index[idx++] = i2;

			index[idx++] = i1;
			index[idx++] = i3;
			index[idx++] = i2;
		}
	}

	D3D11_BUFFER_DESC ibd{};
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = sizeof(UINT) * kIndexNum;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA isd{};
	isd.pSysMem = index;
	Renderer::GetDevice()->CreateBuffer(&ibd, &isd, &m_IndexBuffer);

	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * kVertexNum;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	Renderer::GetDevice()->CreateBuffer(&bd, NULL, &m_VertexBuffer);

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");

	TexMetadata metadata{};
	ScratchImage image{};
	LoadFromWICFile(L"asset\\texture\\water.jpg", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_Texture);
	assert(m_Texture);

	// カリング無効（両面表示）のラスタライザステート ※前回抜けていた部分
	D3D11_RASTERIZER_DESC rasterizerDesc{};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.DepthClipEnable = TRUE;
	Renderer::GetDevice()->CreateRasterizerState(&rasterizerDesc, &m_RasterizerState);

	Update(0.0);
}

void Wave::Uninit()
{
	if (m_VertexBuffer) { m_VertexBuffer->Release(); m_VertexBuffer = nullptr; }
	if (m_IndexBuffer) { m_IndexBuffer->Release();  m_IndexBuffer = nullptr; }
	if (m_VertexLayout) { m_VertexLayout->Release(); m_VertexLayout = nullptr; }
	if (m_VertexShader) { m_VertexShader->Release(); m_VertexShader = nullptr; }
	if (m_PixelShader) { m_PixelShader->Release();  m_PixelShader = nullptr; }
	if (m_Texture) { m_Texture->Release();      m_Texture = nullptr; }
	if (m_RasterizerState) { m_RasterizerState->Release(); m_RasterizerState = nullptr; }

	GameObject::Uninit();
}

void Wave::Update(double deltaTime)
{
	m_Time += static_cast<float>(deltaTime);

	D3D11_MAPPED_SUBRESOURCE msr{};
	Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	for (int z = 0; z <= kDiv; z++)
	{
		for (int x = 0; x <= kDiv; x++)
		{
			int i = z * (kDiv + 1) + x;

			float px = (x / (float)kDiv - 0.5f) * kWidth;
			float pz = (z / (float)kDiv - 0.5f) * kDepth;

			float py = sinf(px * kWaveLengthX + m_Time * kSpeed) * kAmplitude
				+ sinf(pz * kWaveLengthZ + m_Time * kSpeed * 0.8f) * kAmplitude * 0.5f;

			vertex[i].Position = XMFLOAT3(px, py, pz);
			vertex[i].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			vertex[i].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			vertex[i].TexCoord = XMFLOAT2(x / (float)kDiv, z / (float)kDiv);
		}
	}

	Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

	GameObject::Update(deltaTime);
}

void Wave::Draw()
{
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	TransformComponent* transform = GetComponent<TransformComponent>();
	DirectX::XMMATRIX world = transform->GetWorldMatrix();
	Renderer::SetWorldMatrix(world);

	MATERIAL material{};
	material.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	material.TextureEnable = TRUE;
	Renderer::SetMaterial(material);

	Renderer::GetDeviceContext()->RSSetState(m_RasterizerState);

	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
	Renderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Renderer::GetDeviceContext()->DrawIndexed(kIndexNum, 0, 0);

	Renderer::GetDeviceContext()->RSSetState(nullptr);

	GameObject::Draw();
}