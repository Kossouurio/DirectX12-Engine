#pragma once
//#include "Entity.h"
//#include "Component.h"
//#include "UtilsImport.h"

class ComponentTransform : public Component {
private:
    XMFLOAT3 vPosition;

    XMFLOAT3 vSize;
    XMFLOAT3 vScale;

    XMFLOAT3 vDirection;

    XMFLOAT3 vForward;
    XMFLOAT3 vRight;
    XMFLOAT3 vUp;

    XMFLOAT4 qRot;
    XMFLOAT4X4 mRot;

    XMFLOAT4X4 matrix;

public:
    ComponentTransform(Entity* entity);

    virtual void Logs();

    void SetPosition(const XMFLOAT3& pos);
    XMFLOAT3 GetPosition() const;

    void SetSize(const XMFLOAT3& size);
	float GetSize() const;

    void SetScale(const XMFLOAT3& scale);
	float GetScale() const;

	void SetForward(const XMFLOAT3& forward);
	XMFLOAT3 GetForward() const;

	void SetRight(const XMFLOAT3& right);
	XMFLOAT3 GetRight() const;

	void SetUp(const XMFLOAT3& up);
	XMFLOAT3 GetUp() const;

	void MoveBy(const XMFLOAT3& delta);

    void UpdateTransform(float dt);
	void UpdateVectors();

    void Translate(const XMFLOAT3& delta);

    void Identity();
    void FromMatrix(XMFLOAT4X4* matrix);
    void UpdateRotationFromVector();
    void UpdateRotationFromQuaternion();
    void UpdateRotationFromMatrix();
    void UpdateMatrix();

	XMMATRIX GetWorldMatrix() const;

    void Rotate(float yaw, float pitch, float roll);
    void RotateYaw(float angle);
    void RotatePitch(float angle);
    void RotateRoll(float angle);
    void RotateWorld(XMFLOAT4* pMatrix);
    void RotateWorldX(float angle);
    void RotateWorldY(float angle);
    void RotateWorldZ(float angle);
    void RotateAroundAxis(const XMFLOAT3& axis, float angle);

    void PrintRotation() const;
	XMFLOAT4 GetRotation() const;
};
