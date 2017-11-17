#include "pch.h"
#include "SceneManager.h"

SceneManager* SceneManager::instance = NULL;

SceneManager::SceneManager()
{
}

//Use this to delete the instance of it
void SceneManager::ResetInstance()
{
    delete instance;
    instance = NULL;
}

SceneManager::~SceneManager()
{
    /*
    if (instance != NULL)
    {
        delete instance;
    }
    */
}

SceneManager * SceneManager::getInstance()
{
	if (instance == NULL)
	{
		instance = new SceneManager();
	}
	return instance;
}

Scene * SceneManager::GetCurrentScene()
{
	return m_SceneStack.back();
}

void SceneManager::AddScene(Scene* scene)
{
	m_SceneStack.push_back(scene);
}

void SceneManager::PopScene()
{
		m_SceneStack.pop_back();
}

void SceneManager::PopToBeginning()
{
	//Scene* temp = *m_SceneStack.begin();

	//m_SceneStack.empty();
	//m_SceneStack.push_back(temp);

	//m_SceneStack;

	for (int i = m_SceneStack.size(); i == 1; i--)
	{
		m_SceneStack.pop_back();
	}

}

