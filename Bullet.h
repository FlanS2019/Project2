#pragma once

#include "gameObject.h"
#include <d3d11.h>

class Bullet : public GameObject
{
private:
    Vector3 m_Velocity{ 0.0f, 0.0f, 0.0f }; //ë¨ìx


    ID3D11InputLayout* m_VertexLayout  = nullptr;
    ID3D11VertexShader* m_VertexShader = nullptr;
    ID3D11PixelShader* m_PixelShader   = nullptr;


	float m_LifeTime = 1.f; // íeÇÃéıñΩÅiïbÅj


public:
    void Init() override;
    void Uninit() override;
    void Update(double) override;
    void Draw() override;
	void SetVelocity(const Vector3& velocity) { m_Velocity = velocity; }

};