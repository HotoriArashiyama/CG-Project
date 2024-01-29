This is the ZRT's part of the final project.
API used: freeglut+glew, assimp
How to control:
  Move by keyboard: wasd for wandering, and space/z for up down.
  Eyesight follows Mouse.
  Mouse wheel for generally zooming the objects in the scene (skybox not included however). 
  Left click to choose an object in the scene, and left click again to cancel.
    When you choose an object, the keyboard control will be binded to it:
    w/a/s/d for wanderinging, space/z for up and down, x/c for scaling, shift+z/x/c for rotating.
  shift+p to make a screenshot, a .png will be saved in SolutionDir/ScreenShots.

Implemented in this branch:
  Camera.h: control the eyesight and keyboard event related to it.
  Control.h: processing keyboard and mouse events.
  Shader.h: a tool for generating shaders.
  Layout.h: including skybox, terrain, providing interface for Control to operate the object by mouse click, providing interface for .obj loading(not impletemented completely, since there's still bugs). 
  LoadTexture.h: tools for loading textures from files.
  ScreenShot.h: a tool for screen shot.
