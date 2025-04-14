#include "pch.h"
#include "ComponentMesh.h"

ComponentMesh::ComponentMesh(Entity* entity, ID3D12Device* device, const Geometry& geometry)
    : Component(entity)
{
    mMesh = new Mesh(device);
    mMesh->Initialize(geometry);
}

int ComponentMesh::GetID() const
{
    return m_pEntity->GetID();
}

Mesh* ComponentMesh::GetMesh()
{
	return mMesh;
}

XMFLOAT3 ComponentMesh::GetBoundingBox() const {
    if (!mMesh) return XMFLOAT3(1.0f, 1.0f, 1.0f);

    return mMesh->GetBoundingBox();
}
