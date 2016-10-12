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
	float3 color : COLOR;
	uint useVertexColor : USE_VERTEX_COLOR;
};

struct PData
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

PData main(VData vertexIn) {
	PData vertexOut;

	vertexOut.position = mul(float4(vertexIn.position, 1.0f), gMVP);

	if(vertexIn.useVertexColor)
		vertexOut.color = float4(vertexIn.color, 1.0f);
	else
		vertexOut.color.rgb = vertexIn.position + float3(0.5f, 0.5f, 0.5f);
	return vertexOut;
}