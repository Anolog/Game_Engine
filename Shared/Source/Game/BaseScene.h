#ifndef __BaseScene_H__
#define __BaseScene_H__

class SaveScene;
class BaseScene : public Scene
{
private:
	SaveScene* SaveManager;
	SoundManager* m_pSoundGuy;
	float m_ScoreTimer;
	float m_ButtonTimer;
	int m_MusicChannel;
	bool m_Muted;
	bool m_MutedMusic;



public:
    BaseScene(GameCore* pGame, ResourceManager* pResources);
    virtual ~BaseScene();

    virtual void LoadContent();

	virtual void RebuildOpenGLContent();



    virtual void Update(float deltatime);
	virtual void Draw();

};

#endif //__BaseScene_H__
