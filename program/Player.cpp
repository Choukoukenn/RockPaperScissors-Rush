#include "Player.h"
#include "Main.h"
#include "Game.h"

// 回転スピード
constexpr float PLAYER_ROT_SPEED = 2.0f;
// 通常移動速度
constexpr float PLAYER_MOV_SPEED = 0.3f;
// 最大移動速度
constexpr float PLAYER_MAX_SPEED = 0.6f;

//---------------------------------------------------------------------------------
// 初期化処理
//---------------------------------------------------------------------------------
void Player::Init(int model, Float3 pos)
{
	m_model = model;

	// 初期位置の設定
	m_pos.Set(0.0f, 0.0f, -5.0f);

	m_hp = 100;

	m_rot.Clear();

	// 行列の初期化
	m_mat = MGetIdent();

	// ノックバック速度の初期化
	m_kbVelX = 0.0f;
	m_kbVelZ = 0.0f;

	m_startFrame = 0;

	// 加速倍率の初期化
	m_speedAccel = 1.0f;
}

//---------------------------------------------------------------------------------
// 更新処理
//---------------------------------------------------------------------------------
void Player::Update()
{
	// 前フレームの位置を保存
	m_prevPos = m_pos;

	// 徐々に加速する処理
	m_speedAccel += 0.01f;
	if (m_speedAccel > 8.0f) {
		m_speedAccel = 8.0f;
	}

	// キー入力による移動方向の計算
	Float3 moveDir(0.0f, 0.0f, 0.0f);
	bool isMoving = false;

	if (CheckHitKey(KEY_INPUT_UP)) {
		isMoving = true;
	}
	if (CheckHitKey(KEY_INPUT_DOWN)) {
		// 下キー（必要に応じて処理を追加）
	}
	if (CheckHitKey(KEY_INPUT_LEFT)) {
		moveDir.x -= 1.0f; // 左へ
		isMoving = true;
	}
	if (CheckHitKey(KEY_INPUT_RIGHT)) {
		moveDir.x += 1.0f; // 右へ
		isMoving = true;
	}

	// 移動量と回転（角度）の計算
	Float3 mov(0.0f, 0.0f, 0.0f);

	// 目標とする傾き角度
	float targetRotX = 0.0f;
	float targetRotZ = 0.0f;

	if (isMoving) {
		// 移動ベクトルの長さを計算
		float length = sqrtf(moveDir.x * moveDir.x + moveDir.z * moveDir.z);

		// ゼロ除算の防止
		if (length > 0.0001f) {
			moveDir.x /= length;
			moveDir.z /= length;

			// プレイヤーの左右・前後移動量を計算
			mov.x = moveDir.x * m_moveSpeed * 0.2f * (m_speedAccel * 0.3f);
			mov.z = moveDir.z * m_moveSpeed * 0.1f * m_speedAccel;

			// Y軸：左右の向き（旋回）
			float targetRotY = (float)atan2f(moveDir.x, moveDir.z) * (180.0f / DX_PI);

			float rotDiff = targetRotY - m_rot.y;
			while (rotDiff <= -180.0f) rotDiff += 360.0f;
			while (rotDiff > 180.0f) rotDiff -= 360.0f;
			m_rot.y += rotDiff * 0.15f;

			// X軸：前後の傾き
			targetRotX = -10.0f;

			// Z軸：左右の傾き（ロール）
			float safeRotDiff = min(max(-90.0f, rotDiff), 90.0f);
			targetRotZ = -safeRotDiff * 0.5f;
		}
	}
	else {
		// キー入力がない場合
		targetRotX = 0.0f;
		targetRotZ = 0.0f;
	}

	// -------------------------------------------------------------
	// 最終的な角度と位置の適用
	// -------------------------------------------------------------
	// 傾きの補間処理（なめらかに傾ける）
	m_rot.x += (targetRotX - m_rot.x) * 0.1f;
	m_rot.z += (targetRotZ - m_rot.z) * 0.1f;

	// 角度の制限
	m_rot.x = min(max(-30.0f, m_rot.x), 30.0f);
	m_rot.z = min(max(-25.0f, m_rot.z), 25.0f);

	// 回転行列を作成
	MATRIX mat_y = MGetRotY(TO_RADIAN(m_rot.y));
	MATRIX mat_z = MGetRotZ(TO_RADIAN(m_rot.z));
	MATRIX mat_x = MGetRotX(TO_RADIAN(m_rot.x));

	// 回転行列を合成
	m_mat = MMult(mat_z, mat_x);
	m_mat = MMult(m_mat, mat_y);

	// 移動量の加算
	Float3 nextPos = m_pos + mov;

	// 自動前進（Z軸方向へ常時移動）
	nextPos.z += (m_moveSpeed * 0.05f) * m_speedAccel;

	// ノックバック移動量の加算
	nextPos.x += m_kbVelX;
	nextPos.z += m_kbVelZ;

	// ノックバック速度の減衰
	m_kbVelX *= 0.85f;
	m_kbVelZ *= 0.85f;

	if (fabsf(m_kbVelX) < 0.001f) m_kbVelX = 0.0f;
	if (fabsf(m_kbVelZ) < 0.001f) m_kbVelZ = 0.0f;

	// 最新の位置を更新
	m_pos = nextPos;

	// 位置行列を作成して合成
	MATRIX mat_pos = MGetTranslate(m_pos.VGet());
	m_mat = MMult(m_mat, mat_pos);
}

//---------------------------------------------------------------------------------
// 描画処理
//---------------------------------------------------------------------------------
void Player::Render()
{
	// モデルの位置を設定
	MV1SetPosition(m_model, VGet(m_pos.x, m_pos.y, m_pos.z));

	// 回転行列をモデルに適用
	MV1SetRotationMatrix(m_model, m_mat);

	// 3Dモデルを描画
	MV1DrawModel(m_model);
}

//---------------------------------------------------------------------------------
// 終了処理
//---------------------------------------------------------------------------------
void Player::Exit()
{
}