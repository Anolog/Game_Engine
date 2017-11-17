#ifndef __TitleScreen_H__
#define __TitleScreen_H__

class TitleScreen : public Scene
{
private:
	bool m_ShowTitleText;


	TweenFuncs tween;
	float totaltime;


public:
	TitleScreen(GameCore* pGame, ResourceManager* pResources);
    virtual ~TitleScreen();

    virtual void LoadContent();

	virtual void RebuildOpenGLContent();

    virtual void Update(float deltatime);
	virtual void Draw();

};

#endif //__MainMenuScene_H__
