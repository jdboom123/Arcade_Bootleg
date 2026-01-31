# 1-27-2026: Pac-Man Project
Starting to work on a Pac- Man bootleg. Some things that I have noticed:
- Putting **SDL_MAIN_USE_CALLBACKS** as 1 requires four main functions: **SDL_AppInit**, **SDL_AppEvent**, **SDL_AppIterate**, and **SDL_AppQuit**. If these functions are not found, the program will not compile. 
- SDL_FRect will help me draw rectangles, which will be the placeholders for the sprites in Pac-Man

Today's goal will be to draw a yellow square that can move around and consume small pellets

# 1-31-2026: Getting Pac-Man to Move Around
Trying to get Pac-Man to move across the screen by adjusting its x and y position using the arrow keys and then updating the renderer.

Issue: Pac-Man stops rendering when it hits 128 pixels in both the x and y position
- 7 bits of rendering(?)
- Issue with the char data type (Only 8 bits, and since it was signed, we only had 7 bits to describe the numerical part)
- Changed data type to int and changed logic in movement so that it wraps to the other side when going out of bounds

Issue: Going all the way right would cause the sprite to be stuck at -15 in both the x and y direction. 
- Fixed issue removing unsigned from the PACMAN_WINDOW_WIDTH and PACMAN_WINDOW_HEIGHT