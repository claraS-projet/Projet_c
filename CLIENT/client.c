//Clara Schobert

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "io.h"
#include "memory.h"
#include "myassert.h"

#include "service.h"
#include "client_orchestre.h"
#include "client_service.h"

#include "client_arret.h"
#include "client_somme.h"
#include "client_sigma.h"
#include "client_compression.h"


static void usage(const char *exeName, const char *message)
{
    fprintf(stderr, "usage : %s <num_service> ...\n", exeName);
    fprintf(stderr, "        <num_service> : entre -1 et %d\n", SERVICE_NB - 1);
    fprintf(stderr, "                        -1 signifie l'arrêt de l'orchestre\n");
    fprintf(stderr, "        ...           : les paramètres propres au service\n");
    if (message != NULL)
        fprintf(stderr, "message : %s\n", message);
    exit(EXIT_FAILURE);
}

int main(int argc, char * argv[])
{
//Variables globales
	char *tubes2c;
	char *tubec2s;

    if (argc < 2)
        usage(argv[0], "nombre paramètres incorrect");

    int numService = io_strToInt(argv[1]);
    if (numService < -1 || numService >= SERVICE_NB)
        usage(argv[0], "numéro service incorrect");

    // appeler la fonction de vérification des arguments
    //     une fct par service selon numService
    //            . client_arret_verifArgs
    //         ou . client_somme_verifArgs
    //         ou . client_compression_verifArgs
    //         ou . client_sigma_verifArgs

// initialisations diverses s'il y a lieu
    int mysem = openSemClient();

// entrée en section critique pour communiquer avec l'orchestre
    myOpmoins(mysem);
    
// ouverture des tubes avec l'orchestre
    openComClient();

// envoi à l'orchestre du numéro du service
    
    sentNum(numService);
// attente code de retour
    int codeRetour;
    codeRetour = readCoderetour();
// si code d'erreur
//     afficher un message erreur
    if( codeRetour == -1 ){
    	printf("Acces au service demandé impossible\n");
    }
// sinon si demande d'arrêt (i.e. numService == -1)
//     afficher un message
    else if(numService == -1){
    	printf("Vous avez demandé à l'orchestre de s'arrèter\n");
    }else{
// sinon
//     récupération du mot de passe et des noms des 2 tubes
    	
    	int MdpService = readMdp();
 
    	tubes2c = readTubeName();
    	tubec2s = readTubeName();
    // finsi
    	
    }
    //
// envoi d'un accusé de réception à l'orchestre
// fermeture des tubes avec l'orchestre
// on prévient l'orchestre qu'on a fini la communication (cf. orchestre.c)
    	finTransactionClient();
    // sortie de la section critique
    //
    // si pas d'erreur et service normal
    //     ouverture des tubes avec le service
    //		openComcs(); (non implémenté)
    //     envoi du mot de passe au service
    //		sentMdpcs(MdpService);  (non implémenté)
    //     attente de l'accusé de réception du service
    //		codeService = readCodeRetourService();  (non implémenté)
    //     si mot de passe non accepté
    //		if (codeService == -1){
    //         message d'erreur
    //		printf("Mot de passe incorrect");
    //		}
    //     sinon
    //         appel de la fonction de communication avec le service :
    //             une fct par service selon numService :
    //                    . client_somme
    //                 ou . client_compression
    //                 ou . client_sigma
    //         envoi d'un accusé de réception au service
    //     finsi
    //     fermeture des tubes avec le service
    //		closeComcs();  (non implémenté)
    // finsi

// libération éventuelle de ressources
    	free(tubes2c);
    	free(tubec2s);
    
    return EXIT_SUCCESS;
}
