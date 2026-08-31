#pragma once

#include "Base.h"

// プレイヤーの当たり判定のサイズ
constexpr float PLAYER_HALF = 1.0f;

//---------------------------------------------------------------------------------
// プレイヤークラス
//---------------------------------------------------------------------------------
class Player : public Base
{
public:

	// 初期化処理
	void Init(int model, Float3 pos) override;

	// 更新処理
	void Update() override;

	// 描画処理
	void Render() override;

	// 終了処理
	void Exit() override;

	// 前フレームの位置を取得
	const Float3& GetPrevPosition() const { return m_prevPos; }

	// ノックバックの適用
	void ApplyKnockback(float kbZ) { m_kbVelZ = kbZ; }

	// HPを取得
	int GetHP() const override { return m_hp; }

	// 角度・向きを取得
	const Float3& GetRotation() const { return m_rot; }

	// ダメージ処理
	void Damage(int amount) { m_hp -= amount; if (m_hp < 0) m_hp = 0; }

private:

	MATRIX	m_mat = MGetIdent();	// 行列（姿勢・位置用）

	Float3	m_prevPos = Float3(0, 0, 0);	// 前フレームの位置
	float	m_moveSpeed = 3.0f;				// 移動速度

	float	m_kbVelX = 0.0f;	// ノックバック速度（X軸）
	float	m_kbVelZ = 0.0f;	// ノックバック速度（Z軸）

	int		m_hp = 100;			// HP

	float	m_speedAccel = 1.0f;	// 加速倍率

	int		m_startFrame = 0;	// 開始からの経過フレーム数
};