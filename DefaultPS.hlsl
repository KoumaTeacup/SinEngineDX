struct PData
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

float4 main(PData pixelIn) : SV_target
{
	return pixelIn.color;
}