//=============================================================================
//	ShaderVertexTopology.cpp
//
//	シェーダ頂点トポロジーのソースファイル
//
//	Copyright(c) 2019 Ishikawa Takayoshi All Rights Reserved.
//=============================================================================
#include "ShaderVertexTopology.h"
#pragma warning(disable:4668)
#pragma warning(disable:4820)
#pragma warning(disable:4917)
#include <d3dcommon.h>
#pragma warning(default:4917)
#pragma warning(default:4820)
#pragma warning(default:4668)
#include "Ishikawa/Common/Exception/ITExceptions.h"

using namespace CatchAFairy::Shader;


/// <summary>
/// シェーダ頂点トポロジーをD3Dプリミティブトポロジーに変換
/// </summary>
/// <param name="_topology">シェーダ頂点トポロジー</param>
/// <returns>D3Dプリミティブトポロジー</returns>
D3D_PRIMITIVE_TOPOLOGY User::ConvertVertexTopologyToD3DPrimitiveTopology(VertexTopology _topology){
	switch(_topology){
	case VertexTopology::PointList:
		return D3D_PRIMITIVE_TOPOLOGY_POINTLIST;

	case VertexTopology::LineList:
		return D3D_PRIMITIVE_TOPOLOGY_LINELIST;

	case VertexTopology::LineStrip:
		return D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;

	case VertexTopology::TriangleList:
		return D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	case VertexTopology::TriangleStrip:
		return D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	default:
		// 異常終了
		throw ::Ishikawa::Common::Exception::FunctionFailed("_topology is unknown.");
	}
}
