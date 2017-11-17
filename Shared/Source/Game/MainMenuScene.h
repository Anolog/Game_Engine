#ifndef __MainMenuScene_H__
#define __MainMenuScene_H__

class MainMenuScene : public Scene
{
private:
	TweenFuncs tween;

public:
	MainMenuScene(GameCore* pGame, ResourceManager* pResources);
    virtual ~MainMenuScene();

    virtual void LoadContent();

	virtual void RebuildOpenGLContent();

    virtual void Update(float deltatime);
	virtual void Draw();

};

#endif //__MainMenuScene_H__
