#include "pch.h"
#include "Camera.h"

Camera::Camera() :
    mPosition(XMVectorSet(0.0f, 0.0f, -5.0f, 1.0f)),
    mTarget(XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)),
    mUp(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f))
{
    UpdateViewMatrix();
    SetProjection(XM_PIDIV4, 800.0f / 600.0f, 0.1f, 100.0f);
}

Camera::~Camera() {}

void Camera::SetPosition(const XMVECTOR& position) {
    mPosition = position;
    UpdateViewMatrix();
}

void Camera::SetTarget(const XMVECTOR& target) {
    mTarget = target;
    UpdateViewMatrix();
}

void Camera::SetUp(const XMVECTOR& up) {
    mUp = up;
    UpdateViewMatrix();
}

void Camera::SetProjection(float fovY, float aspectRatio, float nearZ, float farZ) {
    mProjectionMatrix = XMMatrixPerspectiveFovLH(fovY, aspectRatio, nearZ, farZ);
}

XMMATRIX Camera::GetViewMatrix() const {
    return mViewMatrix;
}

XMMATRIX Camera::GetProjectionMatrix() const {
    return mProjectionMatrix;
}

void Camera::UpdateViewMatrix() {
    mViewMatrix = XMMatrixLookAtLH(mPosition, mTarget, mUp);
}