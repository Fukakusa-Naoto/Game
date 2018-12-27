//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		PMDData.h
//!
//! @summary	PMDモデル構造体のヘッダファイル
//!
//! @date		2018.10.22
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
	namespace Graphic3D
	{
		namespace Model
		{
			// 構造体の定義 ======================================================
			struct PMDHeader
			{
				char magic[3];					// "Pmd"
				float version;					// バージョン
				std::string modelName;			// モデル名
				std::string nameEnglish;		// モデル名(English)
				std::string comment;			// コメント
				std::string commentEnglish;		// コメント(English)
			};


			struct PMDVertex
			{
				float position[3];			// 座標
				float normal[3];			// 法線
				float uv[2];				// UV座標
				uint16_t boneIndex[2];		// ボーン番号
				uint8_t boneWeight;			// ボーンウェイト
				bool edgeInvisible;			// エッジ不可視 0:通常、1:エッジ無効
			};


			struct PMDMaterial
			{
				float diffuse[4];				// 減衰色
				float specularity;				// 光沢度
				float specular[3];				// 光沢色
				float ambient[3];				// 環境色
				uint8_t toonIndex;				// トーンインデックス
				uint8_t edgeFlag;				// 輪郭、影
				uint32_t indexCount;			// 面頂点数
				std::string textureFilename;	// テクスチャファイル名
				std::string sphereFilename;		// スフィアファイル名
			};


			enum PMDBoneType : uint8_t
			{
				PMD_ROTATION,				// 回転
				PMD_ROTATION_AND_MOVE,		// 回転と移動
				PMD_IK_EFFECTOR,			// IK
				PMD_UNKNOWN,				// 不明
				PMD_IK_EFFECTABLE,			// IK影響下
				PMD_ROTATION_EFFECTABLE,	// 回転影響下
				PMD_IK_TARGET,				// IK接続先
				PMD_INVISIBLE,				// 非表示
				PMD_TWIST,					// 捻り
				PMD_ROTATION_MOVEMENT		// 回転運動
			};


			struct PMDBone
			{
				std::string name;					// ボーン名
				std::string nameEnglish;			// ボーン名(English)
				uint16_t parentBoneIndex;			// 親ボーン番号　(ない場合は0xFFFF)
				uint16_t tailPosBoneIndex;			// 末端ボーン番号
				PMDBoneType boneType;					// ボーン種類
				uint16_t ikParentBoneIndex;			// IKボーン番号 (ない場合は0)
				float boneHeadPos[3];				// ボーンのヘッドの位置
			};


			struct PMDIk
			{
				uint16_t ikBoneIndex;							// IKボーン番号
				uint16_t targetBoneIndex;						// IKターゲットボーン番号
				uint16_t interations;							// 再帰回数
				float angleLimit;								// 角度制限
				std::vector<uint16_t> ikChildBoneIndex;			// 影響下ボーン番号
			};


			struct PMDFaceVertex
			{
				int index;				// 表情用の頂点の番号(頂点リストにある番号)
				float position[3];		// 表情用の頂点の座標(頂点自体の座標)
			};


			enum PMDFaceCategory : uint8_t
			{
				PMD_BASE,		// ベース
				PMD_EYEBROW,	// 眉
				PMD_EYE,		// 目
				PMD_MOUTH,		// 口
				PMD_OTHER,		// その他
			};


			struct PMDFace
			{
				std::string name;						// 表情名
				PMDFaceCategory type;					// 表情の種類
				std::vector<PMDFaceVertex> vertices;	// 表情用の頂点データ
				std::string nameEnglish;				// 表情名(English)
			};


			// <ボーン枠用の枠名>
			struct PMDBoneDispName
			{
				std::string boneDispName;
				std::string boneDispNameEnglish;
			};


			struct PMDBoneDisp
			{
				uint16_t boneIndex;
				uint8_t boneDispIndex;
			};


			// 衝突形状
			enum PMDRigidBodyShape : uint8_t
			{
				PMD_SPHERE = 0,		// 球
				PMD_BOX    = 1,		// 直方体
				PMD_CPUSEL = 2		// カプセル
			};


			// 剛体タイプ
			enum PMDRigidBodyType : uint8_t
			{
				PMD_BONE_CONNECTED    = 0,		// ボーン追従
				PMD_PHYSICS           = 1,		// 物理演算
				PMD_CONNECTED_PHYSICS = 2		// 物理演算(Bone位置合せ)
			};


			// 剛体
			struct PMDRigidBody
			{
				std::string name;				// 名前
				uint16_t relatedBoneIndex;		// 関連ボーン番号
				uint8_t groupIndex;				// グループ番号
				uint16_t mask;					// マスク
				PMDRigidBodyShape shape;		// 形状
				float size[3];					// 大きさ
				float position[3];				// 座標
				float orientation[3];			// 回転
				float weight;					// 質量
				float linearDamping;			// 移動ダンピング
				float anglarDamping;			// 回転ダンピング
				float restitution;				// 反発係数
				float friction;					// 摩擦係数
				PMDRigidBodyType rigidType;		// 演算方法
			};


			// 剛体の拘束
			struct PMDConstraint
			{
				std::string name;					// 名前
				uint32_t rigidBodyIndex[2];			// 剛体のインデックス
				float position[3];					// 座標
				float orientation[3];				// 回転
				float linearLowerLimit[3];			// 最小移動制限
				float linearUpperLimit[3];			// 最大移動制限
				float angularLowerLimit[3];			// 最小回転制限
				float angularUpperLimit[3];			// 最大回転制限
				float linearStiffness[3];			// 移動に対する復元力
				float angularStiffness[3];			// 回転に対する復元力
			};


			struct PMDModel
			{
				PMDHeader header;							// ヘッダー
				std::vector<PMDVertex> 	vertexes;			// 頂点データ
				std::vector<unsigned short> indices;		// インデックスデータ
				std::vector<PMDMaterial> materials;			// マテリアル
				std::vector<PMDBone> bones;					// ボーン
				std::vector<PMDIk> IKs;						// IKリスト
				std::vector<PMDFace> faces;					// 表情リスト
				std::vector<uint16_t> facesIndices;			// 表情インデックス
				std::vector<PMDBoneDispName> boneDispName;	// ボーン枠用枠名リスト
				std::vector<PMDBoneDisp> boneDisp;
				std::vector<std::string> toonFilenames;
				std::vector<PMDRigidBody> rigidbodies;		// 剛体リスト
				std::vector<PMDConstraint> constraints;		// 剛体の拘束リスト
			};

		}
	}
}
