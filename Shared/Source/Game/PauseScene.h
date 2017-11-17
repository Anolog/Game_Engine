#ifndef __PauseScene_H__
#define __PauseScene_H__

class PauseScene : public Scene
{
private:

public:
	PauseScene(GameCore* pGame, ResourceManager* pResources);
    virtual ~PauseScene();

    virtual void LoadContent();

	virtual void RebuildOpenGLContent();

    virtual void Update(float deltatime);
	virtual void Draw();

};

#endif //__PauseScene_H__
