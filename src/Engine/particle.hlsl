//particle.hlsl
struct VSInput
{
    float3 position : POSITION;
    float3 velocity : VELOCITY;
    float4 color : COLOR;
    float size : SIZE;
    float life : LIFE;
};

struct VSOutput
{
    float3 position : POSITION;
    float4 color : COLOR;
    float size : SIZE;
};

struct GSOutput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 texCoord : TEXCOORD;
};

// Constant Buffer
cbuffer cbPerObject : register(b0)
{
    matrix viewProjection;
    float3 cameraPosition;
    float deltaTime;
};

// Vertex Shader
VSOutput VS(VSInput input)
{
    VSOutput output;
    output.position = input.position;
    output.color = input.color;
    output.size = input.size;
    return output;
}

// Geometry Shader - Cree les billboards
[maxvertexcount(4)]
void GS(point VSOutput input[1], inout TriangleStream<GSOutput> outputStream)
{
    float3 position = input[0].position;
    float size = input[0].size;
    
    // Calculer les vecteurs pour le billboard
    float3 up = float3(0, 1, 0);
    float3 look = cameraPosition - position;
    look = normalize(look);
    float3 right = cross(up, look);
    right = normalize(right);
    up = cross(look, right);
    
    // Creer les 4 coins du quad
    float halfSize = size * 0.5f;
    float4 v[4];
    v[0] = float4(position + halfSize * (-right - up), 1.0f); // Bottom-left
    v[1] = float4(position + halfSize * (right - up), 1.0f); // Bottom-right
    v[2] = float4(position + halfSize * (-right + up), 1.0f); // Top-left
    v[3] = float4(position + halfSize * (right + up), 1.0f); // Top-right
    
    // UV coords
    float2 texCoords[4] =
    {
        float2(0, 1),
        float2(1, 1),
        float2(0, 0),
        float2(1, 0)
    };
    
    // Output vertices
    GSOutput output;
    [unroll]
    for (int i = 0; i < 4; i++)
    {
        output.position = mul(v[i], viewProjection);
        output.color = input[0].color;
        output.texCoord = texCoords[i];
        outputStream.Append(output);
    }
}

// Pixel Shader
float4 PS(GSOutput input) : SV_TARGET
{
    // Ajouter un effet de fondu sur les bords
    float dist = length(input.texCoord - float2(0.5f, 0.5f));
    float alpha = 1.0f - smoothstep(0.0f, 0.5f, dist);
    return float4(input.color.rgb, input.color.a * alpha);
}