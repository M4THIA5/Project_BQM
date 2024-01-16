#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#define WINDOW_WIDTH 1080
#define WINDOW_HEIGHT 720
const SDL_Keycode ALPHABET [] = {SDLK_a, SDLK_b, SDLK_c, SDLK_d, SDLK_e, SDLK_f, SDLK_g, SDLK_h, SDLK_i,
						SDLK_j, SDLK_k, SDLK_l, SDLK_m, SDLK_n, SDLK_o, SDLK_p, SDLK_q, SDLK_r,
						SDLK_s, SDLK_t, SDLK_u, SDLK_v, SDLK_w, SDLK_x, SDLK_y, SDLK_z};
const SDL_KeyCode ZERO_TO_NINE [] = {SDLK_0, SDLK_1, SDLK_2, SDLK_3, SDLK_4, SDLK_5, SDLK_6, SDLK_7, SDLK_8, SDLK_9,};
							// SDLK_KP_0, SDLK_KP_1, SDLK_KP_2, SDLK_KP_3, SDLK_KP_4, SDLK_KP_5, SDLK_KP_6, SDLK_KP_7, SDLK_KP_8, SDLK_KP_9

const SDL_KeyCode SYMBOLS [] = {SDLK_EXCLAIM, SDLK_QUOTE, SDLK_QUOTEDBL, SDLK_HASH, SDLK_DOLLAR, SDLK_COLON,
						SDLK_SEMICOLON, SDLK_LESS, SDLK_EQUALS, SDLK_GREATER, SDLK_QUESTION, SDLK_AT, SDLK_LEFTBRACKET,
						SDLK_BACKSLASH, SDLK_RIGHTBRACKET, SDLK_CARET, SDLK_UNDERSCORE, SDLK_BACKQUOTE, SDLK_AMPERSAND, 
						SDLK_LEFTPAREN, SDLK_RIGHTPAREN, SDLK_ASTERISK, SDLK_PLUS, SDLK_COMMA, SDLK_MINUS, SDLK_PERIOD,
						SDLK_SLASH, SDLK_KP_PERIOD, SDLK_KP_DIVIDE, SDLK_KP_MINUS, SDLK_KP_MULTIPLY, SDLK_KP_PLUS, 
						SDLK_KP_EQUALS, SDLK_SPACE, SDLK_BACKSPACE, SDLK_TAB,};
const SDL_KeyCode ACTION [] = {SDLK_DELETE, SDLK_KP_ENTER, SDLK_UP, SDLK_DOWN, SDLK_RIGHT, SDLK_LEFT};




// Erreurs :

// Les symboles en dessous des chiffres ne n'affiches pas !! 
// line 139






// rappel de compilation

// Windows :   gcc src/main.c -o bin/main -I include -L lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
			// bin/main.exe

// Fonctions 
void SDL_ExitWithError(const char* message, ...);

void SDL_WriteCharInFile(SDL_RWops* io,const char* character);

void SDL_SelectCharAction(SDL_RWops* io,const char* action);


int main(int argc, char* argv[]){

	SDL_Window* firstWindow = NULL;
	SDL_Renderer* renderer = NULL;

	SDL_Surface* texte_surf = NULL;
	SDL_Texture* texture = NULL;
	SDL_Rect rect = { 0, 0, 0, 0 }; 	// position.x , position.y , largeur , hauteur

	TTF_Font* font = NULL;
	SDL_Color color_txt = { 0, 0, 0 };
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

	font = TTF_OpenFont("fonts\\arial.ttf", 20);

	if(font == NULL){
		TTF_CloseFont(font);
		SDL_ExitWithError("Add font failed");
	}

	SDL_RWops *io = SDL_RWFromFile("files\\test.txt", "w+");
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
					char pressedChar;
					SDL_bool valideChar = SDL_FALSE;
					for(int i = 0; i < sizeof(ALPHABET)/4 ; i++){	// = 26 soit le nb de lettres
						if(event.key.keysym.sym == ALPHABET[i]){
							pressedChar = event.key.keysym.mod & KMOD_SHIFT ? 
                                toupper(event.key.keysym.sym) : event.key.keysym.sym;
							valideChar = SDL_TRUE;
							break; // Sort de la boucle 
						}
					}

					if( !(event.key.keysym.mod & KMOD_SHIFT) & valideChar == SDL_FALSE){
						for(int i=0; i<sizeof(ZERO_TO_NINE)/4; i++){
							if(event.key.keysym.sym == ZERO_TO_NINE[i]){
								pressedChar = event.key.keysym.sym;
								valideChar = SDL_TRUE;
								break; // Sort de la boucle 
							}
						}
					}
						
					if( valideChar == SDL_FALSE){
						for(int i=0; i<sizeof(SYMBOLS)/4; i++){
							if(event.key.keysym.sym == SYMBOLS[i]){
								pressedChar = event.key.keysym.sym;
								valideChar = SDL_TRUE;
								break; // Sort de la boucle 
							}
						}
					}
					

					for(int i=0; i<sizeof(ACTION)/4; i++){
						if(event.key.keysym.sym == ACTION[i]){
							pressedChar = event.key.keysym.sym;
							//pas de valideChar ici
							break; // Sort de la boucle
						}
					}
					SDL_Log("%d -> %c.", event.key.keysym.sym, event.key.keysym.sym);
					if(valideChar){
						// affichage du caractère
						SDL_Surface * gc = TTF_RenderText_Blended(font, &pressedChar, color_txt);
						if (gc == NULL) {
							SDL_FreeSurface(gc);
							TTF_CloseFont(font);
							SDL_ExitWithError("Creating surface failed");
						}
						texture = SDL_CreateTextureFromSurface(renderer, gc);
						if (texture == NULL) {
							SDL_FreeSurface(gc);
							TTF_CloseFont(font);
							SDL_ExitWithError("Creating texture failed");
						}
						//Ecrire les lettres cote à cote
						SDL_QueryTexture(texture, NULL, NULL, 0, 0);
						rect.x += rect.w;
						rect.w = gc->w;
						rect.h = gc->h;
						SDL_RenderCopy(renderer, texture, NULL, &rect);
						SDL_RenderPresent(renderer);
						
						SDL_Delay(10);



						SDL_WriteCharInFile(io, &pressedChar);
						SDL_Log("File updated");
					}

					break;

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
	SDL_RWclose(io);
	//TTF_CloseFont(font);
	SDL_DestroyTexture(texture);
	TTF_Quit();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(firstWindow);
	SDL_ClearError();
	SDL_Log("SDL successfully exited");
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

void SDL_WriteCharInFile(SDL_RWops* io,const char* character){
	size_t len = strlen(character);
    if (SDL_RWwrite(io, character, 1, len) != len) {
        SDL_ExitWithError("string not written in file", io);
    }
}

void SDL_SelectCharAction(SDL_RWops* io,const char* action){
	switch(*action){
		// case ACTION[0]:
			// fonction qui fait l'action voulu dans le fichier
		default:
			break;
	}
}