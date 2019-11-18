//=============================================================================
//	CameraEyeReflectionLightShaderData.hlsli
//
//	header file of camera eye reflection light shader.
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================

struct VSInput{
	float3 position			: SV_POSITION0;
	float4 color			: COLOR0;
	float2 radiusAndLumen	: TEXCOORD0;
};

struct VSOutput{
	float4 position			: SV_POSITION0;
	float4 color			: COLOR0;
	float2 texcoord			: TEXCOORD0;
	float4 lightViewPos		: TEXCOORD1;
	float2 radiusAndLumen	: TEXCOORD3;
};

typedef VSOutput PSInput;

struct PSOutput{
	float4 reflectionColor : SV_TARGET0;
};


cbuffer ConstantBuffer : register(b0){
	float4x4 worldViewProjectionMatrix;
	float4x4 worldViewMatrix;
	float4x4 invertProjectionMatrix;
};
