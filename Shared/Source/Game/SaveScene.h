#ifndef __SaveScene_H__
#define __SaveScene_H__

class SaveScene
{
private:
	Scene* sceneInstance;

public:
	SaveScene(Scene * currentScene);
	~SaveScene();
	void LoadState();
	void SaveState();
};

#endif //__SaveScene_H__
