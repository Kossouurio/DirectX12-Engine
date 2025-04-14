#pragma once


class ComponentTransform;

class Camera {
public:
    Camera(ComponentTransform* transform);
    ~Camera();

    void SetPositionMatrix(const XMVECTOR& position);
	XMVECTOR GetPositionMatrix() const;

    void SetTarget(const XMVECTOR& target);
	XMVECTOR GetTarget() const;

	void SetForward(const XMVECTOR& forward);
	XMVECTOR GetForward() const;
	void SetRight(const XMVECTOR& right);
	XMVECTOR GetRight() const;
    void SetUp(const XMVECTOR& up);
	XMVECTOR GetUp() const;

    XMFLOAT3 GetPosition() const;
    XMFLOAT3 GetDirection() const;
    float GetFovY() const;
    float GetAspectRatio() const;

    void SetProjectionMatrix(float fovY, float aspectRatio, float nearZ, float farZ);

    XMMATRIX GetViewMatrix() const;
    XMMATRIX GetProjectionMatrix() const;

    void UpdateViewMatrix();

    void UpdateFromTransform(const ComponentTransform& transform);

private:
	XMVECTOR mPositionMatrix;
    XMVECTOR mTarget;
	XMVECTOR mForward;
	XMVECTOR mRight;
    XMVECTOR mUp;  
    XMMATRIX mViewMatrix;
    XMMATRIX mProjectionMatrix;
    float mFovY;
    float mAspectRatio;
    float mNearZ;
    float mFarZ;
};
