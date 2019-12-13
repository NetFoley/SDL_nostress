// Example program:
// Using SDL_Point in some places of your code

#include "SDL.h"
#include "SDL_image.h"
#include <stdio.h>
#include <math.h>

#define PI 3.14159265
#define MAXBUCHES 161
#define VIEMAX 100

struct buche{
    SDL_Rect rect;
    int nAngle;
    int nVie;
}buche;

int main(int argc, char *argv[]) {

    SDL_Window * window;
    SDL_Renderer * renderer;
    SDL_Event event;

    SDL_Texture * axeTexture;
    SDL_Surface * axeSurface;

    SDL_Texture * blockTexture;
    SDL_Surface * blockSurface;

    SDL_Point mouse_position;             //    Mouse position coords

    SDL_Init(SDL_INIT_VIDEO);             //    Initialize SDL2


    if (SDL_CreateWindowAndRenderer(700, 600, SDL_WINDOW_BORDERLESS, &window, &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 3;
    }
    //Creation de la texture
    axeSurface = IMG_Load("axe.png");
    if (!axeSurface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s", SDL_GetError());
    }

    axeTexture = SDL_CreateTextureFromSurface(renderer, axeSurface);
    if (!axeTexture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
    }

    blockSurface = IMG_Load("whiteBlock.png");
    if (!blockSurface) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create surface from image: %s", SDL_GetError());
    }

    blockTexture = SDL_CreateTextureFromSurface(renderer, blockSurface);
    if (!blockTexture) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture from surface: %s", SDL_GetError());
    }

    SDL_Rect axePos;
    SDL_Point axeMid;

    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    axePos.x = w/2.5;
    axePos.y = h/8;
    axePos.w = axeSurface->w/2;
    axePos.h = axeSurface->h/2;

    axeMid.x = 10;
    axeMid.y = axePos.h - 20;

    SDL_Rect buche;
    buche.x = axePos.x + axePos.h - 103;
    buche.y = axePos.y + axePos.h + 36;
    buche.w = 120;
    buche.h = buche.w * 1.618033;

    struct buche buches[MAXBUCHES];
    int nbrBuche = 2;
    int decalage = 7;
    for(int i = 0; i < MAXBUCHES; i++)
    {
        buches[i].rect.w = 35;
        buches[i].rect.h = buches[i].rect.w *2 * 1.618033;
        if(i % 2 == 0)
            buches[i].rect.x = buche.x + buche.w/2 - decalage;
        else
            buches[i].rect.x = buche.x + buche.w/2 - 1.85*buches[i].rect.w/2;
        buches[i].rect.y = buche.y - buches[i].rect.h;
        buches[i].nAngle = 0;
        buches[i].nVie = VIEMAX;

    }

    SDL_Point contactAxe;
    contactAxe.x = 0;
    contactAxe.y = 0;

    SDL_Point bucheMid;
    bucheMid.x = buches[0].rect.w/2;
    bucheMid.y = buches[0].rect.h;

    // Check that the window was successfully made
    if (window == NULL) {
        SDL_Log("Could not create window: %s", SDL_GetError());
        return 1;
    }
    int compteur = 0;
    double alpha = 1;
    double beta = 0;

    double ecart = 0;
    float vitesse = 0;

    mouse_position.x = 0;
    mouse_position.y = 0;
    int lastHitTime = 0;

    while (1) {
            compteur++;
            SDL_GetGlobalMouseState(                    //    Sets mouse_position to...
                &mouse_position.x,                // ...mouse arrow coords on window
                &mouse_position.y
            );
            int x,y;
            SDL_GetWindowPosition(window, &x,&y);
            mouse_position.x -= x;
            mouse_position.y -= y;
        while(SDL_PollEvent(&event))
        {

           // if(SDL_GetMouseState(NULL,NULL))
            }

            float hypo = sqrt((mouse_position.x - (axeMid.x + axePos.x)) * (mouse_position.x - (axeMid.x + axePos.x)) + (mouse_position.y - (axeMid.y + axePos.y))* ( mouse_position.y - (axeMid.y + axePos.y) ));
            float adj = sqrt(((axeMid.y + axePos.y)- mouse_position.y) * ((axeMid.y+ axePos.y) - mouse_position.y));
            float opp = sqrt(( axeMid.x + axePos.x-mouse_position.x ) * (axeMid.x+ axePos.x- mouse_position.x ));

            //CALCUL DE L'ANGLE ENTRE LA HACHE ET LA SOURIS
            alpha = acos(adj / hypo) * 180 / PI;

            if(mouse_position.x >= axeMid.x + axePos.x && mouse_position.y >= axeMid.y + axePos.y)
                alpha = 180 - alpha;

            else if(mouse_position.x <= axeMid.x + axePos.x && mouse_position.y >= axeMid.y + axePos.y)
                alpha = 180 + alpha;

            else if(mouse_position.x <= axeMid.x + axePos.x && mouse_position.y <= axeMid.y + axePos.y)
                alpha = 360 - alpha;




        contactAxe.x = axePos.x + axeMid.x + cos((beta-75)/180*PI)*(axePos.h-50);
        contactAxe.y = axePos.y + axeMid.y + sin((beta-75)/180*PI)*(axePos.h-50);

        /*****************************************************
        GESTION COLLISION HACHE ET BUCHE
        *****************************************************/
        if(SDL_PointInRect(&contactAxe, &buches[nbrBuche-2].rect))
        {
            if(vitesse > 0.2)
            {
                buches[nbrBuche-1].nVie -= vitesse*30;
                buches[nbrBuche-2].nVie -= vitesse*30;
                buches[nbrBuche-1].nAngle = -(3 * (VIEMAX -buches[nbrBuche-1].nVie)/VIEMAX);
                buches[nbrBuche-2].nAngle =  3* (VIEMAX -buches[nbrBuche-1].nVie)/VIEMAX;
                buches[nbrBuche-2].rect.x = buche.x + buche.w/2 - decalage + 10*(VIEMAX -buches[nbrBuche-1].nVie)/VIEMAX;
                lastHitTime = SDL_GetTicks();
            }
            if(buches[nbrBuche-2].nVie > 0)
                vitesse = 0;
        }
            if(buches[nbrBuche-2].nVie <= 0 && SDL_GetTicks() - lastHitTime > 1200)
            {
                nbrBuche += 2;
            }

        ecart = beta - alpha;

        if(fabs(beta - (360 + alpha)) < fabs(ecart))
            ecart = beta - (360 + alpha);
        if(fabs(beta + 360 - alpha) < fabs(ecart))
            ecart = beta + 360 - alpha;

        SDL_bool lancer = 0;
        if(vitesse > 0.1 && ecart > 0)
            lancer = 1;
        if(ecart > 0 && lancer != 1)
            vitesse = fabs(ecart) / 1000;
        else if(ecart <= 0 && !SDL_PointInRect(&contactAxe, &buches[nbrBuche-2].rect))
            vitesse += fabs(ecart) / 200000;

        if(ecart < 0 || lancer)
        {
            beta+= vitesse;
        }
        else
        {
            beta-=vitesse;
        }
        if(beta > 90 && beta < 210)
        {
            beta = 90;
            vitesse = 0;
            lancer = 0;
        }
        if(beta < 230 && beta > 210)
            beta = 230;

        if(beta > 360)
            beta -= 360;

        if(beta < 0)
            beta += 360;



        //layer_getInput(&Layer1, event);
        if (event.type == SDL_QUIT) {
            break;
        }

        else if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
            break;
        }
        //RENDER ZONE
        SDL_SetRenderDrawColor(renderer, 0x35, 0xA5, 0xDF, 0x00);
        SDL_RenderClear(renderer);


        SDL_RenderCopyEx(renderer, axeTexture, NULL, &axePos, beta, &axeMid, SDL_FLIP_NONE);

        for(int i = 0; i < nbrBuche; i++)
        {
            if(buches[i].nVie <= 0)
                {
                    if(compteur%2 == 0)
                    {
                        buches[i].nAngle++;
                        buches[i].rect.y++;
                        if(i % 2 == 0)
                            buches[i].rect.x--;
                        else
                            buches[i].rect.x++;


                    }

                SDL_RenderCopyEx(renderer, blockTexture, NULL, &buches[i].rect, buches[i].nAngle,  NULL, SDL_FLIP_NONE);
                }
            else
                SDL_RenderCopyEx(renderer, blockTexture, NULL, &buches[i].rect, buches[i].nAngle, &bucheMid, SDL_FLIP_NONE);
        }

        SDL_RenderCopyEx(renderer, blockTexture, NULL, &buche, 0, NULL, SDL_FLIP_NONE);


        //printf("%i %i\n", Layer1.Totue.frame, Layer1.Totue.attacking);
        SDL_RenderPresent(renderer);
    }

    // Close and destroy the window
    SDL_DestroyWindow(window);

    // Clean up
    SDL_Quit();
    return 0;
}
