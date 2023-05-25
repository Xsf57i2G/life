/*
 * Conway's Game of Life
 */

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct game
{
    int w;
    int h;
    int **cell;
};

void init(struct game *g)
{
    g->cell = malloc(g->w * sizeof(int *));
    for (int i = 0; i < g->w; i++)
    {
        g->cell[i] = malloc(g->h * sizeof(int));
    }

    srand(time(NULL));
    for (int i = 0; i < g->w; i++)
    {
        for (int j = 0; j < g->h; j++)
        {
            g->cell[i][j] = rand() % 2;
        }
    }
}

void tick(struct game *g)
{
    int **next = malloc(g->w * sizeof(int *));
    for (int i = 0; i < g->w; i++)
    {
        next[i] = malloc(g->h * sizeof(int));
    }

    for (int i = 0; i < g->w; i++)
    {
        for (int j = 0; j < g->h; j++)
        {
            int alive = 0;
            for (int x = i - 1; x <= i + 1; x++)
            {
                for (int y = j - 1; y <= j + 1; y++)
                {
                    if (x == i && y == j)
                    {
                        continue;
                    }
                    if (x < 0 || x >= g->w || y < 0 || y >= g->h)
                    {
                        continue;
                    }
                    if (g->cell[x][y])
                    {
                        alive++;
                    }
                }
            }

            if (g->cell[i][j])
            {
                if (alive < 2 || alive > 3)
                {
                    next[i][j] = 0;
                }
                else
                {
                    next[i][j] = 1;
                }
            }
            else
            {
                if (alive == 3)
                {
                    next[i][j] = 1;
                }
                else
                {
                    next[i][j] = 0;
                }
            }
        }
    }

    for (int i = 0; i < g->w; i++)
    {
        for (int j = 0; j < g->h; j++)
        {
            g->cell[i][j] = next[i][j];
        }
    }

    for (int i = 0; i < g->w; i++)
    {
        free(next[i]);
    }
    free(next);
}

SDL_Window *create_window(int w, int h)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *win = SDL_CreateWindow("", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
    return win;
}

void draw(SDL_Renderer *renderer, struct game *g)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (int i = 0; i < g->w; i++)
    {
        for (int j = 0; j < g->h; j++)
        {
            if (g->cell[i][j])
            {
                int size = 4;
                SDL_Rect rect = {i * size, j * size, size, size};
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

int main(void)
{
    struct game g;
    g.w = 640;
    g.h = 480;

    init(&g);

    SDL_Window *win = create_window(g.w, g.h);
    SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    while (!SDL_QuitRequested())
    {
        draw(renderer, &g);
        tick(&g);
        SDL_Delay(.1);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();

    exit(EXIT_SUCCESS);
}