#ifndef __JumpingEnemy_H__
#define __JumpingEnemy_H__



class JumpingEnemy : public GameObject
{
protected:
	bool m_CanJump;
	bool m_Alive;
	float m_DetectionRange;
	float m_JumpTimer;
	float m_DeathTimer;
	float m_StartXPos;

public:
	JumpingEnemy(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, const char* meshname, const char* shadername, const char* texturename);
	virtual ~JumpingEnemy();

	virtual void Update(float deltatime);
	void setIsAlive(bool alive) { m_Alive = alive; }
	virtual void BeginContact(b2Fixture* pOtherFixture, vec2 worldPosition, vec2 worldNormal);
	virtual void EndContact(b2Fixture* pOtherFixture);

};

#endif //__JumpingEnemy_H__