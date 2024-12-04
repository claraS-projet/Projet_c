#ifndef ORCHESTRE_SERVICE_H
#define ORCHESTRE_SERVICE_H

// Ici toutes les communications entre l'orchestre et les services :
// - le tube anonyme pour que l'orchestre envoie des données au service
// - le sémaphore pour que  le service indique à l'orchestre la fin
//   d'un traitement

void initTubeComos();
void initTubeService(int numService);
int creationSemos(int numService);
void destructionSemos(int idSem);
void creationComcs(int numService);
void destructionComcs(int numService);

#endif
