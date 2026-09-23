#pragma once

#include "gameObject.h"
#include <d3d11.h>

class Enemy : public GameObject
{
private:
    Vector3 m_Velocity{ 0.0f, 0.0f, 0.0f }; //‘¬“x


    ID3D11InputLayout* m_VertexLayout;
    ID3D11VertexShader* m_VertexShader;
    ID3D11PixelShader* m_PixelShader;



public:
    void Init() override;
    void Uninit() override;
    void Update(double) override;
    void Draw() override;
};