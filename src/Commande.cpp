#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <sstream>
#include "../include/commande.h"
#include "../include/produit.h"
class Panier;

/*
Méthode d'affichage du Commande nous permet du voir les produits commandés sous
forme du facture qui affiche les produits, leur quantités, leur TVA pourcentage
 et prix, leur prix HT et le prix total.
 La facture sera affichée de cette forme:
--------------------------------------------------------------

--- Facture N: xxxxx ---
Produit         | Quantité | TVA% | Prix TVA | Prix HT 
prod1                xx      xx%    xxx.xxDH   xxx.xxDH
prod2                xx      xx%    xxx.xxDH   xxx.xxDH
prod3                xx      xx%    xxx.xxDH   xxx.xxDH
prod4                xx      xx%    xxx.xxDH   xxx.xxDH
-------------------------------------------------------
                                 Total :       xxx.xxDH
                                 Total TVA:    xxx.xxDH
                                 Total TTC:    xxx.xxDH

--------------------------------------------------------------
*/

void Commande::AfficheCommande() {
    //initialisation des variables à utiliser;        
    int i, col1=15, col2=8, col3=4,col4=8, col5=8;

    for(auto& pdt : getProduits()){
        std::stringstream ss1, ss2;
        ss1 << std::fixed << std::setprecision(2) << pdt->calculerTVA();
        ss2 << std::fixed << std::setprecision(2) << pdt->getPrix();
        col1 = std::max(col1, (int)pdt->getNomProduit().length() +2);
        col3 = std::max(col3, (int)std::to_string(pdt->getTVA()).length() +2);
        col4 = std::max(col4, (int)ss1.str().length() +2);
        col5 = std::max(col5, (int)ss2.str().length() +2);
    }
    for(auto& qte : Quantity){
        col2 = std::max(col2, (int)(std::to_string(qte)).length() +2);
    }

    //Affichage du facture;
    std::cout << "\n--- Facture N: " << IdCommande << " ---" <<std::endl;
    std::cout << std::left << std::setw(col1) << "Produit" << " | ";
    std::cout << std::setw(col2) << "Quantite" << " | ";
    std::cout << std::setw(col3) << "TVA%" << " | ";
    std::cout << std::setw(col4) << "Prix TVA" << " | ";
    std::cout << std::setw(col5) << "Prix HT" <<std::endl;
    for (i=0; i<ProductsList.size(); i++){
        std::stringstream ss1, ss2;
        ss1 << std::fixed << std::setprecision(2) << ProductsList[i]->calculerTVA();
        ss2 << std::fixed << std::setprecision(2) << ProductsList[i]->getPrix();
        std::cout << std::left << std::setw(col1+3) << ProductsList[i]->getNomProduit();
        std::cout << std::setw(col2+3) << Quantity[i];
        std::cout << std::setw(col3+3) << ProductsList[i]->getTVA() << "%";
        std::cout << std::setw(col4+3) << ss1.str() << "DH";
        std::cout << std::setw(col5) << ss2.str() << "DH" <<std::endl;
    }
    std::cout << std::left << std::setfill('-') << std::setw(col1+col2+col3+col4+col5+12) <<std::endl;
    std::cout << std::left << std::setw(col1+col2+col3+6) << "Total :" << std::setw(col4+3) << Prix_HT << "DH" <<std::endl;
    std::cout << std::left << std::setw(col1+col2+col3+6) << "Total TVA:" << std::setw(col4) << Prix_HT << "DH" <<std::endl;
    std::cout << std::left << std::setw(col1+col2+col3+6) << "Prix TTC :" << std::setw(col4) << Prix_HT << "DH" <<std::endl;
}