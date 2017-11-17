#ifndef __GameHeaders_H__
#define __GameHeaders_H__

#include "../../Libraries/lodepng/lodepng.h"
#include "../../Libraries/Box2D/Box2D/Box2D/Box2D.h"
#include "../../Libraries/cJSON/cJSON.h"
#include "../Source/Core/Utility/TweenFuncs.h"
#include <SDL.h>
#include <SDL_mixer.h>


#include "Core/CoreHeaders.h"

#if ANDROID
#define sprintf_s(a,b,c,d) sprintf(a,c,d)
#endif //ANDROID

#include "Game/Events/Event.h"
#include "Game/Events/InputEvent.h"
#include "Game/Events/RemoveFromSceneEvent.h"

#include "Game/Mesh/VertexFormat.h"
#include "Game/Mesh/Mesh.h"
#include "Game/Mesh/OBJLoader.h"
#include "Game/FBODefinition.h"

#include "Game/Box2D/Box2DContactListener.h"
#include "Game/Box2D/Box2DDebugDraw.h"
#include "Game/Box2D/Box2DWorld.h"

#include "Game/CategoryFilters.h"

#include "Game/GameObjects/PlayerController.h"
#include "Game/GameObjects/GameObject.h"
#include "Game/GameObjects/CameraObject.h"
#include "Game/GameObjects/PlayerObject.h"
#include "Game/GameObjects/TurretEnemy.h"
#include "Game/GameObjects/JumpingEnemy.h"
#include "Game/GameObjects/PlayerBullet.h"
#include "Game/GameObjects/EnemyBullet.h"
#include "Game/GameObjects/Collectable.h"

#include "Game/Base/ResourceManager.h"
#include "Game/Base/Scene.h"

#include "Game/SoundManager.h"
#include "Game/SceneManager.h"

#include "Game/SimplePool.h"
#include "Game/MainMenuScene.h"
#include "Game/TitleScreen.h"
#include "Game/GameOverScene.h"
#include "Game/PauseScene.h"
#include "Game/BaseScene.h"
#include "Game/FlashmanScene.h"
#include "Game/SaveScene.h"


#include "Game/GameCore.h"

#endif //__GameHeaders_H__
