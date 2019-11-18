//=============================================================================
//	PlayGUIVertexShader.hlsl
//
//	vertex shader file of play scene graphic user interface shader.
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "PlayGUIShaderData.hlsli"

VSOutput main(VSInput _input){
	VSOutput output = (VSOutput)0;

	output.position = float4((2.0f*_input.position/textureViewSize.xy)-float2(1.0f,1.0f), 0.0f, 1.0f);
	output.color = _input.color;
	output.shapeTexcoord = _input.shapeTexcoord;
	output.viewTexcoord = _input.viewTexcoord;
	output.size = _input.size / textureViewSize.xy;

	return output;
}
