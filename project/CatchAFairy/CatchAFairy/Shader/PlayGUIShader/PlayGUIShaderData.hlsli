//=============================================================================
//	PlayGUIShaderData.hlsli
//
//	header file of play scene graphic user interface shader.
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================

struct VSInput{
	float2 position			: SV_POSITION0;
	float4 color			: COLOR0;
	float4 shapeTexcoord	: TEXCOORD0;
	float4 viewTexcoord		: TEXCOORD1;
	float2 size				: TEXCOORD2;
};

struct VSOutput{
	float4 position			: SV_POSITION0;
	float4 color			: COLOR0;
	float4 shapeTexcoord	: TEXCOORD0;
	float4 viewTexcoord		: TEXCOORD1;
	float2 size				: TEXCOORD2;
};

typedef VSOutput PSInput;

struct PSOutput{
	float4 color : SV_TARGET0;
};


cbuffer ConstantBuffer : register(b0){
	float4 textureViewSize;
};
