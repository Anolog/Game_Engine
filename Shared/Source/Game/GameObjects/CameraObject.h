#ifndef __CameraObject_H__
#define __CameraObject_H__

class CameraObject : public GameObject
{
protected:
    mat4 m_ViewMatrix;

public:
    CameraObject(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, const char* meshname, const char* shadername, const char* texturename);
    virtual ~CameraObject();

    virtual void Update(float deltatime);

    mat4* GetViewMatrix() { return &m_ViewMatrix; }
};

class WindowCameraObject : public GameObject
{
protected:
	mat4 m_ViewMatrix;

public:
	WindowCameraObject(Scene* pScene, std::string name, vec3 pos, vec3 rot, vec3 scale, const char* meshname, const char* shadername, const char* texturename);
	virtual ~WindowCameraObject();

	float m_RightBound;
	float m_LeftBound;
	float m_UpBound;
	float m_DownBound;

	virtual void Update(float deltatime);

	mat4* GetViewMatrix() { return &m_ViewMatrix; }
};

#endif //__CameraObject_H__
