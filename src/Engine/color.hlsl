cbuffer cbPerObject : register(b0)
{
    float4x4 gWorldViewProj;
};

struct VertexIn
{
    float3 position : POSITION;
    float4 color : COLOR;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float4 Color : COLOR;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;
    // Transform to homogeneous clip space.
    vout.PosH = mul(float4(vin.position, 1.0f), gWorldViewProj);
    // Just pass vertex color and texture coordinates into the pixel shader.
    vout.Color = vin.color;
    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    return pin.Color; // Utiliser la couleur passee depuis le vertex shader
}
