#pragma once
#include <d3d11.h>
#include "vector3.h"
#include "gameObject.h"

class MeshField :public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11Buffer* m_IndexBuffer = nullptr;
	int m_IndexCount = 0;

	ID3D11InputLayout* m_VertexLayout = nullptr;
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;

	ID3D11ShaderResourceView* m_Texture = nullptr;

public:
	void Init();
	void Uninit();
	void Update(double);
	void Draw();
};