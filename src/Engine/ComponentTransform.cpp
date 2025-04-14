#include "pch.h"
#include "ComponentTransform.h"
#include "ComponentPhysics.h"

ComponentTransform::ComponentTransform(Entity* entity)
    : Component(entity),
    vPosition({ 0.0f, 0.0f, 0.0f }),
    vSize({ 1.0f, 1.0f, 1.0f }),
    vScale({ 1.0f, 1.0f, 1.0f })
{
    Identity();
    UpdateMatrix();
}

void ComponentTransform::Logs() {
     HEAD("sys_TRANSFORM") << "Entity " << GetEntity()->GetID() << " Position: ("
            << vPosition.x << ", " << vPosition.y << ", " << vPosition.z << ")"
            << " Rotation: (" << qRot.x << ", " << qRot.y << ", " << qRot.z << ")"
            << ENDL;
   
}


void ComponentTransform::SetPosition(const XMFLOAT3& pos) 
{
    vPosition = pos;
    UpdateMatrix();
}

XMFLOAT3 ComponentTransform::GetPosition() const
{
	return vPosition;
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


void ComponentTransform::SetForward(const XMFLOAT3& forward)
{
	vDirection = forward;
}

XMFLOAT3 ComponentTransform::GetForward() const
{
	return vDirection;
}

void ComponentTransform::SetRight(const XMFLOAT3& right)
{
	vRight = right;
}

XMFLOAT3 ComponentTransform::GetRight() const
{
	return vRight;
}

void ComponentTransform::SetUp(const XMFLOAT3& up)
{
	vUp = up;
}

XMFLOAT3 ComponentTransform::GetUp() const
{
	return vUp;
}

void ComponentTransform::MoveBy(const XMFLOAT3& delta) {
    vPosition.x += delta.x;
    vPosition.y += delta.y;
    vPosition.z += delta.z;
}

void ComponentTransform::UpdateTransform(float dt) {
    ComponentPhysics* physics = GetEntity()->GetComponent<ComponentPhysics>();
    if (physics) {
        physics->ApplyPhysics(dt);

        XMFLOAT3 newPosition = GetPosition();
        XMFLOAT3 velocity = physics->GetVelocity();

        newPosition.x += velocity.x * dt;
        newPosition.y += velocity.y * dt;
        newPosition.z += velocity.z * dt;

        if (newPosition.y <= 0.0f) {
            newPosition.y = 0.0f;
            physics->SetIsGrounded(true);
        }
        else {
            physics->SetIsGrounded(false);
        }

        SetPosition(newPosition);
    }

    UpdateMatrix();
    UpdateVectors();
}




void ComponentTransform::UpdateVectors() {
    XMMATRIX rotationMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&qRot));

    XMVECTOR forward = XMVector3Transform(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotationMatrix);
    XMVECTOR right = XMVector3Transform(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), rotationMatrix);
    XMVECTOR up = XMVector3Transform(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), rotationMatrix);

    float pitchLimit = 0.99f;
    float forwardY = XMVectorGetY(forward);
    if (forwardY > pitchLimit) {
        forward = XMVectorSetY(forward, pitchLimit);
    }
    if (forwardY < -pitchLimit) {
        forward = XMVectorSetY(forward, -pitchLimit);
    }

    right = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), forward));
    up = XMVector3Normalize(XMVector3Cross(forward, right));

    SetForward(XMFLOAT3(XMVectorGetX(forward), XMVectorGetY(forward), XMVectorGetZ(forward)));
    SetRight(XMFLOAT3(XMVectorGetX(right), XMVectorGetY(right), XMVectorGetZ(right)));
    SetUp(XMFLOAT3(XMVectorGetX(up), XMVectorGetY(up), XMVectorGetZ(up)));
}




void ComponentTransform::Translate(const XMFLOAT3& delta) 
{
    vPosition.x += delta.x;
    vPosition.y += delta.y;
    vPosition.z += delta.z;
    UpdateMatrix();
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
    /*XMMATRIX mWorld = mvScale * mRot * mTrans;*/

    XMStoreFloat4x4(&matrix, mWorld);
}

XMMATRIX ComponentTransform::GetWorldMatrix() const {
    XMFLOAT4X4 worldMatrixFloat;
    XMStoreFloat4x4(&worldMatrixFloat, XMLoadFloat4x4(&matrix));

    return XMLoadFloat4x4(&matrix);
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
    UpdateVectors();
}

void ComponentTransform::RotateYaw(float angle) {
    XMVECTOR rotationQuat = XMQuaternionRotationRollPitchYaw(0, angle, 0);
    XMVECTOR currentQuat = XMLoadFloat4(&qRot);
    currentQuat = XMQuaternionMultiply(currentQuat, rotationQuat);
    currentQuat = XMQuaternionNormalize(currentQuat); 

    XMStoreFloat4(&qRot, currentQuat);
    UpdateVectors();
}

void ComponentTransform::RotatePitch(float angle) {
    XMVECTOR rotationQuat = XMQuaternionRotationRollPitchYaw(angle, 0, 0);
    XMVECTOR currentQuat = XMLoadFloat4(&qRot);
    currentQuat = XMQuaternionMultiply(currentQuat, rotationQuat);
    currentQuat = XMQuaternionNormalize(currentQuat);  

    XMStoreFloat4(&qRot, currentQuat);
    UpdateVectors();
}

void ComponentTransform::RotateRoll(float angle) {
    XMVECTOR rotationQuat = XMQuaternionRotationRollPitchYaw(0, 0, angle);
    XMVECTOR currentQuat = XMLoadFloat4(&qRot);
    currentQuat = XMQuaternionMultiply(currentQuat, rotationQuat);
    XMStoreFloat4(&qRot, currentQuat);

    /*UpdateVectors();*/
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

void ComponentTransform::RotateAroundAxis(const XMFLOAT3& axis, float angle) {
    XMVECTOR rotationAxis = XMLoadFloat3(&axis);
    XMVECTOR rotationQuat = XMQuaternionRotationAxis(rotationAxis, angle);

    XMVECTOR currentQuat = XMLoadFloat4(&qRot);
    currentQuat = XMQuaternionMultiply(currentQuat, rotationQuat);
    currentQuat = XMQuaternionNormalize(currentQuat);

    XMStoreFloat4(&qRot, currentQuat);
    UpdateVectors(); 
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

XMFLOAT4 ComponentTransform::GetRotation() const {
    return qRot;
}