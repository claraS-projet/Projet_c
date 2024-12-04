#include "myassert.h"

#include "orchestre_service.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <assert.h>

//Initialisation des variables globales
int tubeOrchestreService[2];
int semos;
key_t cle;

void initTubeComos(){
// - création d'un tube anonyme pour converser (orchestre vers service)
	int ret = pipe(tubeOrchestreService);
	assert (ret == 0);
	
//Attention il faudra penser à fermer les bonnes extremitées du tube après l'exec 
}

void initTubeService(int numService){
//Fermeture de l'extremité inutile
	close(tubeOrchestreService[1]);
	printf(" Je suis le service %d (avant exec)\n", getpid());
	
	char nomTubeServiceClient[15];
	char nomTubeClientService[15];
	char num[2];
	char fileDescriptor[16];
	char *argv[6];

//Conversion des valeurs en String	
	sprintf(nomTubeServiceClient, "pipe_s2c_%d", numService);
	sprintf(nomTubeClientService, "pipe_c2s_%d", numService);
	sprintf(num, "%d", numService);
	sprintf(fileDescriptor, "%d", tubeOrchestreService[0]);
	
	printf("nom du 1er tube :%s\n", nomTubeServiceClient);
	printf("nom du 2eme tube :%s\n", nomTubeClientService);
	printf("Le fileDescriptor : %d; et la valeur de fileDescriptor : %s\n", tubeOrchestreService[0], fileDescriptor);

//Création du tableau d'argument a passer en paramètre du exec	
	argv[0] = "service";
	argv[1] = num;
// La cle du semaphore est key_t cle = ftok("../ORCHESTRE_SERVICE/orchestre_service.h", 1+numService);
//ici on indique simplement le chemin et on accedera à la clé directement dans le service
	argv[2] = "../ORCHESTRE_SERVICE/orchestre_service.h";
	argv[3] = fileDescriptor;
	argv[4] = nomTubeServiceClient;
        argv[5] = nomTubeClientService;
        
        execv(argv[0], argv);
}

int creationSemos(int numService){
// - création d'un sémaphore pour que le service préviene l'orchestre de la
//  fin d'un traitement
    cle = ftok("../ORCHESTRE_SERVICE/orchestre_service.h", numService+1);
    assert( cle != -1);
    
    semos = semget(cle, 1, IPC_CREAT | 0641);
    assert( semos != -1);
    return semos;
}

void destructionSemos(int idSem){
//Destruction du semaphore entre les services et l'orchestre
	int ret = semctl(idSem, -1, IPC_RMID);
}



void creationComcs(int numService){

// - création de deux tubes nommés (pour chaque service) pour les
//   communications entre les clients et les services
// Création du tube pour le sens service vers client
    char tubes2c[12];
	
    sprintf(tubes2c, "pipe_s2c_%d", numService);

    int ret;
    ret = mkfifo(tubes2c, 0644); 
    assert(ret != -1);
    

// Création du tube pour le sens client vers service
    char tubec2s[12];
	
    sprintf(tubec2s, "pipe_c2s_%d", numService);

    ret = mkfifo(tubec2s, 0644); 
    assert(ret != -1);
    
}

void destructionComcs(int numService){
// Destruction des tubes pour le sens service vers client
    char tubes2c[12];
	
    sprintf(tubes2c, "pipe_s2c_%d", numService);
    
    int ret;
    ret = unlink(tubes2c);
    assert(ret == 0);

// Destruction du tube pour le sens client vers service
    char tubec2s[12];
	
    sprintf(tubec2s, "pipe_c2s_%d", numService);

    ret = unlink(tubec2s);
    assert(ret == 0);

}
