#pragma once

#include "gameObject.h"
#include <d3d11.h>
#include <string>

//class Audio;
class Player : public GameObject
{
private:
    Vector3 m_Velocity{ 0.0f, 0.0f, 0.0f }; //‘¬“x
    float m_Speed = 40.0f; //ˆÚ“®‘¬“x

    ID3D11InputLayout* m_VertexLayout = nullptr;
    ID3D11VertexShader* m_VertexShader = nullptr;
    ID3D11PixelShader* m_PixelShader = nullptr;
    bool m_Ground = true;
    float m_MoveAnimation = 0.0f;
    class Audio* m_JumpSE;

    GameObject* m_Child{};
	class Shadow* m_Shadow{};
    class AnimationModel* m_AnimationModel{};
    int m_AnimationFrame = 0;
    std::string m_AnimationName;
	int m_NextAnimationFrame = 0;
	std::string m_NextAnimationName;
    float m_Blend = 0.0f;


public:
    void Init() override;
    void Uninit() override;
    void Update(double) override;
    void Draw() override;
    void SetAnimation(const char* AnimationName);
};