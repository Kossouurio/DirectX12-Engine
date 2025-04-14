#include "pch.h"
#include "ComponentCamera.h"

ComponentCamera::ComponentCamera(Entity* entity) : Component(entity) {
    camera = new Camera(); 

    position = { 0.0f, 0.0f, -2.0f };
    rotation = { 0.0f, 0.0f, 0.0f };
    camera->SetPosition(XMLoadFloat3(&position)); 
}

Camera* ComponentCamera::GetCamera() const {
	return camera;
}

//void ComponentCamera::Move(const XMFLOAT3& delta) {
//    XMVECTOR pos = XMLoadFloat3(&position);
//    XMVECTOR move = XMLoadFloat3(&delta);
//    pos = XMVectorAdd(pos, move);
//    XMStoreFloat3(&position, pos);
//
//    camera->SetPosition(pos);
//
//    HEAD("CAMERA_MOVE") << "Nouvelle Position: (" << position.x << ", " << position.y << ", " << position.z << ")" << ENDL;
//}

void ComponentCamera::Move(const XMFLOAT3& delta) {
    XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
    XMVECTOR moveVec = XMVector3Transform(XMLoadFloat3(&delta), rotationMatrix);

    XMVECTOR pos = XMLoadFloat3(&position);
    pos = XMVectorAdd(pos, moveVec);
    XMStoreFloat3(&position, pos);

    camera->SetPosition(pos);
}


void ComponentCamera::Rotate(const XMFLOAT3& rotationDelta) {
    rotation.x += rotationDelta.x;
    rotation.y += rotationDelta.y;
    rotation.z += rotationDelta.z;

    XMVECTOR newTarget = XMVectorSet(rotation.x, rotation.y, rotation.z, 1.0f);
    camera->SetTarget(newTarget);
}


DirectX::XMMATRIX ComponentCamera::GetViewMatrix() const {
    DirectX::XMVECTOR eye = DirectX::XMLoadFloat3(&position);
    DirectX::XMVECTOR focus = DirectX::XMVectorAdd(eye, DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f));
    DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    return DirectX::XMMatrixLookAtLH(eye, focus, up);
}

DirectX::XMMATRIX ComponentCamera::GetProjectionMatrix() const {
    return DirectX::XMMatrixPerspectiveFovLH(XMConvertToRadians(fieldOfView), 800.0f / 600.0f, 0.1f, 100.0f);
}
