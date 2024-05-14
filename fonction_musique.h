#ifndef FONCTION_MUSIQUE_H
#define FONCTION_MUSIQUE_H

#include <SDL_ttf.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <fmod.h>
#include <fmod_errors.h>
#include <SDL.h>
#define MAX_MUSIC_COUNT 100
#define MAX_MUSIC_NAME_LENGTH 256

typedef struct {
    char names[MAX_MUSIC_COUNT][MAX_MUSIC_NAME_LENGTH];
    int count;
} MusicList;
typedef struct {
    char paths[MAX_MUSIC_COUNT][MAX_MUSIC_NAME_LENGTH];
} PathList;

// Prototypes des fonctions pour la gestion de la musique
int  lireListeMusiques(const char* filename, MusicList* musicList);
int lireChemins(const char* filename, PathList* pathList);
int lireCorrespondanceMusique(FMOD_SYSTEM* system, FMOD_SOUND* sound, FMOD_RESULT result, MusicList* musicList, PathList* pathList);
int modeUtilisateur(FMOD_SYSTEM* system, FMOD_SOUND* sound, FMOD_RESULT result, SDL_Window* window);
int modeAdministrateur(FMOD_SYSTEM* system, FMOD_SOUND* sound, FMOD_RESULT result);
int afficherMenuPrincipal();
int songwindow(SDL_Window* window);
void selectionWindow(SDL_Renderer* renderer, char musiques[][256], int count);

#endif /* FONCTION_MUSIQUE_H */
