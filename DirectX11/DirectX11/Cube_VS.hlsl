#include "Cube.hlsli"

VertexOut VS(VertexIn vIn)
{
	VertexOut vOut;
	vOut.posH = mul(float4(vIn.posL, 1.0f), gWorld);  // mul ���Ǿ���˷�, �����*Ҫ���������Ϊ
	vOut.posH = mul(vOut.posH, gView);               // ��������ȵ��������󣬽��Ϊ
	vOut.posH = mul(vOut.posH, gProj);               // Cij = Aij * Bij
	vOut.color = vIn.color;                         // ����alphaͨ����ֵĬ��Ϊ1.0
	return vOut;
}