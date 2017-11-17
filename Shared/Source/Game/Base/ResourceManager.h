#ifndef __ResourceManager_H__
#define __ResourceManager_H__

class ResourceManager;

class ShaderProgram;
class Mesh;

class ResourceManager
{
protected:
    std::map<std::string, Mesh*> m_pMeshes;
    std::map<std::string, ShaderProgram*> m_pShaders;
    std::map<std::string, GLuint> m_pTextures;

public:
    ResourceManager();
    virtual ~ResourceManager();

    virtual void OnSurfaceChanged(unsigned int width, unsigned int height);

    void AddMesh(const char* name, Mesh* pMesh) { m_pMeshes[name] = pMesh; }
    void AddShader(const char* name, ShaderProgram* pShader) { m_pShaders[name] = pShader; }
    void AddTexture(const char* name, GLuint texturehandle) { m_pTextures[name] = texturehandle; }


    Mesh* GetMesh(const char* name) { return m_pMeshes[name]; }
    ShaderProgram* GetShader(const char* name) { return m_pShaders[name]; }
    GLuint GetTexture(const char* name) { return m_pTextures[name]; }

};

#endif //__ResourceManager_H__
