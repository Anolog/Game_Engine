#include "pch.h"
#include "SaveScene.h"


SaveScene::SaveScene(Scene* currentScene)
{
	sceneInstance = currentScene;
}

SaveScene::~SaveScene()
{

}
void SaveString(char* filename, char* string)
{
	FILE* filehandle;
	errno_t error = fopen_s(&filehandle, filename, "wb");
	if (filehandle)
	{
		fwrite(string, 1, strlen(string), filehandle);
		fclose(filehandle);
	}
}

char* LoadString(char* filename)
{
	FILE* filehandle;
	errno_t error = fopen_s(&filehandle, filename, "rb");
	if (filehandle)
	{
		// find the length of the file
		fseek(filehandle, 0, SEEK_END); // go to the end
		long size = ftell(filehandle); // read the position in bytes
		rewind(filehandle); // go back to the beginning
							// before we can read, we need to allocate a buffer of the right size
		char* buffer = new char[size];
		fread(buffer, size, 1, filehandle);
		fclose(filehandle);

		return buffer;

	}
	return "NO FILE";
}

void SaveScene::LoadState()
{

	char* str = LoadString("Save.txt");

	if (str != "NO FILE")
	{

		cJSON* jRoot = cJSON_Parse(str);

		cJSON* jPlayer = cJSON_GetObjectItem(jRoot, "Player");
		
		cJSON* jScore = cJSON_GetObjectItem(jPlayer, "Score:");
		cJSON* jPosX = cJSON_GetObjectItem(jPlayer, "Pos X:");
		cJSON* jPosY = cJSON_GetObjectItem(jPlayer, "Pos Y:");
		cJSON* jPosZ = cJSON_GetObjectItem(jPlayer, "Pos Z:");

		//Reset the Scene
		sceneInstance->Reset();
		
		//Set the Player's Loaded Pos,Score
		sceneInstance->GetGameObject("Player")->SetPosition(vec3(jPosX->valuedouble, jPosY->valuedouble, jPosZ->valuedouble));
	
		if (jScore->valueint>0)
		sceneInstance->SetScore(jScore->valueint);

		cJSON_Delete(jRoot);

		delete[] str;
	}
}

void SaveScene::SaveState()
{
	cJSON *jRoot, *jPlayer;
	jRoot = cJSON_CreateObject();

	vec3 Pos = sceneInstance->GetGameObject("Player")->GetPosition();
	int Score = sceneInstance->GetScore();
	if (Score < 0)
		Score = 0;

	cJSON_AddItemToObject(jRoot, "Player", jPlayer = cJSON_CreateObject());
	
	cJSON_AddNumberToObject(jPlayer, "Score:", Score);
	cJSON_AddNumberToObject(jPlayer, "Pos X:", Pos.x);
	cJSON_AddNumberToObject(jPlayer, "Pos Y:", Pos.y);
	cJSON_AddNumberToObject(jPlayer, "Pos Z:", Pos.z);

	char * jSonstr = cJSON_Print(jRoot);
	SaveString("Save.txt", jSonstr);

	cJSON_Delete(jRoot);
	free(jSonstr);
}

