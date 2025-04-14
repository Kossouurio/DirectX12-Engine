#include "pch.h"
#include "ComponentCamera.h"

ComponentCamera::ComponentCamera(Entity* entity) : Component(entity) {
    ComponentTransform* transform = entity->GetComponent<ComponentTransform>();
    camera = new Camera(transform); 
}

void ComponentCamera::Logs() {
    Camera* cam = GetCamera();
    if (!cam) return;

    XMFLOAT4 rotation = GetEntity()->GetComponent<ComponentTransform>()->GetRotation();

    HEAD("sys_CAMERA") << "Camera Position: ("
        << cam->GetPositionMatrix().m128_f32[0] << ", "
        << cam->GetPositionMatrix().m128_f32[1] << ", "
        << cam->GetPositionMatrix().m128_f32[2] << ")" << ENDL;

    HEAD("sys_CAMERA") << "Camera Rotation: ("
        << rotation.x << ", "
        << rotation.y << ", "
        << rotation.z << ", "
        << rotation.w << ")" << ENDL;

    HEAD("sys_CAMERA") << "Camera LookAt: ("
        << cam->GetViewMatrix().r[2].m128_f32[0] << ", "
        << cam->GetViewMatrix().r[2].m128_f32[1] << ", "
        << cam->GetViewMatrix().r[2].m128_f32[2] << ")" << ENDL;

    HEAD("sys_CAMERA") << "Target: (" << cam->GetTarget().m128_f32[0] << ", "
        << cam->GetTarget().m128_f32[1] << ", "
        << cam->GetTarget().m128_f32[2] << ")" << ENDL;

    HEAD("sys_CAMERA") << "Forward: (" << cam->GetForward().m128_f32[0] << ", "
        << cam->GetForward().m128_f32[1] << ", "
        << cam->GetForward().m128_f32[2] << ")" << ENDL;

    HEAD("sys_CAMERA") << "Right: (" << cam->GetRight().m128_f32[0] << ", "
        << cam->GetRight().m128_f32[1] << ", "
        << cam->GetRight().m128_f32[2] << ")" << ENDL;
}


Camera* ComponentCamera::GetCamera() const {
    return camera;
}

void ComponentCamera::SyncWithTransform() {
    ComponentTransform* transform = m_pEntity->GetComponent<ComponentTransform>();
    if (transform) {
        // Met a jour la position
        XMFLOAT3 position = transform->GetPosition();

        camera->SetPositionMatrix(XMLoadFloat3(&position));

        // Recuperer les vecteurs depuis `ComponentTransform`
        XMFLOAT3 forward = transform->GetForward();
        XMFLOAT3 right = transform->GetRight();
        XMFLOAT3 up = transform->GetUp();

        //// Clamp l'angle Pitch
        //float pitchAngle = XMVectorGetY(forwardVector);
        //pitchAngle = clamp(pitchAngle, -0.99f, 0.99f); 
        //forwardVector = XMVectorSetY(forwardVector, pitchAngle);

        // Met a jour les vecteurs Forward et Right dans la camera
        camera->SetForward(XMLoadFloat3(&forward));
        camera->SetRight(XMLoadFloat3(&right));
        camera->SetUp(XMLoadFloat3(&up));

        // On met toujours a jour `Target` dynamiquement
        XMVECTOR target = XMVectorAdd(XMLoadFloat3(&position), XMLoadFloat3(&forward));
        camera->SetTarget(target);

        // Mise a jour de la matrice de vue
        camera->UpdateViewMatrix();
    }
}

