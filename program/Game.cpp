#include "Game.h"
#include <vector>
#include "SceneManager.h"
#include "SceneTitle.h"
#include "ScenePlaying.h"
#include "SceneResult.h"

// グローバル変数：ゲーム状態と汎用シーンマネージャー
GameState g_gameState = GameState::Title;
SceneManager g_sceneManager;

// ゲーム全体の初期化処理
void GameInit()
{
	// 初期状態をタイトル画面に設定
	g_gameState = GameState::Title;

	// シーンマネージャーに最初のシーンをセット
	g_sceneManager.ChangeScene(new SceneTitle());
}

// ゲーム全体の更新処理（メインループから毎フレーム呼ばれる）
void GameUpdate()
{
	GameState nextState = g_sceneManager.Update();

	if (nextState != g_gameState)
	{
		int finalScore = 0;

		// プレイ中からリザルトへ遷移する場合、スコアを引き継ぐ
		if (g_gameState == GameState::Playing)
		{
			// ★ SceneManager から現在の Scene を取得してキャスト
			ScenePlaying* playingScene = dynamic_cast<ScenePlaying*>(g_sceneManager.GetCurrentScene());
			if (playingScene != nullptr)
			{
				finalScore = playingScene->GetScore();
			}
		}

		g_gameState = nextState;

		switch (g_gameState)
		{
		case GameState::Title:
			g_sceneManager.ChangeScene(new SceneTitle());
			break;

		case GameState::Playing:
			g_sceneManager.ChangeScene(new ScenePlaying());
			break;

		case GameState::Clear:
		case GameState::GameOver:
			g_sceneManager.ChangeScene(new SceneResult(g_gameState, finalScore));
			break;
		}
	}
}

// 描画処理
void GameRender()
{
	// 描画処理をシーンマネージャーに委任
	g_sceneManager.Render();
}

// 終了処理
void GameExit()
{
	// メモリ解放処理をシーンマネージャーに委任
	g_sceneManager.Exit();
}