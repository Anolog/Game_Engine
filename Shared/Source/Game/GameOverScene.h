#ifndef __GameOver_Scene_H__
#define __GameOver_Scene_H__

class GameOverScene : public Scene
{
private:

public:
	GameOverScene(GameCore* pGame, ResourceManager* pResources);
    virtual ~GameOverScene();

    virtual void LoadContent();

	virtual void RebuildOpenGLContent();

    virtual void Update(float deltatime);
	virtual void Draw();

};

#endif //__GameOver_Scene_H__
