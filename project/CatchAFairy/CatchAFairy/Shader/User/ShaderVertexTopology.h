//=============================================================================
//	ShaderVertexTopology.h
//
//	シェーダ頂点トポロジーのヘッダファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#pragma once
#ifndef __SHADER_VERTEX_TOPOLOGY_H__
#define __SHADER_VERTEX_TOPOLOGY_H__

#pragma warning(disable:4471)
enum D3D_PRIMITIVE_TOPOLOGY;
#pragma warning(default:4471)


namespace CatchAFairy{
	namespace Shader{
		namespace User{
			/// <summary>
			/// シェーダ頂点トポロジー
			/// </summary>
			enum class VertexTopology : int{
				/// <summary>ポイントリスト</summary>
				PointList,
				/// <summary>線分リスト</summary>
				LineList,
				/// <summary>線分ストリップ</summary>
				LineStrip,
				/// <summary>三角形リスト</summary>
				TriangleList,
				/// <summary>三角形ストリップ</summary>
				TriangleStrip
			};

			// シェーダ頂点トポロジーをD3Dプリミティブトポロジーに変換
			D3D_PRIMITIVE_TOPOLOGY ConvertVertexTopologyToD3DPrimitiveTopology(VertexTopology _topology);
		}
	}
}

#endif
