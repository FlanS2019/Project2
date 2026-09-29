#pragma once

#include "gameObject.h"
#include <d3d11.h>

class Car : public GameObject
{
private:
    ID3D11InputLayout* m_VertexLayout;
    ID3D11VertexShader* m_VertexShader;
    ID3D11PixelShader* m_PixelShader;

    class AnimationModel* m_AnimationModel{};

public:
    void Init() override;
    void Uninit() override;
    void Update(double) override;
    void Draw() override;
};
