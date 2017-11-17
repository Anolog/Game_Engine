#ifndef __PlayerObject_H__
#define __PlayerObject_H__

#include "../SimplePool.h"

class PlayerObject : public GameObject
{
protected:
    bool m_CanJump;
	bool m_IsAlive;
	float m_bulletTimerCounter;
	float m_WalkSoundTimer;
	float m_AnimTimer;
	float m_DeathTimer;
	float m_BulletAimPos;

    PlayerController* m_pController;

	SoundManager* m_pSoundDude;

	SimplePool<GameObject> m_PlayerBulletPool;
	std::vector<GameObject*> m_pActiveBullets;

public:
    PlayerObject(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, const char* meshname, const char* shadername, const char* texturename);
    virtual ~PlayerObject();

    void SetController(PlayerController* pController);
	void setIsAlive(bool alive) { m_IsAlive = alive; }
    virtual void Update(float deltatime);

    virtual void BeginContact(b2Fixture* pOtherFixture, vec2 worldPosition, vec2 worldNormal);
    virtual void EndContact(b2Fixture* pOtherFixture);
};

#endif //__PlayerObject_H__
