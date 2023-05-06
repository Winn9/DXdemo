Texture2D g_Texture1uv : register(t0);
Texture2D g_Texture2uv : register(t1);
Texture2D g_Texture3uv : register(t2);
Texture2D g_Texture4uv : register(t3);
Texture2D g_Texture5uv : register(t4);
Texture2D g_Texture6uv : register(t5);
SamplerState g_SamLinear : register(s0);

static const float PI = 3.14159265358979323846;
static const float HalfPI = 1.57079632679489661923;

struct VertexIn
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct VertexOut
{
    float4 posH : SV_POSITION;
    float2 uv : TEXCOORD;
};


