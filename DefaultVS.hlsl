cbuffer cbPerObject
{
	float4x4 gMVP;
};

struct VData
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
	float3 tangent : TANGENT;
};

struct PData
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

PData main(VData vertexIn) {
	PData vertexOut;

	vertexOut.position = mul(float4(vertexIn.position, 1.0f), gMVP);
	vertexOut.color = float4(1.0, 0.0, 0.0, 1.0);
	return vertexOut;
}