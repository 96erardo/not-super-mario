![This repo's execution gameplay](/assets/game.gif "Gameplay")
Simple Super Mario Bros platform builder.

## Configuration
The game configuration is made through text files. If you want to modify the existing level you have to go to the assets/levels/Level_1.txt file and modify its contents.

The Level files consist of commands that tell the engine where and which entity to put on the screen, there are three types of commands:

- **Tile**: Which puts a collisionable object in the screen.
- **Dec**: A Decoration, which draws an image that the player can go through.
- **Player**: Which is the player instance (The should only be one of these).

### Tile a x y
| Param | Param Name | Description |
|-------|------------|-------------|
|a|Animation|Animation to use for the tile|
|x|X| Grid x position to put the tile in|
|y|Y| Grid Y position to put the tile in|

### Dec a x y
| Param | Param Name | Description |
|-------|------------|-------------|
|a|Animation|Animation to use for the decoration|
|x|X| Grid x position to put the decoration in|
|y|Y| Grid Y position to put the decoration in|

### Player x y cx cy speed jump maxspeed gravity weapon
| Param | Param Name | Description |
|-------|------------|-------------|
|x|Animation|Animation to use for the decoration|
|y|X| Grid x position to put the decoration in|
|cx|Collision X|X Size of the collision box|
|cy|Collision Y|Y Size of the collision box|
|speed|Speed|Speed in the x axis|
|jump|Jump Speed|Speed in the y Axis when jumping|
|maxspeed|Maximum Speed|Max speed reachable in the Y axis|
|gravity|Gravity|Gravity acceleration|
|weapon|Weapon|Animation to use for the player Weapon|
