#pragma once

// ゲームの状態（シーン）を表す列挙型
enum class GameState
{
	Title,     // タイトル画面
	Playing,   // プレイ画面
	Clear,     // ゲームクリア画面
	GameOver   // ゲームオーバー画面
};

//---------------------------------------------------------------------------------
// すべてのシーンの親クラス（基本クラス）
//---------------------------------------------------------------------------------
class SceneBase
{
public:

	// デストラクタ
	virtual ~SceneBase() = default;

	// 初期化処理
	virtual void Init() = 0;

	// 終了処理
	virtual void Exit() = 0;

	// 描画処理
	virtual void Render() = 0;

	// 更新処理（次のシーンの状態を返す）
	virtual GameState Update() = 0;
};