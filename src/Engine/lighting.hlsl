//lighting.hlsl

// Constantes du buffer de transformation
cbuffer TransformBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
    matrix WorldViewProj;
    float4 Time; // x = temps total, y = delta-temps, z,w = inutilises
};

// Structure pour les lumieres
struct Light
{
    float4 position; // w=0 pour directionnelle, w=1 pour ponctuelle/spot
    float4 direction; // utilise pour les lumieres directionnelles et spot
    float4 color; // rgb = couleur, a = intensite
    float4 params; // x = rayon, y = falloff, z = spotAngle, w = type
};

// Constantes du buffer de lumiere 
cbuffer LightBuffer : register(b1)
{
    float4 ambientLight; // rgb = couleur ambiante, a = intensite
    float4 eyePosition; // Position de la camera pour les calculs speculaires
    Light lights[8]; // Tableau des lumieres
    uint lightCount; // Nombre de lumieres actives
    float3 padding;
};

// Texture et sampler
Texture2D diffuseTexture : register(t0);
SamplerState textureSampler : register(s0);

// Entree du vertex shader
struct VS_INPUT
{
    float3 position : POSITION;
    float4 color : COLOR;
    float2 texCoord : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
};

// Sortie du vertex shader = entree du pixel shader
struct PS_INPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 texCoord : TEXCOORD0;
    float3 normal : NORMAL;
    float3 worldPos : TEXCOORD1;
    float3 viewDir : TEXCOORD2;
};

// Parametres du materiau (pourrait être ajoute a un constant buffer separe)
static const float3 materialDiffuse = float3(1.2, 1.2, 1.2);
static const float3 materialSpecular = float3(1.0, 1.0, 1.0);
static const float materialShininess = 16.0;

// Vertex Shader
PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
    
    // Transformation de la position
    output.position = mul(float4(input.position, 1.0f), WorldViewProj);
    
    // Position dans le monde pour les calculs d'eclairage
    output.worldPos = mul(float4(input.position, 1.0f), World).xyz;
    
    // Direction de vue (de la position du sommet vers la camera)
    output.viewDir = normalize(eyePosition.xyz - output.worldPos);
    
    // Transformation de la normale (espace monde)
    output.normal = normalize(mul(float4(input.normal, 0.0f), World).xyz);
    
    // Transfert des attributs de couleur et de texture
    output.color = input.color;
    output.texCoord = input.texCoord;
    
    return output;
}

// Function to calculate directional light contribution
float3 CalcDirectionalLight(Light light, float3 normal, float3 viewDir)
{
    float3 lightDir = normalize(-light.direction.xyz);
    
    // Diffuse
    float diffuseFactor = max(dot(normal, lightDir), 0.0);
    float3 diffuse = diffuseFactor * light.color.rgb * materialDiffuse;
    
    // Specular (Blinn-Phong)
    float3 halfwayDir = normalize(lightDir + viewDir);
    float specularFactor = pow(max(dot(normal, halfwayDir), 0.0), materialShininess);
    float3 specular = specularFactor * light.color.rgb * materialSpecular;
    
    return (diffuse + specular) * light.color.a;
}

// Function to calculate point light contribution
float3 CalcPointLight(Light light, float3 normal, float3 fragPos, float3 viewDir)
{
    float3 lightDir = normalize(light.position.xyz - fragPos);
    
    // Diffuse
    float diffuseFactor = max(dot(normal, lightDir), 0.0);
    float3 diffuse = diffuseFactor * light.color.rgb * materialDiffuse;
    
    // Specular (Blinn-Phong)
    float3 halfwayDir = normalize(lightDir + viewDir);
    float specularFactor = pow(max(dot(normal, halfwayDir), 0.0), materialShininess);
    float3 specular = specularFactor * light.color.rgb * materialSpecular;
    
    // Attenuation
    float distance = length(light.position.xyz - fragPos);
    float attenuation = 1.0 / (1.0 + 0.5 * light.params.y * (distance / light.params.x) +
                 0.25 * light.params.y * (distance / light.params.x) * (distance / light.params.x));
    
    return (diffuse + specular) * attenuation * light.color.a;
}

// Function to calculate spot light contribution
float3 CalcSpotLight(Light light, float3 normal, float3 fragPos, float3 viewDir)
{
    float3 lightDir = normalize(light.position.xyz - fragPos);
    
    // Diffuse
    float diffuseFactor = max(dot(normal, lightDir), 0.0);
    float3 diffuse = diffuseFactor * light.color.rgb * materialDiffuse;
    
    // Specular (Blinn-Phong)
    float3 halfwayDir = normalize(lightDir + viewDir);
    float specularFactor = pow(max(dot(normal, halfwayDir), 0.0), materialShininess);
    float3 specular = specularFactor * light.color.rgb * materialSpecular;
    
    // Attenuation
    float distance = length(light.position.xyz - fragPos);
    float attenuation = 1.0 / (1.0 + 0.5 * light.params.y * (distance / light.params.x) +
                 0.25 * light.params.y * (distance / light.params.x) * (distance / light.params.x));
    
    // Spot effect
    float spotFactor = dot(normalize(-light.direction.xyz), lightDir);
    float spotCutoff = cos(light.params.z);
    float epsilon = 0.1;
    float intensity = clamp((spotFactor - spotCutoff) / epsilon, 0.0, 1.0);
    
    return (diffuse + specular) * attenuation * intensity * light.color.a;
}

// Pixel Shader
float4 PS(PS_INPUT input) : SV_TARGET
{
    // Normaliser les vecteurs
    float3 normal = normalize(input.normal);
    float3 viewDir = normalize(input.viewDir);
    
    //float4 texColor = diffuseTexture.Sample(textureSampler, input.texCoord);
    float4 texColor = input.color; // Utiliser la couleur du vertex
    
    // Lumiere ambiante
    float3 result = ambientLight.rgb * ambientLight.a * texColor.rgb * materialDiffuse;
    
    // Calcul de la contribution de chaque lumiere active
    for (uint i = 0; i < lightCount; i++)
    {
        if (lights[i].params.w == 0)
        { // Directional light
            result += CalcDirectionalLight(lights[i], normal, viewDir) * texColor.rgb;
        }
        else if (lights[i].params.w == 1)
        { // Point light
            result += CalcPointLight(lights[i], normal, input.worldPos, viewDir) * texColor.rgb;
        }
        else if (lights[i].params.w == 2)
        { // Spot light
            result += CalcSpotLight(lights[i], normal, input.worldPos, viewDir) * texColor.rgb;
        }
    }
    
    // Retourner la couleur calculee
    return float4(result, 1.0); // Conserver l'alpha de la texture
}
