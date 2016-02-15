#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL.h>

#define WIDTH  800
#define HEIGHT 600
#define AMOUNT 18
#define PI 3.1415926

typedef struct {
    int   dist;
    int   size;
    int   intensity;
    float angle;
} Particle;

typedef struct {
    int cX;
    int cY;
    int r;
    int amount;
    SDL_Texture* circle;
    Particle particle[20];
} Firework;

Firework fire[AMOUNT];

SDL_Renderer* rend;
SDL_Texture* black;
SDL_Texture* circle[6];
SDL_Rect rectCircle;

void initFirework(Firework* firework) {
    firework -> cX = rand() % WIDTH;
    firework -> cY = rand() % HEIGHT;
    firework -> r  = rand() % 80 + 50;
    firework -> amount = rand() % 11 + 10;
    firework -> circle = circle[rand() % 6];

    int i;
    for (i=0; i<firework->amount; i++) {
        firework->particle[i].dist = 0;
        firework->particle[i].size = 7;
        firework->particle[i].angle = i * 2 * PI / firework->amount;
        firework->particle[i].intensity = 255;
    }
}

void moveParticle(Particle* particle, int r) {
    particle->dist++;
    particle->intensity = 255 - (particle->dist * 255 / r);
    if (particle->intensity <= 0) {
        particle->intensity = 0;
    }
}

void updateFirework(Firework* firework) {
    int i;
    for (i=0; i<firework->amount; i++) {
        moveParticle(&firework->particle[i], firework->r);
    }
    if (firework->particle[0].intensity == 0) {
        initFirework(firework);
    }
}

void sketchFirework(Firework* firework) {
    int i;
    for (i=0; i<firework->amount; i++) {
        Particle* particle = &(firework->particle[i]);
        int d = particle->dist;
        rectCircle.x = (int)(d * cos(particle->angle) + firework->cX);
        rectCircle.y = (int)(d * sin(particle->angle) + firework->cY);
        rectCircle.h = particle->size;
        rectCircle.w = particle->size;

        SDL_SetTextureAlphaMod(firework->circle, particle->intensity);
        SDL_RenderCopy(rend, firework->circle, NULL, &rectCircle);
    }
}

int main(int argc, char** argv)
{
    srand(time(NULL));
    SDL_Window* window = SDL_CreateWindow("Hello",
                            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                            WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

    rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Surface* blackSurface  = SDL_LoadBMP("black.bmp");
    SDL_Surface* circleSurface1 = SDL_LoadBMP("circle1.bmp");
    SDL_Surface* circleSurface2 = SDL_LoadBMP("circle2.bmp");
    SDL_Surface* circleSurface3 = SDL_LoadBMP("circle3.bmp");
    SDL_Surface* circleSurface4 = SDL_LoadBMP("circle4.bmp");
    SDL_Surface* circleSurface5 = SDL_LoadBMP("circle5.bmp");
    SDL_Surface* circleSurface6 = SDL_LoadBMP("circle6.bmp");

    SDL_SetColorKey(circleSurface1, SDL_TRUE, SDL_MapRGB(circleSurface1->format, 195, 195, 195));
    SDL_SetColorKey(circleSurface2, SDL_TRUE, SDL_MapRGB(circleSurface2->format, 195, 195, 195));
    SDL_SetColorKey(circleSurface3, SDL_TRUE, SDL_MapRGB(circleSurface3->format, 195, 195, 195));
    SDL_SetColorKey(circleSurface4, SDL_TRUE, SDL_MapRGB(circleSurface4->format, 195, 195, 195));
    SDL_SetColorKey(circleSurface5, SDL_TRUE, SDL_MapRGB(circleSurface5->format, 195, 195, 195));
    SDL_SetColorKey(circleSurface6, SDL_TRUE, SDL_MapRGB(circleSurface6->format, 195, 195, 195));

    black = SDL_CreateTextureFromSurface(rend, blackSurface);
    circle[0] = SDL_CreateTextureFromSurface(rend, circleSurface6);
    circle[1] = SDL_CreateTextureFromSurface(rend, circleSurface1);
    circle[2] = SDL_CreateTextureFromSurface(rend, circleSurface2);
    circle[3] = SDL_CreateTextureFromSurface(rend, circleSurface3);
    circle[4] = SDL_CreateTextureFromSurface(rend, circleSurface4);
    circle[5] = SDL_CreateTextureFromSurface(rend, circleSurface5);

    int i;
    SDL_SetTextureBlendMode(black, SDL_BLENDMODE_BLEND);
    for (i=0; i<6; i++) {
        SDL_SetTextureBlendMode(circle[i], SDL_BLENDMODE_BLEND);
    }

    SDL_SetTextureAlphaMod(black, 50);

    SDL_Rect rectWindow;
    rectWindow.x = 0;
    rectWindow.y = 0;
    rectWindow.h = HEIGHT;
    rectWindow.w = WIDTH;

    for (i=0; i<AMOUNT; i++) {
        initFirework(&fire[i]);
    }

    bool quit = false;
    SDL_Event event;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }


        SDL_RenderCopy(rend, black, NULL, &rectWindow);
        for (i=0; i<AMOUNT; i++) {
            sketchFirework(&fire[i]);
        }
        SDL_RenderPresent(rend);

        for (i=0; i<AMOUNT; i++) {
            updateFirework(&fire[i]);
        }
        SDL_Delay(40);
    }
    SDL_DestroyWindow(window);
    return 0;
}
