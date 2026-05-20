#include "../include/produit.h"
#include "../include/panier.h"
#include "../include/commande.h"

Produit* recommanderProduit(
    std::vector<Produit*> catalogue,
    Panier& panier,
    std::vector<Commande>& historique
) {
    Produit* best = nullptr;
    double bestScore = -1;

    for(auto p : catalogue) {
        double score = p->computerScore(panier, historique);

        if(score > bestScore) {
            bestScore = score;
            best = p;
        }
    }

    return best;
}