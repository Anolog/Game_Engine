#ifndef __Scene_H__
#define __Scene_H__

class GameCore;
class GameObject;
class Box2DWorld;

class Scene
{
protected:
	GameCore* m_pGame;
	ResourceManager* m_pResources;

	std::map<std::string, GameObject*> m_pGameObjects;

	Box2DWorld* m_pBox2DWorld;
	int m_Score;
	bool m_UpdatingWhileInStack;
	bool m_IsLoaded;

public:
	Scene(GameCore* pGame, ResourceManager* pResources);
	virtual ~Scene();

	virtual void OnSurfaceChanged(unsigned int width, unsigned int height);
	virtual void RebuildOpenGLContent();
	virtual void LoadContent();

	virtual void Reset();
	virtual int GetScore() { return m_Score; };
	virtual void SetScore(int score) { m_Score = score;}
	virtual void AddtoScore(int add) { m_Score += add; }
	virtual void SetUpdatingWhileInStack(bool updatingWhileinDaStack) { m_UpdatingWhileInStack = updatingWhileinDaStack; }

	virtual void OnEvent(Event* pEvent);
	virtual void Update(float deltatime);
	virtual void Draw();

	Box2DWorld* GetBox2DWorld() { return m_pBox2DWorld; }
	GameObject* GetGameObject(const char* name) { return m_pGameObjects[name]; }
	GameObject* GetGameObject(std::string name) { return m_pGameObjects[name]; }

	void AddToScene(std::string name, GameObject* object);
	void RemoveFromScene(GameObject* object);

	GameCore* GetGameCore() { return m_pGame; }
	ResourceManager* GetResourceManager() { return m_pResources; }
};

#endif //__Scene_H__
