#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdarg.h>
#define WINDOW_WIDTH 1080
#define WINDOW_HEIGHT 720


//erreur lors de l'écriture : après le premier charactère le programme se stop
// line 91 & 138




// rappel de compilation

// Windows :   gcc src/main.c -o bin/main -I include -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf

void SDL_ExitWithError(const char* message, ...);

int main(int argc, char* argv[]){

	SDL_Window* firstWindow = NULL;
	SDL_Renderer* renderer = NULL;

	SDL_Surface* texte_surf = NULL;
	SDL_Texture* texture = NULL;
	SDL_Rect rect = { 0, 0, 0, 0 }; 	// position.x , position.y , largeur , hauteur

	TTF_Font* font = NULL;
	SDL_Color color_txt = { 25, 25, 25 };
	SDL_Color color_bg = { 225, 225, 225 };



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

	SDL_Rect background;
	background.x = 0;
	background.y = 0;
	background.w = WINDOW_WIDTH;
	background.h = WINDOW_HEIGHT;

	if(SDL_SetRenderDrawColor(renderer, 225, 225, 225, SDL_ALPHA_OPAQUE) != 0){
		SDL_ExitWithError("Change color failed");
	}

	if(SDL_RenderFillRect(renderer, &background) != 0){
		SDL_ExitWithError("Drawing background failed");
	}

	SDL_RenderPresent(renderer);

	font = TTF_OpenFont("fonts/arial.ttf", 20);

	if(font == NULL){
		TTF_CloseFont(font);
		SDL_ExitWithError("Add font failed");
	}

	SDL_RWops *io = SDL_RWFromFile("files/test.txt", "w+");
	if (io == NULL) {
		SDL_ExitWithError("File not found", io);
	}

	
	SDL_bool run = SDL_TRUE;	// Struct booleenne True/False

	//Boucle infini
	while(run){
		SDL_Event event;

		while(SDL_PollEvent(&event)){
			switch(event.type){

				case SDL_KEYDOWN:
					writeCharInFile(io, SDL_GetKeyName(event.key.keysym.sym));
					SDL_Log("File updated");

				//termine le programme la si fenêtre est fermé
				case SDL_QUIT:
					SDL_ClearError();
					SDL_Log("SDL successfully exited");
					run = SDL_FALSE;
					break;

				default:
					break;
			}
		}
	}

	/*-------------------------------------------------------*/
	SDL_RWclose(io);
	TTF_CloseFont(font);
	SDL_DestroyTexture(texture);
	TTF_Quit();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(firstWindow);
	SDL_Quit();
	return EXIT_SUCCESS;
}

void SDL_ExitWithError(const char* message, ...){

	SDL_Log("Error %s > %s\n", message, SDL_GetError());
	
	va_list ap;
	va_start(ap, message);
	
	SDL_RWops* io = va_arg(ap, SDL_RWops*);

	if(io != NULL){
		SDL_RWclose(io);
	}
	va_end(ap);

	TTF_Quit();
	SDL_Quit();
	exit(EXIT_FAILURE);
}

void writeCharInFile(SDL_RWops* io,const char* character){
	if(SDL_RWwrite(io, character, sizeof(*character), 1) != 1){
		SDL_ExitWithError("char not writed in file");
	}
}