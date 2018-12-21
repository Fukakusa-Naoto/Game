//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file		PMXData.h
//!
//! @summary	PMXモデル構造体のヘッダファイル
//!
//! @date		2018.10.25
//!
//! @author		深草直斗
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// 多重インクルードの防止 =========================================================
#pragma once


// ヘッダファイルの読み込み =======================================================
// <標準ヘッダファイル>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>


// 名前空間の定義 ================================================================
namespace Library
{
	namespace Graphic3D
	{
		namespace Model
		{
			// 関数の宣言 ========================================================
			int ReadIndex(std::istream *stream, int size);


			// 構造体の定義 ======================================================
			// インデックス設定
			struct PMXSetting
			{
				// <変数>
				// エンコード方式
				uint8_t encoding;
				// 追加UV数
				uint8_t uv;
				// 頂点インデックスサイズ
				uint8_t vertexIndexSize;
				// テクスチャインデックスサイズ
				uint8_t textureIndexSize;
				// マテリアルインデックスサイズ
				uint8_t materialIndexSize;
				// ボーンインデックスサイズ
				uint8_t boneIndexSize;
				// モーフインデックスサイズ
				uint8_t morphIndexSize;
				// 剛体インデックスサイズ
				uint8_t rigidbodyIndexSize;


				// <コンストラクタ>
				PMXSetting() :
					encoding(0),
					uv(0),
					vertexIndexSize(0),
					textureIndexSize(0),
					materialIndexSize(0),
					boneIndexSize(0),
					morphIndexSize(0),
					rigidbodyIndexSize(0)
				{
					// 何もしない
				}
			};

			// 頂点スキニングタイプ
			enum PMXVertexSkinningType : uint8_t
			{
				BDEF1 = 0,	// ボーンのみ
				BDEF2 = 1,	// ボーン2つと、ボーン1のウェイト値(PMD方式)
				BDEF4 = 2,	// ボーン4つと、それぞれのウェイト値。ウェイト合計が1.0である保障はしない
				SDEF  = 3,	// BDEF2に加え、SDEF用のfloat3(Vector3)が3つ。実際の計算ではさらに補正値の算出が必要(一応そのままBDEF2としても使用可能)と可変サイズになるので注意。
				QDEF  = 4,
			};

			// 頂点スキニング
			class PMXVertexSkinning
			{
			public:
				virtual void Read(std::istream *stream, PMXSetting* setting) = 0;
			};

			class PMXVertexSkinningBDEF1 : public PMXVertexSkinning
			{
				// <変数>
			public:
				// 参照するボーンのインデックス
				int boneIndex;


				// <コンストラクタ>
			public:
				PMXVertexSkinningBDEF1() :
					boneIndex(0)
				{
					// 何もしない
				}


				// <メンバ関数>
			public:
				void Read(std::istream *stream, PMXSetting* setting) override
				{
					this->boneIndex = ReadIndex(stream, setting->boneIndexSize);
				}
			};

			class PMXVertexSkinningBDEF2 : public PMXVertexSkinning
			{
				// <変数>
			public:
				// 参照するボーンのインデックス
				int boneIndex[2];
				// ボーンのウェイト
				float boneWeight;


				// <コンストラクタ>
			public:
				PMXVertexSkinningBDEF2() :
					boneWeight(0.0f)
				{
					boneIndex[0] = boneIndex[1] = 0;
				}


				// <メンバ関数>
			public:
				void Read(std::istream *stream, PMXSetting* setting) override
				{
					this->boneIndex[0] = ReadIndex(stream, setting->boneIndexSize);
					this->boneIndex[1] = ReadIndex(stream, setting->boneIndexSize);
					stream->read((char*)&this->boneWeight, sizeof(float));
				}
			};

			class PMXVertexSkinningBDEF4 : public PMXVertexSkinning
			{
				// <変数>
			public:
				// 参照するボーンのインデックス
				int boneIndex[4];
				// ボーンのウェイト
				float boneWeight[4];


				// <コンストラクタ>
			public:
				PMXVertexSkinningBDEF4()
				{
					boneIndex[0] = boneIndex[1] = boneIndex[2] = boneIndex[3] = 0;
					boneWeight[0] = boneWeight[1] = boneWeight[2] = boneWeight[3] = 0.0f;
				}


				// <メンバ関数>
			public:
				void Read(std::istream *stream, PMXSetting* setting) override
				{
					this->boneIndex[0] = ReadIndex(stream, setting->boneIndexSize);
					this->boneIndex[1] = ReadIndex(stream, setting->boneIndexSize);
					this->boneIndex[2] = ReadIndex(stream, setting->boneIndexSize);
					this->boneIndex[3] = ReadIndex(stream, setting->boneIndexSize);
					stream->read((char*)&this->boneWeight[0], sizeof(float));
					stream->read((char*)&this->boneWeight[1], sizeof(float));
					stream->read((char*)&this->boneWeight[2], sizeof(float));
					stream->read((char*)&this->boneWeight[3], sizeof(float));
				}
			};

			class PMXVertexSkinningSDEF : public PMXVertexSkinning
			{
				// <変数>
			public:
				// 参照するボーンのインデックス
				int boneIndex[2];
				// ボーン0のウェイト
				float boneWeight;

				float sdefC[3];
				float sdefR0[3];
				float sdefR1[3];


				// <コンストラクタ>
				PMXVertexSkinningSDEF() :
					boneWeight(0.0f)
				{
					boneIndex[0] = boneIndex[1] = 0;

					for (int i = 0; i < 3; ++i)
					{
						sdefC[i] = 0.0f;
						sdefR0[i] = 0.0f;
						sdefR1[i] = 0.0f;
					}
				}


				// <メンバ関数>
			public:
				void Read(std::istream *stream, PMXSetting* setting) override
				{
					this->boneIndex[0] = ReadIndex(stream, setting->boneIndexSize);
					this->boneIndex[1] = ReadIndex(stream, setting->boneIndexSize);
					stream->read((char*) &this->boneWeight, sizeof(float));
					stream->read((char*) this->sdefC, sizeof(float) * 3);
					stream->read((char*) this->sdefR0, sizeof(float) * 3);
					stream->read((char*) this->sdefR1, sizeof(float) * 3);
				}
			};

			class PMXVertexSkinningQDEF : public PMXVertexSkinning
			{

				// <変数>
			public:
				// 参照するボーンのインデックス
				int boneIndex[4];
				// ボーンのウェイト
				float boneWeight[4];


				// <コンストラクタ>
			public:
				PMXVertexSkinningQDEF()
				{
					boneIndex[0] = boneIndex[1] = boneIndex[2] = boneIndex[3] = 0;
					boneWeight[0] = boneWeight[1] = boneWeight[2] = boneWeight[3] = 0.0f;
				}


				// <メンバ関数>
			public:
				void Read(std::istream *stream, PMXSetting* setting) override
				{
					this->boneIndex[0] = ReadIndex(stream, setting->boneIndexSize);
					this->boneIndex[1] = ReadIndex(stream, setting->boneIndexSize);
					this->boneIndex[2] = ReadIndex(stream, setting->boneIndexSize);
					this->boneIndex[3] = ReadIndex(stream, setting->boneIndexSize);
					stream->read((char*)&this->boneWeight[0], sizeof(float));
					stream->read((char*)&this->boneWeight[1], sizeof(float));
					stream->read((char*)&this->boneWeight[2], sizeof(float));
					stream->read((char*)&this->boneWeight[3], sizeof(float));
				}
			};

			// 頂点
			struct PMXVertex
			{
				// <変数>
				// 座標
				float position[3];
				// 法線
				float normal[3];
				// テクスチャ座標
				float uv[2];
				// 追加テクスチャ座標
				float uvAddition[4][4];
				// スキニングタイプ
				PMXVertexSkinningType skinningType;
				// スキニング
				PMXVertexSkinning* skinning;
				// エッジ倍率
				float edge;


				// <コンストラクタ>
				PMXVertex() :
					edge(0.0f)
				{
					uv[0] = uv[1] = 0.0f;

					for (int i = 0; i < 3; ++i)
					{
						position[i] = 0.0f;
						normal[i] = 0.0f;
					}

					for (int i = 0; i < 4; ++i)
					{
						for (int j = 0; j < 4; ++j)
						{
							uvAddition[i][j] = 0.0f;
						}
					}
				}


				// <デストラクタ>
				~PMXVertex()
				{
					delete skinning;
					skinning = nullptr;
				}
			};

			// マテリアル
			struct PMXMaterial
			{
				// <変数>
				// マテリアル名
				std::wstring materialName;
				// マテリアル英名
				std::wstring materialEnglishName;
				// 減衰色
				float diffuse[4];
				// 光沢色
				float specular[3];
				// 光沢度
				float specularlity;
				// 環境色
				float ambient[3];
				// 描画フラグ
				uint8_t flag;
				// エッジ色
				float edgeColor[4];
				// エッジサイズ
				float edgeSize;
				// アルベドテクスチャインデックス
				int diffuseTextureIndex;
				// スフィアテクスチャインデックス
				int sphereTextureIndex;
				// スフィアテクスチャ演算モード
				uint8_t sphereOpMode;
				// 共有トゥーンフラグ
				uint8_t commonToonFlag;
				// トゥーンテクスチャインデックス
				int toonTextureIndex;
				// メモ
				std::wstring memo;
				// 頂点インデックス数
				int indexCount;


				// <コンストラクタ>
				PMXMaterial() :
					specularlity(0.0f),
					flag(0),
					edgeSize(0.0f),
					diffuseTextureIndex(0),
					sphereTextureIndex(0),
					sphereOpMode(0),
					commonToonFlag(0),
					toonTextureIndex(0),
					indexCount(0)
				{
					for (int i = 0; i < 3; ++i)
					{
						specular[i] = 0.0f;
						ambient[i] = 0.0f;
						edgeColor[i] = 0.0f;
					}

					for (int i = 0; i < 4; ++i)
					{
						diffuse[i] = 0.0f;
					}
				}
			};

			// リンク
			struct PMXIkLink
			{
				// <変数>
				// リンクボーンインデックス
				int linkTarget;
				// 角度制限
				uint8_t angleLock;
				// 最大制限角度
				float maxRadian[3];
				// 最小制限角度
				float minRadian[3];


				// <コンストラクタ>
				PMXIkLink() :
					linkTarget(0),
					angleLock(0)
				{
					for (int i = 0; i < 3; ++i)
					{
						maxRadian[i] = 0.0f;
						minRadian[i] = 0.0f;
					}
				}
			};

			// ボーン
			struct PMXBone
			{
				// <変数>
				// ボーン名
				std::wstring boneName;
				// ボーン英名
				std::wstring boneEnglishName;
				// 座標
				float position[3];
				// 親ボーンインデックス
				int parentIndex;
				// 階層
				int level;
				// ボーンフラグ
				uint16_t boneFlag;
				// 座標オフセット(has Target)
				float offset[3];
				// 接続先ボーンインデックス(not has Target)
				int targetIndex;
				// 付与親ボーンインデックス
				int grantParentIndex;
				// 付与率
				float grantWeight;
				// 固定軸の方向
				float lockAxisOrientation[3];
				// ローカル軸のX軸方向
				float localAxisOrientationX[3];
				// ローカル軸のY軸方向
				float localAxisOrientationY[3];
				// 外部親変形のkey値
				int key;
				// IKターゲットボーン
				int ikTargetBoneIndex;
				// IKループ回数
				int ikLoop;
				// IKループ計算時の角度制限(ラジアン)
				float ikLoopAngleLimit;
				// IKリンク数
				int ikLinkCount;
				// IKリンク
				std::vector<PMXIkLink> ikLinks;


				// <コンストラクタ>
				PMXBone() :
					parentIndex(0),
					level(0),
					boneFlag(0),
					targetIndex(0),
					grantParentIndex(0),
					grantWeight(0.0f),
					key(0),
					ikTargetBoneIndex(0),
					ikLoop(0),
					ikLoopAngleLimit(0.0f),
					ikLinkCount(0)
				{
					for (int i = 0; i < 3; ++i)
					{
						position[i] = 0.0f;
						offset[i] = 0.0f;
						lockAxisOrientation[i] = 0.0f;
						localAxisOrientationX[i] = 0.0f;
						localAxisOrientationY[i] = 0.0f;
					}
				}
			};

			enum MorphType : uint8_t
			{
				GROUP           = 0,
				VERTEX          = 1,
				BONE            = 2,
				UV              = 3,
				ADDITIONAL_UV1  = 4,
				ADDITIONAL_UV2  = 5,
				ADDITIONAL_UV3  = 6,
				ADDITIONAL_UV4  = 7,
				MATERIAL        = 8,
				FLIP            = 9,
				IMPULSE         = 10,
			};

			enum MorphCategory : uint8_t
			{
				RESERVED_CATEGORY = 0,
				EYEBROW           = 1,
				EYE               = 2,
				MOUTH             = 3,
				OTHER             = 4,
			};

			struct PMXMorphOffset
			{
			};

			struct PMXMorphVertexOffset : public PMXMorphOffset
			{
				// <変数>
				// 頂点インデックス
				int vertexIndex;
				// 座標オフセット量
				float positionOffset[3];


				// <コンストラクタ>
				PMXMorphVertexOffset() :
					vertexIndex(0)
				{
					for (int i = 0; i < 3; ++i)
					{
						positionOffset[i] = 0.0f;
					}
				}
			};

			struct PMXMorphUVOffset : public PMXMorphOffset
			{
				// <変数>
				// 頂点インデックス
				int vertexIndex;
				// UVオフセット量
				float uvOffset[4];


				// <コンストラクタ>
				PMXMorphUVOffset() :
					vertexIndex(0)
				{
					for (int i = 0; i < 4; ++i)
					{
						uvOffset[i] = 0.0f;
					}
				}
			};

			struct PMXMorphBoneOffset : public PMXMorphOffset
			{
				// <変数>
				int boneIndex;
				float translation[3];
				float rotation[4];


				// <コンストラクタ>
				PMXMorphBoneOffset() :
					boneIndex(0)
				{
					for (int i = 0; i < 3; ++i)
					{
						translation[i] = 0.0f;
					}

					for (int i = 0; i < 4; ++i)
					{
						rotation[i] = 0.0f;
					}
				}
			};

			struct PMXMorphMaterialOffset : public PMXMorphOffset
			{
				// 変数
				// マテリアルインデックス
				int materialIndex;
				// オフセット演算形式
				uint8_t offsetOperation;
				// 減衰色
				float diffuse[4];
				// 光沢色
				float specular[3];
				// 光沢度
				float specularity;
				// 環境色
				float ambient[3];
				// エッジ色
				float edgeColor[4];
				// エッジサイズ
				float edgeSize;
				// テクスチャ係数
				float textureArgb[4];
				// スフィアテクスチャ係数
				float sphereTextureArgb[4];
				// トゥーンテクスチャ係数
				float toonTextureArgb[4];


				// <コンストラクタ>
				PMXMorphMaterialOffset() :
					specularity(0.0f),
					edgeSize(0.0f)
				{
					for (int i = 0; i < 3; ++i)
					{
						specular[i] = 0.0f;
						ambient[i] = 0.0f;
					}

					for (int i = 0; i < 4; ++i)
					{
						diffuse[i] = 0.0f;
						edgeColor[i] = 0.0f;
						textureArgb[i] = 0.0f;
						sphereTextureArgb[i] = 0.0f;
						toonTextureArgb[i] = 0.0f;
					}
				}
			};

			struct PMXMorphGroupOffset : public PMXMorphOffset
			{
				// <変数>
				// モーフインデックス
				int morphIndex;
				// モーフ率
				float morphWeight;


				// <コンストラクタ>
				PMXMorphGroupOffset() :
					morphIndex(0),
					morphWeight(0.0f)
				{
					// 何もしない
				}
			};

			struct PMXMorphFlipOffset : public PMXMorphOffset
			{
				// <変数>
				// モーフインデックス
				int morphIndex;
				float morphValue;


				// <コンストラクタ>
				PMXMorphFlipOffset() :
					morphIndex(0),
					morphValue(0.0f)
				{
					// 何もしない
				}
			};

			struct PMXMorphImpulseOffset : public PMXMorphOffset
			{
				// <変数>
				int rigidbodyIndex;
				uint8_t isLocal;
				float velocity[3];
				float angularTorque[3];


				// <コンストラクタ>
				PMXMorphImpulseOffset() :
					rigidbodyIndex(0),
					isLocal(0)
				{
					for (int i = 0; i < 3; ++i)
					{
						velocity[i] = 0.0f;
						angularTorque[i] = 0.0f;
					}
				}
			};

			// モーフ
			struct PMXMorph
			{
				// <変数>
				// モーフ名
				std::wstring morphName;
				// モーフ英名
				std::wstring morphEnglishName;
				// カテゴリ
				MorphCategory category;
				// モーフタイプ
				MorphType morphType;
				// オフセット数
				int offsetCount;
				// 頂点モーフ配列
				std::vector<PMXMorphVertexOffset> vertexOffsets;
				// UVモーフ配列
				std::vector<PMXMorphUVOffset> uvOffsets;
				// ボーンモーフ配列
				std::vector<PMXMorphBoneOffset> boneOffsets;
				// マテリアルモーフ配列
				std::vector<PMXMorphMaterialOffset> materialOffsets;
				// グループモーフ配列
				std::vector<PMXMorphGroupOffset> groupOffsets;
				// フリップモーフ配列
				std::vector<PMXMorphFlipOffset> flipOffsets;
				// インパルスモーフ配列
				std::vector<PMXMorphImpulseOffset> ImpulseOffsets;


				// <コンストラクタ>
				PMXMorph() :
					offsetCount(0)
				{
					// 何もしない
				}
			};

			// 枠内要素
			struct PMXFrameElement
			{
				// <変数>
				// 要素対象
				uint8_t elementTarget;
				// 要素対象インデックス
				int index;


				// <コンストラクタ>
				PMXFrameElement() :
					elementTarget(0),
					index(0)
				{
					// 何もしない
				}
			};

			// 表示枠
			struct PMXFrame
			{
				// <変数>
				// 枠名
				std::wstring frameName;
				// 枠英名
				std::wstring frameEnglishName;
				// 特殊枠フラグ
				uint8_t frameFlag;
				// 枠内要素数
				int elementCount;
				// 枠内要素配列
				std::vector<PMXFrameElement> elements;


				// <コンストラクタ>
				PMXFrame() :
					frameFlag(0),
					elementCount(0)
				{
					// 何もしない
				}
			};

			struct PMXRigidbody
			{
				// <変数>
				// 剛体名
				std::wstring rigidbodyName;
				// 剛体英名
				std::wstring rigidbodyEnglishName;
				// 関連ボーンインデックス
				int targetBone;
				// グループ
				uint8_t group;
				// マスク
				uint16_t mask;
				// 形状
				uint8_t shape;
				// サイズ
				float size[3];
				// 座標
				float position[3];
				// 回転
				float orientation[3];
				// 質量
				float mass;
				// 移動減衰
				float moveAttenuation;
				// 回転減衰
				float rotationAttenuation;
				// 反発力
				float repulsion;
				// 摩擦力
				float friction;
				// 剛体の物理演算
				uint8_t physicsCalcType;


				// <コンストラクタ>
				PMXRigidbody() :
					targetBone(0),
					group(0),
					mask(0),
					shape(0),
					mass(0.0f),
					moveAttenuation(0.0f),
					rotationAttenuation(0.0f),
					repulsion(0.0f),
					friction(0.0f),
					physicsCalcType(0)
				{
					for (int i = 0; i < 3; ++i)
					{
						size[i] = 0.0f;
						position[i] = 0.0f;
						orientation[i] = 0.0f;
					}
				}
			};

			enum PMXJointType : uint8_t
			{
				GENERIC_6_DOF_SPRING = 0,
				GENERIC_6_DOF        = 1,
				POINT_2_POINT        = 2,
				CONE_TWIST           = 3,
				SLIDER               = 5,
				HINGE                = 6
			};

			struct PMXJointParam
			{
				// <変数>
				int rigidbody[2];
				float position[3];
				float orientaiton[3];
				float moveLimitationMin[3];
				float moveLimitationMax[3];
				float rotationLimitationMin[3];
				float rotationLimitationMax[3];
				float springMoveCoefficient[3];
				float springRotationCoefficient[3];


				// <コンストラクタ>
				PMXJointParam()
				{
					rigidbody[0] = rigidbody[1] = 0;
					for (int i = 0; i < 3; ++i)
					{
						position[i] = 0.0f;
						orientaiton[i] = 0.0f;
						moveLimitationMin[i] = 0.0f;
						moveLimitationMax[i] = 0.0f;
						rotationLimitationMin[i] = 0.0f;
						rotationLimitationMax[i] = 0.0f;
						springMoveCoefficient[i] = 0.0f;
						springRotationCoefficient[i] = 0.0f;
					}
				}
			};

			struct PMXJoint
			{
				// <変数>
				std::wstring jointName;
				std::wstring jointEnglishName;
				PMXJointType jointType;
				PMXJointParam param;
			};

			enum PMXSoftBodyFlag : uint8_t
			{
				B_LINK  = 0x01,
				CLUSTER = 0x02,
				LINK    = 0x04
			};

			struct PMXAnchorRigidbody
			{
				// <変数>
				int relatedRigidbody;
				int relatedVertex;
				bool isNear;


				// <コンストラクタ>
				PMXAnchorRigidbody() :
					relatedRigidbody(0),
					relatedVertex(0),
					isNear(false)
				{
					// 何もしない
				}
			};

			struct PMXSoftBody
			{
				std::wstring softBodyName;
				std::wstring softBodyEnglishName;
				uint8_t shape;
				int targetMaterial;
				uint8_t group;
				uint16_t mask;
				PMXSoftBodyFlag flag;
				int blinkDistance;
				int clusterCount;
				float mass;
				float collisioniMargin;
				int aeroModel;
				float VCF;
				float DP;
				float DG;
				float LF;
				float PR;
				float VC;
				float DF;
				float MT;
				float CHR;
				float KHR;
				float SHR;
				float AHR;
				float SRHR_CL;
				float SKHR_CL;
				float SSHR_CL;
				float SR_SPLT_CL;
				float SK_SPLT_CL;
				float SS_SPLT_CL;
				int V_IT;
				int P_IT;
				int D_IT;
				int C_IT;
				float LST;
				float AST;
				float VST;
				int anchorCount;
				std::vector<PMXAnchorRigidbody> anchors;
				int pinVertexCount;
				std::vector<int> pinvertexes;


				// <コンストラクタ>
				PMXSoftBody() :
					shape(0),
					targetMaterial(0),
					group(0),
					mask(0),
					blinkDistance(0),
					clusterCount(0),
					mass(0.0),
					collisioniMargin(0.0),
					aeroModel(0),
					VCF(0.0f),
					DP(0.0f),
					DG(0.0f),
					LF(0.0f),
					PR(0.0f),
					VC(0.0f),
					DF(0.0f),
					MT(0.0f),
					CHR(0.0f),
					KHR(0.0f),
					SHR(0.0f),
					AHR(0.0f),
					SRHR_CL(0.0f),
					SKHR_CL(0.0f),
					SSHR_CL(0.0f),
					SR_SPLT_CL(0.0f),
					SK_SPLT_CL(0.0f),
					SS_SPLT_CL(0.0f),
					V_IT(0),
					P_IT(0),
					D_IT(0),
					C_IT(0),
					LST(0.0f),
					AST(0.0f),
					VST(0.0f),
					anchorCount(0),
					pinVertexCount(0)
				{
					// 何もしない
				}
			};

			// PMXモデル
			struct PMXModel
			{
				// <変数>
				// バージョン
				float version;
				// 設定
				PMXSetting setting;
				// モデル名
				std::wstring modelName;
				// モデル英名
				std::wstring modelEnglishName;
				// コメント
				std::wstring modelComment;
				// 英語コメント
				std::wstring modelEnglishComment;
				// 頂点数
				int vertexCount;
				// 頂点配列
				std::vector<PMXVertex> vertexes;
				// インデックス数
				int indexCount;
				// インデックス配列
				std::vector<int> indices;
				// テクスチャ数
				int textureCount;
				// テクスチャ配列
				std::vector<std::wstring> textures;
				// マテリアル数
				int materialCount;
				// マテリアル
				std::vector<PMXMaterial> materials;
				// ボーン数
				int boneCount;
				// ボーン配列
				std::vector<PMXBone> bones;
				// モーフ数
				int morphCount;
				// モーフ配列
				std::vector<PMXMorph> morphs;
				// 表示枠数
				int frameCount;
				// 表示枠配列
				std::vector<PMXFrame> frames;
				// 剛体数
				int rigidbodyCount;
				// 剛体配列
				std::vector<PMXRigidbody> rigidbodies;
				// ジョイント数
				int jointCount;
				// ジョイント配列
				std::vector<PMXJoint> joints;
				// ソフトボディ数
				int softBodyCount;
				// ソフトボディ配列
				std::vector<PMXSoftBody> softBodies;


				// <コンストラクタ>
				PMXModel() :
					version(0.0f),
					vertexCount(0),
					indexCount(0),
					textureCount(0),
					materialCount(0),
					boneCount(0),
					morphCount(0),
					frameCount(0),
					rigidbodyCount(0),
					jointCount(0),
					softBodyCount(0)
				{
					// 何もしない
				}


				// <デストラクタ>
				~PMXModel()
				{
					vertexes.clear();
				}
			};
		}
	}
}