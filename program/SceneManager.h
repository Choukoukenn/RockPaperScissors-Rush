#pragma once

#include "SceneBase.h"

//---------------------------------------------------------------------------------
// シーン管理者クラス（シーンの切り替えや更新・描画を管理）
//---------------------------------------------------------------------------------
class SceneManager
{
private:
	SceneBase* m_currentScene = nullptr; // 現在実行中のシーン
	SceneBase* m_nextScene = nullptr;    // 次に切り替えるシーン

public:
	SceneManager() = default;
	~SceneManager();

	// 現在のシーンを取得
	SceneBase* GetCurrentScene() const { return m_currentScene; }

	// 新しいシーンへ切り替える
	void ChangeScene(SceneBase* newScene);

	// 更新処理
	GameState Update();

	// 描画処理
	void Render();

	// メモリ解放処理
	void Exit();
};