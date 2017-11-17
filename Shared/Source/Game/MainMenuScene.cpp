#include "pch.h"
#include "MainMenuScene.h"

MainMenuScene::MainMenuScene(GameCore* pGame, ResourceManager* pResources)
	: Scene(pGame, pResources)
{
	m_UpdatingWhileInStack = false;

}

MainMenuScene::~MainMenuScene()
{
}

void MainMenuScene::LoadContent()
{
	if (m_IsLoaded == false)
	{
		Scene::LoadContent();

		//debugdraw -> Change to toggle key later
		m_pGame->SetDebugDrawEnabled(true);

		// create our shaders.
		m_pResources->AddShader("Texture", new ShaderProgram("Data/Shaders/Texture.vert", "Data/Shaders/Texture.frag"));

		// create meshes.

		m_pResources->AddMesh("Sprite", Mesh::CreateBox(vec2(1, 1), false));
		m_pResources->AddMesh("SpriteReversed", Mesh::CreateBox(vec2(1, 1), true));

		// load our textures.
		m_pResources->AddTexture("MainMenu", LoadTexture("Data/Textures/MainMenu.png"));

		// create some game objects.
		{
			// Camera
			//Swap to follow camera - Copy previous files over
			m_pGameObjects["Camera"] = new CameraObject(this, "Camera", Vector3(0, 0, -2), Vector3(0, 0, 0), Vector3(1, 1, 1), 0, 0, 0);

			// player
			m_pGameObjects["MainMenu"] = new GameObject(this, "MainMenu", vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 1, 1), "Sprite", "Texture", "MainMenu");
		}

	CheckForGLErrors();
	}
	m_IsLoaded = true;
}

void MainMenuScene::RebuildOpenGLContent()
{
	Scene::RebuildOpenGLContent();

	m_pResources->GetShader("Texture")->Reload();

	Mesh::CreateBox(vec2(1, 1), false, m_pResources->GetMesh("Sprite"));
	Mesh::CreateBox(vec2(1, 1), true, m_pResources->GetMesh("SpriteReversed)"));
}

void MainMenuScene::Update(float deltatime)
{
	if (m_UpdatingWhileInStack)
	{
		return;
	}

	else if (m_UpdatingWhileInStack == false)
	{
		if (g_KeyStates['B'])
		{
			//Switch scenes
            m_pGame->SwitchScene("TestLevel");
            
            //m_pGameObjects["MainMenu"]->SetPosition(vec3(0, 100, -10));

			OutputMessage("Switching Scenes!\n");

            m_UpdatingWhileInStack = true;
		}
	}

	Scene::Update(deltatime);
}


void MainMenuScene::Draw()
{
	Scene::Draw();

	CheckForGLErrors();
}
