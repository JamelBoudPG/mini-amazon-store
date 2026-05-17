#include <iostream>
#include <string>
#include <vector>
#include "../include/produit.h"
//Declaration Temporaire : à faire Par Mohamed & Mohammed
#include "../include/panier.h"
#include "../include/commande.h"
using namespace std;

std::ostream& operator <<(std::ostream& os, Produit& pdt){
    os << pdt.getNomProduit() << endl;
    os << pdt.getCategory() << endl;
    os << pdt.getDetails() << endl;
    os << pdt.getPrix() << "DH" << endl;
    os << pdt.getStock() << " left.";
    return os;
}

double Electronique::computerScore(Panier& Cart, std::vector<Commande>& History) const{
    int i,j;
    double score = 5.0;
    int maxHistory = 5;
    //Partie de compter dependant du la panier
    for(i=0; i<Cart.getProduits().size(); i++){
        Produit *pdt = Cart.getProduits()[i];
        int qte = Cart.getQuantities()[i];
        if(pdt->getCategory() == "Electronics"){
            score += 0.5 * qte;
        }
        if (pdt->getIdProduit() == IdProduit){
            score -= 2.5;
        }
    }
    //Partie Historique des achats;
    int effectiveHistory = min(maxHistory, (int)History.size());
    int start = History.size() - effectiveHistory;


    for(i=start; i<History.size(); i++){
        double recencyWeight = (double)(i - start + 1) / effectiveHistory;
        //Partie modifiable dépend de la forme du classe Commande;
        for(j=0;j<History[i].getProduits().size(); j++){
            Produit *pdt = History[i].getProduits()[j];
            int qte = History[i].getQuantities()[j];
            if (pdt->getCategory() == "Electronics"){
                score += 0.5 * qte * recencyWeight;
            }
            if (pdt->getIdProduit() == IdProduit){
                score -= 2.5 * recencyWeight;
            }
        }
    }
    if (getStock() == 0)
        score = 0;
    else if (getStock() <= 5)
        score -= 1;
    else
        score += 0.5;
    if(Guarantee == 0)
        score -= 0.75;
    else if (Guarantee >= 6 && Guarantee <= 12)
        score += (0.075 + (double)(Guarantee / 120));
    else score += (0.25 + (double)(Guarantee / 120));
    return max(0.0, min(10.0, score));
}

double Alimentaire::computerScore(Panier& Cart, std::vector<Commande>& History) const{
    int i,j;
    double score = 5.0;
    int maxHistory = 5;
    //Partie de compter dependant du la panier
    for(i=0; i<Cart.getProduits().size(); i++){
        Produit *pdt = Cart.getProduits()[i];
        int qte = Cart.getQuantities()[i];
        if(pdt->getCategory() == "Food & Drinks"){
            score += 0.5 * qte;
        }
        if (pdt->getIdProduit() == IdProduit){
            score += 1.25;
        }
    }
    //Partie Historique des achats;
    int effectiveHistory = min(maxHistory, (int)History.size());
    int start = History.size() - effectiveHistory;


    for(i=start; i<History.size(); i++){
        double recencyWeight = (double)(i - start + 1) / effectiveHistory;
        //Partie modifiable dépend de la forme du classe Commande;
        for(j=0;j<History[i].getProduits().size(); j++){
            Produit *pdt = History[i].getProduits()[j];
            int qte = History[i].getQuantities()[j];
            if (pdt->getCategory() == "Food & Drinks"){
                score += 0.5 * qte * recencyWeight;
            }
            if (pdt->getIdProduit() == IdProduit){
                score += 1.25 * recencyWeight;
            }
        }
    }
    if (Stock == 0)
        score = 0;
    else if (Stock <= 5)
        score += 1;
    else
        score += 0.5;
    if(DureeExp == 0){
        score = 0;
    }
    else if (DureeExp < 7)
        score -= 0.75;
    else if (DureeExp > 14)
        score += 0.75;
    return max(0.0, min(10.0, score));
}

double Vetements::computerScore(Panier& Cart, std::vector<Commande>& History) const{
    int i,j;
    double score = 5.0;
    int maxHistory = 5;
    //Partie de compter dependant du la panier
    for(i=0; i<Cart.getProduits().size(); i++){
        Produit *pdt = Cart.getProduits()[i];
        int qte = Cart.getQuantities()[i];
        if(pdt->getCategory() == "Clothes"){
            score += 0.5 * qte;
        }
        if (pdt->getIdProduit() == IdProduit){
            score += 1.25;
        }
    }
    //Partie Historique des achats;
    int effectiveHistory = min(maxHistory, (int)History.size());
    int start = History.size() - effectiveHistory;


    for(i=start; i<History.size(); i++){
        double recencyWeight = (double)(i - start + 1) / effectiveHistory;
        //Partie modifiable dépend de la forme du classe Commande;
        for(j=0;j<History[i].getProduits().size(); j++){
            Produit *pdt = History[i].getProduits()[j];
            int qte = History[i].getQuantities()[j];
            if (pdt->getCategory() == "Clothes"){
                score += 0.5 * qte * recencyWeight;
            }
            if (pdt->getIdProduit() == IdProduit){
                score -= 1.25 * recencyWeight;
            }
        }
    }
    if (Stock == 0)
        score = 0;
    else if (Stock <= 5)
        score -= 1;
    else
        score += 0.5;
    if(Branded)
        score += 0.5;
    else score -= 0.5;
    if(Prix < 100 || Prix > 500)
        score -= (0.25 * (Prix / 100));
    else score += 0.25;
    return max(0.0, min(10.0, score));
}

double Articles::computerScore(Panier& Cart, std::vector<Commande>& History) const{
    int i,j;
    double score = 5.0;
    int maxHistory = 5;
    //Partie de compter dependant du la panier
    for(i=0; i<Cart.getProduits().size(); i++){
        Produit *pdt = Cart.getProduits()[i];
        int qte = Cart.getQuantities()[i];
        if(pdt->getCategory() == "Books & Articles"){
            score += 0.5 * qte;
        }
        if (pdt->getIdProduit() == IdProduit){
            score -= 0.75;
        }
        Articles* a = dynamic_cast<Articles*>(pdt);
        if (a && a->getAuthor() == Auteur)
            score += 1.25 * qte;
    }
    //Partie Historique des achats;
    int effectiveHistory = min(maxHistory, (int)History.size());
    int start = History.size() - effectiveHistory;


    for(i=start; i<History.size(); i++){
        double recencyWeight = (double)(i - start + 1) / effectiveHistory;
        //Partie modifiable dépend de la forme du classe Commande;
        for(j=0;j<History[i].getProduits().size(); j++){
            Produit *pdt = History[i].getProduits()[j];
            int qte = History[i].getQuantities()[j];
            if (pdt->getCategory() == "Books & Articles"){
                score += 0.5 * qte * recencyWeight;
            }
            if (pdt->getIdProduit() == IdProduit){
                score -= 1.25 * recencyWeight;
            }
            Articles* a = dynamic_cast<Articles*>(pdt);
            if (a && a->getAuthor() == Auteur)
                score += 1.75 * qte * recencyWeight;
        }
    }
    if (Stock == 0)
        score = 0;
    else if (Stock <= 5)
        score += 1;
    else
        score += 0.5;
    return max(0.0, min(10.0, score));
}

double Meubles::computerScore(Panier& Cart, std::vector<Commande>& History) const{
    int i,j;
    double score = 5.0;
    int maxHistory = 5;
    int furnitureScale = 0;
    //Partie de compter dependant du la panier
    for(i=0; i<Cart.getProduits().size(); i++){
        Produit *pdt = Cart.getProduits()[i];
        int qte = Cart.getQuantities()[i];
        if(pdt->getCategory() == "Furniture & Housing"){
            furnitureScale += qte;
            if (furnitureScale > 3)
                score += 0.5 * qte;
        }
        if (pdt->getIdProduit() == IdProduit){
            score -= 2;
        }
    }
    //Partie Historique des achats;
    int effectiveHistory = min(maxHistory, (int)History.size());
    int start = History.size() - effectiveHistory;


    for(i=start; i<History.size(); i++){
        double recencyWeight = (double)(i - start + 1) / effectiveHistory;
        //Partie modifiable dépend de la forme du classe Commande;
        for(j=0;j<History[i].getProduits().size(); j++){
            Produit *pdt = History[i].getProduits()[j];
            int qte = History[i].getQuantities()[j];
            if (pdt->getCategory() == "Furniture & Housing"){
                score += 0.5 * qte * recencyWeight;
            }
            if (pdt->getIdProduit() == IdProduit){
                score += 1.25 * recencyWeight;
            }
        }
    }
    if (Stock == 0)
        score = 0;
    else if (Stock <= 5)
        score -= 1;
    else
        score += 0.5;
    return max(0.0, min(10.0, score));
}

double Parapharmacie::computerScore(Panier& Cart, std::vector<Commande>& History) const{
    int i,j;
    double score = 5.0;
    int maxHistory = 5;
    //Partie de compter dependant du la panier
    for(i=0; i<Cart.getProduits().size(); i++){
        Produit *pdt = Cart.getProduits()[i];
        int qte = Cart.getQuantities()[i];
        if(pdt->getCategory() == "Medics & Pharmacy"){
            score += 0.5 * qte;
        }
        if (pdt->getIdProduit() == IdProduit){
            score -= 0.75;
        }
    }
    //Partie Historique des achats;
    int effectiveHistory = min(maxHistory, (int)History.size());
    int start = History.size() - effectiveHistory;


    for(i=start; i<History.size(); i++){
        double recencyWeight = (double)(i - start + 1) / effectiveHistory;
        //Partie modifiable dépend de la forme du classe Commande;
        for(j=0;j<History[i].getProduits().size(); j++){
            Produit *pdt = History[i].getProduits()[j];
            int qte = History[i].getQuantities()[j];
            if (pdt->getCategory() == "Medics & Pharmacy"){
                score += 0.5 * qte * recencyWeight;
            }
            if (pdt->getIdProduit() == IdProduit){
                score += 1.25 * recencyWeight;
            }
        }
    }
    if (Stock == 0)
        score = 0;
    else if (Stock <= 5)
        score += 1;
    else
        score += 0.5;
    return max(0.0, min(10.0, score));
}