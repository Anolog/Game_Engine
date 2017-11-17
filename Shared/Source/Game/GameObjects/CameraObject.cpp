#include "pch.h"

CameraObject::CameraObject(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, const char* meshname, const char* shadername, const char* texturename)
: GameObject( pScene, name, pos, rot, scale, meshname, shadername, texturename )
{
}

CameraObject::~CameraObject()
{
}

void CameraObject::Update(float deltatime)
{
#if WIN32
	float camspeed = 10;
	
	if( g_KeyStates[VK_SHIFT] )
        camspeed = 1;

    if( g_KeyStates['J'] )
        m_Position.x -= camspeed * deltatime;
    if( g_KeyStates['L'] )
        m_Position.x += camspeed * deltatime;

    if( g_KeyStates['I'] )
        m_Position.z += camspeed * deltatime;
    if( g_KeyStates['K'] )
        m_Position.z -= camspeed * deltatime;

    if( g_KeyStates['U'] )
        m_Position.y += camspeed * deltatime;
    if( g_KeyStates['M'] )
        m_Position.y -= camspeed * deltatime;
#endif



    // setup camera/view matrix.
    m_ViewMatrix.CreateLookAtViewLeftHanded( m_Position, vec3(0,1,0), vec3(0,0,0) );
}

WindowCameraObject::WindowCameraObject(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, const char* meshname, const char* shadername, const char* texturename)
	: GameObject(pScene, name, pos, rot, scale, meshname, shadername, texturename)
{
	//Init setup
	m_RightBound = 6.0f;
	m_LeftBound = 6.0f;
	m_UpBound = 3.0f;
	m_DownBound = 2.0f;

	m_ViewMatrix.CreateLookAtViewLeftHanded(m_Position, vec3(0, 1, 0), vec3(0, 0, 0));
}

WindowCameraObject::~WindowCameraObject()
{
}

void WindowCameraObject::Update(float deltatime)
{
	GameObject::Update(deltatime);

	if (m_pScene->GetGameObject("Player")->GetPosition().x > m_pScene->GetGameObject("Camera")->GetPosition().x)
	{
		m_pScene->GetGameObject("Camera")->SetPosition(vec3(m_pScene->GetGameObject("Player")->GetPosition().x, m_pScene->GetGameObject("Camera")->GetPosition().y, m_pScene->GetGameObject("Camera")->GetPosition().z));
	}


	if (m_pScene->GetGameObject("Player")->GetPosition().x + ((WindowCameraObject*)m_pScene->GetGameObject("Camera"))->m_LeftBound < m_pScene->GetGameObject("Camera")->GetPosition().x)
	{
		m_pScene->GetGameObject("Camera")->SetPosition(vec3(m_pScene->GetGameObject("Player")->GetPosition().x + ((WindowCameraObject*)m_pScene->GetGameObject("Camera"))->m_RightBound, m_pScene->GetGameObject("Camera")->GetPosition().y, m_pScene->GetGameObject("Camera")->GetPosition().z));
	}

	if (m_pScene->GetGameObject("Player")->GetPosition().y >= m_pScene->GetGameObject("Camera")->GetPosition().y + ((WindowCameraObject*)m_pScene->GetGameObject("Camera"))->m_UpBound)
	{
		m_pScene->GetGameObject("Camera")->SetPosition(vec3(m_pScene->GetGameObject("Camera")->GetPosition().x, m_pScene->GetGameObject("Player")->GetPosition().y, m_pScene->GetGameObject("Camera")->GetPosition().z));
	}

	if (m_pScene->GetGameObject("Player")->GetPosition().y <= m_pScene->GetGameObject("Camera")->GetPosition().y - ((WindowCameraObject*)m_pScene->GetGameObject("Camera"))->m_DownBound)
	{
		m_pScene->GetGameObject("Camera")->SetPosition(vec3(m_pScene->GetGameObject("Camera")->GetPosition().x, m_pScene->GetGameObject("Player")->GetPosition().y + ((WindowCameraObject*)m_pScene->GetGameObject("Camera"))->m_DownBound, m_pScene->GetGameObject("Camera")->GetPosition().z));
	}
	m_ViewMatrix.CreateSRT(m_Scale, m_Rotation, m_Position);
	m_ViewMatrix.Inverse();
}
