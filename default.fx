cbuffer cbPerObject
{
	float4x4 gMVP;
};

struct VData{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
	float3 tangent : TANGENT;
};

struct PData{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

PData VS(VData vertexIn)
{
	PData vertexOut;

	vertexOut.position = mul(float4(vertexIn.position, 1.0f), gMVP);
	vertexOut.color = float4(1.0, 0.0, 0.0, 1.0);
	return vertexOut;
}

float4 PS(PData pixelIn) : SV_target
{
	return pixelIn.color;
}

technique11 DefaultTech {
	pass p0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}