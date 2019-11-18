//=============================================================================
//	CameraEyeShadowPixelShader.hlsl
//
//	pixel shader file of camera eye shadow shader.
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "CameraEyeShadowShaderData.hlsli"

Texture2D cameraPositionTexture : register(t0);
Texture2D cameraNormalTexture : register(t1);
Texture2D lightPositionTexture : register(t2);
SamplerState renderTextureSamplerState : register(s0);

static const float LightColor = 1.0f;
static const float ShadowColor = 0.8f;

PSOutput main(PSInput _input){
	PSOutput output = (PSOutput)0;

	float4 cameraPosition = cameraPositionTexture.Sample(renderTextureSamplerState, _input.texcoord);
	clip(cameraPosition.w - 0.1f);
	float fogRate = 20.0f*max(0.0f, (cameraPosition.z/cameraPosition.w)-0.95f);
	float fog = 1.0f - (1.0f-ShadowColor)*fogRate;

	float4 normal = cameraNormalTexture.Sample(renderTextureSamplerState, _input.texcoord);
	float normalRate = -dot(normalize(normal.xyz), normalize(directionalLightDirection.xyz));
	if(normalRate < 0.0f){
		output.shadow.rgb = min(fog, ShadowColor);
		output.shadow.a = 1.0f;
		return output;
	}

	float4 lightLookingPosition = mul(cameraPosition, cameraProjectionToLightProjectionMatrix);
	lightLookingPosition /= lightLookingPosition.w;
	float2 lightTexcoord = float2(
		0.5f*lightLookingPosition.x + 0.5f,
		0.5 - 0.5f*lightLookingPosition.y
	);

	float delta = lightLookingPosition.z - (lightPositionTexture.Sample(renderTextureSamplerState, lightTexcoord).z + 0.002f);
	float gray = normalRate*LightColor + (1.0f-normalRate)*ShadowColor;
	if(delta > 0.0f) gray = ShadowColor;

	output.shadow.rgb = min(fog, gray);
	output.shadow.a = 1.0f;

	return output;
}
