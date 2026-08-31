#include "SceneResult.h"
#include "../program/Main.h"
#include "DxLib.h"
#include "../program/Game.h"
#include <cstdio>
#include <cstring>

//---------------------------------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------------------------------
SceneResult::SceneResult(GameState state, int finalScore)
	: m_myState(state), m_finalScore(finalScore)
{
}

//---------------------------------------------------------------------------------
// 初期化処理
//---------------------------------------------------------------------------------
void SceneResult::Init() {}

//---------------------------------------------------------------------------------
// 終了処理
//---------------------------------------------------------------------------------
void SceneResult::Exit() {}

//---------------------------------------------------------------------------------
// 更新処理
//---------------------------------------------------------------------------------
GameState SceneResult::Update()
{
	// ENTERキーでタイトルに戻る
	if (PushHitKey(KEY_INPUT_RETURN))
	{
		return GameState::Title;
	}

	// 現在の状態を維持
	return m_myState;
}

//---------------------------------------------------------------------------------
// 描画処理
//---------------------------------------------------------------------------------
void SceneResult::Render()
{
	int screenWidth = SCREEN_W;
	int screenHeight = SCREEN_H;
	float centerX = screenWidth / 2.0f;
	float centerY = screenHeight / 2.0f;

	// UIを描画
	RenderResultUI(screenWidth, screenHeight, centerX, centerY);
}

//---------------------------------------------------------------------------------
// リザルト画面のUI描画
//---------------------------------------------------------------------------------
void SceneResult::RenderResultUI(int screenWidth, int screenHeight, float centerX, float centerY)
{
	// 背景（黒）
	DrawBox(0, 0, screenWidth, screenHeight, GetColor(0, 0, 0), TRUE);

	// ゲームクリア時の表示
	if (m_myState == GameState::Clear)
	{
		const char* titleText = " GAME CLEAR! ";
		char scoreText[64];
		sprintf_s(scoreText, "SCORE: %d", m_finalScore);
		const char* hintText = "[ ENTER ] キーを押してタイトルへ";

		// 文字を中央に表示
		DrawString((int)(centerX - (((float)GetDrawStringWidth(titleText, (int)strlen(titleText))) / 2.0f)), (int)(centerY - 40), titleText, GetColor(0, 255, 0));
		DrawString((int)(centerX - (((float)GetDrawStringWidth(scoreText, (int)strlen(scoreText))) / 2.0f)), (int)centerY, scoreText, GetColor(255, 255, 255));
		DrawString((int)(centerX - (((float)GetDrawStringWidth(hintText, (int)strlen(hintText))) / 2.0f)), (int)(centerY + 50), hintText, GetColor(200, 200, 200));
	}
	// ゲームオーバー時の表示
	else if (m_myState == GameState::GameOver)
	{
		const char* titleText = " GAME OVER! ";

		char scoreText[64];
		sprintf_s(scoreText, "SCORE: %d", m_finalScore);

		const char* hintText = "[ ENTER ] キーを押してタイトルへ";

		// 文字を中央に表示
		DrawString((int)(centerX - (((float)GetDrawStringWidth(scoreText, (int)strlen(scoreText))) / 2.0f)), (int)centerY, scoreText, GetColor(255, 255, 255));
		DrawString((int)(centerX - (((float)GetDrawStringWidth(hintText, (int)strlen(hintText))) / 2.0f)), (int)(centerY + 30), hintText, GetColor(200, 200, 200));
	}
}