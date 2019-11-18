//=============================================================================
//	CameraEyeReflectionLightGeometryShader.hlsl
//
//	geometry shader file of camera eye reflection light shader.
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "CameraEyeReflectionLightShaderData.hlsli"

static const float4 VertexPositions[4] = {
	float4(-1.0f, 1.0f, 0.0f, 1.0f),
	float4( 1.0f, 1.0f, 0.0f, 1.0f),
	float4(-1.0f,-1.0f, 0.0f, 1.0f),
	float4( 1.0f,-1.0f, 0.0f, 1.0f)
};

static const float2 VertexTexcoords[4] = {
	float2(0.0f, 0.0f),
	float2(1.0f, 0.0f),
	float2(0.0f, 1.0f),
	float2(1.0f, 1.0f)
};

[maxvertexcount(4)]
void main(
	point VSOutput _input[1],
	inout TriangleStream<PSInput> _output
){
	float4 viewPos = mul(_input[0].lightViewPos, worldViewMatrix);

	for(int i=0; i<4; ++i){
		PSInput element = (PSInput)0;

		element.position = VertexPositions[i];
		element.color = _input[0].color;
		element.texcoord = VertexTexcoords[i];
		element.lightViewPos = viewPos;
		element.radiusAndLumen = _input[0].radiusAndLumen;

		_output.Append(element);
	}
	_output.RestartStrip();
}
