//Clara Schobert

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "orchestre_service.h"
#include "client_service.h"
#include "service.h"
#include "service_somme.h"
#include "service_compression.h"
#include "service_sigma.h"


static void usage(const char *exeName, const char *message)
{
    fprintf(stderr, "usage : %s <num_service> <clé_sémaphore> <fd_tube_anonyme> "
            "<nom_tube_service_vers_client> <nom_tube_client_vers_service>\n",
            exeName);
    fprintf(stderr, "        <num_service>     : entre 0 et %d\n", SERVICE_NB - 1);
    fprintf(stderr, "        <clé_sémaphore>   : entre ce service et l'orchestre (clé au sens ftok)\n");
    fprintf(stderr, "        <fd_tube_anonyme> : entre ce service et l'orchestre\n");
    fprintf(stderr, "        <nom_tube_...>    : noms des deux tubes nommés reliés à ce service\n");
    if (message != NULL)
        fprintf(stderr, "message : %s\n", message);
    exit(EXIT_FAILURE);
}


/*----------------------------------------------*
 * fonction main
 *----------------------------------------------*/
int main(int argc, char * argv[])
{
    if (argc != 6)
        usage(argv[0], "nombre paramètres incorrect");

    // initialisations diverses : analyse de argv
    bool fin = true;

    while (true)
    {
        // attente d'un code de l'orchestre (via tube anonyme)
        //	int code = readCode();  (non implémenté)
        // si code de fin
        //	if(code == -1){
        //    sortie de la boucle
        //		fin = false; }
        // sinon
        //	else {
        //    réception du mot de passe de l'orchestre
        //	int mdp = readMdp();	(non implémenté)
        //    ouverture des deux tubes nommés avec le client
        //	openComcs(); (non implémenté)
        //    attente du mot de passe du client
        //	int mdpVerif = readMdpClient(); (non implémenté)
        //    si mot de passe incorrect
        //	if (mdp != mdpVerif){
        //        envoi au client d'un code d'erreur
        //		codeErreur(); (non implémenté) }
        //    sinon
        //	else {
        //        envoi au client d'un code d'acceptation
        //		codeAccept(); (non implémenté)
        //        appel de la fonction de communication avec le client :
        //            une fct par service selon numService (cf. argv[1]) :
        //                   . service_somme
        //                ou . service_compression
        //                ou . service_sigma
        //        attente de l'accusé de réception du client
        //		int finCom = readCodeRetour();  (non implémenté)
        //    finsi
        //		if(finCom == -1) {
        //    fermeture ici des deux tubes nommés avec le client
        //		finComsc();  (non implémenté)
        //    modification du sémaphore pour prévenir l'orchestre de la fin
        //		semAdd(); (non implémenté)
        // finsi
    }

    // libération éventuelle de ressources
    
    return EXIT_SUCCESS;
}
