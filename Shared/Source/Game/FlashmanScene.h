#ifndef __FlashmanScene_H__
#define __FlashmanScene_H__

class SaveScene;
class FlashmanScene : public Scene
{
private:
	SaveScene* SaveManager;
	SoundManager* m_pSoundGuy;
	float m_ScoreTimer;
	float m_Score;
	float m_ButtonTimer;
	int m_MusicChannel;
	bool m_Muted;
	bool m_MutedMusic;

public:
	FlashmanScene(GameCore* pGame, ResourceManager* pResources);
    virtual ~FlashmanScene();

    virtual void LoadContent();

	virtual void RebuildOpenGLContent();

	float getScore() { return m_Score; }
	void setScore(float score) { m_Score = score; }
	void addScore(float score) { m_Score += score; }

    virtual void Update(float deltatime);
	virtual void Draw();

};

#endif //__FlashmanScene_H__
