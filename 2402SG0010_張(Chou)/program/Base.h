#pragma once
#include "Float3.h"
//---------------------------------------------------------------------------------
// 基本クラス（すべてのオブジェクトの親クラス）
//---------------------------------------------------------------------------------
class Base
{
public:
	// コンストラクタ
	Base();
	// デストラクタ
	virtual ~Base() {}

	// 各種仮想関数（初期化・更新・描画・終了処理）
	virtual void Init(int model, Float3 pos) = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void Exit() = 0;

	// HPの取得（デフォルトは0）
	virtual int GetHP() const { return 0; }

	// 生存フラグの確認（生きているか）
	virtual bool IsActive() const { return true; }

	// オブジェクトの消滅処理（倒れたときの処理など）
	virtual void Kill() {}

	// 当たり判定用の半径を取得
	virtual float GetRadius() const { return 0.0f; }

	// 位置の取得
	Float3 GetPos() const { return m_pos; }

	// 位置の設定
	void SetPos(const Float3& pos) { m_pos = pos; }

protected:
	Float3	m_pos;		// 位置（X, Y, Z）
	int		m_model;	// モデルのID（ハンドル）
	Float3	m_rot;		// 向き・回転角度（X, Y, Z）
};