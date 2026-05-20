//Declaration Temporaire : à faire Par Mohamed & Mohammed
#ifndef COMMANDE_H
#define COMMANDE_H

#include <vector>
#include "produit.h"
#include "panier.h"

/*
La classe Commande est considérée comme une copie finale du panier qui sera inchangeable.
Donc les valeurs initialisés dans une commande doîvent être tous constantes.
*/
class Commande {
    int IdCommande;
    std::vector<Produit*> ProductsList;
    std::vector<int> Quantity;
    double Prix_HT;
    double Prix_TVA;
    double Prix_TTC;

public:
    //Constructeur Paramétrique
    Commande(int idC, std::vector<Produit*> p, std::vector<int> qte, double price, double tax, double pricettl) 
    : IdCommande(idC), ProductsList(p), Quantity(qte), Prix_HT(price), Prix_TVA(tax), Prix_TTC(pricettl) {}
    //Constructeur par copie
    Commande(const Commande& cmd) : IdCommande(cmd.IdCommande), ProductsList(cmd.ProductsList),
     Quantity(cmd.Quantity), Prix_HT(cmd.Prix_HT), Prix_TVA(cmd.Prix_TVA), Prix_TTC(cmd.Prix_TTC) {}
    /*On peut considérer la commande comme une moule finale du la panier,
     donc on peut construire une commande à partir d'un panier.*/
    Commande(int idC, Panier& cart) : IdCommande(idC), ProductsList(cart.getProduits()), Quantity(cart.getQuantity()),
     Prix_HT(cart.calculTotalHT()), Prix_TVA(cart.calculTVA()), Prix_TTC(cart.calculTotalTTC()) {}

    /*Puisqu'on a des valeurs constantes,
     donc on ne peut pas les modifier,
     alors on ne pense pas de créer des mutateurs pour les attributs*/
    int getIdCommande() const {return IdCommande;}
    std::vector<Produit*> getProduits() const {return ProductsList;}
    std::vector<int> getQuantity() const {return Quantity;}
    double getPrixHT() const {return Prix_HT;}
    double getPrixTVA() const {return Prix_TVA;}
    double getPrixTTC() const {return Prix_TTC;}

    //Méthode d'affichage du la facture du commande;
    void AfficheCommande();
};

#endif