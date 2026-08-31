#pragma once

#include "SceneBase.h"

//---------------------------------------------------------------------------------
// リザルト（結果）画面のシーンクラス
//---------------------------------------------------------------------------------
class SceneResult : public SceneBase
{
public:

	// コンストラクタ（状態と最終スコアを受け取る）
	SceneResult(GameState state, int finalScore);

	// デストラクタ
	virtual ~SceneResult() override = default;

	// 初期化処理
	void Init() override;

	// 終了処理
	void Exit() override;

	// 更新処理
	GameState Update() override;

	// 描画処理
	void Render() override;

private:
	GameState m_myState;  // ゲームの状態（Clear または GameOver）
	int m_finalScore;     // 最終スコア

	// UI描画処理
	void RenderResultUI(int screenWidth, int screenHeight, float centerX, float centerY);
};