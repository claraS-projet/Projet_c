//Clara Schobert

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

//penser a faire des close pour fermer les tubes anonymes

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

    creationComoc();
    
    
// - création d'un sémaphore pour que deux clients ne
//   ne communiquent pas en même temps avec l'orchestre
    
    creationSemoc();
    
//Initaialisation du semaphore
    
    initSemoc();
    
// lancement des services, avec pour chaque service :
   
// - création d'un tube anonyme pour converser (orchestre vers service)
// - un sémaphore pour que le service préviene l'orchestre de la
//   fin d'un traitement
   initTubeComos();
    //int retFork, retFork2, retFork3;
   // retFork = fork();
	//assert (retFork != -1);
    
	//if(retFork == 0) {
    		initTubeService(0);
    		int semos0 = creationSemos(0);
   	// }else {
    		initTubeComos();
    		//retFork2 = fork();
    		//assert (retFork2 != -1);
    	
    		//if(retFork2 == 0) {
    			initTubeService(1);
    			int semos1 = creationSemos(1);
   		// }else {
    			initTubeComos();
    			//retFork3 = fork();
    			//assert (retFork3 != -1);
    			
    			//if(retFork3 == 0) {
    			initTubeService(2);
    			int semos2 = creationSemos(3);
    		//	}
    		//}
    	//}
    	
    
// - création de deux tubes nommés (pour chaque service) pour les
//   communications entre les clients et les services
    creationComcs(0);
    creationComcs(1);
    creationComcs(2);
  // if (retFork != 0 && retFork2 != 0 && retFork3 != 0){
    while (! fin)
    {
// ouverture ici des tubes nommés avec un client
 // attente d'une demande de service du client
        int numService; //Numéro du service demandé par le client
        openComOrchestre();
        
        numService = readNum();
        
// - un sémaphore pour que le service préviene l'orchestre de la
//   fin d'un traitement
 
        
// détecter la fin des traitements lancés précédemment via
// les sémaphores dédiés (attention on n'attend pas la
// fin des traitement, on note juste ceux qui sont finis)

// analyse de la demande du client
// si ordre de fin
//     envoi au client d'un code d'acceptation (via le tube nommé)
//     marquer le booléen de fin de la boucle
        if(numService == -1){
        	codeAcceptationoc();
        	fin = true;
        }
// sinon si service non ouvert
//     envoi au client d'un code d'erreur (via le tube nommé)
        else if(config_isServiceOpen(numService) != false){
        	codeErreuroc();
        }
// sinon si service déjà en cours de traitement
// envoi au client d'un code d'erreur (via le tube nommé)
// codeErreuroc();
// sinon
        else {
       		codeAcceptationoc();
//     envoi au client d'un code d'acceptation (via le tube nommé)
//     génération d'un mot de passe 
// 1 + numService
        	int mdpService = numService + 1;
     
//     envoi d'un code de travail au service (via le tube anonyme)
//     envoi du mot de passe au service (via le tube anonyme)
//     envoi du mot de passe au client (via le tube nommé)
        	SentMdp(mdpService);
//     envoi des noms des tubes nommés au client (via le tube nommé)
        	sentTube(numService);	
// finsi
        }

// attente d'un accusé de réception du client
        int numfin;
        numfin = finTransactionOrchestre();
        if(numfin == -1){
        		
        		printf("Fin de la communication avec un client\n");
        		// fermer les tubes vers le client
        		closeComOrchestre();
        	}
        	

        // il peut y avoir un problème si l'orchestre revient en haut de la
        // boucle avant que le client ait eu le temps de fermer les tubes
        // il faut attendre avec un sémaphore.
        // (en attendant on fait une attente d'1 seconde, à supprimer dès
        // que le sémaphore est en place)
        // attendre avec un sémaphore que le client ait fermé les tubes
        sleep(3);
    }
 	// } Fin du if (retFork != 0 && retFork2 != 0 && retFork3 != 0){


// attente de la fin des traitements en cours (via les sémaphores)

// envoi à chaque service d'un code de fin

// attente de la terminaison des processus services

// libération des ressources
    destructionComoc();
    destructionSemoc();
    destructionSemos(semos0);
    destructionSemos(semos1);
    destructionSemos(semos2);
    destructionComcs(0);
    destructionComcs(1);
    destructionComcs(2);
    
    return EXIT_SUCCESS;
}
