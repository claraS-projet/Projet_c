/*
 * Indications (à respecter) :
 * - Les erreurs sont gérées avec des assert ; les erreurs traitées sont :
 *    . appel trop tôt ou trop tard d'une méthode (cf. config.h)
 *    . fichier de configuration inaccessible
 *    . une position erronée
 * - Le fichier (si on arrive à l'ouvrir) est considéré comme bien
 *   formé sans qu'il soit nécessaire de le vérifier
 *
 * Un code minimal est fourni et permet d'utiliser le module "config" dès
 * le début du projet ; il faudra le remplacer par l'utilisation du fichier
 * de configuration.
 * Il est inutile de faire plus que ce qui est demandé
 *
 * Dans cette partie vous avez le droit d'utiliser les entrées-sorties
 * de haut niveau (fopen, fgets, ...)
 */


// TODO include des .h système

#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "myassert.h"

#include "config.h"

#define TAILLE 150

// TODO Définition des données ici
typedef struct {
	int service_id;
	bool is_open;
} Service;

typedef struct {
	int service_nb;
	char *exe_name;
	Service *services;
	bool initialized;
} Config;

static Config config = {0};


void config_init(const char *filename)
{
    // TODO erreur si la fonction est appelée deux fois
	assert(!config.initialized);
    // TODO code vide par défaut, à remplacer
    //      il faut lire le fichier et stocker toutes les informations en
    //      mémoire
    //Ouverture
    FILE *fd = fopen(filename, "r");
    assert(fd != NULL);
    
    //lecture du nombre de services
    fscanf(fd, "%d", &config.service_nb); //On lit un chiffre et on l'affecte a config.service_nb
    
    config.services = malloc(config.service_nb * sizeof(Service)); //Alloue de l'espace pour le tableau de services
    assert(config.services != NULL);
    
    //lecture du chemin de l'exe
    char chemin[TAILLE]; //Quelle taille choisir ???
    fscanf(fd, "%s", chemin);
    config.exe_name = strdup(chemin);
    
    //Lecture des services et de leur status
    int nb = config.service_nb;
    for (int i = 0; i < nb; i++){
    	int service_id;
    	char status[8];
    	fscanf(fd, "%d %s", &service_id, status);
    	
    	config.services[i].service_id = service_id;
    	if (strcmp(status, "ouvert")){
    		config.services[i].is_open = true;
    	}
    	else config.services[i].is_open = false;
    }
    
    fclose(fd);
    config.initialized = true;

void config_exit()
{
    // TODO erreur si la fonction est appelée avant config_init
    assert(config.initialized);

    // TODO code vide par défaut, à remplacer
    //      libération des ressources
    free(config.exe_name);
    free(config.services);
    
    config.initialized = false;
}

int config_getNbServices()
{
    // erreur si la fonction est appelée avant config_init
    
    // erreur si la fonction est appelée après config_exit
    assert(config.initialized);
    
    // code par défaut, à remplacer
    return config.service_nb;
}

const char * config_getExeName()
{
    // TODO erreur si la fonction est appelée avant config_init
    // TODO erreur si la fonction est appelée après config_exit

    assert(config.initialized);

    // TODO code par défaut, à remplacer
    return config.exe_name;
}

bool config_isServiceOpen(int pos)
{
    // TODO erreur si la fonction est appelée avant config_init
    // TODO erreur si la fonction est appelée après config_exit

    assert(config.initialized);
    // TODO erreur si "pos" est incorrect
	assert(pos >= 0 && pos < config.service_nb);
    // TODO code par défaut, à remplacer
    
    return config.services[pos].is_open;
}
