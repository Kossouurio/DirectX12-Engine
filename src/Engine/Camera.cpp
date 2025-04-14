#include "pch.h"
#include "Camera.h"


Camera::Camera(ComponentTransform* transform) {
    if (transform) {
        XMFLOAT3 position = transform->GetPosition();
        XMFLOAT4 rotation = transform->GetRotation();

        mPositionMatrix = XMLoadFloat3(&position);
        XMMATRIX rotationMatrix = XMMatrixRotationQuaternion(XMLoadFloat4(&rotation));
        XMVECTOR forwardVector = XMVector3Transform(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rotationMatrix);
        mTarget = XMVectorAdd(mPositionMatrix, forwardVector);
    }
    else {
        mPositionMatrix = XMVectorZero();
        mTarget = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    }

    mUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    UpdateViewMatrix();

    mFovY = XM_PIDIV4;
    mAspectRatio = static_cast<float>(D3D12Window::GetInstance().mWindowWidth) /
        static_cast<float>(D3D12Window::GetInstance().mWindowHeight);
    mNearZ = 0.1f;
    mFarZ = 100.0f;

    SetProjectionMatrix(mFovY, mAspectRatio, mNearZ, mFarZ);
}


Camera::~Camera() {}

void Camera::SetPositionMatrix(const XMVECTOR& position) {
	mPositionMatrix = position;
	UpdateViewMatrix();
}

XMVECTOR Camera::GetPositionMatrix() const {
	return mPositionMatrix;
}

void Camera::SetTarget(const XMVECTOR& target) {
    mTarget = target;
    UpdateViewMatrix();
}

XMVECTOR Camera::GetTarget() const {
	return mTarget;
}

void Camera::SetForward(const XMVECTOR& forward) {
	mForward = forward;
	UpdateViewMatrix();
}

XMVECTOR Camera::GetForward() const {
	return mForward;
}

void Camera::SetRight(const XMVECTOR& right) {
	mRight = right;
	UpdateViewMatrix();
}

XMVECTOR Camera::GetRight() const {
	return mRight;
}

void Camera::SetUp(const XMVECTOR& up) {
    mUp = up;
    UpdateViewMatrix();
}

XMVECTOR Camera::GetUp() const {
	return mUp;
}

void Camera::SetProjectionMatrix(float fovY, float aspectRatio, float nearZ, float farZ) {
    mFovY = fovY;
    mAspectRatio = aspectRatio;
    mNearZ = nearZ;
    mFarZ = farZ;
    mProjectionMatrix = XMMatrixPerspectiveFovLH(fovY, aspectRatio, nearZ, farZ);
}

XMMATRIX Camera::GetViewMatrix() const {
    return mViewMatrix;
}

XMMATRIX Camera::GetProjectionMatrix() const {
    return mProjectionMatrix;
}

XMFLOAT3 Camera::GetPosition() const {
    XMFLOAT3 position;
    XMStoreFloat3(&position, mPositionMatrix);
    return position;
}

XMFLOAT3 Camera::GetDirection() const {
    XMFLOAT3 direction;
    XMStoreFloat3(&direction, XMVector3Normalize(mForward));
    return direction;
}

float Camera::GetFovY() const {
    return mFovY;
}

float Camera::GetAspectRatio() const {
    return mAspectRatio;
}

void Camera::UpdateViewMatrix() {
    XMVECTOR target = XMVectorAdd(mPositionMatrix, mForward);

    mViewMatrix = XMMatrixLookAtLH(mPositionMatrix, target, mUp);

    XMFLOAT4X4 viewMatrix;
    XMStoreFloat4x4(&viewMatrix, GetViewMatrix());

    XMFLOAT3 pos, computedTarget, up;
    XMStoreFloat3(&pos, mPositionMatrix);
    XMStoreFloat3(&computedTarget, target);
    XMStoreFloat3(&up, mUp);
}

void Camera::UpdateFromTransform(const ComponentTransform& transform) {
    XMFLOAT3 position = transform.GetPosition();
    SetPositionMatrix(XMLoadFloat3(&position));
}
