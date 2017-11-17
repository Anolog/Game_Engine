#ifndef __GameObject_H__
#define __GameObject_H__

class ShaderProgram;
class Mesh;
class Scene;
class SoundManager;

class GameObject
{
protected:
	vec3 m_InitialPosition;
	vec3 m_InitialRotation;

	Scene* m_pScene;
	std::string type;
	unsigned int m_RenderOrder;

	std::string m_Name;

	vec3 m_Position;
	vec3 m_Rotation;
	vec3 m_Scale;

	b2Body* m_pPhysicsBody;



	Mesh* m_pMesh;
	ShaderProgram* m_pShaderProgram;
	GLuint m_TextureHandle;
	int m_Frame;

protected:
	b2Fixture* AddPhysicsShape(b2Shape* shape, bool dynamic, bool active, float density, short category, short mask, bool sensor);

public:
	GameObject(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, const char* meshname, const char* shadername, const char* texturename);
	virtual ~GameObject();

	unsigned int GetRenderOrder() { return m_RenderOrder; }
	void SetRenderOrder(unsigned int order) { m_RenderOrder = order; }

	virtual void Init(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, const char* meshname, const char* shadername, const char* texturename);

	virtual void Reset();
	virtual void AddToScene(Scene* pScene);
	virtual void RemoveFromScene();

	virtual void Update(float deltatime);
	virtual void CustomUniformSetup() {}
	virtual void Draw(int renderorder, mat4* viewmat, mat4* projmat, vec3 campos);

	virtual void BeginContact(b2Fixture* pOtherFixture, vec2 worldPosition, vec2 worldNormal);
	virtual void EndContact(b2Fixture* pOtherFixture);

	b2Fixture* AddPhysicsBox(vec2 size, bool dynamic, bool active, float density, short category, short mask, bool sensor = false);
	b2Fixture* AddPhysicsDentedBox(vec2 size, float indent, bool dynamic, bool active, float density, short category, short mask, bool sensor = false);
	b2Fixture* AddPhysicsChain(b2Vec2* pointlist, int numpoints, bool dynamic, bool active, float density, short category, short mask, bool sensor = false);
	b2Fixture* AddPhysicsCircle(float radius, bool dynamic, bool active, float density, short category, short mask, bool sensor = false);
	void SetMask(FilterMasks mask);
	void SetCategory(FilterCategory category);

	// Getters/Setters
	void SetMesh(Mesh* pMesh) { m_pMesh = pMesh; }
	void SetShader(ShaderProgram* pShader) { m_pShaderProgram = pShader; }
	void SetTexture(GLuint texturehandle) { m_TextureHandle = texturehandle; }
	void SetFrame(int frame) { m_Frame = frame; }

	void SetName(std::string name) { m_Name = name; }
	std::string GetName() { return m_Name; }
	Mesh* GetMesh() { return m_pMesh; }
	ShaderProgram* GetShader() { return m_pShaderProgram; }
	GLuint GetTexture() { return m_TextureHandle; }
	b2Body* GetPhysicsBody() { return m_pPhysicsBody; }

	void SetScene(Scene* pScene) { m_pScene = pScene; }

	void SetPosition(vec3 pos);
	void SetRotation(vec3 rot);
	void SetScale(vec3 scale) { m_Scale = scale; }
	void setType(std::string setType) { type = setType; }
	std::string returnType();
    vec3 GetPosition() { return m_Position; }
    vec3 GetRotation() { return m_Rotation; }
    vec3 GetScale()  { return m_Scale; }
};

#endif //__GameObject_H__
