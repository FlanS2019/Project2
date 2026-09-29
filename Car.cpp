#include "main.h"
#include "car.h"
#include "renderer.h"
#include "DirectXTex.h"
#include "animationModel.h"
#include "input.h"
#include "terrainHeight.h"
#include "transform.h"
#include <imgui.h>

// ダウンロードした.fbxファイルを "mercedes.fbx" にリネームして
// asset\Model\Mercedes\mercedes.fbx に置いてください
static const char* kCarModelPath = "asset\\Model\\Mercedes\\uploads_files_2787791_Mercedes+Benz+GLS+580.fbx";

// 車のモデルは元のスケールが分からないので、ImGuiで調整できるようにしてある
static float g_CarScale = 1.0f;

// 向き（Y軸回転、度数）。ImGuiで調整可能
static float g_CarRotationYDeg = 0.0f;

void Car::Init()
{
    m_Layer = 1;
    SetPosition({ 15.0f, 0.0f, -5.0f }); // お好みで初期位置は調整してください
    SetScale({ g_CarScale, g_CarScale, g_CarScale });

    // Playerと同じ仕組み(AnimationModel/Assimp経由)でfbxを読み込む
    // アニメーションは付けないので、静止したモデルとして表示されます
    m_AnimationModel = AddComponent<AnimationModel>(this);
    m_AnimationModel->Load(kCarModelPath);

    // シェーダー読込
    Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
        "unlitTextureVS.cso");

    Renderer::CreatePixelShader(&m_PixelShader,
        "unlitTexturePS.cso");
}

void Car::Uninit()
{
    if (m_VertexShader) { m_VertexShader->Release(); m_VertexShader = nullptr; }
    if (m_PixelShader) { m_PixelShader->Release(); m_PixelShader = nullptr; }
    if (m_VertexLayout) { m_VertexLayout->Release(); m_VertexLayout = nullptr; }

    GameObject::Uninit();
}

void Car::Update(double deltaTime)
{
    float dt = static_cast<float>(deltaTime);

    // 地形の凸凹に合わせてY座標を調整
    Vector3 pos = GetPosition();
    pos.y = GetTerrainHeight(pos.x, pos.z);
    SetPosition(pos);

    // スケール・向きをImGuiでリアルタイム調整できるように反映
    SetScale({ g_CarScale, g_CarScale, g_CarScale });
    SetRotation({ 0.0f, XMConvertToRadians(g_CarRotationYDeg), 0.0f });

#ifdef _DEBUG
    ImGui::Begin("Car");
    {
        ImGui::SliderFloat("Scale", &g_CarScale, 0.001f, 5.0f, "%.4f");
        ImGui::SliderFloat("Rotation(Y, deg)", &g_CarRotationYDeg, -180.0f, 180.0f, "%.1f");

        Vector3 p = GetPosition();
        float posArr[3] = { p.x, p.y, p.z };
        if (ImGui::SliderFloat3("Position(X,-,Z)", posArr, -60.0f, 60.0f, "%.2f"))
        {
            SetPosition({ posArr[0], p.y, posArr[2] });
        }
    }
    ImGui::End();
#endif // _DEBUG

    GameObject::Update(dt);
}

void Car::Draw()
{
    // インプットレイアウト設定
    Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

    // シェーダー設定
    Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
    Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

    // マトリクス設定 (TransformComponentからワールド行列を取得)
    TransformComponent* transform = GetComponent<TransformComponent>();
    DirectX::XMMATRIX world = transform->GetWorldMatrix();
    Renderer::SetWorldMatrix(world);

    GameObject::Draw();
}
