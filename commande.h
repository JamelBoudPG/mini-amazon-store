#ifndef COMMANDE_H
#define COMMANDE_H

#include <vector>

class Produit;

class Commande {
public:
    std::vector<Produit*> getProduits() { return {}; }
    std::vector<int> getQuantities() { return {}; }
};

#endif