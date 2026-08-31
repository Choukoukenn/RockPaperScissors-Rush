#include "Camera.h"
#include "DxLib.h"
#include "Main.h"
#include "Game.h"
#include "Random.h"

//---------------------------------------------------------------------------------
// 初期化処理
//---------------------------------------------------------------------------------
void Camera::Init()
{
	// カメラの位置を設定
	m_pos.Set(0.0f, 10.0f, -20.0f);
	// カメラの向き（角度）を設定
	m_rot.Set(35.0f, 0.0f, 0.0f);
}

//---------------------------------------------------------------------------------
// 更新処理
//---------------------------------------------------------------------------------
void Camera::Update(const Float3& player_pos, const Float3& player_rot)
{
	float distance = 10.0f; // プレイヤーからの距離
	float height = 10.0f;   // プレイヤーからの高さ

	float radY = TO_RADIAN(m_rot.y);

	// プレイヤーの後ろの上にカメラを移動
	m_pos.x = player_pos.x - sinf(radY) * distance;
	m_pos.y = player_pos.y + height;
	m_pos.z = player_pos.z - cosf(radY) * distance;
}

//---------------------------------------------------------------------------------
// 描画前のカメラ設定
//---------------------------------------------------------------------------------
void Camera::Setting()
{
	// 画角（視野角）を設定
	SetupCamera_Perspective(TO_RADIAN(m_fov));
	// カメラの位置と角度を設定
	SetCameraPositionAndAngle(m_pos.VGet(), TO_RADIAN(m_rot.x), TO_RADIAN(m_rot.y), TO_RADIAN(m_rot.z));
}

//---------------------------------------------------------------------------------
// 終了処理
//---------------------------------------------------------------------------------
void Camera::Exit()
{
}