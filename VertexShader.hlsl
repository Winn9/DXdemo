#include "Header.hlsli"


// ¶¥µã×ÅÉ«Æ÷
VertexOut VS(VertexIn vIn)
{
    VertexOut vOut;
    vOut.posH = float4(vIn.pos, 1.0f);
    vOut.uv = vIn.uv;
    return vOut;
}
