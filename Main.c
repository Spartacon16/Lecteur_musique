//#define DLL_EXPORTS
#include "fonction_musique.h"
const int WIDTH = 800, HEIGHT = 600;

int main(int argc, char** argv)
{
    FMOD_SYSTEM* system;
    FMOD_SOUND* sound = NULL;
    FMOD_RESULT result;
    SDL_Window* window=NULL;
    SDL_Init(SDL_INIT_AUDIO);

    // Initialisation du systeme FMOD
    result = FMOD_System_Create(&system, FMOD_VERSION);
    if (result != FMOD_OK) {
        printf("Erreur lors de l'initialisation de FMOD : %s\n", FMOD_ErrorString(result));
        return EXIT_FAILURE;
    }

    result = FMOD_System_Init(system, 32, FMOD_INIT_NORMAL, NULL);
    if (result != FMOD_OK) {
        printf("Erreur lors de l'initialisation du systeme audio : %s\n", FMOD_ErrorString(result));
        FMOD_System_Release(system);
        return EXIT_FAILURE;
    }

    window = SDL_CreateWindow("Selection de musique", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Erreur lors de la creation de la fenetre : %s\n", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }


    int choix;
    do {
        afficherMenuPrincipal();
        printf("Choix: ");
        // Verification de la valeur de retour de scanf
        if (scanf_s("%d", &choix) != 1) {
            printf("Erreur de saisie. Veuillez entrer un nombre valide.\n");
            // Nettoyer le tampon d'entree
            while (getchar() != '\n');
            choix = -1; // Affecter une valeur non valide pour refaire l'entree
            continue; // Passer a la prochaine iteration de la boucle
        }

        switch (choix) {
        case 1:
            // Mode Utilisateur
            modeUtilisateur(system, sound, result, window);
            break;
        case 2:
            // Mode Administrateur
            modeAdministrateur(system, sound, result);
            break;
        case 3:
            printf("Au revoir.\n");
            break;
        default:
            printf("Choix invalide. Veuillez reessayer.\n");
            break;
        }
    } while (choix != 3);

    SDL_Quit();

    // Nettoyage
    FMOD_Sound_Release(sound);
    FMOD_System_Close(system);
    FMOD_System_Release(system);

    return EXIT_SUCCESS;
}