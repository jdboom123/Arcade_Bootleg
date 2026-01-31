#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define PACMAN_BLOCK_SIZE_IN_PIXELS 16
#define PACMAN_SPEED 4

#define PACMAN_GAME_WIDTH 28
#define PACMAN_GAME_HEIGHT 36

#define SDL_WINDOW_WIDTH           (PACMAN_BLOCK_SIZE_IN_PIXELS * PACMAN_GAME_WIDTH)
#define SDL_WINDOW_HEIGHT          (PACMAN_BLOCK_SIZE_IN_PIXELS * PACMAN_GAME_HEIGHT)

// static SDL_Window *window = NULL;
// static SDL_Renderer *renderer = NULL;

typedef struct
{
    int cells [PACMAN_GAME_WIDTH*PACMAN_GAME_HEIGHT];
    int pacman_xpos;
    int pacman_ypos;
    int pacman_xpos_prev;
    int pacman_ypos_prev;
}PacManContext;

typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    PacManContext pacman_ctx;
    Uint64 last_step;
} AppState;

void pacman_initialize(PacManContext *ctx)
{
    SDL_zeroa(ctx->cells);
    /* Change to appropiate locations later*/
    ctx->pacman_xpos = 0;
    ctx->pacman_ypos = 0;
    ctx->pacman_xpos_prev = 0;
    ctx->pacman_ypos_prev = 0;
}

// void snake_redir(SnakeContext *ctx, SnakeDirection dir)
// {
//     SnakeCell ct = snake_cell_at(ctx, ctx->head_xpos, ctx->head_ypos);
//     if ((dir == SNAKE_DIR_RIGHT && ct != SNAKE_CELL_SLEFT) ||
//         (dir == SNAKE_DIR_UP && ct != SNAKE_CELL_SDOWN) ||
//         (dir == SNAKE_DIR_LEFT && ct != SNAKE_CELL_SRIGHT) ||
//         (dir == SNAKE_DIR_DOWN && ct != SNAKE_CELL_SUP)) {
//         ctx->next_dir = dir;
//     }
// }

static SDL_AppResult handle_key_event_(PacManContext *ctx, SDL_Scancode key_code)
{
    switch (key_code) {
    /* Quit. */
    case SDL_SCANCODE_ESCAPE:
    case SDL_SCANCODE_Q:
        return SDL_APP_SUCCESS;
    /* Restart the game as if the program was launched. */
    case SDL_SCANCODE_R:
        pacman_initialize(ctx);
        break;
    /* Decide new direction of the snake. */
    case SDL_SCANCODE_RIGHT:
        ctx->pacman_xpos += PACMAN_SPEED;
        if (ctx->pacman_xpos >= SDL_WINDOW_WIDTH){
            ctx->pacman_xpos = -1*PACMAN_BLOCK_SIZE_IN_PIXELS + 1;
        }
        printf("%d\n",ctx->pacman_xpos);
        break;
    case SDL_SCANCODE_UP:
        ctx->pacman_ypos -= PACMAN_SPEED;
        if (ctx->pacman_ypos <= -1*PACMAN_BLOCK_SIZE_IN_PIXELS){
            ctx->pacman_ypos = SDL_WINDOW_HEIGHT-1;
        }
        break;
    case SDL_SCANCODE_LEFT:
        ctx->pacman_xpos -= PACMAN_SPEED;
        if (ctx->pacman_xpos <= -1*PACMAN_BLOCK_SIZE_IN_PIXELS){
            ctx->pacman_xpos = SDL_WINDOW_WIDTH-1;
        }
        break;
    case SDL_SCANCODE_DOWN:
        ctx->pacman_ypos += PACMAN_SPEED;
        if (ctx->pacman_ypos >= SDL_WINDOW_HEIGHT){
            ctx->pacman_ypos = -1*PACMAN_BLOCK_SIZE_IN_PIXELS + 1;
        }
        break;
    default:
        break;
    }
    return SDL_APP_CONTINUE;
}

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{   
    AppState *as = (AppState *)SDL_calloc(1, sizeof(AppState));
    if (!as) {
        return SDL_APP_FAILURE;
    }

    *appstate = as;

    /* Create the window */
    if (!SDL_CreateWindowAndRenderer("BOOTLEG PAC-MAN", SDL_WINDOW_WIDTH, SDL_WINDOW_HEIGHT, 0, &as->window, &as->renderer)) {
        SDL_Log("Couldn't create window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(as->renderer, SDL_WINDOW_WIDTH, SDL_WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    pacman_initialize(&as->pacman_ctx);

    return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    PacManContext *ctx = &((AppState *)appstate)-> pacman_ctx;
    switch(event->type){
        case SDL_EVENT_QUIT:
            return SDL_APP_SUCCESS;
        case SDL_EVENT_KEY_DOWN:
            return handle_key_event_(ctx, event->key.scancode);
        default:
            break;
    }
    return SDL_APP_CONTINUE;
}
/* Will be very useful for game controller logic */

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{   
    AppState *as = (AppState *)appstate;
    PacManContext *ctx = &as->pacman_ctx;
    SDL_FRect r;

    // Set up background
    SDL_SetRenderDrawColor(as->renderer, 0, 0, 0, 255);
    SDL_RenderClear(as->renderer);

    r.w = r.h = PACMAN_BLOCK_SIZE_IN_PIXELS;

    // Draw Pac-Man
    SDL_SetRenderDrawColor(as->renderer, 255, 255, 0, 255);
    r.x = ctx->pacman_xpos;
    r.y = ctx->pacman_ypos;
    SDL_RenderFillRect(as->renderer, &r);

    // if (ctx->pacman_xpos != ctx->pacman_xpos_prev || ctx->pacman_ypos != ctx->pacman_ypos_prev){
    //     printf("%f , %f\n", r.x, r.y);
    // }
    ctx->pacman_xpos_prev = ctx->pacman_xpos;
    ctx->pacman_ypos_prev = ctx->pacman_ypos;


    SDL_RenderPresent(as->renderer);
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    if (appstate != NULL) {
        AppState *as = (AppState *)appstate;
        SDL_DestroyRenderer(as->renderer);
        SDL_DestroyWindow(as->window);
        SDL_free(as);
    }
}