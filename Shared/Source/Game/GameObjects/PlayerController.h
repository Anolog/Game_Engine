#ifndef __PlayerController_H__
#define __PlayerController_H__

enum PlayerControllerButtons
{
    PCB_Up    = 0x00000001,
    PCB_Down  = 0x00000002,
    PCB_Left  = 0x00000004,
    PCB_Right = 0x00000008,
    PCB_Space  = 0x00000010,
};

class PlayerController
{
protected:
    unsigned int m_ButtonFlags;

public:
    PlayerController();
    virtual ~PlayerController();

    void Clear();

    void SetButtonPressed(PlayerControllerButtons button);
    void SetButtonReleased(PlayerControllerButtons button);

    bool IsButtonPressed(PlayerControllerButtons button);
};

#endif //__PlayerController_H__