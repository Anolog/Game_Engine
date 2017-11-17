#include "pch.h"

Scene::Scene(GameCore* pGame, ResourceManager* pResources)
{
    m_pGame = pGame;
    m_pResources = pResources;
	m_IsLoaded = false;
    m_pBox2DWorld = 0;
}

Scene::~Scene()
{
	//Okay, so.. if you crash here, this is why..
	//The player bullet or enemy bullet gets put into this->scene
	//Because of that, when this->scene deletes, it deletes the player/enemy that creates it, which is deleting it
	//So it's trying to delete it again, which is causing this issue... but even if it's set to != nullprt... it crashes...?
	//Anyways, this only happens when the bullet is still out of the pool because it's not outside the fallout range

    for( auto object: m_pGameObjects )
    {
		if (object.second && object.second->returnType()!="Pooled")
			delete object.second;
    }

    delete m_pBox2DWorld;
}

void Scene::OnSurfaceChanged(unsigned int width, unsigned int height)
{
}

void Scene::RebuildOpenGLContent()
{
    m_pResources->GetShader( "Box2DDebugShader" )->Reload();
}

void Scene::LoadContent()
{
    //m_pResources->AddShader("Box2DDebugShader", new ShaderProgram("Data/Shaders/Box2DDebug.vert", "Data/Shaders/Box2DDebug.frag"));
    m_pBox2DWorld = new Box2DWorld(m_pResources->GetShader("Box2DDebugShader"), 0, 0);
}

void Scene::Reset()
{
    // reset all of the Scene objects in the list.
    for( auto it = m_pGameObjects.cbegin(); it != m_pGameObjects.cend(); )
    {
        auto object = it;
        it++; // pre-increment the iterator, so if this object is removed we already know the next one
		if (object->second->returnType()!="Pooled")
		{
			object->second->Reset();
		}
		if (object->second->returnType() == "Enemy")
		{
			((JumpingEnemy*)object->second)->setIsAlive(true);
			((JumpingEnemy*)object->second)->SetMask(MASK_ENEMY);
			((JumpingEnemy*)object->second)->SetCategory(CATEGORY_ENEMY);
		}

		if (object->second->GetName()=="Player")
		{
			((PlayerObject*)object->second)->setIsAlive(true);
		}
    }
	//reset score
	m_Score = 0;

}

void Scene::OnEvent(Event* pEvent)
{
}

void Scene::Update(float deltatime)
{
    CheckForGLErrors();

    m_pBox2DWorld->Update( deltatime );

    // update all of the Scene objects in the list.
    for( auto it = m_pGameObjects.cbegin(); it != m_pGameObjects.cend(); )
    {
        auto object = it;
        it++; // pre-increment the iterator, so if this object is removed we already know the next one

        object->second->Update( deltatime );
    }

#if WIN32
    if( g_KeyStates['R'] )
    {
        Reset();
    }
#endif

    CheckForGLErrors();
}

void Scene::Draw()
{
    CheckForGLErrors();

    vec3 campos = GetGameObject("Camera")->GetPosition();

    mat4* pViewMat = ((CameraObject*)GetGameObject("Camera"))->GetViewMatrix();
    mat4* pProjMat = GetGameCore()->GetProjectionMatrix();

    // render all of the Scene objects in the list.
    for( int i=0; i<5; i++ )
    {
        //for( std::map<std::string, GameObject*>::iterator iterator = m_pGameObjects.begin(); iterator != m_pGameObjects.end(); iterator++ )
        for( auto object: m_pGameObjects )
        {

			if( object.second->GetRenderOrder() == i)
            {
				if (object.second->returnType() == "Skybox")
				{
					glClear(GL_DEPTH_BUFFER_BIT);
					glDepthMask(false);
					glDisable(GL_DEPTH_TEST);
				}
				else 
				{
					glDepthMask(true);
					glEnable(GL_DEPTH_TEST);
				}
                object.second->Draw( i, pViewMat, pProjMat, campos );
            }
			
        }
    }

    if( m_pGame->IsDebugDrawEnabled() )
    {
        m_pBox2DWorld->GetWorld()->DrawDebugData();
    }

    CheckForGLErrors();
}

void Scene::AddToScene(std::string name, GameObject* object)
{
    object->SetScene( this );
    m_pGameObjects[name] = object;
}

void Scene::RemoveFromScene(GameObject* object)
{
    m_pGameObjects.erase( object->GetName() );
}
