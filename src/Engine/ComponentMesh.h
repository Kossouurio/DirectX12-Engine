#pragma once
#include "Component.h"
#include "Mesh.h"

class ComponentMesh : public Component
{
private:
    Mesh* mMesh;

public:
    ComponentMesh(Entity* entity, ID3D12Device* device, const Geometry& geometry);

    int GetID() const;
    Mesh* GetMesh();

	XMFLOAT3 GetBoundingBox() const;
};
