#pragma once

#include "SceneBase.h"
#include "../program/Camera.h"
#include "../program/Base.h"
#include "Player.h"
#include <vector>

//---------------------------------------------------------------------------------
// じゃんけんの種類（属性）
//---------------------------------------------------------------------------------
enum class HandType {
	Scissors = 0, // チョキ
	Rock,         // グー
	Paper,        // パー
	Count
};

//---------------------------------------------------------------------------------
// 壁（障害物）の構造体
//---------------------------------------------------------------------------------
struct Wall {
	Float3 pos;         // 位置
	Float3 size;        // サイズ（幅、高さ、奥行き）
	bool isDestroyed;   // 壊れているか
	HandType hand;      // 壁のじゃんけん属性
};

//---------------------------------------------------------------------------------
// ステージ（関門）の構造体
//---------------------------------------------------------------------------------
struct WallStage {
	float wallZ;            // 関門のZ位置
	Wall walls[3];          // 3レーン分の壁
	bool isCleared = false; // クリアしたか
};

//---------------------------------------------------------------------------------
// プレイ画面のシーンクラス
//---------------------------------------------------------------------------------
class ScenePlaying : public SceneBase
{
private:
	// 画像のハンドル
	int m_imgScissors = -1; // チョキの画像
	int m_imgRock = -1;     // グーの画像
	int m_imgPaper = -1;    // パーの画像

	// プレイヤーの現在の属性
	HandType m_playerHand = HandType::Rock;

	int   m_score = 0;          // スコア
	bool  m_isGameOver = false; // ゲームオーバーフラグ

	int   m_playerModel = -1;   // プレイヤーのモデルID
	Player* m_player = nullptr; // プレイヤーのポインタ

	Camera m_camera;            // カメラ

	static const int LANE_COUNT = 3; // レーンの数
	static const int STAGE_NUM = 5;  // 同時に存在する関門の数

	constexpr static float LANE_WIDTH = 10.0f; // レーンの幅
	constexpr static float START_X = -10.0f;   // 左端のレーンのX位置

	constexpr static float FIRST_WALL_Z = 30.0f; // 最初の壁のZ位置
	constexpr static float WALL_SPACING = 50.0f; // 壁と壁の間隔

	WallStage m_stages[STAGE_NUM]; // 関門の配列

	float m_farZ = 0.0f; // 一番遠い壁のZ位置

	// 内部処理用関数
	void InitLanes();                                   // レーンの初期化
	void GenerateStageWall(int stageIndex, float zPos); // 壁の生成
	void UpdateLanes();                                 // 壁の再配置（無限ロード）
	void RenderLanes();                                 // レーンと壁の描画
	void CheckPlayerCollision();                        // 当たり判定処理

	// ヘルパー関数
	bool CanPassWall(HandType player, HandType wall); // 勝敗判定
	int GetHandImageHandle(HandType type);            // 画像の取得

	// 3D描画処理
	void RenderStage3D();

public:
	// コンストラクタ
	ScenePlaying() = default;

	// デストラクタ
	virtual ~ScenePlaying() override;

	// 初期化処理
	void Init() override;

	// 終了処理
	void Exit() override;

	// 更新処理
	GameState Update() override;

	// 描画処理
	void Render() override;

	// スコアを取得
	int GetScore() const { return m_score; }
};