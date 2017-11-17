#ifndef __RemoveFromSceneEvent_H__
#define __RemoveFromSceneEvent_H__

class GameObject;

class RemoveFromSceneEvent : public Event
{
protected:
    GameObject* m_pGameObject;

public:
    RemoveFromSceneEvent(GameObject* gameobject)
    {
        m_EventType = EventType_RemoveFromScene;
        m_pGameObject = gameobject;
    }

    GameObject* GetGameObject() { return m_pGameObject; }
};

#endif //__RemoveFromSceneEvent_H__