#include "box.h"
#include "renderer.h"
//#include "Field.h"
#include "DirectXTex.h"
#include "modelRenderer.h"
#include "input.h"


void Box::Init()
{
    m_Layer = 1;
    SetPosition({ 0.0f, 0.0f, 0.0f });
    SetScale({ 1.0f, 1.0f, 1.0f });

    ModelRenderer* modelRender = AddComponent<ModelRenderer>(this);

    modelRender->Load("asset\\Model\\box.obj");

    // �V�F�[�_�[�Ǎ�
    Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
        "unlitTextureVS.cso");

    Renderer::CreatePixelShader(&m_PixelShader,
        "unlitTexturePS.cso");


}

void Box::Uninit()
{



    if (m_VertexShader) { m_VertexShader->Release(); m_VertexShader = nullptr; }
    if (m_PixelShader) { m_PixelShader->Release(); m_PixelShader = nullptr; }
    if (m_VertexLayout) { m_VertexLayout->Release(); m_VertexLayout = nullptr; }

    GameObject::Uninit();

}
void Box::Update(double deltaTime)
{
    float dt = static_cast<float>(deltaTime);



    GameObject::Update(dt);
}
void Box::Draw()
{
    // �C���v�b�g���C�A�E�g�ݒ�
    Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

    // �V�F�[�_�[�ݒ�
    Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
    Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

    // �}�g���N�X�ݒ� (TransformComponent���烏�[���h�s����擾)
    TransformComponent* transform = GetComponent<TransformComponent>();
    DirectX::XMMATRIX world = transform->GetWorldMatrix();
    Renderer::SetWorldMatrix(world);

    GameObject::Draw();//�p������Draw�i�j���Ăяo�����ƂŁA
    //Player�N���X��Draw()����ModelRenderer�N���X��Draw()���Ăяo�����悤�ɂȂ�B

}
