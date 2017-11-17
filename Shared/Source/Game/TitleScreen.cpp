#include "pch.h"

TitleScreen::TitleScreen(GameCore* pGame, ResourceManager* pResources)
	: Scene(pGame, pResources)
{
	m_UpdatingWhileInStack = false;
	m_ShowTitleText = false;
}

TitleScreen::~TitleScreen()
{
}

void TitleScreen::LoadContent()
{
	Scene::LoadContent();

	totaltime = 0;

	// create our shaders.
	m_pResources->AddShader("Texture", new ShaderProgram("Data/Shaders/Texture.vert", "Data/Shaders/Texture.frag"));
	m_pResources->AddShader("TitleText", new ShaderProgram("Data/Shaders/TitleText.vert", "Data/Shaders/TitleText.frag"));

	// create meshes.

	m_pResources->AddMesh("Sprite", Mesh::CreateBox(vec2(1, 1), false));
	m_pResources->AddMesh("SpriteReversed", Mesh::CreateBox(vec2(1, 1), true));

	// load our textures.
	m_pResources->AddTexture("IntroBG", LoadTexture("Data/Textures/IntroBG.png"));
	m_pResources->AddTexture("TitleText", LoadTexture("Data/Textures/TitleText.png"));

	// create some game objects.
	{
		// Camera
		//Swap to follow camera - Copy previous files over
		m_pGameObjects["Camera"] = new CameraObject(this, "Camera", Vector3(0, 0, -1), Vector3(0, 0, 0), Vector3(1, 1, 1), 0, 0, 0);

		// player
		m_pGameObjects["IntroBG"] = new GameObject(this, "IntroBG", vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 5, 1), "Sprite", "Texture", "IntroBG");
		m_pGameObjects["TitleText"] = new GameObject(this, "TitleText", vec3(100, 0, 0), vec3(0, 0, 0), vec3(1, 1, 1), "Sprite", "TitleText", "TitleText");
	}

	CheckForGLErrors();
}

void TitleScreen::RebuildOpenGLContent()
{
	Scene::RebuildOpenGLContent();

	m_pResources->GetShader("Texture")->Reload();

	Mesh::CreateBox(vec2(1, 1), false, m_pResources->GetMesh("Sprite"));
	Mesh::CreateBox(vec2(1, 1), true, m_pResources->GetMesh("SpriteReversed)"));
}

void TitleScreen::Update(float deltatime)
{
	totaltime += deltatime;
	if (m_pGameObjects["IntroBG"]->GetPosition().y <= 2.0f)
		m_pGameObjects["IntroBG"]->SetPosition(vec3(0.0f, m_pGameObjects["IntroBG"]->GetPosition().y + (0.2 * deltatime), 0.0f));

	if (m_pGameObjects["IntroBG"]->GetPosition().y >= 2.0f)
	{
		m_ShowTitleText = true;
	}

	if (m_ShowTitleText == true)
	{
		m_pGameObjects["TitleText"]->SetPosition(vec3(tween.TweenFunc_ElasticEaseIn(0, 10, deltatime, totaltime), 0, 0));

	}


	if (m_UpdatingWhileInStack)
	{
		return;
	}

	else if (m_UpdatingWhileInStack == false)
	{
		if (g_KeyStates['B'])
		{
			//Switch scenes
            m_pGame->SwitchScene("Flashman");

            m_UpdatingWhileInStack = true;
		}
	}

	Scene::Update(deltatime);
}


void TitleScreen::Draw()
{
	Scene::Draw();

	CheckForGLErrors();
}
