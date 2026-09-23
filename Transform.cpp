#include "Transform.h"
#include "GameObject.h"
XMMATRIX TransformComponent::GetWorldMatrix() const
{
    XMMATRIX scaleM = XMMatrixScaling(scale.x, scale.y, scale.z);
    XMMATRIX rotM = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y + XM_PI, rotation.z);
    XMMATRIX transM = XMMatrixTranslation(position.x, position.y, position.z);

    // ローカル行列（スケール→回転→平行移動）
    XMMATRIX local = scaleM * rotM * transM;

    // 親がいる場合は「親のワールド行列 × ローカル行列」
    if (m_GameObject->GetParent())
    {
        XMMATRIX parentWorld = m_GameObject->GetParent()->GetComponent<TransformComponent>()->GetWorldMatrix();
        return local * parentWorld;
    }

    return local;
}
