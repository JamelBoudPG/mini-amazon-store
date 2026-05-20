#include <iostream>
#include <string>
#include <vector>
#include "../include/produit.h"
#include "../include/panier.h"
#include "../include/commande.h"
using namespace std;


//1- Surcharge d'opérateur d'affichage des informations d'un produit.

ostream& operator <<(ostream& os, Produit& pdt){
    os << pdt.getNomProduit() << endl;
    os << pdt.getCategory() << endl;
    os << pdt.getDetails() << endl;
    os << pdt.getPrix() << "DH" << endl;
    os << pdt.getStock() << " left.";
    return os;
}

/*
2- Méthode de calculer du score de recommendation de chaque produit
============================================================================
 - Partie Logique :
 ---------------------------------------------------------------------------
    * Chaque produit a un note de recommendation compris entre 0 et 10.
    * Chaque produit sera initialisé par un score recommandé moyen (5/10).
    * Ce note sera sélectionné par 3 facteurs principaux : 
       -- La présence du produit et ses semblables
         du même type au panier.
       -- La présence du produit et ses semblables
         dans les 5 derniers commandes.
       -- Le stock du chaque produits.
    * Il y a des autres facteurs explicites notamment :
       -- La garantie pour les produits électroniques.
       -- La durée d'expiration pour les produits alimentaires.
       -- La présence du marque pour les vêtements.
       -- Nom d'auteur pour les livres.
    * Pour la partie d'historique des commande, on utilise un facteur
     du présence du produit nommée "recencyWeight" qui valorise les
     produits achetés les plus récents. Cette valeur diminue si le produit
     était acheté précédemment par le client et ne sera plus acheté après.
    * Chaque score ajouté ou diminué sera multiplié par la quantité que
     le Client a déjà acheté pour ce produit.
 ---------------------------------------------------------------------------
 - Partie Fonctionnelle :
 ---------------------------------------------------------------------------
    * 2.1 - Partie Panier

       -- Pour Electronique, Vêtements et Livres:
            -#- +(𝟬.𝟱 * 𝙦𝙪𝙖𝙣𝙩𝙞𝙩𝙚́) pour chaque produit présent du même type.
            -#- -𝟮.𝟱 si le même produit dans le panier.
       -- Pour Alimentaire:
            -#- +(𝟬.𝟱 * 𝙦𝙪𝙖𝙣𝙩𝙞𝙩𝙚́) pour chaque produit présent du même type.
            -#- +𝟭.𝟮𝟱 si le même produit dans le panier.
       -- Pour Meubles:
            -#- +(𝟬.𝟱 * 𝙦𝙪𝙖𝙣𝙩𝙞𝙩𝙚́) pour chaque produit présent du même type.
                SEULEMENT s'il y a plus de 3 meubles dans le panier.
            -#- -𝟮 si le même produit dans le panier.
       -- Pour Parapharmacie:
            -#- +(𝟬.𝟱 * 𝙦𝙪𝙖𝙣𝙩𝙞𝙩𝙚́) pour chaque produit présent du même type.
            -#- -𝟬.𝟳𝟱 si le même produit dans le panier.

    -------------------
    
    * 2.2 - Partie Historique des Commandes

       -- Pour Electronique, Vêtements et Livres:
            -#- +(𝟬.𝟱 * 𝙦𝙪𝙖𝙣𝙩𝙞𝙩𝙚́ * 𝙧𝙚𝙘𝙚𝙣𝙘𝙮𝙒𝙚𝙞𝙜𝙝𝙩) pour chaque produit
             présent du même type.
            -#- -(𝟮.𝟱 * 𝙧𝙚𝙘𝙚𝙣𝙘𝙮𝙒𝙚𝙞𝙜𝙝𝙩) si le même produit dans
             l'historique.
       -- Pour Alimentaire et Parapharmacie:
            -#- +(𝟬.𝟱 * 𝙦𝙪𝙖𝙣𝙩𝙞𝙩𝙚́ * 𝙧𝙚𝙘𝙚𝙣𝙘𝙮𝙒𝙚𝙞𝙜𝙝𝙩) pour chaque produit
             présent du même type.
            -#- +(𝟭.𝟮𝟱 * 𝙧𝙚𝙘𝙚𝙣𝙘𝙮𝙒𝙚𝙞𝙜𝙝𝙩) si le même produit dans
             l'historique.
       -- Pour Meubles:
            -#- +(𝟬.𝟱 * 𝙦𝙪𝙖𝙣𝙩𝙞𝙩𝙚́ * 𝙧𝙚𝙘𝙚𝙣𝙘𝙮𝙒𝙚𝙞𝙜𝙝𝙩) pour chaque produit
             présent du même type.
            SEULEMENT s'il y a plus de 3 meubles dans l'historique.
            -#- +(𝟬.𝟳𝟱 * 𝙧𝙚𝙘𝙚𝙣𝙘𝙮𝙒𝙚𝙞𝙜𝙝𝙩) si le même produit dans
             l'historique.

    -------------------
    
    * 2.3 - Partie Stock

       -- Pour Electronique, Vêtements et Meubles:
            -#- =𝟬 si le stocke du produit est vide;
            -#- -𝟭 si le stocke est inférieur à cinq.
            -#- +𝟬.𝟱 si le stocke est supérieur à cinq.
       -- Pour Alimentaire, Livres et Parapharmacie:
            -#- =𝟬 si le stocke du produit est vide;
            -#- +𝟭 si le stocke est inférieur à cinq.
            -#- +𝟬.𝟱 si le stocke est supérieur à cinq.

    -------------------
    
    * 2.4 - Partie des facteurs explicites

       -- Electronique:
            -#- -𝟬.𝟳𝟱 pour les produits sans garantie.
            -#- +(𝟬.𝟬𝟳𝟱 + (𝙜𝙖𝙧𝙖𝙣𝙩𝙞𝙚 / 𝟭𝟮𝟬)) si le garantie d'un produit
             est compris entre 6 mois et 1 an.
            -#- +(𝟬.𝟮𝟱 + (𝙜𝙖𝙧𝙖𝙣𝙩𝙞𝙚 / 𝟭𝟮𝟬)) si le garantie d'un produit
             est supérieur d'un an.
       -- Alimentaire:
            -#- =𝟬 pour durée d'expiration nulle.
            -#- -𝟬.𝟳𝟱 si la durée d'expiration est inférieure
             strictement à 7 jours.
            -#- +𝟬.𝟳𝟱 si la durée d'expiration est supérieure
             strictement à 14 jours.
       -- Vêtements:
            -#- +𝟬.𝟱 si la pièce est d'une marque.
            -#- -𝟬.𝟱 si la pièce est "Brand-free"
            -#- +𝟬.𝟮𝟱 si le prix est compris entre 100 et 750 Dirhams.
            -#- -(𝟬.𝟮𝟱 * (𝙥𝙧𝙞𝙭 / 𝟭𝟬𝟬)) si le prix est en le cas
             contraire.
       -- Livres:
            -#- +(𝟭.𝟮𝟱 * 𝙦𝙪𝙖𝙣𝙩𝙞𝙩𝙚́) pour les livres du même auteur dans
             le panier.
            -#- +(𝟭.𝟳𝟱 * 𝙦𝙪𝙖𝙣𝙩𝙞𝙩𝙚́ * 𝙧𝙚𝙘𝙚𝙣𝙘𝙮𝙒𝙚𝙞𝙜𝙝𝙩) pour les livres
             du même auteur dans l'historique.
 ---------------------------------------------------------------------------
============================================================================
*/

double Electronique::computerScore(Panier& Cart, vector<Commande>& History) const{
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
            int qte = History[i].getQuantity()[j];
            if (pdt->getCategory() == "Electronics"){
                score += 0.5 * qte * recencyWeight;
            }
            if (pdt->getIdProduit() == IdProduit){
                score -= 2.5 * recencyWeight;
            }
        }
    }

    //Partie du vérification si le produit est valable ou non.
    if (getStock() == 0)
        score = 0;
    else if (getStock() <= 5)
        score -= 1;
    else
        score += 0.5;

    //Partie explicite du vérification du durée du garantie.
    if(Guarantee == 0)
        score -= 0.75;
    else if (Guarantee >= 6 && Guarantee <= 12)
        score += (0.075 + (double)(Guarantee / 120));
    else score += (0.25 + (double)(Guarantee / 120));
    return max(0.0, min(10.0, score));
}

double Alimentaire::computerScore(Panier& Cart, vector<Commande>& History) const{
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
            int qte = History[i].getQuantity()[j];
            if (pdt->getCategory() == "Food & Drinks"){
                score += 0.5 * qte * recencyWeight;
            }
            if (pdt->getIdProduit() == IdProduit){
                score += 1.25 * recencyWeight;
            }
        }
    }

    //Partie du vérification si le produit est valable ou non.
    if (Stock == 0)
        score = 0;
    else if (Stock <= 5)
        score += 1;
    else
        score += 0.5;
    
    //Partie explicite pour la durée d'expiration.
    if(DureeExp == 0){
        score = 0;
    }
    else if (DureeExp < 7)
        score -= 0.75;
    else if (DureeExp > 14)
        score += 0.75;
    return max(0.0, min(10.0, score));
}

double Vetements::computerScore(Panier& Cart, vector<Commande>& History) const{
    int i,j;
    double score = 5.0;
    int maxHistory = 5;
    //Partie de compter dependant du la panier.
    for(i=0; i<Cart.getProduits().size(); i++){
        Produit *pdt = Cart.getProduits()[i];
        int qte = Cart.getQuantities()[i];
        if(pdt->getCategory() == "Clothes"){
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
            int qte = History[i].getQuantity()[j];
            if (pdt->getCategory() == "Clothes"){
                score += 0.5 * qte * recencyWeight;
            }
            if (pdt->getIdProduit() == IdProduit){
                score -= 2.5 * recencyWeight;
            }
        }
    }

    //Partie du vérification si le produit est valable ou non.
    if (Stock == 0)
        score = 0;
    else if (Stock <= 5)
        score -= 1;
    else
        score += 0.5;
    //Parite explicite du vérification du marque.
    if(Branded)
        score += 0.5;
    else score -= 0.5;
    //Parite explicite du vérification du prix du produit.
    if(Prix < 100 || Prix > 750)
        score -= (0.25 * (Prix / 100));
    else score += 0.25;
    return max(0.0, min(10.0, score));
}

double Livre::computerScore(Panier& Cart, vector<Commande>& History) const{
    int i,j;
    double score = 5.0;
    int maxHistory = 5;
    //Partie de compter dependant du la panier.
    for(i=0; i<Cart.getProduits().size(); i++){
        Produit *pdt = Cart.getProduits()[i];
        int qte = Cart.getQuantities()[i];
        if(pdt->getCategory() == "Books & Articles"){
            score += 0.5 * qte;
        }
        if (pdt->getIdProduit() == IdProduit){
            score -= 2.5;
        }
        Livre* a = dynamic_cast<Livre*>(pdt);
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
            int qte = History[i].getQuantity()[j];
            if (pdt->getCategory() == "Books & Articles"){
                score += 0.5 * qte * recencyWeight;
            }
            if (pdt->getIdProduit() == IdProduit){
                score -= 2.5 * recencyWeight;
            }
            Livre* a = dynamic_cast<Livre*>(pdt);
            if (a && a->getAuthor() == Auteur)
                score += 1.75 * qte * recencyWeight;
        }
    }

    //Partie du vérification si le produit est valable ou non.
    if (Stock == 0)
        score = 0;
    else if (Stock <= 5)
        score += 1;
    else
        score += 0.5;
    return max(0.0, min(10.0, score));
}

double Meuble::computerScore(Panier& Cart, vector<Commande>& History) const{
    int i,j;
    double score = 5.0;
    int maxHistory = 5;
    int furnitureScale = 0;

    //Partie de compter dependant du la panier.
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
            int qte = History[i].getQuantity()[j];
            if (pdt->getCategory() == "Furniture & Housing"){
                furnitureScale += qte;
                if (furnitureScale > 3)
                    score += 0.5 * qte * recencyWeight;
            }
            if (pdt->getIdProduit() == IdProduit){
                score += 0.75 * recencyWeight;
            }
        }
    }

    //Partie du vérification si le produit est valable ou non.
    if (Stock == 0)
        score = 0;
    else if (Stock <= 5)
        score -= 1;
    else
        score += 0.5;
    return max(0.0, min(10.0, score));
}

double Parapharmacie::computerScore(Panier& Cart, vector<Commande>& History) const{
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
            int qte = History[i].getQuantity()[j];
            if (pdt->getCategory() == "Medics & Pharmacy"){
                score += 0.5 * qte * recencyWeight;
            }
            if (pdt->getIdProduit() == IdProduit){
                score += 1.25 * recencyWeight;
            }
        }
    }

    //Partie du vérification si le produit est valable ou non.
    if (Stock == 0)
        score = 0;
    else if (Stock <= 5)
        score += 1;
    else
        score += 0.5;
    return max(0.0, min(10.0, score));
}