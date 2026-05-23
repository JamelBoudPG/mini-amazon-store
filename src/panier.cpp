#include "../include/panier.h"
#include "../include/produit.h"

void Panier::ajouterProduit(Produit* p, int qte) {
    produits.push_back(p);
    quantites.push_back(qte);
}

void Panier::supprimerProduit(int id) {
    for(size_t i = 0; i < produits.size(); i++) {
        if(produits[i]->getIdProduit() == id) {
            produits.erase(produits.begin() + i);
            quantites.erase(quantites.begin() + i);
            return;
        }
    }
}

void Panier::vider() {
    produits.clear();
    quantites.clear();
}

double Panier::calculTotalHT() {
    double total = 0;

    for(size_t i = 0; i < produits.size(); i++)
        total += produits[i]->getPrix() * quantites[i];

    return total;
}

double Panier::calculTVA() {
    double tva = 0;

    for(size_t i = 0; i < produits.size(); i++)
        tva += produits[i]->calculerTVA() * quantites[i];

    return tva;
}

double Panier::calculTotalTTC() {
    return calculTotalHT() + calculTVA();
}

std::vector<Produit*>& Panier::getProduits() {return produits;}

std::vector<int>& Panier::getQuantity() {return quantites;}