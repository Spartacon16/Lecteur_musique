#include "fonction_musique.h"

// Fonction pour lire le fichier list_music.txt et stocker les noms des musiques
int  lireListeMusiques(const char* filename, MusicList* musicList) {
    // Implementez la logique de lecture du fichier list_music.txt ici
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return EXIT_FAILURE;
    }

    musicList->count = 0;

    while (fgets(musicList->names[musicList->count], MAX_MUSIC_NAME_LENGTH, file) != NULL) {
        // Supprimer le saut de ligne a la fin de chaque nom de musique
        size_t len = strlen(musicList->names[musicList->count]);
        if (len > 0 && musicList->names[musicList->count][len - 1] == '\n') {
            musicList->names[musicList->count][len - 1] = '\0';
        }

        musicList->count++;

        // Verifier si nous avons atteint la limite du nombre de musiques
        if (musicList->count >= MAX_MUSIC_COUNT) {
            printf("Attention : Nombre maximum de musiques atteint.\n");
            break;
        }
    }

    fclose(file);

    return EXIT_SUCCESS;
}
int lireChemins(const char* filename, PathList* pathList) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier des chemins");
        return EXIT_FAILURE;
    }

    int index = 0;
    while (fgets(pathList->paths[index], MAX_MUSIC_NAME_LENGTH, file) != NULL) {
        // Supprimer le saut de ligne a la fin du chemin
        size_t len = strlen(pathList->paths[index]);
        if (len > 0 && pathList->paths[index][len - 1] == '\n') {
            pathList->paths[index][len - 1] = '\0';
        }
        index++;
        // Verifier si nous avons atteint la limite du nombre de chemins
        if (index >= MAX_MUSIC_COUNT) {
            printf("Attention : Nombre maximum de chemins atteint.\n");
            break;
        }
    }

    fclose(file);

    return EXIT_SUCCESS;
}
// Fonction pour lire le fichier de correspondance numero de musique - adresse du fichier .mp3
int lireCorrespondanceMusique(FMOD_SYSTEM* system, FMOD_SOUND* sound, FMOD_RESULT result, MusicList* musicList, PathList* pathList) {
    int selection;
    printf("Entrez le numero de la musique que vous souhaitez ecouter : ");
    if (scanf_s("%d", &selection) != 1 || selection < 1 || selection > musicList->count) {
        printf("Numero de musique invalide.\n");
        return EXIT_FAILURE;
    }
    // Utiliser le chemin de la liste des chemins pour construire le chemin complet
    char fullPath[MAX_MUSIC_NAME_LENGTH];
    snprintf(fullPath, sizeof(fullPath), "D:\\simon\\Music\\MUSIQUE\\%s", pathList->paths[selection - 1]);
    
    result = FMOD_System_CreateSound(system, fullPath, FMOD_DEFAULT, 0, &sound);
    if (result != FMOD_OK) {
        printf("Erreur lors de la creation du son : %s\n", FMOD_ErrorString(result));
        return EXIT_FAILURE;
    }

    result = FMOD_System_PlaySound(system,  sound, 0,0, NULL);
    if (result != FMOD_OK) {
        printf("Erreur lors de la lecture du son : %s\n", FMOD_ErrorString(result));
        return EXIT_FAILURE;
    }

    // Attendre la fin de la lecture
    FMOD_BOOL isPlaying;
    do {
        FMOD_Channel_IsPlaying(NULL, &isPlaying);
        SDL_Delay(50); // Pause pour eviter de surcharger le CPU
    } while (isPlaying);

    FMOD_Sound_Release(sound);
    sound = NULL;

    return EXIT_SUCCESS;
}


// Fonction pour gerer le mode utilisateur
int modeUtilisateur(FMOD_SYSTEM* system, FMOD_SOUND* sound, FMOD_RESULT result, SDL_Window* window) {
    printf("Mode Utilisateur selectionne.\n");


    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur lors de l'initialisation de SDL : %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // Creation du renderer a partir de la fenetre existante
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Erreur lors de la creation du renderer : %s\n", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }


    MusicList musicList;
    if (lireListeMusiques("liste_music.txt", &musicList) == EXIT_SUCCESS) {
        printf("Liste des musiques :\n");
        for (int i = 0; i < musicList.count; i++) {
            printf("%s\n", musicList.names[i]);
        }
    }

    PathList pathList;
    if (lireChemins("paths.txt", &pathList) == EXIT_SUCCESS) {
        int selection = -1;
        do {
            selectionWindow(renderer, musicList.names, musicList.count); // Appel a la fonction pour afficher la fenetre de selection
            printf("Entrez le numero de la musique que vous souhaitez ecouter (-1 pour quitter) : ");
            scanf_s("%d", &selection);

            if (selection >= 0 && selection < musicList.count) {
                lireCorrespondanceMusique(system, sound, result, &musicList, &pathList);
            }
            else if (selection != -1) {
                printf("Numero de musique invalide.\n");
            }

        } while (selection != -1);
    }

    // Liberation des ressources
    SDL_DestroyRenderer(renderer);

    return EXIT_SUCCESS;
}

// Fonction pour gerer le mode administrateur
int modeAdministrateur(FMOD_SYSTEM* system, FMOD_SOUND* sound, FMOD_RESULT result) {
    printf("Mode Administrateur selectionne.\n");

    

    return EXIT_SUCCESS;
}

// Fonction pour afficher le menu principal
int afficherMenuPrincipal() {
    printf("Menu Principal:\n");
    printf("1. Mode Utilisateur\n");
    printf("2. Mode Administrateur\n");
    printf("3. Quitter\n");

    return EXIT_SUCCESS;
}
int songwindow(SDL_Window* Window) {
    // Check that the window was successfully created
    if (NULL == Window)
    {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Delay(2000); // Attendre 2 secondes
    SDL_DestroyWindow(Window);

    return EXIT_SUCCESS;
}
void selectionWindow(SDL_Renderer* renderer, char musiques[][256], int count) {
    int selection = -1;

    while (selection != -1) {
        // Effacer le renderer
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Afficher la liste des musiques
        SDL_Color textColor = { 0, 0, 0, 255 };
        SDL_Rect rect = { 10, 10, 600, 20 };
        for (int i = 0; i < count; i++) {
            SDL_Surface* surface = SDL_CreateRGBSurface(0, 600, 20, 32, 0, 0, 0, 0);
            if (!surface) {
                printf("Erreur lors de la creation de la surface : %s\n", SDL_GetError());
                return;
            }

            SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 255, 255));
            SDL_Surface* textSurface = SDL_CreateRGBSurface(0, 600, 20, 32, 0, 0, 0, 0);
            if (!textSurface) {
                printf("Erreur lors de la creation de la surface de texte : %s\n", SDL_GetError());
                SDL_FreeSurface(surface);
                return;
            }

            SDL_BlitSurface(textSurface, NULL, surface, NULL);
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            if (!texture) {
                printf("Erreur lors de la creation de la texture : %s\n", SDL_GetError());
                SDL_FreeSurface(surface);
                SDL_FreeSurface(textSurface);
                return;
            }

            SDL_FreeSurface(surface);
            SDL_FreeSurface(textSurface);

            // ecrire le texte sur la surface de texte
            char text[256];
            snprintf(text, sizeof(text), "%d - %s", i + 1, musiques[i]);
            SDL_Surface* numberSurface = TTF_RenderText_Blended(NULL, text, textColor);
            if (!numberSurface) {
                printf("Erreur lors de la creation de la surface de texte : %s\n", SDL_GetError());
                SDL_DestroyTexture(texture);
                return;
            }

            SDL_Rect textRect = { rect.x, rect.y, numberSurface->w, numberSurface->h };
            SDL_BlitSurface(numberSurface, NULL, surface, &textRect);
            SDL_FreeSurface(numberSurface);

            SDL_RenderCopy(renderer, texture, NULL, &rect);
            SDL_DestroyTexture(texture);

            rect.y += 30; // Decalage pour la prochaine ligne
        }

        // Actualiser le renderer
        SDL_RenderPresent(renderer);

        // Saisie de l'utilisateur
        printf("Entrez le numero de la musique que vous souhaitez ecouter (-1 pour quitter) : ");
        scanf_s("%d", &selection);

        if (selection >= 1 && selection <= count) {
            printf("Vous avez choisi : %s\n", musiques[selection - 1]);
            // Ici vous pouvez appeler votre fonction pour jouer la musique
        }
    }
}

