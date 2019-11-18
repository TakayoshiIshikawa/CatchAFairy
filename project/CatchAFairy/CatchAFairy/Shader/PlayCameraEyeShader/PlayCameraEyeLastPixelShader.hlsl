//=============================================================================
//	PlayCameraEyeLastPixelShader.hlsl
//
//	pixel shader file of play camera eye last shader.
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "PlayCameraEyeLastShaderData.hlsli"

Texture2D diffuseTexture : register(t0);
Texture2D shadowTexture : register(t1);
Texture2D reflectionLightTexture : register(t2);
Texture2D pointLightTexture : register(t3);
SamplerState renderTextureSamplerState : register(s0);

float4 AddLightColor(float4 _baseColor, float2 _texcoord){
	float4 reflectionLight = reflectionLightTexture.Sample(renderTextureSamplerState, _texcoord);
	float4 pointLight = pointLightTexture.Sample(renderTextureSamplerState, _texcoord);
	float4 lightColor = float4(
		pointLight.a*pointLight.rgb + (1.0f-pointLight.a)*reflectionLight.rgb,
		pointLight.a + (1.0f-pointLight.a)*reflectionLight.a
	);

	float4 color = (float4)0;
	color.rgb = _baseColor.rgb + lightColor.a*lightColor.rgb;
	color.a = _baseColor.a + (1.0f-_baseColor.a)*lightColor.a;

	return color;
}

PSOutput main(PSInput _input){
	PSOutput output = (PSOutput)0;

	float4 diffuse = diffuseTexture.Sample(renderTextureSamplerState, _input.texcoord);
	float4 shadow = shadowTexture.Sample(renderTextureSamplerState, _input.texcoord);

	if(shadow.a < 0.1f){
		// draw sky.
		float heightRate = 2.0f*(0.5f - _input.texcoord.y);
		output.color = AddLightColor(
			float4(0.5f+0.1f*heightRate, 0.7f+0.12f*heightRate, 0.9f+0.1f*heightRate, 1.0f),
			_input.texcoord
		);
		return output;
	}

	float4 baseColor = diffuse * directionalLightColor * shadow;

	output.color = AddLightColor(baseColor, _input.texcoord);

	return output;
}
