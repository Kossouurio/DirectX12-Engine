Texture2D gTexture : register(t0);
SamplerState gSampler : register(s0);

struct Light
{
    float4 position; // w=0 pour directionnelle, w=1 pour ponctuelle/spot
    float4 direction; // utilise pour les lumieres directionnelles et spot
    float4 color; // rgb = couleur, a = intensite
    float4 params; // x = rayon, y = falloff, z = spotAngle, w = type
};

cbuffer TransformBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
    matrix WorldViewProj;
    float4 Time; // x = temps total, y = delta-temps, z,w = inutilises
};

cbuffer LightBuffer : register(b1)
{
    float4 ambientLight; // rgb = couleur ambiante, a = intensite
    float4 eyePosition; // Position de la camera pour les calculs speculaires
    Light lights[8]; // Tableau des lumieres
    uint lightCount; // Nombre de lumieres actives
    float3 padding;
};

struct VS_INPUT
{
    float3 position : POSITION;
    float4 color : COLOR;
    float2 texCoord : TEXCOORD;
};

// Sortie du vertex shader = entree du pixel shader
struct PS_INPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 texCoord : TEXCOORD;
};

PS_INPUT VS(VS_INPUT vin)
{
    PS_INPUT vout;
    
    // Transform to World Space
    vout.position = mul(float4(vin.position, 1.0f), World);
    //vout.WorldPos = vout.PosH.xyz;
    // Transform to Camera Space
    vout.position = mul(vout.position, View);
    // Transform to homogeneous clip space.
    vout.position = mul(vout.position, Projection);
    
    // Just pass vertex color into the pixel shader.
    vout.color = vin.color;
    vout.texCoord = vin.texCoord;
    
    return vout;
}

float4 PS(PS_INPUT pin) : SV_Target
{
    float4 litColor = gTexture.Gather(gSampler, pin.texCoord);
    //litColor.rgba *= Color.rgba;
    
    //float dist = distance(pin.WorldPos, lightPos);
    //float intensity = (-1 / lightIntensity) * dist + 1;
    //litColor *= intensity * lightColor;
    
    return litColor;
}