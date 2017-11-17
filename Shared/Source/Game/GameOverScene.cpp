#include "pch.h"

GameOverScene::GameOverScene(GameCore* pGame, ResourceManager* pResources)
	: Scene(pGame, pResources)
{
	m_UpdatingWhileInStack = false;

}

GameOverScene::~GameOverScene()
{
}

void GameOverScene::LoadContent()
{
	if (m_IsLoaded == false)
	{
		Scene::LoadContent();

		// load our textures.
		m_pResources->AddTexture("GameOver", LoadTexture("Data/Textures/GameOver.png"));

		// create some game objects.
		{
			// Camera
			//Swap to follow camera - Copy previous files over
			m_pGameObjects["Camera"] = new CameraObject(this, "Camera", Vector3(0, 0, -1), Vector3(0, 0, 0), Vector3(1, 1, 1), 0, 0, 0);

			// player
			m_pGameObjects["GameOver"] = new GameObject(this, "GameOver", vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 1, 1), "Sprite", "Texture", "GameOver");
		}

	CheckForGLErrors();
	}

	m_IsLoaded = true;
}

void GameOverScene::RebuildOpenGLContent()
{
	Scene::RebuildOpenGLContent();

	m_pResources->GetShader("Texture")->Reload();

	Mesh::CreateBox(vec2(1, 1), false, m_pResources->GetMesh("Sprite"));
	Mesh::CreateBox(vec2(1, 1), true, m_pResources->GetMesh("SpriteReversed)"));
}

void GameOverScene::Update(float deltatime)
{
	if (m_UpdatingWhileInStack)
	{
		return;
	}

	//Known Bug, will not pop because forwhatever reason the scenestack is 0...
	else if (m_UpdatingWhileInStack == false)
	{
		if (g_KeyStates['B'])
		{
			m_pGame->GetSceneManager()->PopToBeginning();
		}
	}

	Scene::Update(deltatime);
}


void GameOverScene::Draw()
{
	Scene::Draw();

	CheckForGLErrors();
}
