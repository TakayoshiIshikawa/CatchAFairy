//=============================================================================
//	PlayGUIGeometryShader.hlsl
//
//	geometry shader file of play scene graphic user interface shader.
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "PlayGUIShaderData.hlsli"

[maxvertexcount(4)]
void main(
	point VSOutput _input[1],
	inout TriangleStream<PSInput> _output
){
	PSInput element[4] = {(PSInput)0, (PSInput)0, (PSInput)0, (PSInput)0};
	int i = 0;

	element[0].position = float4(
		_input[0].position.x - _input[0].size.x,
		_input[0].position.y + _input[0].size.y,
		0.0f,
		1.0f
	);
	element[1].position = float4(
		_input[0].position.x + _input[0].size.x,
		_input[0].position.y + _input[0].size.y,
		0.0f,
		1.0f
	);
	element[2].position = float4(
		_input[0].position.x - _input[0].size.x,
		_input[0].position.y - _input[0].size.y,
		0.0f,
		1.0f
	);
	element[3].position = float4(
		_input[0].position.x + _input[0].size.x,
		_input[0].position.y - _input[0].size.y,
		0.0f,
		1.0f
	);
	for(i=0; i<4; ++i){
		element[i].color = _input[0].color;
		element[i].size = _input[0].size;
	}
	element[0].shapeTexcoord = float4(_input[0].shapeTexcoord.xy, 0.0f, 1.0f);
	element[1].shapeTexcoord = float4(_input[0].shapeTexcoord.zy, 0.0f, 1.0f);
	element[2].shapeTexcoord = float4(_input[0].shapeTexcoord.xw, 0.0f, 1.0f);
	element[3].shapeTexcoord = float4(_input[0].shapeTexcoord.zw, 0.0f, 1.0f);
	element[0].viewTexcoord = float4(_input[0].viewTexcoord.xy, 0.0f, 1.0f);
	element[1].viewTexcoord = float4(_input[0].viewTexcoord.zy, 0.0f, 1.0f);
	element[2].viewTexcoord = float4(_input[0].viewTexcoord.xw, 0.0f, 1.0f);
	element[3].viewTexcoord = float4(_input[0].viewTexcoord.zw, 0.0f, 1.0f);

	for(i=0; i<4; ++i){
		_output.Append(element[i]);
	}
	_output.RestartStrip();
}
