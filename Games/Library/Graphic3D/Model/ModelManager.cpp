//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		ModelManager.cpp
//!
//! @summary	モデルデータ管理クラスのソースファイル
//!
//! @date		2018.09.27
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <標準ヘッダファイル>
#include <string>
#include <fstream>

// <自作ヘッダファイル>
#include "ModelManager.h"
#include "../../Utility/BinaryFileLoader.h"
#include "PMDData.h"
#include "PMXData.h"


// usingディレクティブ =====================================================
using namespace std;
using namespace Library;


// 関数の定義 ==============================================================
//--------------------------------------------------------------------
//! @summary   コンストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Graphic3D::Model::ModelManager::ModelManager()
{
	// 何もしない
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Graphic3D::Model::ModelManager::~ModelManager()
{
	for (map<const wchar_t*, Model*>::iterator itr = m_modelMap.begin(); itr != m_modelMap.end(); ++itr)
	{
		pair<const wchar_t*, Model*> p = (*itr);
		delete p.second;
	}
}



//--------------------------------------------------------------------
//! @summary   PMDのモデルデータの読み込み処理
//!
//! @parameter [fileName] 読み込むモデルデータのファイル名
//!
//! @return    モデルデータのインスタンス
//--------------------------------------------------------------------
Graphic3D::Model::Model* Graphic3D::Model::ModelManager::LoadModelDataFromPMD(const wchar_t* fileName)
{
	if (m_modelMap.count(fileName) == 0)
	{
		// ファイルまでのディレクトリパスを作成
		wstring fullPass = wstring(L"Resources/Models/PMD/") + fileName + wstring(L".pmd");

		// ファイルを開く
		std::ifstream stream(fullPass, std::ios::binary);

		// 失敗
		if (stream.fail()) return nullptr;

		// データの読み込み
		PMDModel* model = CreatePMDModel(&stream);

		// ファイルを閉じる
		stream.close();

		// コンテナに保存
		m_modelMap[fileName] = new Model(model, fileName);

		// モデルデータの削除
		delete model;
	}

	return m_modelMap[fileName];
}



//--------------------------------------------------------------------
//! @summary   PMXのモデルデータの読み込み処理
//!
//! @parameter [fileName] 読み込むモデルデータのファイル名
//!
//! @return    モデルデータのインスタンス
//--------------------------------------------------------------------
Graphic3D::Model::Model* Graphic3D::Model::ModelManager::LoadModelDataFromPMX(const wchar_t* fileName)
{
	if (m_modelMap.count(fileName) == 0)
	{
		// ファイルまでのディレクトリパスを作成
		wstring fullPass = wstring(L"Resources/Models/PMX/") + fileName + wstring(L".pmx");

		// ファイルを開く
		std::ifstream stream(fullPass, std::ios::binary);

		// 失敗
		if (stream.fail()) return nullptr;

		// データの読み込み
		PMXModel* model = CreatePMXModel(&stream);

		// ファイルを閉じる
		stream.close();

		// コンテナに保存
		m_modelMap[fileName] = new Model(model, fileName);

		// モデルデータの削除
		delete model;
	}

	return m_modelMap[fileName];
}



//--------------------------------------------------------------
//! @summary   ファイルからPMDModelを生成する
//!
//! @parameter [stream] 読み込まれたファイル
//!
//! @return    生成されたPMDModel
//--------------------------------------------------------------
Graphic3D::Model::PMDModel* Graphic3D::Model::ModelManager::CreatePMDModel(ifstream* stream)
{
	// 入れ物の作成
	PMDModel* model = new PMDModel();

	// <ファイルのフォーマット>
	{
		stream->read(model->header.magic, 3);

		// PMDファイルでない
		if (model->header.magic[0] != 'P' || model->header.magic[1] != 'm' || model->header.magic[2] != 'd') return nullptr;
	}


	// <バージョン>
	{
		stream->read((char*) &(model->header.version), sizeof(float));

		// 無効なバージョン
		if (model->header.version != 1.0f) return nullptr;
	}


	// <モデル名とコメント>
	{
		// バッファ
		char buffer[256];

		// モデル名
		stream->read(buffer, 20);
		model->header.modelName = std::string(buffer);

		// コメント
		stream->read(buffer, 256);
		model->header.comment = std::string(buffer);
	}


	// <頂点データ>
	{
		// 頂点数の取得
		uint32_t vertexNumber;
		stream->read((char*)&vertexNumber, sizeof(uint32_t));

		// サイズの確保
		model->vertexes.resize(vertexNumber);
		for (uint32_t i = 0; i < vertexNumber; ++i)
		{
			stream->read((char*)model->vertexes[i].position, sizeof(float) * 3);
			stream->read((char*)model->vertexes[i].normal, sizeof(float) * 3);
			stream->read((char*)model->vertexes[i].uv, sizeof(float) * 2);
			stream->read((char*)model->vertexes[i].boneIndex, sizeof(uint16_t) * 2);
			stream->read((char*)&model->vertexes[i].boneWeight, sizeof(uint8_t));
			stream->read((char*)&model->vertexes[i].edgeInvisible, sizeof(uint8_t));
		}
	}


	// <インデックスデータ>
	{
		// インデックス数の取得
		uint32_t indexNumber;
		stream->read((char*)&indexNumber, sizeof(uint32_t));

		// サイズの確保
		model->indices.resize(indexNumber);

		// データの取得
		for (uint32_t i = 0; i < indexNumber; ++i)
		{
			stream->read((char*)&model->indices[i], sizeof(uint16_t));
		}
	}


	// <マテリアル>
	{
		// マテリアル数の取得
		uint32_t materialNumber;
		stream->read((char*)&materialNumber, sizeof(uint32_t));

		// サイズの確保
		model->materials.resize(materialNumber);

		// マテリアルの取得
		for (uint32_t i = 0; i < materialNumber; ++i)
		{
			// バッファ
			char buffer[20];
			stream->read((char*)&model->materials[i].diffuse, sizeof(float) * 4);
			stream->read((char*)&model->materials[i].specularity, sizeof(float));
			stream->read((char*)&model->materials[i].specular, sizeof(float) * 3);
			stream->read((char*)&model->materials[i].ambient, sizeof(float) * 3);
			stream->read((char*)&model->materials[i].toonIndex, sizeof(uint8_t));
			stream->read((char*)&model->materials[i].edgeFlag, sizeof(uint8_t));
			stream->read((char*)&model->materials[i].indexCount, sizeof(uint32_t));

			stream->read((char*)&buffer, sizeof(char) * 20);
			char* pstar = strchr(buffer, '*');
			if (pstar == nullptr)
			{
				model->materials[i].textureFilename = std::string(buffer);
				model->materials[i].sphereFilename.clear();
			}
			else
			{
				*pstar = NULL;
				model->materials[i].textureFilename = std::string(buffer);
				model->materials[i].sphereFilename = std::string(pstar + 1);
			}
		}
	}


	// <ボーン>
	uint16_t boneNumber;
	{
		// ボーン数の取得
		stream->read((char*)&boneNumber, sizeof(uint16_t));

		// サイズの確保
		model->bones.resize(boneNumber);

		// ボーンの取得
		for (uint32_t i = 0; i < boneNumber; i++)
		{
			// バッファ
			char buffer[20];
			// ボーン名の取得
			stream->read(buffer, 20);
			// ボーン名の設定
			model->bones[i].name = std::string(buffer);

			stream->read((char*)&model->bones[i].parentBoneIndex, sizeof(uint16_t));
			stream->read((char*)&model->bones[i].tailPosBoneIndex, sizeof(uint16_t));
			stream->read((char*)&model->bones[i].boneType, sizeof(uint8_t));
			stream->read((char*)&model->bones[i].ikParentBoneIndex, sizeof(uint16_t));
			stream->read((char*)&model->bones[i].boneHeadPos, sizeof(float) * 3);
		}
	}


	// <IKs>
	{
		// IK数の取得
		uint16_t ikNumber;
		stream->read((char*)&ikNumber, sizeof(uint16_t));

		model->IKs.resize(ikNumber);
		for (uint32_t i = 0; i < ikNumber; ++i)
		{
			stream->read((char *)&model->IKs[i].ikBoneIndex, sizeof(uint16_t));
			stream->read((char *)&model->IKs[i].targetBoneIndex, sizeof(uint16_t));

			// 影響下ボーン数の取得
			uint8_t ikChainNumber;
			stream->read((char*)&ikChainNumber, sizeof(uint8_t));

			stream->read((char *)&model->IKs[i].interations, sizeof(uint16_t));
			stream->read((char *)&model->IKs[i].angleLimit, sizeof(float));

			// サイズの確保
			model->IKs[i].ikChildBoneIndex.resize(ikChainNumber);

			// // 影響下ボーン番号の取得
			for (int j = 0; j < ikChainNumber; ++j)
			{
				stream->read((char *)&model->IKs[i].ikChildBoneIndex[j], sizeof(uint16_t));
			}
		}
	}


	// <表情>
	uint16_t faceNumber;
	{
		// 表情の数を取得
		stream->read((char*)&faceNumber, sizeof(uint16_t));

		// サイズの確保
		model->faces.resize(faceNumber);

		// 表情データの取得
		for (uint32_t i = 0; i < faceNumber; ++i)
		{
			// バッファ
			char buffer[20];
			stream->read(buffer, 20);
			model->faces[i].name = std::string(buffer);

			// 頂点数の取得
			int vertexNumber = 0;
			stream->read((char*)&vertexNumber, sizeof(int));

			stream->read((char*)&model->faces[i].type, sizeof(uint8_t));

			// サイズの確保
			model->faces[i].vertices.resize(vertexNumber);
			for (int j = 0; j < vertexNumber; ++j)
			{
				stream->read((char *)&model->faces[i].vertices[j].index, sizeof(int));
				stream->read((char *)model->faces[i].vertices[j].position, sizeof(float) * 3);
			}
		}
	}


	// <表情枠>
	{
		// 表情枠数の取得
		uint8_t faceFrameNumber;
		stream->read((char*)&faceFrameNumber, sizeof(uint8_t));

		// サイズの確保
		model->facesIndices.resize(faceFrameNumber);

		// データの取得
		for (uint32_t i = 0; i < faceFrameNumber; ++i)
		{
			stream->read((char*)&model->facesIndices[i], sizeof(uint16_t));
		}
	}


	// <ボーン枠用枠名>
	{
		// 名前の数を取得
		uint8_t boneDispNumber;
		stream->read((char*)&boneDispNumber, sizeof(uint8_t));

		// サイズの確保
		model->boneDispName.resize(boneDispNumber);

		// ボーン名の取得
		for (uint32_t i = 0; i < boneDispNumber; ++i)
		{
			char buffer[50];
			stream->read(buffer, 50);
			model->boneDispName[i].boneDispName = std::string(buffer);
			model->boneDispName[i].boneDispNameEnglish.clear();
		}
	}


	// <ボーンフレーム>
	{
		// フレーム数の取得
		uint32_t boneFrameNumber;
		stream->read((char*)&boneFrameNumber, sizeof(uint32_t));

		// サイズの確保
		model->boneDisp.resize(boneFrameNumber);

		// ボーンフレームの取得
		for (uint32_t i = 0; i < boneFrameNumber; ++i)
		{
			stream->read((char*)&model->boneDisp[i].boneIndex, sizeof(uint16_t));
			stream->read((char*)&model->boneDisp[i].boneDispIndex, sizeof(uint8_t));
		}
	}


	// <英語名>
	{
		// 英語名があるか調べる
		bool english;
		stream->read((char*)&english, sizeof(char));

		// 英語名がある
		if (english)
		{
			{
				char buffer[256];
				stream->read(buffer, 20);
				model->header.nameEnglish = std::string(buffer);
				stream->read(buffer, 256);
				model->header.commentEnglish = std::string(buffer);
			}

			for (uint32_t i = 0; i < boneNumber; ++i)
			{
				char buffer[20];
				stream->read(buffer, 20);
				model->bones[i].nameEnglish = std::string(buffer);
			}

			for (uint32_t i = 0; i < faceNumber; ++i)
			{
				if (model->faces[i].type == FaceCategory::Base) continue;

				char buffer[20];
				stream->read(buffer, 20);
				model->faces[i].nameEnglish = std::string(buffer);
			}

			for (uint32_t i = 0; i < model->boneDispName.size(); ++i)
			{
				char buffer[50];
				stream->read(buffer, 50);
				model->boneDispName[i].boneDispNameEnglish = std::string(buffer);
			}
		}
	}


	// <トゥーンテクスチャ>
	{
		// バッファ
		char buffer[100];

		if (stream->peek() == std::ios::traits_type::eof())
		{
			model->toonFilenames.clear();
		}
		else
		{
			model->toonFilenames.resize(10);
			for (uint32_t i = 0; i < 10; ++i)
			{
				stream->read(buffer, 100);
				model->toonFilenames[i] = std::string(buffer);
			}
		}
	}


	// <剛体>
	{
		if (stream->peek() == std::ios::traits_type::eof())
		{
			model->rigidbodies.clear();
			model->constraints.clear();
		}
		else
		{
			// 剛体数の取得
			uint32_t rigidbodyNumber;
			stream->read((char*)&rigidbodyNumber, sizeof(uint32_t));

			// サイズの確保
			model->rigidbodies.resize(rigidbodyNumber);

			// 剛体の取得
			for (uint32_t i = 0; i < rigidbodyNumber; ++i)
			{
				char buffer[20];
				stream->read(buffer, sizeof(char) * 20);
				model->rigidbodies[i].name = (std::string(buffer));
				stream->read((char*)&model->rigidbodies[i].relatedBoneIndex, sizeof(uint16_t));
				stream->read((char*)&model->rigidbodies[i].groupIndex, sizeof(uint8_t));
				stream->read((char*)&model->rigidbodies[i].mask, sizeof(uint16_t));
				stream->read((char*)&model->rigidbodies[i].shape, sizeof(uint8_t));
				stream->read((char*)model->rigidbodies[i].size, sizeof(float) * 3);
				stream->read((char*)model->rigidbodies[i].position, sizeof(float) * 3);
				stream->read((char*)model->rigidbodies[i].orientation, sizeof(float) * 3);
				stream->read((char*)&model->rigidbodies[i].weight, sizeof(float));
				stream->read((char*)&model->rigidbodies[i].linearDamping, sizeof(float));
				stream->read((char*)&model->rigidbodies[i].anglarDamping, sizeof(float));
				stream->read((char*)&model->rigidbodies[i].restitution, sizeof(float));
				stream->read((char*)&model->rigidbodies[i].friction, sizeof(float));
				stream->read((char*)&model->rigidbodies[i].rigidType, sizeof(char));
			}

			// 剛体の拘束の数を取得
			uint32_t constraintnumber;
			stream->read((char*)&constraintnumber, sizeof(uint32_t));

			// サイズの確保
			model->constraints.resize(constraintnumber);

			// 剛体の拘束の取得
			for (uint32_t i = 0; i < constraintnumber; ++i)
			{
				char buffer[20];
				stream->read(buffer, 20);
				model->constraints[i].name = std::string(buffer);
				stream->read((char *)&model->constraints[i].rigidBodyIndex[0], sizeof(uint32_t));
				stream->read((char *)&model->constraints[i].rigidBodyIndex[1], sizeof(uint32_t));
				stream->read((char *)model->constraints[i].position, sizeof(float) * 3);
				stream->read((char *)model->constraints[i].orientation, sizeof(float) * 3);
				stream->read((char *)model->constraints[i].linearLowerLimit, sizeof(float) * 3);
				stream->read((char *)model->constraints[i].linearUpperLimit, sizeof(float) * 3);
				stream->read((char *)model->constraints[i].angularLowerLimit, sizeof(float) * 3);
				stream->read((char *)model->constraints[i].angularUpperLimit, sizeof(float) * 3);
				stream->read((char *)model->constraints[i].linearStiffness, sizeof(float) * 3);
				stream->read((char *)model->constraints[i].angularStiffness, sizeof(float) * 3);
			}
		}
	}

	// 不明なデータがある
	if (stream->peek() != std::ios::traits_type::eof()) return nullptr;

	return model;
}



//--------------------------------------------------------------
//! @summary   ファイルからPMXModelを生成する
//!
//! @parameter [stream] 読み込まれたファイル
//!
//! @return    生成されたPMXModel
//--------------------------------------------------------------
Graphic3D::Model::PMXModel* Graphic3D::Model::ModelManager::CreatePMXModel(ifstream* stream)
{
	PMXModel* pmxModel = new PMXModel();

	// <マジック>
	{
		char magic[4];
		stream->read((char*)magic, sizeof(char) * 4);
		// 無効なマジックナンバー
		if (magic[0] != 0x50 || magic[1] != 0x4d || magic[2] != 0x58 || magic[3] != 0x20) return nullptr;
	}

	// <バージョン>
	{
		stream->read((char*)&pmxModel->version, sizeof(float));
		// ver2.0またはver2.1ではない
		if (pmxModel->version != 2.0f && pmxModel->version != 2.1f) return nullptr;
	}

	// <ファイル設定>
	{
		uint8_t count;
		stream->read((char*)&count, sizeof(uint8_t));
		if (count < 8) return nullptr;

		stream->read((char*)&pmxModel->setting.encoding, sizeof(uint8_t));
		stream->read((char*)&pmxModel->setting.uv, sizeof(uint8_t));
		stream->read((char*)&pmxModel->setting.vertexIndexSize, sizeof(uint8_t));
		stream->read((char*)&pmxModel->setting.textureIndexSize, sizeof(uint8_t));
		stream->read((char*)&pmxModel->setting.materialIndexSize, sizeof(uint8_t));
		stream->read((char*)&pmxModel->setting.boneIndexSize, sizeof(uint8_t));
		stream->read((char*)&pmxModel->setting.morphIndexSize, sizeof(uint8_t));
		stream->read((char*)&pmxModel->setting.rigidbodyIndexSize, sizeof(uint8_t));

		uint8_t tmp;
		for (int i = 8; i < count; i++)
		{
			stream->read((char*)&tmp, sizeof(uint8_t));
		}
	}

	// <モデル情報>
	{
		pmxModel->modelName = move(ReadString(stream, pmxModel->setting.encoding));
		pmxModel->modelEnglishName = move(ReadString(stream, pmxModel->setting.encoding));
		pmxModel->modelComment = std::move(ReadString(stream, pmxModel->setting.encoding));
		pmxModel->modelEnglishComment = std::move(ReadString(stream, pmxModel->setting.encoding));
	}

	// <頂点>
	{
		// 頂点数の読み込み
		stream->read((char*)&pmxModel->vertexCount, sizeof(int));

		// サイズの確保
		pmxModel->vertexes = std::vector<PMXVertex>(pmxModel->vertexCount);

		// 頂点データの取得
		for (int i = 0; i < pmxModel->vertexCount; ++i)
		{
			stream->read((char*)pmxModel->vertexes[i].position, sizeof(float) * 3);
			stream->read((char*)pmxModel->vertexes[i].normal, sizeof(float) * 3);
			stream->read((char*)pmxModel->vertexes[i].uv, sizeof(float) * 2);
			for (int j = 0; j < pmxModel->setting.uv; ++j)
			{
				stream->read((char*)pmxModel->vertexes[i].uvAddition[i], sizeof(float) * 4);
			}
			stream->read((char*) &pmxModel->vertexes[i].skinningType, sizeof(PMXVertexSkinningType));
			switch (pmxModel->vertexes[i].skinningType)
			{
			case PMXVertexSkinningType::BDEF1:
				pmxModel->vertexes[i].skinning = new PMXVertexSkinningBDEF1();
				break;
			case PMXVertexSkinningType::BDEF2:
				pmxModel->vertexes[i].skinning = new PMXVertexSkinningBDEF2();
				break;
			case PMXVertexSkinningType::BDEF4:
				pmxModel->vertexes[i].skinning = new PMXVertexSkinningBDEF4();
				break;
			case PMXVertexSkinningType::SDEF:
				pmxModel->vertexes[i].skinning = new PMXVertexSkinningSDEF();
				break;
			case PMXVertexSkinningType::QDEF:
				pmxModel->vertexes[i].skinning = new PMXVertexSkinningQDEF();
				break;
			default:
				break;
			}
			pmxModel->vertexes[i].skinning->Read(stream, &pmxModel->setting);
			stream->read((char*) &pmxModel->vertexes[i].edge, sizeof(float));
		}
	}

	// インデックス
	{
		// インデックス数の取得
		stream->read((char*)&pmxModel->indexCount, sizeof(int));

		// サイズの確保
		pmxModel->indices.resize(pmxModel->indexCount);

		// インデックスデータの取得
		for (int i = 0; i < pmxModel->indexCount; ++i)
		{
			pmxModel->indices[i] = ReadIndex(stream, pmxModel->setting.vertexIndexSize);
		}
	}

	// <テクスチャ>
	{
		// テクスチャ数の取得
		stream->read((char*)&pmxModel->textureCount, sizeof(int));

		// サイズの確保
		pmxModel->textures.resize(pmxModel->textureCount);

		// テクスチャ名の取得
		for (int i = 0; i < pmxModel->textureCount; ++i)
		{
			pmxModel->textures[i] = ReadString(stream, pmxModel->setting.encoding);
		}
	}

	// <マテリアル>
	{
		// マテリアル数の取得
		stream->read((char*)&pmxModel->materialCount, sizeof(int));

		// サイズの確保
		pmxModel->materials.resize(pmxModel->materialCount);

		// マテリアルの取得
		for (int i = 0; i < pmxModel->materialCount; ++i)
		{
			pmxModel->materials[i].materialName = std::move(ReadString(stream, pmxModel->setting.encoding));
			pmxModel->materials[i].materialEnglishName = std::move(ReadString(stream, pmxModel->setting.encoding));
			stream->read((char*)pmxModel->materials[i].diffuse, sizeof(float) * 4);
			stream->read((char*)pmxModel->materials[i].specular, sizeof(float) * 3);
			stream->read((char*) &pmxModel->materials[i].specularlity, sizeof(float));
			stream->read((char*)pmxModel->materials[i].ambient, sizeof(float) * 3);
			stream->read((char*) &pmxModel->materials[i].flag, sizeof(uint8_t));
			stream->read((char*)pmxModel->materials[i].edgeColor, sizeof(float) * 4);
			stream->read((char*) &pmxModel->materials[i].edgeSize, sizeof(float));
			pmxModel->materials[i].diffuseTextureIndex = ReadIndex(stream, pmxModel->setting.textureIndexSize);
			pmxModel->materials[i].sphereTextureIndex = ReadIndex(stream, pmxModel->setting.textureIndexSize);
			stream->read((char*) &pmxModel->materials[i].sphereOpMode, sizeof(uint8_t));
			stream->read((char*) &pmxModel->materials[i].commonToonFlag, sizeof(uint8_t));
			if (pmxModel->materials[i].commonToonFlag)
			{
				stream->read((char*) &pmxModel->materials[i].toonTextureIndex, sizeof(uint8_t));
			}
			else {
				pmxModel->materials[i].toonTextureIndex = ReadIndex(stream, pmxModel->setting.textureIndexSize);
			}
			pmxModel->materials[i].memo = std::move(ReadString(stream, pmxModel->setting.encoding));
			stream->read((char*) &pmxModel->materials[i].indexCount, sizeof(int));
		}
	}

	// <ボーン>
	{
		// ボーン数の取得
		stream->read((char*) &pmxModel->boneCount, sizeof(int));

		// サイズの確保
		pmxModel->bones.resize(pmxModel->boneCount);

		// データの取得
		for (int i = 0; i < pmxModel->boneCount; ++i)
		{
			pmxModel->bones[i].boneName = std::move(ReadString(stream, pmxModel->setting.encoding));
			pmxModel->bones[i].boneEnglishName = std::move(ReadString(stream, pmxModel->setting.encoding));
			stream->read((char*)pmxModel->bones[i].position, sizeof(float) * 3);
			pmxModel->bones[i].parentIndex = ReadIndex(stream, pmxModel->setting.boneIndexSize);
			stream->read((char*) &pmxModel->bones[i].level, sizeof(int));
			stream->read((char*) &pmxModel->bones[i].boneFlag, sizeof(uint16_t));
			if (pmxModel->bones[i].boneFlag & 0x0001)
			{
				pmxModel->bones[i].targetIndex = ReadIndex(stream, pmxModel->setting.boneIndexSize);
			}
			else
			{
				stream->read((char*)pmxModel->bones[i].offset, sizeof(float) * 3);
			}

			if (pmxModel->bones[i].boneFlag & (0x0100 | 0x0200))
			{
				pmxModel->bones[i].grantParentIndex = ReadIndex(stream, pmxModel->setting.boneIndexSize);
				stream->read((char*) &pmxModel->bones[i].grantWeight, sizeof(float));
			}

			if (pmxModel->bones[i].boneFlag & 0x0400)
			{
				stream->read((char*)pmxModel->bones[i].lockAxisOrientation, sizeof(float) * 3);
			}

			if (pmxModel->bones[i].boneFlag & 0x0800)
			{
				stream->read((char*)pmxModel->bones[i].localAxisOrientationX, sizeof(float) * 3);
				stream->read((char*)pmxModel->bones[i].localAxisOrientationY, sizeof(float) * 3);
			}

			if (pmxModel->bones[i].boneFlag & 0x2000)
			{
				stream->read((char*) &pmxModel->bones[i].key, sizeof(int));
			}

			if (pmxModel->bones[i].boneFlag & 0x0020)
			{
				pmxModel->bones[i].ikTargetBoneIndex = ReadIndex(stream, pmxModel->setting.boneIndexSize);
				stream->read((char*)&pmxModel->bones[i].ikLoop, sizeof(int));
				stream->read((char*)&pmxModel->bones[i].ikLoopAngleLimit, sizeof(float));
				stream->read((char*)&pmxModel->bones[i].ikLinkCount, sizeof(int));

				// サイズの確保
				pmxModel->bones[i].ikLinks.resize(pmxModel->bones[i].ikLinkCount);
				for (int j = 0; j < pmxModel->bones[i].ikLinkCount; ++j)
				{
					pmxModel->bones[i].ikLinks[j].linkTarget = ReadIndex(stream, pmxModel->setting.boneIndexSize);
					stream->read((char*) &pmxModel->bones[i].ikLinks[j].angleLock, sizeof(uint8_t));
					if (pmxModel->bones[i].ikLinks[j].angleLock == 1)
					{
						stream->read((char*)pmxModel->bones[i].ikLinks[j].maxRadian, sizeof(float) * 3);
						stream->read((char*)pmxModel->bones[i].ikLinks[j].minRadian, sizeof(float) * 3);
					}
				}
			}
		}
	}

	// <モーフ>
	{
		// モーフ数の取得
		stream->read((char*) &pmxModel->morphCount, sizeof(int));

		// サイズの確保
		pmxModel->morphs.resize(pmxModel->morphCount);

		// データの取得
		for (int i = 0; i < pmxModel->morphCount; ++i)
		{
			pmxModel->morphs[i].morphName = ReadString(stream, pmxModel->setting.encoding);
			pmxModel->morphs[i].morphEnglishName = ReadString(stream, pmxModel->setting.encoding);
			stream->read((char*)&pmxModel->morphs[i].category, sizeof(MorphCategory));
			stream->read((char*)&pmxModel->morphs[i].morphType, sizeof(MorphType));
			stream->read((char*) &pmxModel->morphs[i].offsetCount, sizeof(int));

			switch (pmxModel->morphs[i].morphType)
			{
			case MorphType::GROUP:
				// サイズの確保
				pmxModel->morphs[i].groupOffsets.resize(pmxModel->morphs[i].offsetCount);
				for (int j = 0; j < pmxModel->morphs[i].offsetCount; ++j)
				{
					pmxModel->morphs[i].groupOffsets[j].morphIndex = ReadIndex(stream, pmxModel->setting.morphIndexSize);
					stream->read((char*) &pmxModel->morphs[i].groupOffsets[j].morphWeight, sizeof(float));
				}
				break;
			case MorphType::VERTEX:
				// サイズの確保
				pmxModel->morphs[i].vertexOffsets.resize(pmxModel->morphs[i].offsetCount);
				for (int j = 0; j < pmxModel->morphs[i].offsetCount; ++j)
				{
					pmxModel->morphs[i].vertexOffsets[j].vertexIndex = ReadIndex(stream, pmxModel->setting.vertexIndexSize);
					stream->read((char*)pmxModel->morphs[i].vertexOffsets[j].positionOffset, sizeof(float) * 3);
				}
				break;
			case MorphType::BONE:
				// サイズの確保
				pmxModel->morphs[i].boneOffsets.resize(pmxModel->morphs[i].offsetCount);
				for (int j = 0; j < pmxModel->morphs[i].offsetCount; ++j)
				{
					pmxModel->morphs[i].boneOffsets[j].boneIndex = ReadIndex(stream, pmxModel->setting.boneIndexSize);
					stream->read((char*)pmxModel->morphs[i].boneOffsets[j].translation, sizeof(float) * 3);
					stream->read((char*)pmxModel->morphs[i].boneOffsets[j].rotation, sizeof(float) * 4);
				}
				break;
			case MorphType::MATERIAL:
				// サイズの確保
				pmxModel->morphs[i].materialOffsets.resize(pmxModel->morphs[i].offsetCount);
				for (int j = 0; j < pmxModel->morphs[i].offsetCount; ++j)
				{
					pmxModel->morphs[i].materialOffsets[j].materialIndex = ReadIndex(stream, pmxModel->setting.materialIndexSize);
					stream->read((char*) &pmxModel->morphs[i].materialOffsets[j].offsetOperation, sizeof(uint8_t));
					stream->read((char*)pmxModel->morphs[i].materialOffsets[j].diffuse, sizeof(float) * 4);
					stream->read((char*)pmxModel->morphs[i].materialOffsets[j].specular, sizeof(float) * 3);
					stream->read((char*) &pmxModel->morphs[i].materialOffsets[j].specularity, sizeof(float));
					stream->read((char*)pmxModel->morphs[i].materialOffsets[j].ambient, sizeof(float) * 3);
					stream->read((char*)pmxModel->morphs[i].materialOffsets[j].edgeColor, sizeof(float) * 4);
					stream->read((char*) &pmxModel->morphs[i].materialOffsets[j].edgeSize, sizeof(float));
					stream->read((char*)pmxModel->morphs[i].materialOffsets[j].textureArgb, sizeof(float) * 4);
					stream->read((char*)pmxModel->morphs[i].materialOffsets[j].sphereTextureArgb, sizeof(float) * 4);
					stream->read((char*)pmxModel->morphs[i].materialOffsets[j].toonTextureArgb, sizeof(float) * 4);
				}
				break;
			case MorphType::UV:
			case MorphType::ADDITIONAL_UV1:
			case MorphType::ADDITIONAL_UV2:
			case MorphType::ADDITIONAL_UV3:
			case MorphType::ADDITIONAL_UV4:
				// サイズの確保
				pmxModel->morphs[i].uvOffsets.resize(pmxModel->morphs[i].offsetCount);
				for (int j = 0; j < pmxModel->morphs[i].offsetCount; ++j)
				{
					pmxModel->morphs[i].uvOffsets[j].vertexIndex = ReadIndex(stream, pmxModel->setting.vertexIndexSize);
					stream->read((char*)pmxModel->morphs[i].uvOffsets[j].uvOffset, sizeof(float) * 4);
				}
				break;
			default:
				break;
			}
		}
	}

	// <表示枠>
	{
		// 表示枠数の取得
		stream->read((char*) &pmxModel->frameCount, sizeof(int));

		// サイズの確保
		pmxModel->frames.resize(pmxModel->frameCount);

		// データの取得
		for (int i = 0; i < pmxModel->frameCount; ++i)
		{
			pmxModel->frames[i].frameName = ReadString(stream, pmxModel->setting.encoding);
			pmxModel->frames[i].frameEnglishName = ReadString(stream, pmxModel->setting.encoding);
			stream->read((char*) &pmxModel->frames[i].frameFlag, sizeof(uint8_t));

			// 要素数の取得
			stream->read((char*) &pmxModel->frames[i].elementCount, sizeof(int));

			// サイズの確保
			pmxModel->frames[i].elements.resize(pmxModel->frames[i].elementCount);

			// データの取得
			for (int j = 0; j < pmxModel->frames[i].elementCount; ++j)
			{
				stream->read((char*) &pmxModel->frames[i].elements[j].elementTarget, sizeof(uint8_t));
				if (pmxModel->frames[i].elements[j].elementTarget == 0x00)
				{
					pmxModel->frames[i].elements[j].index = ReadIndex(stream, pmxModel->setting.boneIndexSize);
				}
				else
				{
					pmxModel->frames[i].elements[j].index = ReadIndex(stream, pmxModel->setting.morphIndexSize);
				}
			}
		}
	}

	// <剛体>
	{
		// 剛体数の取得
		stream->read((char*) &pmxModel->rigidbodyCount, sizeof(int));

		// サイズの確保
		pmxModel->rigidbodies.resize(pmxModel->rigidbodyCount);

		// データの取得
		for (int i = 0; i < pmxModel->rigidbodyCount; ++i)
		{
			pmxModel->rigidbodies[i].rigidbodyName = ReadString(stream, pmxModel->setting.encoding);
			pmxModel->rigidbodies[i].rigidbodyEnglishName = ReadString(stream, pmxModel->setting.encoding);
			pmxModel->rigidbodies[i].targetBone = ReadIndex(stream, pmxModel->setting.boneIndexSize);
			stream->read((char*)&pmxModel->rigidbodies[i].group, sizeof(uint8_t));
			stream->read((char*)&pmxModel->rigidbodies[i].mask, sizeof(uint16_t));
			stream->read((char*)&pmxModel->rigidbodies[i].shape, sizeof(uint8_t));
			stream->read((char*)pmxModel->rigidbodies[i].size, sizeof(float) * 3);
			stream->read((char*)pmxModel->rigidbodies[i].position, sizeof(float) * 3);
			stream->read((char*)pmxModel->rigidbodies[i].orientation, sizeof(float) * 3);
			stream->read((char*)&pmxModel->rigidbodies[i].mass, sizeof(float));
			stream->read((char*)&pmxModel->rigidbodies[i].moveAttenuation, sizeof(float));
			stream->read((char*)&pmxModel->rigidbodies[i].rotationAttenuation, sizeof(float));
			stream->read((char*)&pmxModel->rigidbodies[i].repulsion, sizeof(float));
			stream->read((char*)&pmxModel->rigidbodies[i].friction, sizeof(float));
			stream->read((char*)&pmxModel->rigidbodies[i].physicsCalcType, sizeof(uint8_t));
		}
	}

	// <ジョイント>
	{
		// ジョイント数の取得
		stream->read((char*) &pmxModel->jointCount, sizeof(int));

		// サイズの確保
		pmxModel->joints.resize(pmxModel->jointCount);

		// データの取得
		for (int i = 0; i < pmxModel->jointCount; ++i)
		{
			pmxModel->joints[i].jointName = ReadString(stream, pmxModel->setting.encoding);
			pmxModel->joints[i].jointEnglishName = ReadString(stream, pmxModel->setting.encoding);
			stream->read((char*) &pmxModel->joints[i].jointType, sizeof(uint8_t));

			pmxModel->joints[i].param.rigidbody[0] = ReadIndex(stream, pmxModel->setting.rigidbodyIndexSize);
			pmxModel->joints[i].param.rigidbody[1] = ReadIndex(stream, pmxModel->setting.rigidbodyIndexSize);
			stream->read((char*)pmxModel->joints[i].param.position, sizeof(float) * 3);
			stream->read((char*)pmxModel->joints[i].param.orientaiton, sizeof(float) * 3);
			stream->read((char*)pmxModel->joints[i].param.moveLimitationMin, sizeof(float) * 3);
			stream->read((char*)pmxModel->joints[i].param.moveLimitationMax, sizeof(float) * 3);
			stream->read((char*)pmxModel->joints[i].param.rotationLimitationMin, sizeof(float) * 3);
			stream->read((char*)pmxModel->joints[i].param.rotationLimitationMax, sizeof(float) * 3);
			stream->read((char*)pmxModel->joints[i].param.springMoveCoefficient, sizeof(float) * 3);
			stream->read((char*)pmxModel->joints[i].param.springRotationCoefficient, sizeof(float) * 3);
		}
	}

	// <ソフトボディ>
	{
		// バージョンのチェック
		if (pmxModel->version == 2.1f)
		{
			// ソフトボディ数の取得
			stream->read((char*) &pmxModel->softBodyCount, sizeof(int));

			// サイズの確保
			pmxModel->softBodies.resize(pmxModel->softBodyCount);

			// データの取得
			for (int i = 0; i < pmxModel->softBodyCount; ++i)
			{
				// 未実装
			}
		}
	}

	return pmxModel;
}



//--------------------------------------------------------------
//! @summary   文字列を読み込む
//!
//! @parameter [stream] 読み込まれたファイル
//! @parameter [encoding] エンコード
//!
//! @return    読み込んだ文字列
//--------------------------------------------------------------
wstring Graphic3D::Model::ModelManager::ReadString(istream* stream, uint8_t encoding)
{
	Utility::String::EncodingConverter converter = Utility::String::EncodingConverter();
	int size;
	stream->read((char*)&size, sizeof(int));
	std::vector<char> buffer;
	if (size == 0)
	{
		return wstring(L"");
	}
	buffer.reserve(size);
	stream->read((char*)buffer.data(), size);
	if (encoding == 0)
	{
		// UTF16
		return wstring((wchar_t*)buffer.data(), size / 2);
	}
	else
	{
		// UTF8
		wstring result;
		converter.Utf8ToUtf16(buffer.data(), size, &result);
		return result;
	}
}
