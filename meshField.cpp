#include "main.h"
#include "renderer.h"
#include "meshField.h"
#include "DirectXTex.h"
#include "transform.h"
#include "audio.h"
#include "terrainHeight.h"
#include <vector>

constexpr int   kDiv = 40;    // 分割数（多いほど凹凸が滑らか）
constexpr float kWidth = 120.0f; // 地形の幅（X）
constexpr float kDepth = 120.0f; // 地形の奥行き（Z）

void MeshField::Init()
{
	m_Layer = 1;

	// ----- 頂点データ（GetTerrainHeightで高さを計算してそのまま焼き込む） -----
	std::vector<VERTEX_3D> vertex((kDiv + 1) * (kDiv + 1));

	for (int z = 0; z <= kDiv; z++)
	{
		for (int x = 0; x <= kDiv; x++)
		{
			int i = z * (kDiv + 1) + x;

			float px = (x / (float)kDiv - 0.5f) * kWidth;
			float pz = (z / (float)kDiv - 0.5f) * kDepth;
			float py = GetTerrainHeight(px, pz);

			vertex[i].Position = XMFLOAT3(px, py, pz);
			vertex[i].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			vertex[i].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			// テクスチャを何度も繰り返して貼る（引き伸ばしでぼやけないように）
			vertex[i].TexCoord = XMFLOAT2(x / (float)kDiv * 20.0f, z / (float)kDiv * 20.0f);
		}
	}

	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT; // 地形は動かないのでDEFAULTでOK
	bd.ByteWidth = sizeof(VERTEX_3D) * (UINT)vertex.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex.data();
	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	// ----- インデックスデータ -----
	std::vector<UINT> index(kDiv * kDiv * 6);
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
	m_IndexCount = (int)index.size();

	D3D11_BUFFER_DESC ibd{};
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = sizeof(UINT) * (UINT)index.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA isd{};
	isd.pSysMem = index.data();
	Renderer::GetDevice()->CreateBuffer(&ibd, &isd, &m_IndexBuffer);

	// ----- シェーダー -----
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader, "unlitTexturePS.cso");

	// ----- テクスチャ -----
	TexMetadata metadata{};
	ScratchImage image{};
	LoadFromWICFile(L"asset\\texture\\jimen.jpg", WIC_FLAGS_NONE, &metadata, image);
	CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), metadata, &m_Texture);
	assert(m_Texture);

	// ----- BGM（元のMeshFieldにあった処理をそのまま引き継ぎ） -----
	Audio* bgm = AddComponent<Audio>(this);
	bgm->Load("asset\\audio\\bgm.wav");
	//bgm->Play(true);
}

void MeshField::Uninit()
{
	if (m_VertexBuffer) { m_VertexBuffer->Release(); m_VertexBuffer = nullptr; }
	if (m_IndexBuffer) { m_IndexBuffer->Release();  m_IndexBuffer = nullptr; }
	if (m_VertexLayout) { m_VertexLayout->Release(); m_VertexLayout = nullptr; }
	if (m_VertexShader) { m_VertexShader->Release(); m_VertexShader = nullptr; }
	if (m_PixelShader) { m_PixelShader->Release();  m_PixelShader = nullptr; }
	if (m_Texture) { m_Texture->Release();      m_Texture = nullptr; }

	GameObject::Uninit();
}

void MeshField::Update(double deltaTime)
{
}

void MeshField::Draw()
{
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	TransformComponent* transform = GetComponent<TransformComponent>();
	Renderer::SetWorldMatrix(transform->GetWorldMatrix());

	MATERIAL material{};
	material.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	material.TextureEnable = TRUE;
	Renderer::SetMaterial(material);

	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
	Renderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Renderer::GetDeviceContext()->DrawIndexed(m_IndexCount, 0, 0);

	GameObject::Draw();
}