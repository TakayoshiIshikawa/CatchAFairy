//=============================================================================
//	CameraEyeShadowShaderData.hlsli
//
//	header file of camera eye shadow shader.
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================

struct VSInput{
	float2 position : SV_POSITION0;
	float2 texcoord : TEXCOORD0;
};

struct VSOutput{
	float4 position : SV_POSITION0;
	float2 texcoord : TEXCOORD0;
};

typedef VSOutput PSInput;

struct PSOutput{
	float4 shadow : SV_TARGET0;
};


cbuffer ConstantBuffer : register(b0){
	float4x4 cameraProjectionToLightProjectionMatrix;
	float4 directionalLightDirection;
};
