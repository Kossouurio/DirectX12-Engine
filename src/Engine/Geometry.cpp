#include "pch.h"

Geometry GeometryGenerator::GenerateCube(float size)
{
	Geometry geometry;

	geometry.vertexNumber = 24;
	geometry.indiceNumber = 36;

	geometry.vertices.resize(geometry.vertexNumber);
	geometry.indices.resize(geometry.indiceNumber);

    XMFLOAT3 positions[24] = {
        // Front face
        { -size/2, -size/2, -size/2 },
        {  size/2, -size/2, -size/2 },
        {  size/2,  size/2, -size/2 },
        { -size/2,  size/2, -size/2 },

        // Right face
        {  size/2, -size/2, -size/2 },
        {  size/2, -size/2,  size/2 },
        {  size/2,  size/2,  size/2 },
        {  size/2,  size/2, -size/2 },

        // Back face
        {  size/2, -size/2,  size/2 },
        { -size/2, -size/2,  size/2 },
        { -size/2,  size/2,  size/2 },
        {  size/2,  size/2,  size/2 },

        // Left face
        { -size/2, -size/2,  size/2 },
        { -size/2, -size/2, -size/2 },
        { -size/2,  size/2, -size/2 },
        { -size/2,  size/2,  size/2 },

        // Top face
        { -size/2,  size/2, -size/2 },
        {  size/2,  size/2, -size/2 },
        {  size/2,  size/2,  size/2 },
        { -size/2,  size/2,  size/2 },

        // Bottom face
        { -size/2, -size/2,  size/2 },
        {  size/2, -size/2,  size/2 },
        {  size/2, -size/2, -size/2 },
        { -size/2, -size/2, -size/2 }
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

	for (size_t i = 0; i < geometry.vertexNumber; i++)
	{
		float r = (positions[i].x + size / 2) / size;  
		float g = (positions[i].y + size / 2) / size;  
		float b = (positions[i].z + size / 2) / size;  

		geometry.vertices[i].pos = positions[i];
		geometry.vertices[i].color = { r, g, b, 1.0f }; 
		geometry.vertices[i].uv = cubeUVs[i];
		geometry.vertices[i].normal = cubeNormals[i];
		geometry.vertices[i].tangent = { 1.0f, 0.0f, 0.0f };
		geometry.vertices[i].binormal = { 0.0f, 1.0f, 0.0f };
	}

	for (size_t i = 0; i < geometry.indiceNumber; i++)
	{
		geometry.indices[i] = cubeIndices[i];
	}

	for (size_t i = 0; i < geometry.indices.size(); i += 3) {
		std::swap(geometry.indices[i], geometry.indices[i + 2]);
	}

	return geometry;
}

Geometry GeometryGenerator::GenerateSphere(float radius, int numSegments)
{
	Geometry geometry;

	geometry.vertexNumber = (numSegments + 1) * (numSegments + 1);
	geometry.indiceNumber = numSegments * numSegments * 6;

	geometry.vertices.resize(geometry.vertexNumber);
	geometry.indices.resize(geometry.indiceNumber);

	for (int i = 0; i <= numSegments; i++) {
		float phi = XM_PI * i / numSegments;
		for (int j = 0; j <= numSegments; j++) {
			float theta = XM_2PI * j / numSegments;
			float x = radius * sinf(phi) * cosf(theta);
			float y = radius * cosf(phi);
			float z = radius * sinf(phi) * sinf(theta);

			int index = i * (numSegments + 1) + j;

			float r = (x + radius) / (2 * radius);
			float g = (y + radius) / (2 * radius);
			float b = (z + radius) / (2 * radius);

			geometry.vertices[index].pos = { x, y, z };
			geometry.vertices[index].color = { r, g, b, 1.0f };
			geometry.vertices[index].uv = { (float)j / numSegments, 1.0f - (float)i / numSegments };

			XMVECTOR normal = XMVector3Normalize(XMLoadFloat3(&geometry.vertices[index].pos));
			XMStoreFloat3(&geometry.vertices[index].normal, normal);
		}
	}

	int idx = 0;
	for (int i = 0; i < numSegments; i++) {
		for (int j = 0; j < numSegments; j++) {
			int index1 = i * (numSegments + 1) + j;
			int index2 = index1 + numSegments + 1;
			int index3 = index1 + 1;
			int index4 = index2 + 1;

			geometry.indices[idx++] = index1;
			geometry.indices[idx++] = index3;
			geometry.indices[idx++] = index2;

			geometry.indices[idx++] = index3;
			geometry.indices[idx++] = index4;
			geometry.indices[idx++] = index2;
		}
	}

	return geometry;
}



Geometry GeometryGenerator::GenerateCircle(float radius, int numSegments)
{
	Geometry geometry;

	float angleStep = XM_2PI / numSegments;
	float angle = 0.0f;

	geometry.vertices.push_back({ {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f}, {0.0f, 0.0f, 1.0f} });

	for (int i = 0; i < numSegments; i++) {
		float x = radius * cosf(angle);
		float y = radius * sinf(angle);

		float r = (x + radius) / (2 * radius);
		float g = (y + radius) / (2 * radius);
		float b = 0.5f;

		geometry.vertices.push_back({ {x, y, 0.0f}, {r, g, b, 1.0f}, {x / (2 * radius) + 0.5f, y / (2 * radius) + 0.5f}, {0.0f, 0.0f, 1.0f} });

		angle += angleStep;
	}

	for (int i = 1; i < numSegments; i++) {
		geometry.indices.push_back(0);
		geometry.indices.push_back(i + 1);
		geometry.indices.push_back(i);
	}
	geometry.indices.push_back(0);
	geometry.indices.push_back(1);
	geometry.indices.push_back(numSegments);

	geometry.vertexNumber = geometry.vertices.size();
	geometry.indiceNumber = geometry.indices.size();

	return geometry;
}





Geometry GeometryGenerator::GeneratePlane(float width, float height)
{
	Geometry geometry;

	geometry.vertices = {
        {{ -width / 2, 0.0f, -height / 2 }, {0, 0, 0, 1}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{  width / 2, 0.0f, -height / 2 }, {0, 0, 0, 1}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{  width / 2, 0.0f,  height / 2 }, {0, 0, 0, 1}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{ -width / 2, 0.0f,  height / 2 }, {0, 0, 0, 1}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}}
    };
    
	for (auto& vertex : geometry.vertices)
	{
		float r = (vertex.pos.x + width / 2) / width;
		float g = (vertex.pos.y + height / 2) / height;
		float b = (vertex.pos.z + 1) / 2;
		vertex.color = { r, g, b, 1.0f };
	}
    
    geometry.indices = { 0, 1, 2, 0, 2, 3 };
    geometry.vertexNumber = geometry.vertices.size();
    geometry.indiceNumber = geometry.indices.size();

	return geometry;
}



Geometry GeometryGenerator::GenerateCylinder(float radius, float height, int numSegments)
{
	Geometry geometry;

	geometry.vertexNumber = (numSegments + 1) * 2 + 2;
	geometry.indiceNumber = numSegments * 6 + numSegments * 6;

	geometry.vertices.resize(geometry.vertexNumber);
	geometry.indices.resize(geometry.indiceNumber);

	for (int i = 0; i <= numSegments; i++) {
		float angle = XM_2PI * i / numSegments;
		float x = radius * cosf(angle);
		float z = radius * sinf(angle);

		float r = (x + radius) / (2 * radius);
		float g = (z + radius) / (2 * radius);
		float b = 0.5f;

		XMFLOAT3 normal = { x / radius, 0.0f, z / radius };

		geometry.vertices[i].pos = { x, -height / 2, z };
		geometry.vertices[i].color = { r, g, b, 1.0f };
		geometry.vertices[i].normal = normal;
		geometry.vertices[i].uv = { (float)i / numSegments, 1.0f };

		geometry.vertices[i + numSegments + 1].pos = { x, height / 2, z };
		geometry.vertices[i + numSegments + 1].color = { r, g, b, 1.0f };
		geometry.vertices[i + numSegments + 1].normal = normal;
		geometry.vertices[i + numSegments + 1].uv = { (float)i / numSegments, 0.0f };
	}

	int centerBottomIndex = (numSegments + 1) * 2;
	int centerTopIndex = centerBottomIndex + 1;

	geometry.vertices[centerBottomIndex].pos = { 0.0f, -height / 2, 0.0f };
	geometry.vertices[centerBottomIndex].color = { 0.5f, 0.5f, 0.5f, 1.0f };
	geometry.vertices[centerBottomIndex].normal = { 0.0f, -1.0f, 0.0f };

	geometry.vertices[centerTopIndex].pos = { 0.0f, height / 2, 0.0f };
	geometry.vertices[centerTopIndex].color = { 0.5f, 0.5f, 0.5f, 1.0f };
	geometry.vertices[centerTopIndex].normal = { 0.0f, 1.0f, 0.0f };

	int idx = 0;
	for (int i = 0; i < numSegments; i++) {
		int base1 = i;
		int base2 = i + 1;
		int top1 = i + numSegments + 1;
		int top2 = i + numSegments + 2;

		geometry.indices[idx++] = base1;
		geometry.indices[idx++] = top1;
		geometry.indices[idx++] = base2;

		geometry.indices[idx++] = base2;
		geometry.indices[idx++] = top1;
		geometry.indices[idx++] = top2;
	}

	for (int i = 0; i < numSegments; i++) {
		int next = (i + 1) % numSegments;

		geometry.indices[idx++] = centerBottomIndex;
		geometry.indices[idx++] = i;
		geometry.indices[idx++] = next;

		geometry.indices[idx++] = centerTopIndex;
		geometry.indices[idx++] = next + numSegments + 1;
		geometry.indices[idx++] = i + numSegments + 1;
	}

	geometry.vertexNumber = geometry.vertices.size();
	geometry.indiceNumber = geometry.indices.size();

	return geometry;
}

Geometry GeometryGenerator::GenerateCone(float radius, float height, int numSegments)
{
	Geometry geometry;

	geometry.vertexNumber = (numSegments + 1) + 2;
	geometry.indiceNumber = numSegments * 3 + numSegments * 3;
	geometry.vertices.resize(geometry.vertexNumber);
	geometry.indices.resize(geometry.indiceNumber);
	for (int i = 0; i <= numSegments; i++) {
		float angle = XM_2PI * i / numSegments;
		float x = radius * cosf(angle);
		float z = radius * sinf(angle);
		float r = (x + radius) / (2 * radius);
		float g = (z + radius) / (2 * radius);
		float b = 0.5f;
		XMFLOAT3 normal = { x / radius, 0.0f, z / radius };
		geometry.vertices[i].pos = { x, -height / 2, z };
		geometry.vertices[i].color = { r, g, b, 1.0f };
		geometry.vertices[i].normal = normal;
		geometry.vertices[i].uv = { (float)i / numSegments, 1.0f };
	}
	int centerIndex = numSegments + 1;
	int topIndex = centerIndex + 1;
	geometry.vertices[centerIndex].pos = { 0.0f, height / 2, 0.0f };
	geometry.vertices[centerIndex].color = { 0.5f, 0.5f, 0.5f, 1.0f };
	geometry.vertices[centerIndex].normal = { 0.0f, 1.0f, 0.0f };
	geometry.vertices[topIndex].pos = { 0.0f, -height / 2, 0.0f };
	geometry.vertices[topIndex].color = { 0.5f, 0.5f, 0.5f, 1.0f };
	geometry.vertices[topIndex].normal = { 0.0f, -1.0f, 0.0f };
	int idx = 0;
	for (int i = 0; i < numSegments; i++) {
		int base1 = i;
		int base2 = i + 1;
		geometry.indices[idx++] = base1;
		geometry.indices[idx++] = centerIndex;
		geometry.indices[idx++] = base2;
		geometry.indices[idx++] = base1;
		geometry.indices[idx++] = base2;
		geometry.indices[idx++] = topIndex;
	}

	geometry.vertexNumber = geometry.vertices.size();
	geometry.indiceNumber = geometry.indices.size();

	return geometry;
}

Geometry GeometryGenerator::GenerateTorus(float radius, float thickness, int numSegments, int numSides)
{
	Geometry geometry;

	geometry.vertexNumber = (numSegments + 1) * (numSides + 1);
	geometry.indiceNumber = numSegments * numSides * 6;

	geometry.vertices.resize(geometry.vertexNumber);
	geometry.indices.resize(geometry.indiceNumber);

	float segmentStep = XM_2PI / numSegments;
	float sideStep = XM_2PI / numSides;

	for (int i = 0; i <= numSegments; ++i) {
		float segmentAngle = i * segmentStep;
		XMFLOAT3 segmentCenter = { radius * cosf(segmentAngle), radius * sinf(segmentAngle), 0.0f };

		for (int j = 0; j <= numSides; ++j) {
			float sideAngle = j * sideStep;
			float x = (radius + thickness * cosf(sideAngle)) * cosf(segmentAngle);
			float y = (radius + thickness * cosf(sideAngle)) * sinf(segmentAngle);
			float z = thickness * sinf(sideAngle);

			int index = i * (numSides + 1) + j;

			float r = (x + radius + thickness) / (2 * (radius + thickness));
			float g = (y + radius + thickness) / (2 * (radius + thickness));
			float b = (z + thickness) / (2 * thickness);

			geometry.vertices[index].pos = { x, y, z };
			geometry.vertices[index].color = { r, g, b, 1.0f };
			geometry.vertices[index].uv = { (float)j / numSides, (float)i / numSegments };

			XMVECTOR normal = XMVector3Normalize(XMLoadFloat3(&geometry.vertices[index].pos) - XMLoadFloat3(&segmentCenter));
			XMStoreFloat3(&geometry.vertices[index].normal, normal);
		}
	}

	int idx = 0;
	for (int i = 0; i < numSegments; ++i) {
		for (int j = 0; j < numSides; ++j) {
			int index1 = i * (numSides + 1) + j;
			int index2 = index1 + numSides + 1;
			int index3 = index1 + 1;
			int index4 = index2 + 1;

			geometry.indices[idx++] = index1;
			geometry.indices[idx++] = index2;
			geometry.indices[idx++] = index3;

			geometry.indices[idx++] = index3;
			geometry.indices[idx++] = index2;
			geometry.indices[idx++] = index4;
		}
	}

	return geometry;
}


Geometry GeometryGenerator::GeneratePyramid(float size)
{
	Geometry geometry;

	// Definir les sommets de la pyramide
	geometry.vertexNumber = 5;
	geometry.indiceNumber = 18;

	geometry.vertices.resize(geometry.vertexNumber);
	geometry.indices.resize(geometry.indiceNumber);

	float halfSize = size / 2.0f;

	// Sommets de la base carree
	XMFLOAT3 positions[5] = {
		{ -halfSize, 0.0f, -halfSize }, // 0: bas-gauche
		{  halfSize, 0.0f, -halfSize }, // 1: bas-droite
		{  halfSize, 0.0f,  halfSize }, // 2: haut-droite
		{ -halfSize, 0.0f,  halfSize }, // 3: haut-gauche
		{  0.0f,    size,  0.0f }       // 4: sommet
	};

	// Definir les couleurs, UVs et normales pour chaque sommet
	for (size_t i = 0; i < geometry.vertexNumber; i++)
	{
		float r = (positions[i].x + halfSize) / size;
		float g = (positions[i].y + size) / size;
		float b = (positions[i].z + halfSize) / size;

		geometry.vertices[i].pos = positions[i];
		geometry.vertices[i].color = { r, g, b, 1.0f };
		geometry.vertices[i].uv = { (positions[i].x + halfSize) / size, (positions[i].z + halfSize) / size };
		geometry.vertices[i].normal = { 0.0f, 1.0f, 0.0f }; // Normale par defaut, sera ajustee plus tard
	}

	// Definir les indices pour les triangles de la base (ordre antihoraire)
	uint16_t baseIndices[6] = {
		0, 1, 2,
		0, 2, 3
	};

	// Definir les indices pour les triangles des faces laterales (ordre antihoraire)
	uint16_t sideIndices[12] = {
		0, 4, 1,
		1, 4, 2,
		2, 4, 3,
		3, 4, 0
	};

	// Copier les indices dans le tableau d'indices de la geometrie
	std::copy(baseIndices, baseIndices + 6, geometry.indices.begin());
	std::copy(sideIndices, sideIndices + 12, geometry.indices.begin() + 6);

	// Calculer les normales pour chaque face
	for (size_t i = 0; i < geometry.indices.size(); i += 3)
	{
		XMFLOAT3& v0 = geometry.vertices[geometry.indices[i]].pos;
		XMFLOAT3& v1 = geometry.vertices[geometry.indices[i + 1]].pos;
		XMFLOAT3& v2 = geometry.vertices[geometry.indices[i + 2]].pos;

		XMVECTOR edge1 = XMVectorSubtract(XMLoadFloat3(&v1), XMLoadFloat3(&v0));
		XMVECTOR edge2 = XMVectorSubtract(XMLoadFloat3(&v2), XMLoadFloat3(&v0));
		XMVECTOR normal = XMVector3Normalize(XMVector3Cross(edge1, edge2));

		XMStoreFloat3(&geometry.vertices[geometry.indices[i]].normal, normal);
		XMStoreFloat3(&geometry.vertices[geometry.indices[i + 1]].normal, normal);
		XMStoreFloat3(&geometry.vertices[geometry.indices[i + 2]].normal, normal);
	}

	return geometry;
}

Geometry GeometryGenerator::GenerateSkybox(float size)
{
	Geometry geometry;
	float numSegments = 20;
	float radius = size;

	geometry.vertexNumber = (numSegments + 1) * (numSegments + 1);
	geometry.indiceNumber = numSegments * numSegments * 6;

	geometry.vertices.resize(geometry.vertexNumber);
	geometry.indices.resize(geometry.indiceNumber);

	for (int i = 0; i <= numSegments; i++) {
		float phi = XM_PI * i / numSegments;
		for (int j = 0; j <= numSegments; j++) {
			float theta = XM_2PI * j / numSegments;
			float x = radius * sinf(phi) * cosf(theta);
			float y = radius * cosf(phi);
			float z = radius * sinf(phi) * sinf(theta);

			int index = i * (numSegments + 1) + j;

			float r = (x + radius) / (2 * radius);
			float g = (y + radius) / (2 * radius);
			float b = (z + radius) / (2 * radius);

			geometry.vertices[index].pos = { x, y, z };
			geometry.vertices[index].color = { r, g, b, 1.0f };
			geometry.vertices[index].uv = { (float)j / numSegments, 1.0f - (float)i / numSegments };

			XMVECTOR normal = XMVector3Normalize(XMLoadFloat3(&geometry.vertices[index].pos));
			XMStoreFloat3(&geometry.vertices[index].normal, normal);
		}
	}

	int idx = 0;
	for (int i = 0; i < numSegments; i++) {
		for (int j = 0; j < numSegments; j++) {
			int index1 = i * (numSegments + 1) + j;
			int index2 = index1 + numSegments + 1;
			int index3 = index1 + 1;
			int index4 = index2 + 1;

			geometry.indices[idx++] = index1;
			geometry.indices[idx++] = index3;
			geometry.indices[idx++] = index2;

			geometry.indices[idx++] = index3;
			geometry.indices[idx++] = index4;
			geometry.indices[idx++] = index2;
		}
	}
	
	// Inverser l'ordre des indices pour chaque face
	for (size_t i = 0; i < geometry.indices.size(); i += 3) {
		std::swap(geometry.indices[i + 1], geometry.indices[i + 2]);
	}

	return geometry;
}

Geometry GeometryGenerator::GenerateGeometryFromFile(const std::string& filePath)
{
	std::ifstream file(filePath); // Ouvrir le fichier 
	if (!file)
	{
		throw std::runtime_error("Impossible d'ouvrir le fichier " + filePath);
	}

	Geometry geometry;

	UINT vcount = 0;
	UINT tcount = 0;
	std::string ignore;

	file >> ignore >> vcount; // Lire le nombre de sommets
	file >> ignore >> tcount; // Lire le nombre de triangles
	file >> ignore >> ignore >> ignore >> ignore; // Lire les lignes restantes

	std::vector<Vertex> vertices(vcount);
	for (UINT i = 0; i < vcount; ++i)
	{
		file >> vertices[i].pos.x >> vertices[i].pos.y >> vertices[i].pos.z;
		file >> vertices[i].normal.x >> vertices[i].normal.y >> vertices[i].normal.z;

		// Model does not have texture coordinates, so just zero them out.
		vertices[i].uv = { 0.0f, 0.0f };
	}

	file >> ignore;
	file >> ignore;
	file >> ignore;

	std::vector<std::uint16_t> indices(3 * tcount);
	for (UINT i = 0; i < tcount; ++i)
	{
		file >> indices[i * 3 + 0] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
	}

	file.close();

	geometry.vertexNumber = vcount;
	geometry.indiceNumber = 3 * tcount;

	geometry.vertices = vertices;
	geometry.indices = indices;

	return geometry;

}



Geometry::Geometry()
{
	vertexNumber = 0;
	indiceNumber = 0;

	vertices.clear();
	indices.clear();
}
