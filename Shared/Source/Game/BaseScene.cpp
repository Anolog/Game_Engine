#include "pch.h"
#include "BaseScene.h"

//Use this num to create unique names for all the turret bullets for multiple turrets
static int TURRET_NUM = 0;


BaseScene::BaseScene(GameCore* pGame, ResourceManager* pResources)
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

BaseScene::~BaseScene()
{
	if (SaveManager != nullptr)
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

void BaseScene::LoadContent()
{
	Scene::LoadContent();

	m_pSoundGuy->initialize();
	//debugdraw -> Change to toggle key later
	m_pGame->SetDebugDrawEnabled(true);
	m_MusicChannel = m_pSoundGuy->playSound(Sound_Music);                     //comment this out to stop music

	 //SoundGuy->stopSound(musicChannel); 
	// create our shaders.
	m_pResources->AddShader("White", new ShaderProgram("Data/Shaders/white.vert", "Data/Shaders/white.frag"));
	m_pResources->AddShader("Lighting", new ShaderProgram("Data/Shaders/Lighting.vert", "Data/Shaders/Lighting.frag"));
	//CubeMap Shader
	m_pResources->AddShader("CubeS", new ShaderProgram("Data/Shaders/Cube.vert", "Data/Shaders/Cube.frag"));

	//Cube Reflection
	m_pResources->AddShader("CubeReflection", new ShaderProgram("Data/Shaders/CubeReflection.vert", "Data/Shaders/CubeReflection.frag"));
	//Number Shader
	m_pResources->AddShader("Font", new ShaderProgram("Data/Shaders/font.vert", "Data/Shaders/font.frag"));

	// create meshes.
	//inverted faced cube mesh
	m_pResources->AddMesh("Cube", Mesh::SkyCube(vec3(5, 5, 5), vec2(1.0f, 1.0f), true));

	//reflection
	m_pResources->AddMesh("Reflection", Mesh::CreateCube(vec3(5, 5, 5))); // name, location

	// load our textures.
	m_pResources->AddTexture("Megaman", LoadTexture("Data/Textures/Megaman.png"));
	m_pResources->AddTexture("TestPlayer", LoadTexture("Data/Textures/Player.png"));
	m_pResources->AddTexture("JumpingEnemy", LoadTexture("Data/Textures/JumpingEnemy.png"));
	m_pResources->AddTexture("Bullet", LoadTexture("Data/Textures/Meteor.png"));
	m_pResources->AddTexture("Numbers", LoadTexture("Data/Textures/Numbers.png"));
	m_pResources->AddTexture("Turret", LoadTexture("Data/Textures/Turret.png"));
	m_pResources->AddTexture("Floor", LoadTexture("Data/Textures/Floor1.png"));
	m_pResources->AddTexture("Pizza", LoadTexture("Data/Textures/Pizza.png"));
	//cube map textures
	//order
	//#define GL_TEXTURE_CUBE_MAP_POSITIVE_X    0x8515
	//#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X    0x8516
	//#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y    0x8517
	//#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y    0x8518
	//#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z    0x8519
	//#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z    0x851A

	const char* CubeFileNames[6];
	CubeFileNames[0] = "Data/Textures/posx.png";
	CubeFileNames[1] = "Data/Textures/negx.png";
	CubeFileNames[2] = "Data/Textures/posy.png";
	CubeFileNames[3] = "Data/Textures/negy.png";
	CubeFileNames[4] = "Data/Textures/posz.png";
	CubeFileNames[5] = "Data/Textures/negz.png";


	m_pResources->AddTexture("CubeTexture", LoadTextureCubemap(CubeFileNames));

	// create some game objects.
	{
		//create the game's skybox.
		m_pGameObjects["Skybox"] = new GameObject(this, "Skybox", vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 1, 1), "Cube", "CubeS", "CubeTexture");
		m_pGameObjects["Skybox"]->setType("Skybox");
		m_pGameObjects["Skybox"]->SetRenderOrder(0);

		
		m_pGameObjects["Reflection1"] = new Collectable(this, "Reflection1", vec3(0,1, 0), vec3(0, 0, 0), vec3(0.2, 0.2, 0.2), "Reflection", "CubeReflection", "CubeTexture",500);
		m_pGameObjects["Reflection1"]->AddPhysicsBox(vec2(0.75f, 0.75f), false, true, 1.0f, CATEGORY_PICKUP, MASK_PICKUP, true);



		m_pGameObjects["Reflection1"]->SetRenderOrder(1);

		// Camera
		//Swap to follow camera - Copy previous files over
		m_pGameObjects["Camera"] = new WindowCameraObject(this, "Camera", Vector3(0, 0, -15), Vector3(0, 0, 0), Vector3(1, 1, 1), 0, 0, 0);
		m_pGameObjects["Camera"]->SetPosition(vec3(0, 0, -15));

		// player
		m_pGameObjects["Player"] = new PlayerObject(this, "Player", vec3(-5, 0, 0), vec3(0, 0, 0), vec3(1, 1, 1), "Sprite", "Font", "TestPlayer");
		((PlayerObject*)m_pGameObjects["Player"])->SetController(m_pGame->GetController(0));

		m_pGameObjects["Player"]->AddPhysicsBox(vec2(0.75f, 0.75f), true, true, 1.0f, CATEGORY_PLAYER, MASK_PLAYER);
		//Make player unable to rotate
		m_pGameObjects["Player"]->GetPhysicsBody()->SetFixedRotation(true);
		m_pGameObjects["Player"]->SetRenderOrder(2);


		m_pGameObjects["Turret"] = new TurretEnemy(this, "Turret", vec3(6, 0, 0), vec3(0, 0, 0), vec3(1, 1, 1), "Sprite", "Texture", "Turret", TURRET_NUM);
		TURRET_NUM++;
		m_pGameObjects["Turret"]->SetRenderOrder(1);

		m_pGameObjects["Jumper"] = new JumpingEnemy(this, "Jumper", vec3(3.5, 0.5, 0), vec3(0, 0, 0), vec3(1, 1, 1), "Sprite", "Font", "JumpingEnemy");
		m_pGameObjects["Jumper"]->AddPhysicsBox(vec2(0.75f, 0.75f), true, true, 1.0f, CATEGORY_ENEMY, MASK_ENEMY);
		m_pGameObjects["Jumper"]->GetPhysicsBody()->SetFixedRotation(true);
		m_pGameObjects["Jumper"]->SetRenderOrder(2);


		m_pGameObjects["Coin"] = new Collectable(this, "Coin", vec3(-1, 0.5, 0), vec3(0, 0, 0), vec3(1, 1, 1), "Sprite", "Lighting", "Pizza", 300);

		m_pGameObjects["Coin"] = new Collectable(this, "Coin", vec3(-1, 0.5, 0), vec3(0, 0, 0), vec3(1, 1, 1), "Sprite", "Lighting", "Pizza", 1111);

		m_pGameObjects["Coin"] = new Collectable(this, "Coin", vec3(-1, 0.5, 0), vec3(0, 0, 0), vec3(1, 1, 1), "Sprite", "Texture", "Pizza", 1111);

		m_pGameObjects["Coin"]->AddPhysicsCircle(0.5f, false, true, 1.0f, CATEGORY_PICKUP, MASK_PICKUP, true);




		//Create base ground
		m_pGameObjects["Ground"] = new GameObject(this, "Ground", vec3(0, -5, 0), vec3(0, 0, 0), vec3(50, 1, 1), "Sprite", "Lighting", "Floor");
		m_pGameObjects["Ground"]->AddPhysicsBox(vec2(50, 1), false, true, 1.0f, CATEGORY_GROUND, MASK_GROUND_NO_BULLETS);
		m_pGameObjects["Ground"]->SetRenderOrder(1);


		// UI
		m_pGameObjects["Number1000s"] = new GameObject(this, "Number1000s", vec3(-3, -1, -5), vec3(0, 0, 0), vec3(0.5, 0.5, 0.5), "Sprite", "Font", "Numbers"); //pushed forward in z
		m_pGameObjects["Number100s"] = new GameObject(this, "Number100s", vec3(-1, -1, -5), vec3(0, 0, 0), vec3(0.5, 0.5, 0.5), "Sprite", "Font", "Numbers");
		m_pGameObjects["Number10s"] = new GameObject(this, "Number10s", vec3(1, -1, -5), vec3(0, 0, 0), vec3(0.5, 0.5, 0.5), "Sprite", "Font", "Numbers");
		m_pGameObjects["Number1s"] = new GameObject(this, "Number1s", vec3(3, -1, -5), vec3(0, 0, 0), vec3(0.5, 0.5, 0.5), "Sprite", "Font", "Numbers");


		m_pGameObjects["Number1000s"]->SetRenderOrder(2);
		m_pGameObjects["Number100s"]->SetRenderOrder(2);
		m_pGameObjects["Number10s"]->SetRenderOrder(2);
		m_pGameObjects["Number1s"]->SetRenderOrder(2);



		/*
		m_pGameObjects["Wall"] = new GameObject(this, "Wall", vec3(5, -2, 0), vec3(0, 0, 90), vec3(10, 1, 1), "Sprite", "Lighting", "Player");
		m_pGameObjects["Wall"]->AddPhysicsBox(vec2(10, 1), false, true, 1.0f, CATEGORY_GROUND, MASK_GROUND_NO_BULLETS);
		m_pGameObjects["Wall"]->SetRenderOrder(1);
		*/
	}

	CheckForGLErrors();
}

void BaseScene::RebuildOpenGLContent()
{
	Scene::RebuildOpenGLContent();

	m_pResources->GetShader("Texture")->Reload();

	Mesh::CreateBox(vec2(1, 1), false, m_pResources->GetMesh("Sprite"));
	Mesh::CreateBox(vec2(1, 1), true, m_pResources->GetMesh("SpriteReversed)"));
}

void BaseScene::Update(float deltatime)
{
	if (m_UpdatingWhileInStack)
	{
		Scene::Update(deltatime);

		{
			int temp1000s = (int)GetScore() / 1000;
			int temp100s = (int)(GetScore() - (temp1000s * 1000)) / 100;
			int temp10s = (int)((GetScore() - (temp1000s * 1000) - (temp100s * 100)) / 10);
			int temp1s = (int)(GetScore() - (temp1000s * 1000) - (temp100s * 100) - temp10s * 10);

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
		//load and save

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

		if (g_KeyStates['Z'])
			SaveManager->LoadState();

		if (g_KeyStates['X'])
			SaveManager->SaveState();
		

		
		//OutputMessage("Camera pos: %f , %f \n", m_pGameObjects["Camera"]->GetPosition().x, m_pGameObjects["Camera"]->GetPosition().y);
	}
}


void BaseScene::Draw()
{
	Scene::Draw();

	CheckForGLErrors();
}
