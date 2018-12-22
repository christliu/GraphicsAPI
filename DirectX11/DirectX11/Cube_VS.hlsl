#include "Cube.hlsli"

VertexOut VS(VertexIn vIn)
{
	VertexOut vOut;
	vOut.posH = mul(float4(vIn.posL, 1.0f), gWorld);  // mul 才是矩阵乘法, 运算符*要求操作对象为
	vOut.posH = mul(vOut.posH, gView);               // 行列数相等的两个矩阵，结果为
	vOut.posH = mul(vOut.posH, gProj);               // Cij = Aij * Bij
	vOut.color = vIn.color;                         // 这里alpha通道的值默认为1.0
	return vOut;
}