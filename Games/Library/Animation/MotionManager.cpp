//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		MotionManager.cpp
//!
//! @summary	モーションデータ管理クラスのソースファイル
//!
//! @date		2018.10.26
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み =================================================
// <標準ヘッダファイル>
#include <fstream>

// <自作ヘッダファイル>
#include "MotionManager.h"


// usingディレクティブ =====================================================
using namespace std;
using namespace Library;


// 関数の定義 ==============================================================
//--------------------------------------------------------------------
//! @summary   コンストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Animation::MotionManager::MotionManager()
{
	// 何もしない
}



//--------------------------------------------------------------------
//! @summary   デストラクタ
//!
//! @parameter [void] なし
//--------------------------------------------------------------------
Animation::MotionManager::~MotionManager()
{
	// 何もしない
}



//--------------------------------------------------------------------
//! @summary   VMDのモーションデータの読み込み処理
//!
//! @parameter [fileName] 読み込むモーションデータのファイル名
//!
//! @return    モーションデータのインスタンス
//--------------------------------------------------------------------
void Animation::MotionManager::LoadMotionDataFromVMD(const wchar_t* fileName)
{
	//if (m_modelMap.count(fileName) == 0)
	{
		// ファイルまでのディレクトリパスを作成
		wstring fullPass = wstring(L"Resources/Motions/") + fileName + wstring(L".vmd");

		// ファイルを開く
		std::ifstream stream(fullPass, std::ios::binary);

		// 失敗
		if (stream.fail()) return /*nullptr*/;

		// データの読み込み
		VMDMotion* motion = CreateVMDMotion(&stream);

		// ファイルを閉じる
		stream.close();

		// コンテナに保存
		//m_modelMap[fileName] = new Model(model, fileName);

		// モデルデータの削除
		delete motion;
	}

	//return m_modelMap[fileName];
}



//--------------------------------------------------------------
//! @summary   ファイルからVMDMotionを生成する
//!
//! @parameter [stream] 読み込まれたファイル
//!
//! @return    生成されたVMDMotion
//--------------------------------------------------------------
Animation::VMDMotion* Animation::MotionManager::CreateVMDMotion(ifstream* stream)
{
	VMDMotion* vmdMotion = new VMDMotion();

	// <マジックとバージョン>
	{
		char buffer[30];
		stream->read((char*)buffer, 30);

		// 無効なVMDファイル
		if (strncmp(buffer, "Vocaloid Motion Data", 20)) return nullptr;
		vmdMotion->version = std::atoi(buffer + 20);
	}

	// <名前>
	{
		char buffer[20];
		stream->read(buffer, 20);
		vmdMotion->motionName = std::string(buffer);
	}

	// <ボーンフレーム>
	{
		// ボーンフレーム数の取得
		int boneFrameNumber;
		stream->read((char*)&boneFrameNumber, sizeof(int));

		// サイズの確保
		vmdMotion->boneFrames.resize(boneFrameNumber);

		// データの取得
		for (int i = 0; i < boneFrameNumber; ++i)
		{
			char buffer[15];
			stream->read((char*)buffer, sizeof(char) * 15);
			vmdMotion->boneFrames[i].name = std::string(buffer);
			stream->read((char*)&vmdMotion->boneFrames[i].frame, sizeof(int));
			stream->read((char*)vmdMotion->boneFrames[i].position, sizeof(float) * 3);
			stream->read((char*)vmdMotion->boneFrames[i].orientation, sizeof(float) * 4);
			stream->read((char*)vmdMotion->boneFrames[i].interpolation, sizeof(char) * 4 * 4 * 4);
		}
	}

	// <フェイスフレーム>
	{
		// フェイスフレーム数の取得
		int faceFrameNumber;
		stream->read((char*)&faceFrameNumber, sizeof(int));

		// サイズの確保
		vmdMotion->faceFrames.resize(faceFrameNumber);

		// データの取得
		for (int i = 0; i < faceFrameNumber; ++i)
		{
			char buffer[15];
			stream->read((char*)&buffer, sizeof(char) * 15);
			vmdMotion->faceFrames[i].faceName = std::string(buffer);
			stream->read((char*)&vmdMotion->faceFrames[i].frame, sizeof(int));
			stream->read((char*)&vmdMotion->faceFrames[i].weight, sizeof(float));
		}
	}

	// <カメラフレーム>
	{
		// カメラフレーム数の取得
		int cameraFrameNumber;
		stream->read((char*)&cameraFrameNumber, sizeof(int));

		// サイズの確保
		vmdMotion->cameraFrames.resize(cameraFrameNumber);

		// データの取得
		for (int i = 0; i < cameraFrameNumber; ++i)
		{
			stream->read((char*)&vmdMotion->cameraFrames[i].frame, sizeof(int));
			stream->read((char*)&vmdMotion->cameraFrames[i].distance, sizeof(float));
			stream->read((char*)vmdMotion->cameraFrames[i].position, sizeof(float) * 3);
			stream->read((char*)vmdMotion->cameraFrames[i].orientation, sizeof(float) * 3);
			stream->read((char*)vmdMotion->cameraFrames[i].interpolation, sizeof(char) * 24);
			stream->read((char*)&vmdMotion->cameraFrames[i].angle, sizeof(float));
			stream->read((char*)vmdMotion->cameraFrames[i].unknown, sizeof(char) * 3);
		}
	}

	// <ライトフレーム>
	{
		// ライトフレーム数の取得
		int lightFrameNumber;
		stream->read((char*)&lightFrameNumber, sizeof(int));

		// サイズの確保
		vmdMotion->lightFrames.resize(lightFrameNumber);

		// データの取得
		for (int i = 0; i < lightFrameNumber; ++i)
		{
			stream->read((char*)&vmdMotion->lightFrames[i].frame, sizeof(int));
			stream->read((char*)vmdMotion->lightFrames[i].color, sizeof(float) * 3);
			stream->read((char*)vmdMotion->lightFrames[i].position, sizeof(float) * 3);
		}
	}

	// 不明なデータ
	{
		char buffer[4];
		stream->read(buffer, 4);
	}

	// <IKフレーム>
	{
		if (stream->peek() != std::ios::traits_type::eof())
		{
			// IKフレーム数の取得
			int ikNumber;
			stream->read((char*)&ikNumber, sizeof(int));

			// サイズの確保
			vmdMotion->ikFrames.resize(ikNumber);

			// データの取得
			for (int i = 0; i < ikNumber; ++i)
			{
				stream->read((char*)&vmdMotion->ikFrames[i].frame, sizeof(int));
				stream->read((char*)&vmdMotion->ikFrames[i].display, sizeof(uint8_t));

				// IK数の取得
				int ikCount;
				stream->read((char*)&ikCount, sizeof(int));

				// サイズの確保
				vmdMotion->ikFrames[i].ikEnable.resize(ikCount);

				// データの取得
				for (int j = 0; j < ikCount; j++)
				{
					char buffer[20];
					stream->read(buffer, 20);
					vmdMotion->ikFrames[i].ikEnable[j].ikName = std::string(buffer);
					stream->read((char*)&vmdMotion->ikFrames[i].ikEnable[j].enable, sizeof(uint8_t));
				}
			}
		}
	}


	// VMDストリームに不明なデータがある
	if (stream->peek() != std::ios::traits_type::eof()) return nullptr;

	return vmdMotion;
}