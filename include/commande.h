//Declaration Temporaire : à faire Par Mohamed & Mohammed
#ifndef COMMANDE_H
#define COMMANDE_H

#include <vector>

class Produit;
class Panier;

/*
La classe Commande est considérée comme une copie finale du panier qui sera inchangeable.
Donc les valeurs initialisés dans une commande doîvent être tous constantes.
*/
class Commande {
    int IdCommande;
    std::vector<Produit*> const ProductsList;
    std::vector<int> const Quantity;
    double const Prix_HT;
    double const Prix_TVA;
    double const Prix_TTC;

public:
    //Constructeur Paramétrique
    Commande(int idC, std::vector<Produit*> p, std::vector<int> qte, double price, double tax, double pricettl) 
    : IdCommande(idC), ProductsList(p), Quantity(qte), Prix_HT(price), Prix_TVA(tax), Prix_TTC(pricettl) {}
    //Constructeur par copie
    Commande(const Commande& cmd) : IdCommande(cmd.IdCommande), ProductsList(cmd.ProductsList),
     Quantity(cmd.Quantity), Prix_HT(cmd.Prix_HT), Prix_TVA(cmd.Prix_TVA), Prix_TTC(cmd.Prix_TTC) {}
    /*On peut considérer la commande comme une moule finale du la panier,
     donc on peut construire une commande à partir d'un panier.*/
    Commande(const Panier& cart);

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