#pragma once



struct Vertex
{
	XMFLOAT3 pos;
	XMFLOAT4 color;
	XMFLOAT2 uv;
	XMFLOAT3 normal;
	XMFLOAT3 tangent;
	XMFLOAT3 binormal;
};

struct Geometry
{
public:
	Geometry();
	virtual ~Geometry() {}

	std::size_t vertexNumber;
	std::size_t indiceNumber;

	std::vector<Vertex> vertices;
	std::vector<std::uint16_t> indices;
};

class GeometryGenerator
{
public:
	static Geometry GenerateCube(float size);
	static Geometry GenerateSphere(float radius, int numSegments);
	static Geometry GenerateCircle(float radius, int numSegments);
	static Geometry GeneratePlane(float width, float height);
	static Geometry GenerateCylinder(float radius, float height, int numSegments);
	static Geometry GenerateCone(float radius, float height, int numSegments);
	static Geometry GenerateTorus(float radius, float thickness, int numSegments, int numSides);
	static Geometry GeneratePyramid(float size);
	static Geometry GenerateSkybox(float size);

	// genarate a geometry with a given file path
	static Geometry GenerateGeometryFromFile(const std::string& filePath);
};