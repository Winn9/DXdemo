#include "Header.hlsli"

half4 CubeProject(float3 p)
{
    float ax = abs(p.x);
    float ay = abs(p.y);
    float az = abs(p.z);
        
    if (ay >= ax && ay >= az && p.y >= 0) //top
    {
        float2 face_xy_norm2 = float2(p.x / ay, -p.z / ay);
        float2 face_uv = (face_xy_norm2 + 1) / 2;
        return g_Texture1uv.Sample(g_SamLinear, face_uv);
    }
    if (az >= ax && az >= ay && p.z >= 0) //back
    {
        float2 face_xy_norm2 = float2(-p.x / az, -p.y / az);
        float2 face_uv = (face_xy_norm2 + 1) / 2;
        return g_Texture2uv.Sample(g_SamLinear, face_uv);
    }
    if (ay >= ax && ay >= az && p.y < 0) //bottom
    {
        float2 face_xy_norm2 = float2(p.x / ay, p.z / ay);
        float2 face_uv = (face_xy_norm2 + 1) / 2;
        return g_Texture3uv.Sample(g_SamLinear, face_uv);
    }
    if (ax >= ay && ax >= az && p.x < 0) //left
    {
        float2 face_xy_norm2 = float2(-p.z / ax, -p.y / ax);
        float2 face_uv = (face_xy_norm2 + 1) / 2;
        return g_Texture4uv.Sample(g_SamLinear, face_uv);
    }
    if (az >= ax && az >= ay && p.z < 0) //front
    {
        float2 face_xy_norm2 = float2(p.x / az, -p.y / az);
        float2 face_uv = (face_xy_norm2 + 1) / 2;
        return g_Texture5uv.Sample(g_SamLinear, face_uv);
    }
    if (ax >= ay && ax >= az && p.x >= 0) //right
    {
        float2 face_xy_norm2 = float2(p.z / ax, -p.y / ax);
        float2 face_uv = (face_xy_norm2 + 1) / 2;
        return g_Texture6uv.Sample(g_SamLinear, face_uv);
    }
    return half4(0, 0, 0, 1);
}

float2 ErpUVToThetaPhi(float2 uv)
{
    float theta = (2 * uv.x - 1) * PI;
    float phi = (1 - 2 * uv.y) * HalfPI;
    return float2(theta, phi);
}

float3 NormalVector(float3 i)
{
    return i / sqrt(i.x * i.x + i.y * i.y + i.z * i.z);
}

float3 ThetaPhiToVector(float theta, float phi)
{
    return NormalVector(float3(
                cos(phi) * sin(theta),
                sin(phi),
                -cos(phi) * cos(theta)
        ));
}

float4 ErpProject(float2 uv)
{
    float2 phiTheta = ErpUVToThetaPhi(uv);
    return CubeProject(ThetaPhiToVector(phiTheta.x, phiTheta.y));
}

float4 ErpProject4x(float2 uv)
{
    half4 color1 = ErpProject(float2(uv.x + 1.0 / 30720, uv.y + 1.0 / 15360));
    half4 color2 = ErpProject(float2(uv.x + 1.0 / 30720, uv.y - 1.0 / 15360));
    half4 color3 = ErpProject(float2(uv.x - 1.0 / 30720, uv.y + 1.0 / 15360));
    half4 color4 = ErpProject(float2(uv.x - 1.0 / 30720, uv.y - 1.0 / 15360));
    return (color1 + color2 + color3 + color4) / 4.0;
}


// ÏñËØ×ÅÉ«Æ÷
float4 PS(VertexOut pIn) : SV_Target
{
    float4 OutColor;
    //OutColor = ErpProject(pIn.uv);
    OutColor = ErpProject4x(pIn.uv);
    return OutColor;
}

