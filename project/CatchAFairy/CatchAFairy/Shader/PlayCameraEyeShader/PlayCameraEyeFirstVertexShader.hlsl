//=============================================================================
//	PlayCameraEyeFirstVertexShader.hlsl
//
//	vertex shader file of play camera eye first shader.
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "PlayCameraEyeFirstShaderData.hlsli"

VSOutput main(VSInput _input){
	VSOutput output = (VSOutput)0;

	output.position = mul(float4(_input.position, 1.0f), modelToProjectionMatrix);
	output.normal	= mul(float4(_input.normal, 0.0f), modelToProjectionMatrix);
	output.texcoord = _input.texcoord;

	float3 _tan = cross(_input.normal, float3(0.0f, 1.0f, 0.0f));
	if(dot(_tan, _tan) < 0.0001f){
		output.tangent = float4(1.0f, 0.0f, 0.0f, 0.0f);
		output.binormal = float4(0.0f, 0.0f, 1.0f, 0.0f);
	}
	else{
		output.tangent = mul(float4(normalize(_tan), 0.0f), modelToProjectionMatrix);
		output.binormal = mul(float4(normalize(cross(_input.normal, _tan)), 0.0f), modelToProjectionMatrix);
	}

	return output;
}
