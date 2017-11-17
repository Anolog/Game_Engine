#ifndef __GameCore_H__
#define __GameCore_H__

class GameCore;

class Scene;
class ResourceManager;

class GameCore
{
protected:
    Scene* m_pScene;
    Scene* m_pTestScene;
	Scene* m_pFlashman;
	Scene* m_pTitleScene;
	Scene* m_pGameOverScene;
	Scene* m_pPauseScene;

    ResourceManager* m_pResources;

    mat4 m_ProjectionMatrix;
    unsigned int m_WindowWidth;
    unsigned int m_WindowHeight;

    bool m_IsDebugDrawEnabled;

    std::queue<Event*> m_EventQueue;

    PlayerController m_Controllers[4];

	SceneManager* m_pSceneManager;

public:
    GameCore();
    virtual ~GameCore();

    virtual void OnSurfaceChanged(unsigned int width, unsigned int height);
    virtual void RebuildOpenGLContent();
    virtual void LoadContent();
	virtual void SwitchScene(std::string sceneName);
    virtual void SwitchScene(int sceneid);

    virtual void QueueEvent(Event* pEvent);
    virtual void ProcessEvents();
    virtual void OnEvent(Event* pEvent);

    virtual void Update(float deltatime);
    virtual void Draw();

	SceneManager* GetSceneManager();

    bool IsDebugDrawEnabled() { return m_IsDebugDrawEnabled; }
    void SetDebugDrawEnabled(bool enabled) { m_IsDebugDrawEnabled = enabled; }

    PlayerController* GetController(int index) { return &m_Controllers[index]; }

    mat4* GetProjectionMatrix() { return &m_ProjectionMatrix; }
};

#endif //__GameCore_H__
