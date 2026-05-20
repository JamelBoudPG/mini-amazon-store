#include "../include/client.h"
#include "../include/commande.h"

void Client::listerCommandes(){
    for (Commande c : HistoryCmd){
        c.AfficheCommande();
    }
}
