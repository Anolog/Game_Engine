#ifndef __SceneManager_H__
#define __SceneManager_H__

//Have a map of all the scenes loaded into it.
//Use the map of scenes to push that one into the stack/vector of the scenes that are in the gamne currently/being used
//Might need to reset the scenes so they can be set back easily

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	//Holds all the scenes
	std::map<std::string, Scene*> m_SceneList;

	static SceneManager* getInstance();
	Scene* GetCurrentScene();
	void AddScene(Scene* scene);
	void PopScene();

	void PopToBeginning();

    static void ResetInstance();

private:
	static SceneManager* instance;
	std::vector<Scene *> m_SceneStack;

};

#endif //__SceneManager_H__
