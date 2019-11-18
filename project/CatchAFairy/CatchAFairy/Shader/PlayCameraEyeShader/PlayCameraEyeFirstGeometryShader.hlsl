//=============================================================================
//	PlayCameraEyeFirstGeometryShader.hlsl
//
//	geometry shader file of play camera eye first shader.
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "PlayCameraEyeFirstShaderData.hlsli"

[maxvertexcount(3)]
void main(
	triangle VSOutput _input[3],
	inout TriangleStream<PSInput> _output
){
	for(int i=0; i<3; ++i){
		PSInput element = (PSInput)0;

		element.position = _input[i].position;
		element.normal = float4(_input[i].normal.xyz, 1.0f);
		element.texcoord = _input[i].texcoord;
		element.texcoord.y = 1.0f - element.texcoord.y;
		element._pos = element.position;
		element.tangent = float4(_input[i].tangent.xyz, 1.0f);
		element.binormal = float4(_input[i].binormal.xyz, 1.0f);

		_output.Append(element);
	}
	_output.RestartStrip();
}
