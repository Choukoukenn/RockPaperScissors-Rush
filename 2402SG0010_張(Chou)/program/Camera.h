#pragma once

#include "Float3.h"

//---------------------------------------------------------------------------------
// カメラクラス
//---------------------------------------------------------------------------------
class Camera
{
public:

	void Init();		// 初期化処理
	void Update(const Float3& player_pos, const Float3& player_rot); // 更新処理
	void Setting();		// 描画前のカメラ設定
	void Exit();		// 終了処理

private:
	Float3	m_pos;		// 位置（X, Y, Z）
	Float3	m_rot;		// 向き・角度（X, Y, Z）

	float m_fov = 60.0f;           // 視野角（FOV）
	float m_shakeIntensity = 0.0f; // 画面の揺れの強さ
};