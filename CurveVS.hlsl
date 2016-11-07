cbuffer cbPerObject
{
	float4x4 gMVP;
};

struct VData
{
	float3 position : POSITION;
};

struct PData
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

PData main(VData vertexIn) {
	PData vertexOut;

	vertexOut.position = mul(float4(vertexIn.position, 1.0f), gMVP);
	vertexOut.color = float4(1.0f, 0.0f, 0.0f, 1.0f);
	return vertexOut;
}