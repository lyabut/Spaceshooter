# Spaceshooter
Spaceshooter is game created for CSCI 3317 51 Computer Game Programming Class. Controls to move are the arrow keys and spacebar to shoot. Main objective is to destroy invaders past the asteroids. ALL asteroids must be destroyed in order to spawn the invaders. Different sized asteroids require more bullets to destroy. Invaders get larger, faster, and stronger every time one of them dies.

## Prerequisites

- [Visual Studios](https://visualstudio.microsoft.com/downloads/)
- Minimum of [DirectX 9](https://www.microsoft.com/en-us/download/details.aspx?id=34429) 
### IN VISUAL STUDIOS
Project properties must be set properly in order to run the game. The settings are set to my previous pathts. Adjust to match yours. Right click the project and select properties. 

**First:** in VC++ Directories set the Include Directories the path to the Include folder in which you installed D3DX (Ex: C:\D3DX\Include;). 

**Second:** in VC++ Directories set the Library Directories the path to the x86 folder (Ex:C:\D3DX\Lib\x86;).

**Third:** in Input under Linker drop-down, the following should be in Additional Dependencies: **d3d9.lib; d3dx9.lib; dxerr.lib; legacy_stdio_definitions.lib;**

## Running the Game

After all properties are set simply click **Local Windows Debugger** within Visual Studios
## Acknowledgments
- Author Jonathan S. Harbour of *Beginning Game Programming Fourth Edition* for parts of the code used within the project.
- [DarkPhotton](https://alex1197.itch.io/16x16-tiny-atlas-1) for the Ship sprite
- [Ravenmore](https://ravenmore.itch.io/pixel-space-shooter-assets) for Bullets and Asteroid sprites

