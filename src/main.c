#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#define WINDOW_WIDTH 1080
#define WINDOW_HEIGHT 720







// erreur lors de l'importation de la font !!!

// ligne 56







// rappel de compilation

// Windows :   gcc src/main.c -o bin/main -I include -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf

void SDL_ExitWithError(const char* message);

int main(int argc, char* argv[]){

	SDL_Window* firstWindow = NULL;
	SDL_Renderer* renderer = NULL;

	SDL_Surface* surface = NULL;
	SDL_Texture* texture = NULL;
	SDL_Rect rect = { 0, 0, 0, 0 }; 	// position.x , position.y , largeur , hauteur

	TTF_Font* font = NULL;
	SDL_Color color = { 255, 255, 255 };


	//Lancement SDL
	if(SDL_Init(SDL_INIT_VIDEO) != 0){ 	// renvoie 0 si tout va bien
		SDL_ExitWithError("initializing SDL");
	}
	SDL_Log("SDL successfully initialized\n");

	if(TTF_Init() != 0){
		SDL_ExitWithError("initializing TTF");
	}
	SDL_Log("TTF successfully initialized\n");

	//Création fenêtre + rendu
	if(SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &firstWindow, &renderer) != 0){
		SDL_ExitWithError("Creating window & renderer failed");
	}

	font = TTF_OpenFont("../font/arial.ttf", 25);

	if(font == NULL){
		TTF_CloseFont(font);
		SDL_ExitWithError("Add font failed");
	}

	/*-------------------------------------------------------*/
	
	surface = TTF_RenderText_Solid(font, "test test", color);
	if(surface == NULL){
		TTF_CloseFont(font);
		SDL_FreeSurface(surface);
		SDL_ExitWithError("Add surface failed");
	}

	// Une fois la texture crée, plus besoin de la font
	TTF_CloseFont(font);

	texture = SDL_CreateTextureFromSurface(renderer, surface);

	// Une fois la texture crée, plus besoin de la surface
	SDL_FreeSurface(surface);

	if(texture == NULL){
		SDL_ExitWithError("Add texture failed");
	}

	if(SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h)){
		SDL_ExitWithError("texture loading failed");
	}

	rect.x = (WINDOW_WIDTH - rect.w) / 2;

	if(SDL_RenderCopy(renderer, texture, NULL, &rect) != 0){
		SDL_ExitWithError("print message failed");
	}

	/*-------------------------------------------------------*/

	SDL_RenderPresent(renderer);
	SDL_Delay(5000);

	
	SDL_bool run = SDL_TRUE;	// Struct booleenne True/False

	//Boucle infini
	while(run){
		SDL_Event event;

		while(SDL_PollEvent(&event)){
			switch(event.type){

				case SDL_KEYDOWN:
					switch(event.key.keysym.sym){

						default:
							continue;
					}

				//termine le programme la si fenêtre est fermé
				case SDL_QUIT:
					run = SDL_FALSE;
					break;

				default:
					break;
			}
		}
	}

	/*-------------------------------------------------------*/
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
	TTF_Quit();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(firstWindow);
	SDL_Quit();
	return EXIT_SUCCESS;
}

void SDL_ExitWithError(const char* message){
	SDL_Log("Error %s > %s\n", message, SDL_GetError());
	TTF_Quit();
	SDL_Quit();
	exit(EXIT_FAILURE);
}