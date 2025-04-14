struct VS_INPUT
{
    float4 pos : POSITION;
    float2 texCoord : TEXCOORD;
    float4 color : COLOR; // Ajout de la couleur en entree
};

//struct VS_OUTPUT
//{
//    float4 pos : SV_POSITION;
//    float2 texCoord : TEXCOORD;
//    float4 color : COLOR; // Passer la couleur au pixel shader
//};

//cbuffer ConstantBuffer : register(b0)
//{
//    float4x4 worldMatrix; // Matrice de transformation monde
//    float4x4 wvpMat; // Matrice World-View-Projection
//};

//VS_OUTPUT main(VS_INPUT input)
//{
//    VS_OUTPUT output;

//    // Transformer la position en espace monde
//    float4 worldPos = mul(input.pos, worldMatrix);

    // Transformer en espace ecran
    output.pos = mul(worldPos, wvpMat);
    output.texCoord = input.texCoord;

    // Calculer la couleur en fonction de la position monde (et non locale)
    output.color = float4(
        (worldPos.x + 1.0f) * 0.5f, // Rouge base sur X monde
        (worldPos.y + 1.0f) * 0.5f, // Vert base sur Y monde
        (worldPos.z + 1.0f) * 0.5f, // Bleu base sur Z monde
        1.0f
    );

//    return output;
//}
