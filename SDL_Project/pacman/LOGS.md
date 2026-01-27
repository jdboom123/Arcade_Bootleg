# 1-27-2026: Pac-Man Project
Starting to work on a Pac- Man bootleg. Some things that I have noticed:
- Putting **SDL_MAIN_USE_CALLBACKS** as 1 requires four main functions: **SDL_AppInit**, **SDL_AppEvent**, **SDL_AppIterate**, and **SDL_AppQuit**. If these functions are not found, the program will not compile. 
- SDL_FRect will help me draw rectangles, which will be the placeholders for the sprites in Pac-Man

Today's goal will be to draw a yellow square that can move around and consume small pellets