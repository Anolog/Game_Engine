#include "pch.h"

//Use this num to create unique names for all the turret bullets for multiple turrets
static int TURRET_NUM = 0;


FlashmanScene::FlashmanScene(GameCore* pGame, ResourceManager* pResources)
	: Scene(pGame, pResources)
{
	SaveManager = new SaveScene(this);
	m_pSoundGuy = new SoundManager();
	m_UpdatingWhileInStack = true;
	m_Score = 0;
	m_ScoreTimer = 0;
	m_ButtonTimer = 0;
	m_MusicChannel = 0;
	m_Muted = false;
	m_MutedMusic = false;
}

FlashmanScene::~FlashmanScene()
{
	if (SaveManager != nullptr) // fixed your memory leak BRANNDDOOONNNNN :P   (mark)
	{
		delete SaveManager;
		SaveManager = nullptr;
	}
	if (m_pSoundGuy != nullptr)
	{
		delete m_pSoundGuy;
		m_pSoundGuy = nullptr;
	}
}

void FlashmanScene::LoadContent()
{
	Scene::LoadContent();

	m_pSoundGuy->initialize();

	int musicChannel = m_pSoundGuy->playSound(Sound_Music);                     //comment this out to stop music

	//int musicChannel = m_pSoundGuy->playSound(Sound_Music);                     //comment this out to stop music
	m_MusicChannel = m_pSoundGuy->playSound(Sound_Music);                     //comment this out to stop music

	 //SoundGuy->stopSound(musicChannel); 

	// create our shaders - REMEMBER TO GET RID OF SHADERS THAT HAVE ALREADY BEEN CREATED -
	m_pResources->AddShader("White", new ShaderProgram("Data/Shaders/white.vert", "Data/Shaders/white.frag"));
	m_pResources->AddShader("Lighting", new ShaderProgram("Data/Shaders/Lighting.vert", "Data/Shaders/Lighting.frag"));
	//m_pResources->AddShader("Texture", new ShaderProgram("Data/Shaders/Texture.vert", "Data/Shaders/Texture.frag"));

	//Number Shader
	m_pResources->AddShader("Font", new ShaderProgram("Data/Shaders/font.vert", "Data/Shaders/font.frag"));

	// load our textures.
	m_pResources->AddTexture("Player", LoadTexture("Data/Textures/Megaman.png"));
	m_pResources->AddTexture("Bullet", LoadTexture("Data/Textures/Meteor.png"));
	m_pResources->AddTexture("Numbers", LoadTexture("Data/Textures/Numbers.png"));
	m_pResources->AddTexture("TestPlayer", LoadTexture("Data/Textures/Player.png"));

	m_pResources->AddTexture("FlashmanBG", LoadTexture("Data/Textures/flashmanstage.png"));

	// create some game objects.
	{
		m_pGame->SetDebugDrawEnabled(true);

		// Camera
		//Swap to follow camera - Copy previous files over
		m_pGameObjects["Camera"] = new WindowCameraObject(this, "Camera", Vector3(0, 0, -15), Vector3(0, 0, 0), Vector3(1, 1, 1), 0, 0, 0);
		m_pGameObjects["Camera"]->SetPosition(vec3(0, 0, -7));

		//Background
		m_pGameObjects["Background"] = new GameObject(this, "Background", vec3(45, -36, 0), vec3(0, 0, 0), vec3(100, 75, 50), "Sprite", "Texture", "FlashmanBG");
		m_pGameObjects["Background"]->SetRenderOrder(0);
		
		// player
		m_pGameObjects["Player"] = new PlayerObject(this, "Player", vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 1, 1), "Sprite", "Font", "TestPlayer");
		((PlayerObject*)m_pGameObjects["Player"])->SetController(m_pGame->GetController(0));

		m_pGameObjects["Player"]->AddPhysicsBox(vec2(0.75f, 0.75f), true, true, 1.0f, CATEGORY_PLAYER, MASK_PLAYER);
		//Make player unable to rotate
		m_pGameObjects["Player"]->GetPhysicsBody()->SetFixedRotation(true);
		m_pGameObjects["Player"]->SetRenderOrder(2);


		//ground
		m_pGameObjects["Ground1"] = new GameObject(this, "Ground1", vec3(0, -6, 0), vec3(0, 0, 0), vec3(25, 1, 1), 0, 0, 0);
		m_pGameObjects["Ground1"]->AddPhysicsBox(vec2(25, 3), false, true, 1.0f, CATEGORY_GROUND, MASK_GROUND_NO_BULLETS);

		m_pGameObjects["Ground2"] = new GameObject(this, "Ground2", vec3(25, -7, 0), vec3(0, 0, 0), vec3(25, 1, 1), 0, 0, 0);
		m_pGameObjects["Ground2"]->AddPhysicsBox(vec2(40, 1), false, true, 1.0f, CATEGORY_GROUND, MASK_GROUND_NO_BULLETS);

		m_pGameObjects["Ground3"] = new GameObject(this, "Ground3", vec3(6.5, -2.5, 0), vec3(0, 0, 0), vec3(7, 1, 0), 0, 0, 0);
		m_pGameObjects["Ground3"]->AddPhysicsBox(vec2(7, 1), false, true, 1.0f, CATEGORY_GROUND, MASK_GROUND_NO_BULLETS);

		m_pGameObjects["Ground4"] = new GameObject(this, "Ground4", vec3(3, -1, 0), vec3(0, 0, 0), vec3(1, 5, 0), 0, 0, 0);
		m_pGameObjects["Ground4"]->AddPhysicsBox(vec2(1, 5), false, true, 1.0f, CATEGORY_GROUND, MASK_GROUND_NO_BULLETS);

		m_pGameObjects["Ground5"] = new GameObject(this, "Ground5", vec3(10, -1, 0), vec3(0, 0, 0), vec3(1, 1, 0), 0, 0, 0);
		m_pGameObjects["Ground5"]->AddPhysicsBox(vec2(1, 1), false, true, 1.0f, CATEGORY_GROUND, MASK_GROUND_NO_BULLETS);

		m_pGameObjects["Ground6"] = new GameObject(this, "Ground6", vec3(14, -1, 0), vec3(0, 0, 0), vec3(8, 1, 0), 0, 0, 0);
		m_pGameObjects["Ground6"]->AddPhysicsBox(vec2(8, 1), false, true, 1.0f, CATEGORY_GROUND, MASK_GROUND_NO_BULLETS);

		m_pGameObjects["Ground7"] = new GameObject(this, "Ground7", vec3(16, -2.5, 0), vec3(0, 0, 0), vec3(9, 0.5, 0), 0, 0, 0);
		m_pGameObjects["Ground7"]->AddPhysicsBox(vec2(9, 0.5), false, true, 1.0f, CATEGORY_GROUND, MASK_GROUND_NO_BULLETS);

		m_pGameObjects["Ground8"] = new GameObject(this, "Ground8", vec3(21, -4.9, 0), vec3(0, 0, 0), vec3(14, 1, 0), 0, 0, 0);
		m_pGameObjects["Ground8"]->AddPhysicsBox(vec2(14, 1), false, true, 1.0f, CATEGORY_GROUND, MASK_GROUND_NO_BULLETS);

		m_pGameObjects["Ground9"] = new GameObject(this, "Ground9", vec3(23, -1, 0), vec3(0, 0, 0), vec3(8, 1, 0), 0, 0, 0);
		m_pGameObjects["Ground9"]->AddPhysicsBox(vec2(8, 1), false, true, 1.0f, CATEGORY_GROUND, MASK_GROUND_NO_BULLETS);

		m_pGameObjects["Ground10"] = new GameObject(this, "Ground10", vec3(18, 1, 0), vec3(0, 0, 0), vec3(0.25, 4, 0), 0, 0, 0);
		m_pGameObjects["Ground10"]->AddPhysicsBox(vec2(0.25, 4), false, true, 1.0f, CATEGORY_GROUND, MASK_GROUND_NO_BULLETS);

		m_pGameObjects["Ground11"] = new GameObject(this, "Ground11", vec3(18, 1.5, 0), vec3(0, 0, 0), vec3(20, 1, 0), 0, 0, 0);
		m_pGameObjects["Ground11"]->AddPhysicsBox(vec2(20, 1), false, true, 1.0f, CATEGORY_GROUND, MASK_GROUND_NO_BULLETS);

		m_pGameObjects["Ground12"] = new GameObject(this, "Ground12", vec3(28.5, 0.5, 0), vec3(0, 0, 0), vec3(1, 5, 0), 0, 0, 0);
		m_pGameObjects["Ground12"]->AddPhysicsBox(vec2(1, 5), false, true, 1.0f, CATEGORY_GROUND, MASK_GROUND_NO_BULLETS);

		m_pGameObjects["Ground13"] = new GameObject(this, "Ground13", vec3(32, -2, 0), vec3(0, 0, 0), vec3(1, 16, 0), 0, 0, 0);
		m_pGameObjects["Ground13"]->AddPhysicsBox(vec2(1, 10), false, true, 1.0f, CATEGORY_GROUND, MASK_GROUND_NO_BULLETS);

		m_pGameObjects["Ground14"] = new GameObject(this, "Ground14", vec3(25, -1.5, 0), vec3(0, 0, 0), vec3(4, 2, 0), 0, 0, 0);
		m_pGameObjects["Ground14"]->AddPhysicsBox(vec2(4, 2), false, true, 1.0f, CATEGORY_GROUND, MASK_GROUND_NO_BULLETS);

		m_pGameObjects["Ground15"] = new GameObject(this, "Ground15", vec3(31.5, -5, 0), vec3(0, 0, 0), vec3(3, 1, 0), 0, 0, 0);
		m_pGameObjects["Ground15"]->AddPhysicsBox(vec2(3, 1), false, true, 1.0f, CATEGORY_GROUND, MASK_GROUND_NO_BULLETS);

		// UI
		m_pGameObjects["Number1000s"] = new GameObject(this, "Number1000s", vec3(-3, -1, -5), vec3(0, 0, 0), vec3(0.5, 0.5, 0.5), "Sprite", "Font", "Numbers"); //pushed forward in z
		m_pGameObjects["Number100s"] = new GameObject(this, "Number100s", vec3(-1, -1, -5), vec3(0, 0, 0), vec3(0.5, 0.5, 0.5), "Sprite", "Font", "Numbers");
		m_pGameObjects["Number10s"] = new GameObject(this, "Number10s", vec3(1, -1, -5), vec3(0, 0, 0), vec3(0.5, 0.5, 0.5), "Sprite", "Font", "Numbers");
		m_pGameObjects["Number1s"] = new GameObject(this, "Number1s", vec3(3, -1, -5), vec3(0, 0, 0), vec3(0.5, 0.5, 0.5), "Sprite", "Font", "Numbers");

		m_pGameObjects["Number1000s"]->SetRenderOrder(2);
		m_pGameObjects["Number100s"]->SetRenderOrder(2);
		m_pGameObjects["Number10s"]->SetRenderOrder(2);
		m_pGameObjects["Number1s"]->SetRenderOrder(2);

	}

	CheckForGLErrors();
}

void FlashmanScene::RebuildOpenGLContent()
{
	Scene::RebuildOpenGLContent();

	m_pResources->GetShader("Texture")->Reload();

	Mesh::CreateBox(vec2(1, 1), false, m_pResources->GetMesh("Sprite"));
	Mesh::CreateBox(vec2(1, 1), true, m_pResources->GetMesh("SpriteReversed)"));
}

void FlashmanScene::Update(float deltatime)
{
	if (m_UpdatingWhileInStack)
	{
		Scene::Update(deltatime);
		m_ScoreTimer += deltatime;

		if (m_ScoreTimer >= 1.0f)
		{
			addScore(5);
			m_ScoreTimer = 0;

			OutputMessage("Player Pos: %f, %f\n", m_pGameObjects["Player"]->GetPosition().x, m_pGameObjects["Player"]->GetPosition().y);
		}

		//Score
		{
			int temp1000s = (int) m_Score / 1000;
			int temp100s = (int) (m_Score - (temp1000s * 1000)) / 100;
			int temp10s = (int)((m_Score - (temp1000s * 1000) - (temp100s * 100)) / 10);
			int temp1s = (int) (m_Score - (temp1000s * 1000) - (temp100s * 100) - temp10s * 10);
			
			m_pGameObjects["Number1000s"]->SetFrame(temp1000s);
			m_pGameObjects["Number100s"]->SetFrame(temp100s);
			m_pGameObjects["Number10s"]->SetFrame(temp10s);
			m_pGameObjects["Number1s"]->SetFrame(temp1s);

			//update UI movement
			m_pGameObjects["Number1000s"]->SetPosition(vec3(m_pGameObjects["Camera"]->GetPosition().x + 2 - 1.0f, m_pGameObjects["Camera"]->GetPosition().y + 3.5f, m_pGameObjects["Number1000s"]->GetPosition().z));
			m_pGameObjects["Number100s"]->SetPosition(vec3(m_pGameObjects["Camera"]->GetPosition().x + 2 - 0.5f, m_pGameObjects["Camera"]->GetPosition().y + 3.5f, m_pGameObjects["Number1000s"]->GetPosition().z));
			m_pGameObjects["Number10s"]->SetPosition(vec3(m_pGameObjects["Camera"]->GetPosition().x + 2 + 0.0f, m_pGameObjects["Camera"]->GetPosition().y + 3.5f, m_pGameObjects["Number1000s"]->GetPosition().z));
			m_pGameObjects["Number1s"]->SetPosition(vec3(m_pGameObjects["Camera"]->GetPosition().x + 2 + 0.5f, m_pGameObjects["Camera"]->GetPosition().y + 3.5f, m_pGameObjects["Number1000s"]->GetPosition().z));


		}

		if (m_pGameObjects["Player"]->GetPosition().x >= 30)
		{
			OutputMessage("Player Is At End\n");
			m_pGame->SwitchScene("TestLevel");
		}

		if (g_KeyStates['N'])
		{
			m_ButtonTimer += deltatime;
			if (m_ButtonTimer >= 0.1)
			{
				if (m_Muted == false)
				{
					m_pSoundGuy->MuteSound();
					m_Muted = true;
					m_MutedMusic = true;
				}

				else
				{
					m_pSoundGuy->UnMuteSound();
					m_Muted = false;
					m_MutedMusic = false;
				}
				m_ButtonTimer = 0;
			}
		}
		if (g_KeyStates['M'])
		{
			m_ButtonTimer += deltatime;
			if (m_ButtonTimer >= 0.1)
			{
				if (m_MutedMusic == false)
				{
					m_pSoundGuy->MuteMusic(m_MusicChannel);
					m_MutedMusic = true;
				}

				else
				{
					m_pSoundGuy->UnMuteMusic(m_MusicChannel);
					m_MutedMusic = false;
				}
				m_ButtonTimer = 0;
			}
		}
		//load and save
		if (g_KeyStates['Z'])
			SaveManager->LoadState();

		if (g_KeyStates['X'])
			SaveManager->SaveState();

	}
}


void FlashmanScene::Draw()
{
	Scene::Draw();

	CheckForGLErrors();
}
