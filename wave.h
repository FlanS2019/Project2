#pragma once

#include "gameObject.h"
#include <d3d11.h>

class Wave : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11Buffer* m_IndexBuffer = nullptr;

	ID3D11InputLayout* m_VertexLayout = nullptr;
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;

	ID3D11ShaderResourceView* m_Texture = nullptr;
	ID3D11RasterizerState* m_RasterizerState = nullptr; // í«â¡ÅFóºñ ï`âÊóp

	float m_Time = 0.0f;

public:
	void Init() override;
	void Uninit() override;
	void Update(double) override;
	void Draw() override;
};