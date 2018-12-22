
cbuffer ConstantBuffer : register(b0)
{
	matrix gWorld; // matrix������float4x4���������row_major������£�����Ĭ��Ϊ��������
	matrix gView;  // ������ǰ�����row_major��ʾ��������
	matrix gProj;  // �ý̳�����ʹ��Ĭ�ϵ��������󣬵���Ҫ��C++�����Ԥ�Ƚ��������ת�á�
}


struct VertexIn
{
	float3 posL : POSITION;
	float4 color : COLOR;
};

struct VertexOut
{
	float4 posH : SV_POSITION;
	float4 color : COLOR;
};
