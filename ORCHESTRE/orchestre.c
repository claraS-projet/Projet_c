#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <fcntl.h>




#include "config.h"
#include "client_orchestre.h"
#include "orchestre_service.h"
#include "service.h"


static void usage(const char *exeName, const char *message)
{
    fprintf(stderr, "usage : %s <fichier config>\n", exeName);
    if (message != NULL)
        fprintf(stderr, "message : %s\n", message);
    exit(EXIT_FAILURE);
}

int main(int argc, char * argv[])
{
    if (argc != 2)
        usage(argv[0], "nombre paramètres incorrect");
    
    bool fin = false;

    // lecture du fichier de configuration
    config_init(argv[1]);

    // Pour la communication avec les clients
    // - création de 2 tubes nommés pour converser avec les clients
    	// Création du tube pour le sens orchestre vers client
    int o2c = mkfifo("pipe_o2c", 0644); 
    assert(o2c == 0);
    
    o2c = unlink("pipe_o2c");
    assert(o2c == 0);
    	// Création du tube pour le sens client vers orchestre
    int c2o = mkfifo("pipe_c2o", 0644); 
    assert(c2o == 0);
    
    c2o = unlink("pipe_c2o");
    assert(c2o == 0);
    
    
    
    // - création d'un sémaphore pour que deux clients ne
    //   ne communiquent pas en même temps avec l'orchestre
    
    int semoc = semget(5, 1, 0644);
    assert( semoc != -1);
    
    //Initaialisation du semaphore
    
    int initsem = semctl(semoc, 0, SETVAL, 1);
    assert(initsem != -1);
    
    // lancement des services, avec pour chaque service :
    // - création d'un tube anonyme pour converser (orchestre vers service)
    // - un sémaphore pour que le service préviene l'orchestre de la
    //   fin d'un traitement
    // - création de deux tubes nommés (pour chaque service) pour les
    //   communications entre les clients et les services

    while (! fin)
    {
        // ouverture ici des tubes nommés avec un client
        // attente d'une demande de service du client
        
        int c2olect = open("pipe_c2o", O_RDONLY);
        assert( c2olect != -1);
        printf("L'orchestre a ouvert le tube c2o en lecture !\n");
        
        int o2cwrite = open("pipe_o2c", O_WRONLY);
        assert (o2c != -1);
        printf("L'orchestre a ouvert le tube o2c en ecriture !\n");

        // détecter la fin des traitements lancés précédemment via
        // les sémaphores dédiés (attention on n'attend pas la
        // fin des traitement, on note juste ceux qui sont finis)

        // analyse de la demande du client
        // si ordre de fin
        //     envoi au client d'un code d'acceptation (via le tube nommé)
        //     marquer le booléen de fin de la boucle
        // sinon si service non ouvert
        //     envoi au client d'un code d'erreur (via le tube nommé)
        // sinon si service déjà en cours de traitement
        //     envoi au client d'un code d'erreur (via le tube nommé)
        // sinon
        //     envoi au client d'un code d'acceptation (via le tube nommé)
        //     génération d'un mot de passe
        //     envoi d'un code de travail au service (via le tube anonyme)
        //     envoi du mot de passe au service (via le tube anonyme)
        //     envoi du mot de passe au client (via le tube nommé)
        //     envoi des noms des tubes nommés au client (via le tube nommé)
        // finsi

        // attente d'un accusé de réception du client
        // fermer les tubes vers le client

        // il peut y avoir un problème si l'orchestre revient en haut de la
        // boucle avant que le client ait eu le temps de fermer les tubes
        // il faut attendre avec un sémaphore.
        // (en attendant on fait une attente d'1 seconde, à supprimer dès
        // que le sémaphore est en place)
        // attendre avec un sémaphore que le client ait fermé les tubes
        sleep(1);   // à supprimer
    }

    // attente de la fin des traitements en cours (via les sémaphores)

    // envoi à chaque service d'un code de fin

    // attente de la terminaison des processus services

    // libération des ressources
    
    return EXIT_SUCCESS;
}
