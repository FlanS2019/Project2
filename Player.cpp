#include "main.h"
#include "manager.h"
#include "player.h"
#include "renderer.h"
//#include "Field.h"
#include "DirectXTex.h"

#include "input.h"
#include "transform.h"
#include "camera.h"
#include "bullet.h"
#include <imgui.h>
#include "tree.h"
#include "box.h"
#include "audio.h"
#include "shadow.h"
#include "animationModel.h"
#include "modelRenderer.h"
#include "terrainHeight.h"

// Akaiモデルの原点（ピボット）が足元とズレているので、見た目だけ持ち上げる補正値
// ※当たり判定・影の位置には影響しません。埋まって見える/浮いて見える場合はここを調整
constexpr float kModelYOffset = 0.8f;

void Player::Init()
{
    m_Layer = 1;
    TransformComponent* transform = GetComponent<TransformComponent>();
    transform->SetPosition({ 0.0f, 0.0f, 0.0f });
    transform->SetScale({ 0.01f, 0.01f, 0.01f });
    transform->SetRotation({ 0.0f,90.0f, 0.0f });
    //ModelRenderer* modelRender = AddComponent<ModelRenderer>(this);
 //
 //   modelRender->Load("asset\\Model\\player.obj");

    m_AnimationModel = AddComponent<AnimationModel>(this);
    m_AnimationModel->Load("asset\\Model\\Akai.fbx");
    m_AnimationModel->LoadAnimation("asset\\Model\\Akai_Run.fbx", "Run");
    m_AnimationModel->LoadAnimation("asset\\Model\\Akai_Idle.fbx", "Idle");

    m_AnimationName = "Idle";
    m_NextAnimationName = "Idle";
    m_AnimationFrame = 0;
    m_NextAnimationFrame = 0;
    m_Blend = 0.0f;

    // シェーダー読込
    Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
        "unlitTextureVS.cso");

    Renderer::CreatePixelShader(&m_PixelShader,
        "unlitTexturePS.cso");
    m_JumpSE = AddComponent<Audio>(this);
    m_JumpSE->Load("asset\\audio\\wan.wav");


    m_Child = Manager::AddGameObject<GameObject>();
    m_Child->SetParent(this);
    m_Child->SetPosition({ 10.0f, 50.1f, 0.0f });
    ModelRenderer* childModelRender = m_Child->AddComponent<ModelRenderer>(m_Child);
    childModelRender->Load("asset\\Model\\box.obj");

    m_Shadow = Manager::AddGameObject<Shadow>();
    //m_Shadow->SetParent(this);
    m_Shadow->SetPosition({ 0.0f, 0.0f, 0.0f });
    m_Shadow->SetScale({ 0.05f,0.05f,0.05f });
}

void Player::Uninit()
{



    if (m_VertexShader) { m_VertexShader->Release(); m_VertexShader = nullptr; }
    if (m_PixelShader) { m_PixelShader->Release(); m_PixelShader = nullptr; }
    if (m_VertexLayout) { m_VertexLayout->Release(); m_VertexLayout = nullptr; }

    GameObject::Uninit();

}
void Player::Update(double deltaTime)
{
    // フリーカメラモード中はプレイヤーの操作・移動を完全に止める
    if (Camera::IsFreeMode())
    {
        GameObject::Update(deltaTime);
        return;
    }

    float dt = static_cast<float>(deltaTime);



    // TransformComponentを取得して使う
    TransformComponent* transform = GetComponent<TransformComponent>();
    Camera* camera = Manager::GetGameObject<Camera>();


    Vector3 olsPos = transform->GetPosition();

    Vector3 pos = transform->GetPosition();
    Vector3 forward = camera->GetForward();
    Vector3 right = camera->GetRight();

    forward.y = 0.0f; // 水平方向の移動にするためy成分を0に
    forward.normalize(); // 正規化して単位ベクトルにする
    right.y = 0.0f; // 水平方向の移動にするためy成分を0に
    right.normalize(); // 正規化して単位ベクトルにする


    Vector3 moveDir{ 0.0f, 0.0f, 0.0f };
    if (Input::GetKeyPress('D'))
    {
        m_Velocity += right * m_Speed * dt;
        moveDir += right;
    }
    if (Input::GetKeyPress('A'))
    {
        m_Velocity -= right * m_Speed * dt;
        moveDir -= right;
    }
    if (Input::GetKeyPress('W'))
    {
        m_Velocity += forward * m_Speed * dt;
        moveDir += forward;
    }
    if (Input::GetKeyPress('S'))
    {
        m_Velocity -= forward * m_Speed * dt;
        moveDir -= forward;
    }

    // 移動方向の水平成分を計算
    moveDir.y = 0.0f;
    float moveLenSq = moveDir.x * moveDir.x + moveDir.z * moveDir.z;

    if (moveLenSq > 0.001f)
    {
        // 移動中は移動方向に向く
        transform->SetRotation({ 0.0f, atan2f(moveDir.x, moveDir.z), 0.0f });
    }
    else
    {
        // 移動していないときはQ/Eキーで手動回転
        Vector3 rotation = transform->GetRotation();
        if (Input::GetKeyPress('Q'))
        {
            rotation.y -= 2.0f * dt;
        }
        if (Input::GetKeyPress('E'))
        {
            rotation.y += 2.0f * dt;
        }
        transform->SetRotation(rotation);
    }

    if (Input::GetKeyTrigger(VK_SPACE))
    {
        m_Velocity.y += 20.0f;

        //SetScale({0.5f,2.0f,0.5f});
        m_JumpSE->Play();
    }

    //SetScale(GetScale().x+(1.0f-GetScale().x)*0.1f,);

    float x = GetScale().x;
    float y = GetScale().y;
    float z = GetScale().z;

    x += (1.0f - x) * 0.1f;
    y += (1.0f - y) * 0.1f;
    z += (1.0f - z) * 0.1f;
    // SetScale({x,y,z});




    m_Velocity.y += -50.f * dt;
    m_Velocity.x += -m_Velocity.x * 5.0f * dt;
    m_Velocity.z += -m_Velocity.z * 5.0f * dt;

    pos += m_Velocity * dt;

    bool oldGround = m_Ground;
    m_Ground = false;

    // 地形の凸凹に合わせて接地判定をする（以前は Y < 0.0f の決め打ちだった）
    float groundY = GetTerrainHeight(pos.x, pos.z);
    if (pos.y < groundY)
    {
        pos.y = groundY;
        m_Velocity.y = 0.0f;
        m_Ground = true;
    }

    transform->SetPosition(pos);  // ← TransformComponentだけ更新


    auto trees = Manager::GetGameObjects<Tree>();

    for (auto tree : trees) {
        Vector3 treePosition = tree->GetPosition();
        Vector3 playerPosition = pos;


        treePosition.y = 0.0f;
        playerPosition.y = 0.0f;


        Vector3 dir = playerPosition - treePosition;
        float lenght = dir.length();

        if (lenght < 1.5f && lenght > 0.001f)
        {
            dir /= lenght;
            dir *= 1.5f - lenght;
            pos += dir;
        }

    }


    auto boxs = Manager::GetGameObjects<Box>();
    for (auto box : boxs) {
        Vector3 boxPos = box->GetPosition();
        Vector3 boxScale = box->GetScale();


        if (boxPos.x - boxScale.x < pos.x &&
            pos.x < boxPos.x + boxScale.x &&
            boxPos.z - boxScale.z < pos.z &&
            pos.z < boxPos.z + boxScale.z)
        {
            if (boxPos.y + boxScale.y < pos.y &&
                pos.y < boxPos.y + boxScale.y * 2.0 && m_Velocity.y < 0.0f)
            {
                //UP
                pos.y = boxPos.y + boxScale.y * 2.0f;
                m_Velocity.y = 0.0f;
                m_Ground = true;
            }
            else if (boxPos.y - boxScale.y < pos.y &&
                pos.y < boxPos.y + boxScale.y)
            {
                //側面

                pos.x = olsPos.x;
                pos.z = olsPos.z;
                m_Velocity.x = 0.0f;
                m_Velocity.z = 0.0f;


            }
        }
    }

    if (!oldGround && m_Ground) {

        //SetScale({ 0.5f,2.0f,0.5f });

    }


    transform->SetPosition(pos);


    if (Input::GetKeyTrigger('F')) {
        Bullet* bullet = Manager::AddGameObject<Bullet>();
        bullet->SetPosition(pos);
        bullet->SetVelocity(transform->GetForward() * 50.0f);
    }

#ifdef _DEBUG

    ImGui::Begin("PlayerMove");
    {
        ImGui::SliderFloat
        ("speed",
            &m_Speed, 0.5f, 500.0f, "%.2f");



    }
    ImGui::End();


#endif // DEBUG

    if (m_Ground) {
        m_MoveAnimation += m_Velocity.length() * dt;
        float ly = GetScale().y;
        ly += sinf(m_MoveAnimation * 3.0f) * 0.03f;
        // SetScale({ GetScale().x,ly,GetScale().z});
    }


    // 影も地形の高さに合わせる
    Vector3 shadowPos = pos;
    shadowPos.y = GetTerrainHeight(pos.x, pos.z) + 0.1f;
    m_Shadow->SetPosition(shadowPos);


    // ----------------------------
// Animation切り替え
// ----------------------------

// Y方向（ジャンプ・落下）の速度を除外
    float horizontalSpeed = sqrtf(
        m_Velocity.x * m_Velocity.x +
        m_Velocity.z * m_Velocity.z
    );

    if (horizontalSpeed > 1.0f)
    {
        SetAnimation("Run");
    }
    else
    {
        SetAnimation("Idle");
    }

    m_AnimationFrame++;
    m_NextAnimationFrame++;

    m_Blend += 0.01f;

    if (m_Blend > 1.0f) {
        m_Blend = 1.0f;
    }

    GameObject::Update(deltaTime);

}

void Player::Draw()
{
    Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);
    Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
    Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

    // TransformComponentからワールド行列を取得
    TransformComponent* transform = GetComponent<TransformComponent>();

    // 見た目だけ地面との埋まりを補正（ワールド空間で純粋にYだけ持ち上げる）
    XMMATRIX world = transform->GetWorldMatrix() * XMMatrixTranslation(0.0f, kModelYOffset, 0.0f);
    Renderer::SetWorldMatrix(world);

    m_AnimationModel->Update(
        m_AnimationName.c_str(), m_AnimationFrame,
        m_NextAnimationName.c_str(), m_NextAnimationFrame,
        m_Blend);

    GameObject::Draw();
}

void Player::SetAnimation(const char* AnimationName)
{
    if (m_NextAnimationName != AnimationName)
    {
        m_AnimationName = m_NextAnimationName;
        m_AnimationFrame = m_NextAnimationFrame;

        m_NextAnimationName = AnimationName;
        m_NextAnimationFrame = 0;

        m_Blend = 0.0f;
    }

}
