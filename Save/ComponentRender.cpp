#include "pch.h"
#include "ComponentRender.h"
#include <iostream>

ComponentRender::ComponentRender(Entity* entity)
    : Component(entity), color({ 1.0f, 1.0f, 1.0f, 1.0f }), texturePath(""), visible(true) 
{
}

ComponentRender::~ComponentRender() 
{
}

void ComponentRender::SetColor(const XMFLOAT4& newColor) 
{
    color = newColor;
}

XMFLOAT4 ComponentRender::GetColor() const
{
	return color;
}

void ComponentRender::SetTexture(const std::string& path) 
{
    texturePath = path;
}

std::string ComponentRender::GetTexture() const
{
	return texturePath;
}

void ComponentRender::SetVisibility(bool isVisible) 
{
    visible = isVisible;
}

bool ComponentRender::GetVisibility() const
{
	return visible;
}

void ComponentRender::Render() 
{
    if (!visible) return;

    std::cout << "Rendering Entity ID: " << GetID() << std::endl;
    std::cout << "Color: (" << color.x << ", " << color.y << ", " << color.z << ", " << color.w << ")" << std::endl;

    if (!texturePath.empty()) {
        std::cout << "Using texture: " << texturePath << std::endl;
    }
    else {
        std::cout << "No texture applied." << std::endl;
    }
}
