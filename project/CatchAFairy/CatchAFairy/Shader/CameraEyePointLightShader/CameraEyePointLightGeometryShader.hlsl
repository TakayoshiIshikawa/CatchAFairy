//=============================================================================
//	CameraEyePointLightGeometryShader.hlsl
//
//	geometry shader file of camera eye point light shader.
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "CameraEyePointLightShaderData.hlsli"

static const float4 VertexPositions[4] = {
	float4(-2.0f, 2.0f,-0.5f, 0.0f),
	float4( 2.0f, 2.0f,-0.5f, 0.0f),
	float4(-2.0f,-2.0f,-0.5f, 0.0f),
	float4( 2.0f,-2.0f,-0.5f, 0.0f)
};

static const float2 VertexTexcoords[4] = {
	float2(-1.0f,-1.0f),
	float2( 1.0f,-1.0f),
	float2(-1.0f, 1.0f),
	float2( 1.0f, 1.0f)
};

[maxvertexcount(4)]
void main(
	point VSOutput _input[1],
	inout TriangleStream<PSInput> _output
){
	float4 viewPos = mul(_input[0].lightViewPos, worldViewMatrix);

	for(int i=0; i<4; ++i){
		PSInput element = (PSInput)0;

		element.viewPos = viewPos + (_input[0].radiusAndLumen.x+_input[0].radiusAndLumen.y)*VertexPositions[i];
		float4 pos = mul(element.viewPos, projectionMatrix);
		element.position = pos;
		element.color = _input[0].color;
		element.texcoord = float2(0.5f*pos.x/pos.w + 0.5f, 0.5f - 0.5f*pos.y/pos.w);
		element.lightViewPos = viewPos;
		element.radiusAndLumen = _input[0].radiusAndLumen;
		element.texcoord2 = VertexTexcoords[i];

		_output.Append(element);
	}
	_output.RestartStrip();
}
