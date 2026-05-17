//Declaration Temporaire : à faire Par Mohamed & Mohammed
#ifndef PANIER_H
#define PANIER_H
#include <vector>
class Produit;

class Panier {
public:
    std::vector<Produit*> getProduits() { return {}; }
    std::vector<int> getQuantities() { return {}; }
};
#endif