#include "ScenePlaying.h"
#include "../program/Game.h"
#include "DxLib.h"
#include "../program/Player.h"
#include "../program/Random.h"
#include <algorithm>
#include <cmath>

//---------------------------------------------------------------------------------
// デストラクタ
//---------------------------------------------------------------------------------
ScenePlaying::~ScenePlaying() {
}

//---------------------------------------------------------------------------------
// 初期化処理
//---------------------------------------------------------------------------------
void ScenePlaying::Init() {
	m_isGameOver = false;

	// リソースの読み込み（モデル・画像）
	m_playerModel = MV1LoadModel("data/player.mv1");

	m_imgScissors = LoadGraph("data/scissors.png"); // チョキ
	m_imgRock = LoadGraph("data/rock.png");     // グー
	m_imgPaper = LoadGraph("data/paper.png");    // パー

	// プレイヤーの初期属性（手）をランダムに決定
	m_playerHand = static_cast<HandType>(GetRand(static_cast<int>(HandType::Count) - 1));

	// カメラの初期化
	m_camera.Init();

	// プレイヤーの生成と初期化
	m_player = new Player();
	m_player->Init(m_playerModel, Float3(0.0f, 0.0f, 0.0f));

	// スコアのリセット
	m_score = 0;

	// レーン・壁の初期化
	InitLanes();
}

//---------------------------------------------------------------------------------
// 壁データの生成（属性をランダムに割り当て）
//---------------------------------------------------------------------------------
void ScenePlaying::GenerateStageWall(int stageIndex, float zPos) {
	m_stages[stageIndex].wallZ = zPos;
	m_stages[stageIndex].isCleared = false;

	// 1. じゃんけんの手を生成
	HandType hands[LANE_COUNT] = {
		HandType::Scissors, // チョキ
		HandType::Rock,     // グー
		HandType::Paper     // パー
	};

	// 2. シャッフル処理
	for (int i = LANE_COUNT - 1; i > 0; i--) {
		int r = GetRand(i);
		std::swap(hands[i], hands[r]);
	}

	// 3. 各レーンの壁の設定
	for (int i = 0; i < LANE_COUNT; i++) {
		float xPos = START_X + (i * LANE_WIDTH);

		m_stages[stageIndex].walls[i].pos = Float3(xPos, 2.5f, zPos);
		m_stages[stageIndex].walls[i].size = Float3(8.0f, 5.0f, 3.0f);
		m_stages[stageIndex].walls[i].isDestroyed = false;
		m_stages[stageIndex].walls[i].hand = hands[i];
	}
}

//---------------------------------------------------------------------------------
// レーンの初期配置
//---------------------------------------------------------------------------------
void ScenePlaying::InitLanes() {
	m_farZ = FIRST_WALL_Z;

	for (int s = 0; s < STAGE_NUM; s++) {
		GenerateStageWall(s, m_farZ);
		m_farZ += WALL_SPACING; // 次の壁のZ位置
	}
}

//---------------------------------------------------------------------------------
// レーンの再配置（無限ロード処理）
//---------------------------------------------------------------------------------
void ScenePlaying::UpdateLanes() {
	if (m_player == nullptr) return;

	float playerZ = m_player->GetPos().z;

	for (int s = 0; s < STAGE_NUM; s++) {
		// プレイヤーが通り過ぎたら前方へ移動
		if (playerZ > m_stages[s].wallZ + 15.0f) {
			GenerateStageWall(s, m_farZ);
			m_farZ += WALL_SPACING;
		}
	}
}

//---------------------------------------------------------------------------------
// 更新処理
//---------------------------------------------------------------------------------
GameState ScenePlaying::Update() {
	// プレイヤーの更新
	if (m_player != nullptr) { m_player->Update(); }

	// レーンの更新
	UpdateLanes();

	// 衝突判定
	CheckPlayerCollision();

	// カメラの追従
	if (m_player != nullptr) { m_camera.Update(m_player->GetPos(), m_player->GetRotation()); }

	// ゲームオーバー判定（HPが0以下）
	if (m_player != nullptr && m_player->GetHP() <= 0) {
		m_isGameOver = true;
	}

	if (m_isGameOver) {
		return GameState::GameOver;
	}

	return GameState::Playing;
}

//---------------------------------------------------------------------------------
// 描画処理
//---------------------------------------------------------------------------------
void ScenePlaying::Render() {
	RenderStage3D(); // 3D空間の描画

	// ----------------------------------------------------
	// プレイヤー頭上のUI描画
	// ----------------------------------------------------
	if (m_player != nullptr) {
		Float3 pPos = m_player->GetPos();
		VECTOR headWorldPos = VGet(pPos.x, pPos.y + 2.0f, pPos.z);
		VECTOR headScreenPos = ConvWorldPosToScreenPos(headWorldPos);

		if (headScreenPos.z >= 0.0f && headScreenPos.z <= 1.0f) {
			int playerImg = GetHandImageHandle(m_playerHand);

			if (playerImg != -1) {
				int screenX = static_cast<int>(roundf(headScreenPos.x));
				int screenY = static_cast<int>(roundf(headScreenPos.y));

				unsigned int bgColor = GetColor(0, 0, 0);
				switch (m_playerHand) {
				case HandType::Scissors: bgColor = GetColor(50, 120, 240); break; // チョキ
				case HandType::Rock:     bgColor = GetColor(255, 200, 50); break; // グー
				case HandType::Paper:    bgColor = GetColor(220, 50, 50);  break; // パー
				}

				// 背景円の描画
				DrawCircle(screenX, screenY, 38, GetColor(0, 0, 0), TRUE);
				DrawCircle(screenX, screenY, 34, bgColor, TRUE);

				// アイコン画像の描画
				int iconHalf = 28;
				DrawExtendGraph(screenX - iconHalf, screenY - iconHalf, screenX + iconHalf, screenY + iconHalf, playerImg, TRUE);
			}
		}
	}

	// ----------------------------------------------------
	// 2D UI 描画（スコア・HP）
	// ----------------------------------------------------
	DrawFormatString(20, 20, GetColor(255, 255, 255), "SCORE: %d", m_score);
	if (m_player)
	{
		int hp = m_player->GetHP();
		unsigned int hpColor;
		// HPの残量によって文字色を切り替え
		if (hp > 50) {
			hpColor = GetColor(0, 255, 100);  
		}
		else if (hp > 20) {
			hpColor = GetColor(255, 200, 0);  
		}
		else {
			hpColor = GetColor(255, 50, 50); 
		}

		DrawFormatString(20, 40, hpColor, "HP: %d / 100", hp);
	}
}

//---------------------------------------------------------------------------------
// 終了・メモリ解放処理
//---------------------------------------------------------------------------------
void ScenePlaying::Exit() {
	if (m_player != nullptr) { m_player->Exit(); delete m_player; m_player = nullptr; }

	MV1DeleteModel(m_playerModel);
	DeleteGraph(m_imgScissors);
	DeleteGraph(m_imgRock);
	DeleteGraph(m_imgPaper);
}

//---------------------------------------------------------------------------------
// 勝敗判定（プレイヤーが勝っているか）
//---------------------------------------------------------------------------------
bool ScenePlaying::CanPassWall(HandType player, HandType wall) {
	if (player == HandType::Scissors && wall == HandType::Paper) return true;
	if (player == HandType::Rock && wall == HandType::Scissors) return true;
	if (player == HandType::Paper && wall == HandType::Rock) return true;
	return false;
}

//---------------------------------------------------------------------------------
// 画像ハンドルの取得
//---------------------------------------------------------------------------------
int ScenePlaying::GetHandImageHandle(HandType type) {
	switch (type) {
	case HandType::Scissors: return m_imgScissors;
	case HandType::Rock:     return m_imgRock;
	case HandType::Paper:    return m_imgPaper;
	}
	return -1;
}

//---------------------------------------------------------------------------------
// 3Dステージ・壁の描画
//---------------------------------------------------------------------------------
void ScenePlaying::RenderLanes() {
	if (m_player == nullptr) return;

	float playerZ = m_player->GetPos().z;

	// 1. 地面の線の描画
	float lineStartZ = max(0.0f, playerZ - 20.0f);
	float lineEndZ = m_farZ + 30.0f;

	for (int i = 0; i < LANE_COUNT; i++) {
		float x = START_X + (i * LANE_WIDTH);
		DrawLine3D(VGet(x, 0.1f, lineStartZ), VGet(x, 0.1f, lineEndZ), GetColor(80, 80, 80));
	}

	// 2. 壁とフェンスの描画
	for (int s = 0; s < STAGE_NUM; s++) {
		float wallZ = m_stages[s].wallZ;

		if (wallZ < playerZ - 20.0f) continue;

		// フェンス描画
		float guardStartZ = wallZ - 4.0f;
		float guardEndZ = wallZ + 4.0f;

		for (int i = 0; i <= LANE_COUNT; i++) {
			float fenceX = (START_X - LANE_WIDTH / 2.0f) + (i * LANE_WIDTH);
			VECTOR minPos = VGet(fenceX - 0.3f, 0.0f, guardStartZ);
			VECTOR maxPos = VGet(fenceX + 0.3f, 3.0f, guardEndZ);
			DrawCube3D(minPos, maxPos, GetColor(150, 150, 150), GetColor(255, 255, 255), TRUE);
		}

		// 壁とポリゴンの描画
		for (int i = 0; i < LANE_COUNT; i++) {
			const Wall& wall = m_stages[s].walls[i];

			if (wall.isDestroyed) continue;

			float halfX = wall.size.x / 2.0f;
			float halfZ = wall.size.z / 2.0f;

			VECTOR minPos = VGet(wall.pos.x - halfX, 0.0f, wall.pos.z - halfZ);
			VECTOR maxPos = VGet(wall.pos.x + halfX, wall.size.y, wall.pos.z + halfZ);

			// 壁の色設定
			unsigned int wallColor = GetColor(180, 180, 180);
			switch (wall.hand) {
			case HandType::Scissors: wallColor = GetColor(255, 200, 50); break;
			case HandType::Rock:     wallColor = GetColor(220, 50, 50);  break;
			case HandType::Paper:    wallColor = GetColor(50, 120, 240); break;
			}

			DrawCube3D(minPos, maxPos, wallColor, GetColor(255, 255, 255), TRUE);

			// テクスチャ（画像）の描画
			int wallImgHandle = GetHandImageHandle(wall.hand);
			if (wallImgHandle != -1) {
				float frontZ = wall.pos.z - halfZ - 0.01f;

				VERTEX3D Vertex[4];
				memset(Vertex, 0, sizeof(Vertex));

				COLOR_U8 whiteColor = GetColorU8(255, 255, 255, 255);
				VECTOR normalVec = VGet(0.0f, 0.0f, -1.0f);

				Vertex[0].pos = VGet(wall.pos.x - halfX, wall.size.y, frontZ);
				Vertex[0].norm = normalVec; Vertex[0].dif = whiteColor;
				Vertex[0].u = 0.0f; Vertex[0].v = 0.0f;

				Vertex[1].pos = VGet(wall.pos.x + halfX, wall.size.y, frontZ);
				Vertex[1].norm = normalVec; Vertex[1].dif = whiteColor;
				Vertex[1].u = 1.0f; Vertex[1].v = 0.0f;

				Vertex[2].pos = VGet(wall.pos.x - halfX, 0.0f, frontZ);
				Vertex[2].norm = normalVec; Vertex[2].dif = whiteColor;
				Vertex[2].u = 0.0f; Vertex[2].v = 1.0f;

				Vertex[3].pos = VGet(wall.pos.x + halfX, 0.0f, frontZ);
				Vertex[3].norm = normalVec; Vertex[3].dif = whiteColor;
				Vertex[3].u = 1.0f; Vertex[3].v = 1.0f;

				WORD PolygonIndices[6] = { 0, 1, 2, 2, 1, 3 };
				DrawPolygonIndexed3D(Vertex, 4, PolygonIndices, 2, wallImgHandle, TRUE);
			}
		}
	}
}

//---------------------------------------------------------------------------------
// 衝突判定とダメージ処理
//---------------------------------------------------------------------------------
void ScenePlaying::CheckPlayerCollision() {
	if (m_player == nullptr) return;

	Float3 pPos = m_player->GetPos();

	// 1. 移動範囲（左右）の制限
	float overallMinX = START_X - (LANE_WIDTH / 2.0f) + PLAYER_HALF;
	float overallMaxX = START_X + (LANE_COUNT - 1) * LANE_WIDTH + (LANE_WIDTH / 2.0f) - PLAYER_HALF;

	if (pPos.x < overallMinX) pPos.x = overallMinX;
	if (pPos.x > overallMaxX) pPos.x = overallMaxX;
	m_player->SetPos(pPos);

	// 2. レーン進入時の移動制限
	for (int s = 0; s < STAGE_NUM; s++) {
		float wallZ = m_stages[s].wallZ;
		float lockStartZ = wallZ - 4.0f;
		float lockEndZ = wallZ + 4.0f;

		if (pPos.z >= lockStartZ && pPos.z <= lockEndZ) {
			int currentLane = (int)floorf((pPos.x - (START_X - LANE_WIDTH / 2.0f)) / LANE_WIDTH);
			if (currentLane < 0) currentLane = 0;
			if (currentLane >= LANE_COUNT) currentLane = LANE_COUNT - 1;

			float laneMinX = (START_X + currentLane * LANE_WIDTH) - (LANE_WIDTH / 2.0f) + PLAYER_HALF;
			float laneMaxX = (START_X + currentLane * LANE_WIDTH) + (LANE_WIDTH / 2.0f) - PLAYER_HALF;

			if (pPos.x < laneMinX) pPos.x = laneMinX;
			if (pPos.x > laneMaxX) pPos.x = laneMaxX;

			m_player->SetPos(pPos);
			break;
		}
	}

	// 3. 壁との当たり判定処理
	for (int s = 0; s < STAGE_NUM; s++) {
		for (int i = 0; i < LANE_COUNT; i++) {
			Wall& wall = m_stages[s].walls[i];

			if (wall.isDestroyed) continue;

			float pMinX = pPos.x - PLAYER_HALF;
			float pMaxX = pPos.x + PLAYER_HALF;
			float pMinZ = pPos.z - PLAYER_HALF;
			float pMaxZ = pPos.z + PLAYER_HALF;

			float wMinX = wall.pos.x - (wall.size.x / 2.0f);
			float wMaxX = wall.pos.x + (wall.size.x / 2.0f);
			float wMinZ = wall.pos.z - (wall.size.z / 2.0f);
			float wMaxZ = wall.pos.z + (wall.size.z / 2.0f);

			if (pMaxX > wMinX && pMinX < wMaxX &&
				pMaxZ > wMinZ && pMinZ < wMaxZ)
			{
				if (CanPassWall(m_playerHand, wall.hand)) {
					// 勝利：壁を破壊しスコア加算
					wall.isDestroyed = true;
					m_playerHand = static_cast<HandType>(GetRand(static_cast<int>(HandType::Count) - 1));

					if (!m_stages[s].isCleared) {
						m_stages[s].isCleared = true;
						m_score += 100;
					}
				}
				else {
					// 敗北・引き分け：押し戻しとダメージ
					m_player->SetPos(m_player->GetPrevPosition());

					float knockbackForce = 5.0f;
					if (pPos.z < wall.pos.z) {
						m_player->ApplyKnockback(-knockbackForce);
					}
					else {
						m_player->ApplyKnockback(knockbackForce);
					}

					m_player->Damage(10);
				}
			}
		}
	}
}

//---------------------------------------------------------------------------------
// 3D空間の描画
//---------------------------------------------------------------------------------
void ScenePlaying::RenderStage3D()
{
	m_camera.Setting();
	if (m_player != nullptr) { m_player->Render(); }
	RenderLanes();
}