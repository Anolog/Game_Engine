This game engine has included a project that I created in it to use as an example to show how it works.

KNOWN BUGS:
- Scenemanager's stack will randomly reset, I don't know why. But because of this, you can't pop the stack, so you can't
go back to other scenes.

- Bullets (player and enemy), cause a crash on shutdown with the memory. It was found that it is added to the scene and
deleted twice, because of that it was causing the issue. But, after trying to fix somethings, it led to another error
on shutdown. It crashes with mymemory.

Controls:
WASD - Move
Space - Shoot
K - Pause
B - Advance through the title screen/exit pause menu
