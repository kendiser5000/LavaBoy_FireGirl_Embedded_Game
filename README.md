# lab10-sikender-and-amr1
lab10-sikender-and-amr1 created by GitHub Classroom


Inspired by popular flash game [Fireboy and Lavagirl](https://www.coolmathgames.com/0-fireboy-watergirl-forest-temple)

Created game engine and level generator for ability to create multiple game modes
Implemented Game Modes:
- Original Lava Boy and Firegirl Puzzle Mode (Up to 2 players)
- One Shot One Kill (2 Player Battle)
- Jet Pack Runner Game
- Competitive Race/Runner (2 Player)

Structure Examples for Level Creation:
-rect: {x, y, w, h, colour}
-LevelRects: {rects[10], size}
-Sprites: {x, y, *bitmap, width, height, cType}
-Bullet: {*image, velocity, direction, reset position, end position, 0}
-mPlatform: {*image, 0, axis, direction, min position, max position, 0, 0, 0}
-Button {*image, *platform it triggers, 0}
