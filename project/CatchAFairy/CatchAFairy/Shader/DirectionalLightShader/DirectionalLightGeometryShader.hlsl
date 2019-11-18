//=============================================================================
//	DirectionalLightGeometryShader.hlsl
//
//	geometry shader file of directional light shader.
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "DirectionalLightShaderData.hlsli"

[maxvertexcount(3)]
void main(
	triangle VSOutput _input[3],
	inout TriangleStream<PSInput> _output
){
	for(int i=0; i<3; ++i){
		PSInput element = (PSInput)0;

		element.position = mul(_input[i].position, modelToProjectionMatrix);

		element.normal = mul(_input[i].normal, modelToProjectionMatrix);
		element.normal.xyz = normalize(element.normal.xyz);
		element.normal.w = 1.0f;

		element.texcoord = _input[i].texcoord;
		element.texcoord.y = 1.0f - element.texcoord.y;

		element._pos = element.position;

		_output.Append(element);
	}
	_output.RestartStrip();
}
