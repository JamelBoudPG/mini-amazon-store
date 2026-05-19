#ifndef PANIER_H
#define PANIER_H

#include <vector>

class Produit;

class Panier {
private:
    std::vector<Produit*> produits;
    std::vector<int> quantites;

public:
    void ajouterProduit(Produit* p, int qte = 1);
    void supprimerProduit(int id);
    void vider();

    double calculTotalHT();
    double calculTVA();
    double calculTotalTTC();

    std::vector<Produit*> getProduits();
    std::vector<int> getQuantities();
};

#endif