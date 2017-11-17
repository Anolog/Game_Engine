#include "pch.h"
#include "GameCore.h"

GameCore::GameCore()
{
    m_pScene = 0;
    m_pTestScene = 0;
	m_pFlashman = 0;
	m_pGameOverScene = 0;
	m_pTitleScene = 0;
	m_pPauseScene = 0;

    m_pResources = 0;
	m_pSceneManager = 0;
    
    m_IsDebugDrawEnabled = false;
}

GameCore::~GameCore()
{
    while( m_EventQueue.size() != 0 )
    {
        Event* pEvent = m_EventQueue.front();
        m_EventQueue.pop();
        delete pEvent;
    }

	//Loops through the map and deletes memory
	std::pair<std::string, Scene*> temp;
	for (auto temp : m_pSceneManager->getInstance()->m_SceneList)
	{
		delete temp.second;
	}
	
    m_pSceneManager->ResetInstance();
    delete m_pSceneManager;

    delete m_pResources;

}

void GameCore::OnSurfaceChanged(unsigned int width, unsigned int height)
{
    glViewport( 0, 0, width, height );

    m_WindowWidth = width;
    m_WindowHeight = height;

    //m_ProjectionMatrix.CreateOrtho( -6, 6, -6, 6, -100, 100 );
    m_ProjectionMatrix.CreatePerspectiveVFoV( 45, (float)width/height, 0.01f, 1000.0f );
}

void GameCore::RebuildOpenGLContent()
{
    m_pScene->RebuildOpenGLContent();

    // turn on various gl modes
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );

    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
    glFrontFace( GL_CW );

    // turn on alpha blending.
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
}

void GameCore::LoadContent()
{
    // turn on various gl modes
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );

    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
    glFrontFace( GL_CW );

    // turn on alpha blending.
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    m_pResources = new ResourceManager();

	m_pSceneManager = new SceneManager();

	m_pScene = new MainMenuScene(this, m_pResources);
	m_pTestScene = new BaseScene(this, m_pResources);
	m_pFlashman = new FlashmanScene(this, m_pResources);
	m_pTitleScene = new TitleScreen(this, m_pResources);
	m_pGameOverScene = new GameOverScene(this, m_pResources);
	m_pPauseScene = new PauseScene(this, m_pResources);


    m_pResources->AddShader("Box2DDebugShader", new ShaderProgram("Data/Shaders/Box2DDebug.vert", "Data/Shaders/Box2DDebug.frag"));

	//Adds scene into the list of them available
	m_pSceneManager->getInstance()->m_SceneList.insert(std::pair<std::string, Scene*>("MainMenu", m_pScene));
    m_pSceneManager->getInstance()->m_SceneList.insert(std::pair<std::string, Scene*>("TestLevel", m_pTestScene));
	m_pSceneManager->getInstance()->m_SceneList.insert(std::pair<std::string, Scene*>("Flashman", m_pFlashman));
	m_pSceneManager->getInstance()->m_SceneList.insert(std::pair<std::string, Scene*>("Title", m_pTitleScene));
	m_pSceneManager->getInstance()->m_SceneList.insert(std::pair<std::string, Scene*>("GameOver", m_pGameOverScene));
	m_pSceneManager->getInstance()->m_SceneList.insert(std::pair<std::string, Scene*>("Pause", m_pPauseScene));
	
    SwitchScene( "Title" );

    CheckForGLErrors();
}

void GameCore::SwitchScene(std::string sceneName)
{
	//What the fuck is this call.
	m_pSceneManager->getInstance()->AddScene(m_pSceneManager->getInstance()->m_SceneList.find(sceneName)->second);
	m_pSceneManager->getInstance()->GetCurrentScene()->LoadContent();
	
}

void GameCore::SwitchScene(int sceneid)
{

    if( sceneid > 9 )
        return;

    delete m_pScene;

	//m_pScene = new BaseScene(this, m_pResources);


    m_pScene->LoadContent();
}

void GameCore::QueueEvent(Event* pEvent)
{
    m_EventQueue.push( pEvent );
}

void GameCore::ProcessEvents()
{
    while( m_EventQueue.size() != 0 )
    {
        Event* pEvent = m_EventQueue.front();
        m_EventQueue.pop();

        OnEvent( pEvent );
        delete pEvent;
    }
}

void GameCore::OnEvent(Event* pEvent)
{
    m_pScene->OnEvent( pEvent );

    if( pEvent->GetEventType() == EventType_RemoveFromScene )
    {
        RemoveFromSceneEvent* pRemoveFromSceneEvent = (RemoveFromSceneEvent*)pEvent;

        GameObject* pGameObject = pRemoveFromSceneEvent->GetGameObject();
        pGameObject->RemoveFromScene();
    }

    if( pEvent->GetEventType() == EventType_Input )
    {
        InputEvent* pInput = (InputEvent*)pEvent;

#if _WIN32
        if( pInput->GetInputDeviceType() == InputDeviceType_Keyboard )
        {
            int id = pInput->GetID();

            if( pInput->GetInputState() == InputState_Pressed )
            {
                if( id >= '1' && id <= '9' )
                {
                    SwitchScene( id - '0' - 1 );
                }

                if( id == VK_F5 )
                {
                    RebuildOpenGLContent();
                }

                if( id == VK_F9 )
                {
                    m_IsDebugDrawEnabled = !m_IsDebugDrawEnabled;
                }

                if( id == VK_UP || id == 'W' )
                    m_Controllers[0].SetButtonPressed( PCB_Up );
                if( id == VK_DOWN || id == 'S' )
                    m_Controllers[0].SetButtonPressed( PCB_Down );
                if( id == VK_LEFT || id == 'A' )
                    m_Controllers[0].SetButtonPressed( PCB_Left );
                if( id == VK_RIGHT || id == 'D' )
                    m_Controllers[0].SetButtonPressed( PCB_Right );
				if (id == VK_SPACE || id == ' ')
					m_Controllers[0].SetButtonPressed(PCB_Space);
            }

            if( pInput->GetInputState() == InputState_Released )
            {
                if( id == VK_UP || id == 'W' )
                    m_Controllers[0].SetButtonReleased( PCB_Up );
                if( id == VK_DOWN || id == 'S' )
                    m_Controllers[0].SetButtonReleased( PCB_Down );
                if( id == VK_LEFT || id == 'A' )
                    m_Controllers[0].SetButtonReleased( PCB_Left );
                if( id == VK_RIGHT || id == 'D' )
                    m_Controllers[0].SetButtonReleased( PCB_Right );
				if (id == VK_SPACE || id == ' ')
					m_Controllers[0].SetButtonReleased(PCB_Space);
            }
        }
#endif // _WIN32

        if( pInput->GetInputDeviceType() == InputDeviceType_Gamepad )
        {
            if( pInput->GetInputState() == InputState_Held )
            {
                if( pInput->GetGamepadID() == GamepadID_LeftStick )
                {
                    if( pInput->GetPosition().y > 0 )
                        m_Controllers[0].SetButtonPressed( PCB_Up );
                    if( pInput->GetPosition().y < 0 )
                        m_Controllers[0].SetButtonPressed( PCB_Down );
                    if( pInput->GetPosition().x < 0 )
                        m_Controllers[0].SetButtonPressed( PCB_Left );
                    if( pInput->GetPosition().x > 0 )
                        m_Controllers[0].SetButtonPressed( PCB_Right );

                    if( pInput->GetPosition().y == 0 )
                    {
                        m_Controllers[0].SetButtonReleased( PCB_Up );
                        m_Controllers[0].SetButtonReleased( PCB_Down );
                    }
                    if( pInput->GetPosition().x == 0 )
                    {
                        m_Controllers[0].SetButtonReleased( PCB_Left );
                        m_Controllers[0].SetButtonReleased( PCB_Right );
                    }
                }
            }
        }
    }
}

void GameCore::Update(float deltatime)
{
    CheckForGLErrors();

    ProcessEvents();
    //m_pScene->Update( deltatime );

    m_pSceneManager->getInstance()->GetCurrentScene()->Update(deltatime);

    CheckForGLErrors();
}

void GameCore::Draw()
{
    CheckForGLErrors();

    // clear the screen to dark blue.
    glClearColor( 0.0f, 0.0f, 0.3f, 0.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    //m_pScene->Draw();
    m_pSceneManager->getInstance()->GetCurrentScene()->Draw();
    CheckForGLErrors();
}

SceneManager * GameCore::GetSceneManager()
{
	return m_pSceneManager;
}
