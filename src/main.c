#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string.h>
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




// Fonctions 
void SDL_ExitWithError(const char* message, ...);

void SDL_WriteCharInFile(SDL_RWops* io,const char* character);

void SDL_SelectCharAction(SDL_RWops* io,const char* action);

void append(char *s, char c);

void pop(char *s);
void drawTextEntry(SDL_Renderer* renderer, TTF_Font* font, const char* text, SDL_Rect* rect, SDL_Color textColor) {
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, rect);

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect textRect = { rect->x + 5, rect->y + 5, textSurface->w, textSurface->h };
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}


int main(int argc, char* argv[]){

	char* list = NULL;
	if ((list = (char*)calloc(1000, sizeof(char))) == NULL) {
		exit(EXIT_FAILURE);
	}

	SDL_Window* firstWindow = NULL;
	SDL_Renderer* renderer = NULL;
    SDL_Rect entryRect = { 170, 10, 900, 650 }; // Rectangle à droite du menu principal
	SDL_Surface* texte_surf = NULL;
	SDL_Texture* texture = NULL;
	SDL_Rect rect = { 0, 0, 0, 0 }; 	// position.x , position.y , largeur , hauteur

	TTF_Font* font = NULL;
	SDL_Color color_txt = { 25, 25, 25 };
	SDL_Color color_bg = { 225, 225, 225 };

	char* pathFile = NULL;
	if ((pathFile = (char*)calloc(105, sizeof(char))) == NULL) {
        exit(EXIT_FAILURE);
    }

	char* pathFont = "..\\fonts\\arial.ttf";

	char* folder = "..\\files\\";
	char file[50];
	printf("entrez le nom du fichier à éditer\n");
	printf("exemple : -> test.txt\n");
	printf("-> ");
	fgets(file, 50, stdin);
	file[strcspn(file, "\n")] = '\0'; 

	strcat(pathFile, folder);
	strcat(pathFile, file);
	// printf("%s\n", pathFile);

	

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

	font = TTF_OpenFont(pathFont, 20);

	if(font == NULL){
		free(pathFile);
		TTF_CloseFont(font);
		SDL_ExitWithError("Failed to load font");
	}

	SDL_RWops *io = SDL_RWFromFile(pathFile, "w+");
	if (io == NULL) {
		SDL_ExitWithError("File not found", io);
	}

	int selectedOption = -1;
    SDL_bool menuOpen = SDL_TRUE; // Afficher le menu principal au démarrage
    SDL_bool subMenuOpen = SDL_FALSE; // Ne pas afficher le sous-menu au démarrage
	SDL_bool run = SDL_TRUE;

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
					// SDL_Log("%d -> %c.", event.key.keysym.sym, event.key.keysym.sym);
					if(valideChar){
						//Liste
						char voide[] = " ";
						if (pressedChar == SDLK_BACKSPACE || pressedChar == SDLK_DELETE){
							(strlen(list)>1)?pop(list):strcpy(list,voide);
						}else{
							append(list, pressedChar);
							}
						if(SDL_SetRenderDrawColor(renderer, 225, 225, 225, SDL_ALPHA_OPAQUE) != 0){
							SDL_ExitWithError("Change color failed");
						}

						if(SDL_RenderFillRect(renderer, &background) != 0){
							SDL_ExitWithError("Drawing background failed");
						}
						SDL_RenderPresent(renderer);
						// affichage du caractère
						SDL_Surface * gc = TTF_RenderText_Blended(font, list, color_txt);
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
						rect.w = gc->w;
						rect.h = gc->h;
						SDL_RenderCopy(renderer, texture, NULL, &rect);
						SDL_RenderPresent(renderer);
						
						SDL_Delay(10);



						SDL_WriteCharInFile(io, &pressedChar);
						SDL_Log("File updated");
					}

					break;

				case SDL_MOUSEBUTTONDOWN:
					if (event.button.button == SDL_BUTTON_LEFT){
						//----------------------------- Rajouts -----------------------------------------------
						SDL_Rect rect = {10, 10, 150, 30};    // Menu principal
						SDL_Rect subRect = {10, 40, 150, 120}; // Sous-menu (options)

						// Vérifier si le clic est dans le menu principal
						const SDL_Point pt = {event.motion.x, event.motion.y};
						if (SDL_PointInRect(&pt, &rect)){
							// Afficher ou masquer le sous-menu lors du clic sur le menu principal
							subMenuOpen = !subMenuOpen;
						}
						else if (subMenuOpen && SDL_PointInRect(&pt, &subRect)){
							// Gérer les clics dans le sous-menu
							int optionClicked = (event.motion.y - 40) / 30 + 1; // Calculer l'option cliquée

							switch (optionClicked){
								case 1:
									SDL_WriteCharInFile(io, "Nouveau selected!\n");
									break;
								case 2:
									SDL_WriteCharInFile(io, "Ouvrir selected!\n");
									break;
								case 3:
									SDL_WriteCharInFile(io, "Sauvegarder selected!\n");
									break;
								case 4:
									SDL_WriteCharInFile(io, "Quitter selected!\n");
									run = SDL_FALSE;
									break;
								default:
									break;
							}
						}
					}
					break;

            	case SDL_MOUSEMOTION: 

                    if (menuOpen){
						SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
						SDL_RenderFillRect(renderer, &(SDL_Rect){10, 10, 150, 30});

						SDL_Color textColor2 = {0, 0, 0};
						SDL_Surface *textSurface2 = TTF_RenderText_Solid(font, "Menu", textColor2);
						SDL_Texture *textTexture2 = SDL_CreateTextureFromSurface(renderer, textSurface2);
						SDL_Rect textRect3 = {15, 10, textSurface2->w, textSurface2->h};
						SDL_RenderCopy(renderer, textTexture2, NULL, &textRect3);

						if (subMenuOpen){
							SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
							SDL_RenderFillRect(renderer, &(SDL_Rect){10, 40, 150, 120});

							// Dessiner les options du sous-menu ici
							// Vous pouvez ajuster les positions et les textes en conséquence
							// Exemple basique avec "Option 1", "Option 2", etc.
							SDL_Color textColor = {0, 0, 0};
							for (int i = 1; i <= 4; ++i){
								char optionText[20];
								snprintf(optionText, sizeof(optionText), "Option %d", i);

								SDL_Surface *textSurface = TTF_RenderText_Solid(font, optionText, textColor);
								SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
								SDL_Rect textRect = {15, 40 + (i - 1) * 30, textSurface->w, textSurface->h};
								SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

								SDL_FreeSurface(textSurface);
								SDL_DestroyTexture(textTexture);
							}
                            drawTextEntry(renderer, font, "Votre texte ici", &entryRect, textColor);
                            
						}

						SDL_FreeSurface(textSurface2);
						SDL_DestroyTexture(textTexture2);
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
//pas certin du bail
/*---------------------------------------------------------------------------------*/

		// Effacer le fond
        SDL_SetRenderDrawColor(renderer, 225, 225, 225, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(renderer, &background);
        
        if (menuOpen){
                // Dessiner le menu principal
                    

                    SDL_Color textColor = {0, 0, 0};
                    SDL_Surface *textSurface = TTF_RenderText_Solid(font, "Menu", textColor);
                    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                    SDL_Rect textRect = {15, 10, textSurface->w, textSurface->h};
                    
                    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
                    SDL_RenderFillRect(renderer, &(SDL_Rect){10, 10, 100, textSurface->h + 5});
                    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
            if ( subMenuOpen){
                char** tab[4];
                char* new ="Nouveau";
                char * open = "Ouvrir";
                char * save = "Sauvegarder";
                char * quit ="Quitter";

                tab[0]=&new;
                tab[1]=&open;
                tab[2]=&save;
                tab[3]=&quit;

                int position = textSurface->h + textRect.y;

                for (int i = 0 ; i < 4 ; i++){
                    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
                    SDL_Rect RCT = {10, position + 5, 100, textSurface->h + 5};
                    SDL_RenderFillRect(renderer, &RCT);
                    textSurface = TTF_RenderText_Solid(font, *tab[i], textColor);
                    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                    position += textSurface->h + 5;
                    
                    SDL_RenderCopy(renderer, textTexture, NULL, &RCT);
                    SDL_FreeSurface(textSurface);
                    SDL_DestroyTexture(textTexture);
                }
                drawTextEntry(renderer, font, "Votre texte ici", &entryRect, textColor);
                        
            }

                    
                    


                
        }
        

        
    
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
	}

	/*-------------------------------------------------------*/
	free(pathFile);
	SDL_RWclose(io);
	TTF_CloseFont(font);
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

void append(char *s, char c)
{
    int len = strlen(s);
    s[len] = c;
    s[len + 1] = '\0';
}

void pop(char *s)
{
    int len = strlen(s);
    if (len > 0)
    {
        s[len - 1] = '\0';
    }
    else
    {
        printf("Cannot suppress : empty !");
    }
}