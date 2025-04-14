#include "pch.h"
#include "UtilsImport.h"

void GeometryGenerator::GenerateCube(Geometry& geometry)
{
	geometry.vertexNumber = 24;
	geometry.indiceNumber = 36;

	geometry.vertices.resize(geometry.vertexNumber);
	geometry.indices.resize(geometry.indiceNumber);

    XMFLOAT3 positions[24] = {
        // Front face
        { -0.5f, -0.5f, -0.5f },
        {  0.5f, -0.5f, -0.5f },
        {  0.5f,  0.5f, -0.5f },
        { -0.5f,  0.5f, -0.5f },

        // Right face
        {  0.5f, -0.5f, -0.5f },
        {  0.5f, -0.5f,  0.5f },
        {  0.5f,  0.5f,  0.5f },
        {  0.5f,  0.5f, -0.5f },

        // Back face
        {  0.5f, -0.5f,  0.5f },
        { -0.5f, -0.5f,  0.5f },
        { -0.5f,  0.5f,  0.5f },
        {  0.5f,  0.5f,  0.5f },

        // Left face
        { -0.5f, -0.5f,  0.5f },
        { -0.5f, -0.5f, -0.5f },
        { -0.5f,  0.5f, -0.5f },
        { -0.5f,  0.5f,  0.5f },

        // Top face
        { -0.5f,  0.5f, -0.5f },
        {  0.5f,  0.5f, -0.5f },
        {  0.5f,  0.5f,  0.5f },
        { -0.5f,  0.5f,  0.5f },

        // Bottom face
        { -0.5f, -0.5f,  0.5f },
        {  0.5f, -0.5f,  0.5f },
        {  0.5f, -0.5f, -0.5f },
        { -0.5f, -0.5f, -0.5f }
    };

    uint16_t cubeIndices[36] = {
        0,  1,  2,  0,  2,  3,
        4,  5,  6,  4,  6,  7,
        8,  9, 10,  8, 10, 11,
       12, 13, 14, 12, 14, 15,
       16, 17, 18, 16, 18, 19,
       20, 21, 22, 20, 22, 23
    };

    XMFLOAT2 cubeUVs[24] = {
        // Front face
        { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f },
        // Right face
        { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f },
        // Back face
        { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f },
        // Left face
        { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f },
        // Top face
        { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f },
        // Bottom face
        { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f }, { 0.0f, 0.0f }
    };

    // Define normals for each vertex (24 vertices)
    XMFLOAT3 cubeNormals[24] = {
        // Front face
        { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f, -1.0f },
        // Right face
        { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f },
        // Back face
        { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f },
        // Left face
        { -1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f },
        // Top face
        { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f },
        // Bottom face
        { 0.0f, -1.0f, 0.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, -1.0f, 0.0f }
    };

	// Copy the vertex data into the geometry object
	for (size_t i = 0; i < geometry.vertexNumber; i++)
	{
		geometry.vertices[i].pos = positions[i];
		geometry.vertices[i].color = { ToolBox::RandomFloat(0.0f, 1.0f), ToolBox::RandomFloat(0.0f, 1.0f), ToolBox::RandomFloat(0.0f, 1.0f), 1.0f };
		geometry.vertices[i].uv = cubeUVs[i];
		geometry.vertices[i].normal = cubeNormals[i];
		geometry.vertices[i].tangent = { 0.0f, 0.0f, 0.0f };
		geometry.vertices[i].binormal = { 0.0f, 0.0f, 0.0f };
	}

	// Copy the index data into the geometry object
	for (size_t i = 0; i < geometry.indiceNumber; i++)
	{
		geometry.indices[i] = cubeIndices[i];
	}

    // Adjust face winding for back face culling (clockwise) 
    for (size_t i = 0; i < geometry.indices.size(); i += 3) {
        std::swap(geometry.indices[i], geometry.indices[i + 2]);
    }
}

void GeometryGenerator::GenerateSphere(Geometry& geometry, float radius, int numSegments)
{
	geometry.vertexNumber = (numSegments + 1) * (numSegments + 1);
	geometry.indiceNumber = numSegments * numSegments * 6;

	geometry.vertices.resize(geometry.vertexNumber);
	geometry.indices.resize(geometry.indiceNumber);

	// Generate the sphere vertices
    for (int i = 0; i <= numSegments; i++) {

        float phi = XM_PI * i / numSegments;
        for (int j = 0; j <= numSegments; j++) {
            float theta = XM_2PI * j / numSegments;
            float x = radius * std::sin(phi) * std::cos(theta);
            float y = radius * std::cos(phi);
            float z = radius * std::sin(phi) * std::sin(theta);
            geometry.vertices[i * (numSegments + 1) + j].pos = { x, y, z };
			// add random color
			geometry.vertices[i * (numSegments + 1) + j].color = { ToolBox::RandomFloat(0.0f, 1.0f), ToolBox::RandomFloat(0.0f, 1.0f), ToolBox::RandomFloat(0.0f, 1.0f), 1.0f };

            float u = 1 - (j / numSegments);
            float v = 1 - (i / numSegments);
            geometry.vertices[i * (numSegments + 1) + j].uv = { u, v };

            XMFLOAT3 normal(x, y, z);
            XMVECTOR normalVec = XMVector3Normalize(XMLoadFloat3(&normal));
            XMStoreFloat3(&normal, normalVec);
            geometry.vertices[i * (numSegments + 1) + j].normal = normal;
            geometry.vertices[i * (numSegments + 1) + j].tangent = { 0.0f, 0.0f, 0.0f };
            geometry.vertices[i * (numSegments + 1) + j].binormal = { 0.0f, 0.0f, 0.0f };
        }
    }

	// Generate the sphere indices
	for (int i = 0; i < numSegments; i++) {
		for (int j = 0; j < numSegments; j++) {
			int index = i * (numSegments + 1) + j;
			geometry.indices.push_back(index);
			geometry.indices.push_back(index + 1);
			geometry.indices.push_back(index + numSegments + 1);

			geometry.indices.push_back(index + 1);
			geometry.indices.push_back(index + numSegments + 2);
			geometry.indices.push_back(index + numSegments + 1);
		}
	}
}

void GeometryGenerator::GenerateCircle(Geometry& geometry, float radius, int numSegments)
{
    float angleStep = XM_2PI / numSegments;
    float angle = 0.0f;

    // Add center vertex
	geometry.vertices.push_back({ {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} });

    // Add vertices around the circumference
	for (int i = 0; i < numSegments; i++) {
		float x = radius * std::cos(angle);
		float y = radius * std::sin(angle);
		float u = (x / radius) * 0.5f + 0.5f;
		float v = (-y / radius) * 0.5f + 0.5f;
		geometry.vertices.push_back({ {x, y, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}, {u, v}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} });
        
        angle += angleStep;
	}

	// Add indices to form the circle
	for (int i = 1; i < numSegments; i++) {
		geometry.indices.push_back(0);          // Center vertex
		geometry.indices.push_back(i);          // Current vertex
		geometry.indices.push_back(i + 1);      // Next vertex
	}
	// Connect the last vertex to the first vertex
	geometry.indices.push_back(0);
	geometry.indices.push_back(numSegments);
	geometry.indices.push_back(1);

	geometry.vertexNumber = geometry.vertices.size();
	geometry.indiceNumber = geometry.indices.size();
}

void GeometryGenerator::GeneratePlane(Geometry& geometry, float width, float height, float thickness)
{
	geometry.vertexNumber = 4;
	geometry.indiceNumber = 6;

	geometry.vertices.resize(geometry.vertexNumber);
	geometry.indices.resize(geometry.indiceNumber);

	// Define the plane vertices
	geometry.vertices[0].pos = { -width / 2, -height / 2, 0.0f };
	geometry.vertices[1].pos = { width / 2, -height / 2, 0.0f };
	geometry.vertices[2].pos = { width / 2, height / 2, 0.0f };
	geometry.vertices[3].pos = { -width / 2, height / 2, 0.0f };

	// Define the plane indices
	geometry.indices[0] = 0;
	geometry.indices[1] = 1;
	geometry.indices[2] = 2;
	geometry.indices[3] = 0;
	geometry.indices[4] = 2;
	geometry.indices[5] = 3;

	// Define the plane UVs
	geometry.vertices[0].uv = { 0.0f, 1.0f };
	geometry.vertices[1].uv = { 1.0f, 1.0f };
	geometry.vertices[2].uv = { 1.0f, 0.0f };
	geometry.vertices[3].uv = { 0.0f, 0.0f };

	// Define the plane normals
	geometry.vertices[0].normal = { 0.0f, 0.0f, 1.0f };
	geometry.vertices[1].normal = { 0.0f, 0.0f, 1.0f };
	geometry.vertices[2].normal = { 0.0f, 0.0f, 1.0f };
	geometry.vertices[3].normal = { 0.0f, 0.0f, 1.0f };

	// Define the plane tangents
	geometry.vertices[0].tangent = { 1.0f, 0.0f, 0.0f };
	geometry.vertices[1].tangent = { 1.0f, 0.0f, 0.0f };
	geometry.vertices[2].tangent = { 1.0f, 0.0f, 0.0f };
	geometry.vertices[3].tangent = { 1.0f, 0.0f, 0.0f };

	// Define the plane binormals
	geometry.vertices[0].binormal = { 0.0f, 1.0f, 0.0f };
	geometry.vertices[1].binormal = { 0.0f, 1.0f, 0.0f };
	geometry.vertices[2].binormal = { 0.0f, 1.0f, 0.0f };
	geometry.vertices[3].binormal = { 0.0f, 1.0f, 0.0f };

	for (size_t i = 0; i < geometry.vertexNumber; i++)
	{
		geometry.vertices[i].color = { ToolBox::RandomFloat(0.0f, 1.0f), ToolBox::RandomFloat(0.0f, 1.0f), ToolBox::RandomFloat(0.0f, 1.0f), 1.0f };
	}

	geometry.vertexNumber = geometry.vertices.size();
	geometry.indiceNumber = geometry.indices.size();
}

Geometry::Geometry()
{
	vertexNumber = 0;
	indiceNumber = 0;

	vertices.clear();
	indices.clear();
}
