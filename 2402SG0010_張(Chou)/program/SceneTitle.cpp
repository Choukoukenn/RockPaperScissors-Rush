#include "SceneTitle.h"
#include "../program/Main.h"
#include "DxLib.h"
#include "../program/Game.h" 
#include <cstring>

//---------------------------------------------------------------------------------
// 更新処理
//---------------------------------------------------------------------------------
GameState SceneTitle::Update()
{
	// ENTERキーでゲームを開始
	if (PushHitKey(KEY_INPUT_RETURN))
	{
		return GameState::Playing;
	}

	// 現在の画面（タイトル）を維持
	return GameState::Title;
}

//---------------------------------------------------------------------------------
// 初期化処理
//---------------------------------------------------------------------------------
void SceneTitle::Init()
{

}

//---------------------------------------------------------------------------------
// 終了処理
//---------------------------------------------------------------------------------
void SceneTitle::Exit()
{

}

//---------------------------------------------------------------------------------
// 描画処理
//---------------------------------------------------------------------------------
void SceneTitle::Render()
{
	int screenWidth = SCREEN_W;
	int screenHeight = SCREEN_H;
	float centerX = screenWidth / 2.0f;
	float centerY = screenHeight / 2.0f;

	// タイトルの画面UIを描画
	RenderTitleUI(screenWidth, screenHeight, centerX, centerY);
}

//---------------------------------------------------------------------------------
// タイトル画面のUI描画
//---------------------------------------------------------------------------------
void SceneTitle::RenderTitleUI(int screenWidth, int screenHeight, float centerX, float centerY)
{
	// 背景（黒）
	DrawBox(0, 0, screenWidth, screenHeight, GetColor(0, 0, 0), TRUE);

	// 表示するテキスト
	const char* titleText = "--- じゃんけんラッシュ ---";
	const char* startText = "[ ENTER ] キーを押してスタート";
	const char* controlText = "【操作方法】 [ ← / → ] 移動";
	const char* goalText = "【ルール】 じゃんけんに勝って壁を突破しよう！";

	// テキストを中央に表示
	int titleX = (int)(centerX - ((float)GetDrawStringWidth(titleText, (int)strlen(titleText)) / 2.0f));
	DrawString(titleX, (int)(centerY - 60), titleText, GetColor(0, 255, 255));

	int startX = (int)(centerX - ((float)GetDrawStringWidth(startText, (int)strlen(startText)) / 2.0f));
	DrawString(startX, (int)centerY, startText, GetColor(255, 255, 0));

	int controlX = (int)(centerX - ((float)GetDrawStringWidth(controlText, (int)strlen(controlText)) / 2.0f));
	DrawString(controlX, (int)(centerY + 60), controlText, GetColor(180, 180, 180));

	int goalX = (int)(centerX - ((float)GetDrawStringWidth(goalText, (int)strlen(goalText)) / 2.0f));
	DrawString(goalX, (int)(centerY + 90), goalText, GetColor(180, 180, 180));
}