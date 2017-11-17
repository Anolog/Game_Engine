#include "pch.h"

PauseScene::PauseScene(GameCore* pGame, ResourceManager* pResources)
	: Scene(pGame, pResources)
{
	m_UpdatingWhileInStack = false;

}

PauseScene::~PauseScene()
{
}

void PauseScene::LoadContent()
{
	if (m_IsLoaded == false)
	{
		Scene::LoadContent();

		// load our textures.
		m_pResources->AddTexture("Pause", LoadTexture("Data/Textures/Pause.png"));

		// create some game objects.
		{
			// Camera
			//Swap to follow camera - Copy previous files over
			m_pGameObjects["Camera"] = new CameraObject(this, "Camera", Vector3(0, 0, -1), Vector3(0, 0, 0), Vector3(1, 1, 1), 0, 0, 0);

			// player
			m_pGameObjects["Pause"] = new GameObject(this, "Pause", vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 1, 1), "Sprite", "Texture", "Pause");
		}

	CheckForGLErrors();
	}

	m_IsLoaded = true;
}

void PauseScene::RebuildOpenGLContent()
{
	Scene::RebuildOpenGLContent();

	m_pResources->GetShader("Texture")->Reload();

	Mesh::CreateBox(vec2(1, 1), false, m_pResources->GetMesh("Sprite"));
	Mesh::CreateBox(vec2(1, 1), true, m_pResources->GetMesh("SpriteReversed)"));
}

void PauseScene::Update(float deltatime)
{
		if (g_KeyStates['B'])
		{
			if (m_pGame->GetSceneManager())
			{
				m_pGame->GetSceneManager()->PopScene();
			}
		
		}

	Scene::Update(deltatime);
}


void PauseScene::Draw()
{
	Scene::Draw();

	CheckForGLErrors();
}
