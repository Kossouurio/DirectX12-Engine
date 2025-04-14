#include "pch.h"
#include "ComponentTransform.h"

ComponentTransform::ComponentTransform(Entity* entity)
    : Component(entity),
    vPosition({ 0.0f, 0.0f, 0.0f }),
    vSize({ 1.0f, 1.0f, 1.0f }),
    vScale({ 1.0f, 1.0f, 1.0f }),
    vVelocity({ 0.0f, 0.0f, 0.0f }),
    vAcceleration({ 0.0f, 0.0f, 0.0f })
{
    Identity();
}



ComponentTransform::~ComponentTransform() {
}

void ComponentTransform::SetPosition(const XMFLOAT3& pos) 
{
    vPosition = pos;
}

Vector2 ComponentTransform::GetPosition() const
{
	return { vPosition.x, vPosition.y };
}

void ComponentTransform::SetSize(const XMFLOAT3& sz) 
{
    vSize = sz;
}

float ComponentTransform::GetSize() const
{
	return vSize.x;
}

void ComponentTransform::SetScale(const XMFLOAT3& sca) 
{
    vScale = sca;
}

float ComponentTransform::GetScale() const
{
	return vScale.x;
}

void ComponentTransform::SetVelocity(const XMFLOAT3& vel) 
{
    vVelocity = vel;
}

Vector2 ComponentTransform::GetVelocity() const
{
	return { vVelocity.x, vVelocity.y };
}

void ComponentTransform::Translate(const XMFLOAT3& delta) 
{
    vPosition.x += delta.x;
    vPosition.y += delta.y;
    vPosition.z += delta.z;
}

void ComponentTransform::Identity() {
    vScale = XMFLOAT3(1, 1, 1);
    vDirection = XMFLOAT3(0, 0, 1);
    vUp = XMFLOAT3(0, 1, 0);
    vRight = XMFLOAT3(1, 0, 0);
    qRot = XMFLOAT4(0, 0, 0, 1);
    vPosition = XMFLOAT3(0, 0, 0);
    XMStoreFloat4x4(&mRot, XMMatrixIdentity());
    XMStoreFloat4x4(&matrix, XMMatrixIdentity());
}

void ComponentTransform::FromMatrix(XMFLOAT4X4* matrix) {
    XMStoreFloat4x4(matrix, XMLoadFloat4x4(matrix));
}

void ComponentTransform::UpdateRotationFromVector() {
    XMStoreFloat4x4(&mRot, XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&vDirection)));
}

void ComponentTransform::UpdateRotationFromQuaternion() {
    XMStoreFloat4x4(&mRot, XMMatrixRotationQuaternion(XMLoadFloat4(&qRot)));
}

void ComponentTransform::UpdateRotationFromMatrix() {
    XMStoreFloat4x4(&mRot, XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&vDirection)));
}

void ComponentTransform::UpdateMatrix() {
    XMMATRIX mRot = XMMatrixRotationQuaternion(XMLoadFloat4(&qRot));
    XMMATRIX mvScale = XMMatrixScalingFromVector(XMLoadFloat3(&vScale));
    XMMATRIX mTrans = XMMatrixTranslationFromVector(XMLoadFloat3(&vPosition));
    XMMATRIX mWorld = mRot * mvScale * mTrans;
    XMStoreFloat4x4(&matrix, mWorld);
}

void ComponentTransform::Rotate(float yaw, float pitch, float roll) {
    XMVECTOR qYaw = XMQuaternionRotationRollPitchYaw(0, yaw, 0);
    XMVECTOR qPitch = XMQuaternionRotationRollPitchYaw(pitch, 0, 0);
    XMVECTOR qRoll = XMQuaternionRotationRollPitchYaw(0, 0, roll);

    XMVECTOR qRotVector = XMLoadFloat4(&qRot);
    qRotVector = XMQuaternionMultiply(qRotVector, qYaw);
    qRotVector = XMQuaternionMultiply(qRotVector, qPitch);
    qRotVector = XMQuaternionMultiply(qRotVector, qRoll);

    XMStoreFloat4(&qRot, qRotVector);
}

void ComponentTransform::RotateYaw(float angle) {
    XMVECTOR rotationQuat = XMQuaternionRotationRollPitchYaw(0, angle, 0); 
    XMVECTOR currentQuat = XMLoadFloat4(&qRot);
    currentQuat = XMQuaternionMultiply(currentQuat, rotationQuat); 
    XMStoreFloat4(&qRot, currentQuat);
}

void ComponentTransform::RotatePitch(float angle) {
    XMVECTOR rotationQuat = XMQuaternionRotationRollPitchYaw(angle, 0, 0); 
    XMVECTOR currentQuat = XMLoadFloat4(&qRot);
    currentQuat = XMQuaternionMultiply(currentQuat, rotationQuat);
    XMStoreFloat4(&qRot, currentQuat);
}

void ComponentTransform::RotateRoll(float angle) {
    XMVECTOR rotationQuat = XMQuaternionRotationRollPitchYaw(0, 0, angle);
    XMVECTOR currentQuat = XMLoadFloat4(&qRot);
    currentQuat = XMQuaternionMultiply(currentQuat, rotationQuat);
    XMStoreFloat4(&qRot, currentQuat);
}

void ComponentTransform::RotateWorld(XMFLOAT4* pMatrix) {
    XMMATRIX rotationMatrix = XMLoadFloat4x4(reinterpret_cast<XMFLOAT4X4*>(pMatrix)); 
    XMVECTOR currentQuat = XMLoadFloat4(&qRot); 
    XMVECTOR rotationQuat = XMQuaternionRotationMatrix(rotationMatrix);
    currentQuat = XMQuaternionMultiply(currentQuat, rotationQuat);
    XMStoreFloat4(&qRot, currentQuat); 
}

void ComponentTransform::RotateWorldX(float angle) {
    XMVECTOR rotationAxis = XMVectorSet(1, 0, 0, 0);
    XMVECTOR rotationQuat = XMQuaternionRotationAxis(rotationAxis, angle); 
    XMVECTOR currentQuat = XMLoadFloat4(&qRot);
    currentQuat = XMQuaternionMultiply(currentQuat, rotationQuat);
    XMStoreFloat4(&qRot, currentQuat);
}

void ComponentTransform::RotateWorldY(float angle) {
    XMVECTOR rotationAxis = XMVectorSet(0, 1, 0, 0);
    XMVECTOR rotationQuat = XMQuaternionRotationAxis(rotationAxis, angle);
    XMVECTOR currentQuat = XMLoadFloat4(&qRot);
    currentQuat = XMQuaternionMultiply(currentQuat, rotationQuat);
    XMStoreFloat4(&qRot, currentQuat);
}

void ComponentTransform::RotateWorldZ(float angle) {
    XMVECTOR rotationAxis = XMVectorSet(0, 0, 1, 0); 
    XMVECTOR rotationQuat = XMQuaternionRotationAxis(rotationAxis, angle);
    XMVECTOR currentQuat = XMLoadFloat4(&qRot);
    currentQuat = XMQuaternionMultiply(currentQuat, rotationQuat);
    XMStoreFloat4(&qRot, currentQuat);
}

void ComponentTransform::PrintRotation() const {
    XMVECTOR quaternion = XMLoadFloat4(&qRot);
    XMVECTOR axis;
    float angle;

    XMQuaternionToAxisAngle(&axis, &angle, quaternion);

    angle = XMConvertToDegrees(angle);

    XMFLOAT3 axisFloat;
    XMStoreFloat3(&axisFloat, axis);

    std::cout << "Rotation actuelle : Angle = " << angle
        << "° autour de l'axe (" << axisFloat.x
        << ", " << axisFloat.y
        << ", " << axisFloat.z << ")" << std::endl;
}
