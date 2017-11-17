#ifndef __Collectable_H__
#define __Collectable_H__



class Collectable : public GameObject
{
protected:
	bool m_IsPicked;
	int m_PointsWorth;

	SoundManager* m_pSoundDude;

public:
	Collectable(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, const char* meshname, const char* shadername, const char* texturename, int pointsAdded);
	virtual ~Collectable();

	virtual void Update(float deltatime);

	virtual void BeginContact(b2Fixture* pOtherFixture, vec2 worldPosition, vec2 worldNormal);
	virtual void EndContact(b2Fixture* pOtherFixture);

};

#endif //__JumpingEnemy_H__