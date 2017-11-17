#ifndef __TurretEnemy_H__
#define __TurretEnemy_H__

#include "../SimplePool.h"

class TurretEnemy : public GameObject
{
protected:
	bool m_isHit;
	bool m_disableOnce;
	float m_BulletTimer;
	float m_DetectionRange;
	vec3 m_AimPos;
	int m_ID;

	SimplePool<GameObject> m_pTurretBullets;
	std::vector<GameObject*> m_pActiveBullets;

public:
	TurretEnemy(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, const char* meshname, const char* shadername, const char* texturename, int ID);
	virtual ~TurretEnemy();

	virtual void Update(float deltatime);

	virtual void BeginContact(b2Fixture* pOtherFixture, vec2 worldPosition, vec2 worldNormal);
	virtual void EndContact(b2Fixture* pOtherFixture);

};

#endif //__TurretEnemy_H__