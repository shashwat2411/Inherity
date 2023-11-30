#pragma once
#include "main.h"
#include "renderer.h"
#include "gameobject.h"

class POSTPROCESS
{
private:

	ID3D11Buffer*				m_VertexBuffer = NULL;
	ID3D11ShaderResourceView*	m_Texture = NULL;

	//ここに	シェーダー関連の変数を追加
	ID3D11VertexShader* m_VertexShader; //頂点シェーダーオブジェクト
	ID3D11PixelShader* m_PixelShader; //ピクセルシェーダーオブジェクト
	ID3D11InputLayout* m_VertexLayout; //頂点レイアウトオブジェクト


public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};


//class COMP
//{
//protected:
//	int base = 0;
//	bool ab = false;
//
//public:
//
//	COMP() { base = 1; }
//	virtual ~COMP() {}
//
//	virtual void Temp() = 0;
//
//	template<class Archive>
//	void serialize(Archive & archive)
//	{
//		archive(
//			CEREAL_NVP(base),
//			CEREAL_NVP(ab)
//		);
//	}
//};
//
//
//class COMP_INHERIT : public COMP
//{
//private:
//	int inherited = 0;
//	bool abcd = true;
//
//public:
//	void Temp() override {}
//
//	template<class Archive>
//	void serialize(Archive & archive)
//	{
//		archive(cereal::make_nvp("base",cereal::virtual_base_class<COMP>(this)),
//			CEREAL_NVP(inherited),
//			CEREAL_NVP(abcd)
//		);
//	}
//};