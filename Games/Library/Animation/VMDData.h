//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		VMDData.h
//!
//! @summary	VMDモーション構造体のヘッダファイル
//!
//! @date		2018.10.26
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 =========================================================
#pragma once


// ヘッダファイルの読み込み =======================================================
// <標準ヘッダファイル>
#include <vector>
#include <string>


// 名前空間の定義 ================================================================
namespace Library
{
	namespace Animation
	{
		// 構造体の定義 ==========================================================
		// ボーンフレーム
		struct VMDBoneFrame
		{
			// ボーン名
			std::string name;
			// フレーム番号
			int frame;
			// 位置
			float position[3];
			// 回転
			float orientation[4];
			// 補間曲線
			char interpolation[4][4][4];
		};

		// 表情フレーム
		struct VMDFaceFrame
		{
			// 表情名
			std::string faceName;
			// 表情の重み
			float weight;
			// フレーム番号
			uint32_t frame;
		};

		// カメラフレーム
		struct VMDCameraFrame
		{
			// フレーム番号
			int frame;
			// 距離
			float distance;
			// 位置
			float position[3];
			// 回転
			float orientation[3];
			// 補間曲線
			char interpolation[6][4];
			// 視野角
			float angle;
			// 不明データ
			char unknown[3];
		};

		// ライトフレーム
		struct VMDLightFrame
		{
			// フレーム番号
			int frame;
			// 色
			float color[3];
			// 位置
			float position[3];
		};

		// IKの有効無効
		struct VMDIkEnable
		{
			std::string ikName;
			bool enable;
		};

		// IKフレーム
		struct VMDIkFrame
		{
			int frame;
			bool display;
			std::vector<VMDIkEnable> ikEnable;
		};

		// VMDモーション
		struct VMDMotion
		{
			// モーション名
			std::string motionName;
			// バージョン
			int version;
			// ボーンフレーム
			std::vector<VMDBoneFrame> boneFrames;
			// 表情フレーム
			std::vector<VMDFaceFrame> faceFrames;
			// カメラフレーム
			std::vector<VMDCameraFrame> cameraFrames;
			// ライトフレーム
			std::vector<VMDLightFrame> lightFrames;
			// IKフレーム
			std::vector<VMDIkFrame> ikFrames;
		};
	}
}