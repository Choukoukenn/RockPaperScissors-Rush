#include "SceneManager.h"

//---------------------------------------------------------------------------------
// デストラクタ
//---------------------------------------------------------------------------------
SceneManager::~SceneManager()
{
}

//---------------------------------------------------------------------------------
// 次のシーンの切り替え予約
//---------------------------------------------------------------------------------
void SceneManager::ChangeScene(SceneBase* newScene)
{
	if (m_nextScene != nullptr)
	{
		delete m_nextScene;
	}
	m_nextScene = newScene;
}

//---------------------------------------------------------------------------------
// 更新処理
//---------------------------------------------------------------------------------
GameState SceneManager::Update()
{
	// 予約された新しいシーンがあれば切り替える
	if (m_nextScene != nullptr)
	{
		if (m_currentScene != nullptr)
		{
			m_currentScene->Exit();
			delete m_currentScene;
			m_currentScene = nullptr;
		}

		m_currentScene = m_nextScene;
		m_nextScene = nullptr;

		if (m_currentScene != nullptr)
		{
			m_currentScene->Init();
		}
	}

	// 現在のシーンの更新処理を実行
	if (m_currentScene != nullptr)
	{
		return m_currentScene->Update();
	}

	return GameState::Title;
}

//---------------------------------------------------------------------------------
// 描画処理
//---------------------------------------------------------------------------------
void SceneManager::Render()
{
	if (m_currentScene != nullptr)
	{
		m_currentScene->Render();
	}
}

//---------------------------------------------------------------------------------
// 終了・メモリ解放処理
//---------------------------------------------------------------------------------
void SceneManager::Exit()
{
	if (m_currentScene != nullptr)
	{
		m_currentScene->Exit();
		delete m_currentScene;
		m_currentScene = nullptr;
	}

	if (m_nextScene != nullptr)
	{
		delete m_nextScene;
		m_nextScene = nullptr;
	}
}