#ifndef PRODUIT_H
#define PRODUIT_H

#include <string>
#include <vector>

class Panier;
class Commande;

class Produit {
protected:
    int IdProduit;
    std::string NomProduit;
    float Prix;

public:
    Produit() {}

    int getIdProduit() const { return IdProduit; }
    float getPrix() const { return Prix; }

    float calculerTVA(int percentage = 20) const {
        return Prix * (percentage / 100.0f);
    }

    virtual double computerScore(
        Panier& cart,
        std::vector<Commande>& history
    ) const = 0;
};

#endif