#ifndef PRODUIT_H
#define PRODUIT_H
//fichier header du classe produit
//on fait l'inclusion du "string" library pour permettre l'utilisation du type string
#include <string>
#include <vector>
//Declaration Temporaire : à faire Par Mohamed & Mohammed
#include "../include/panier.h"
#include "../include/commande.h"

class Produit {
    /*puisque on aura des classes qui hérite du cette classe,
    donc on definie les attribut sous protection "protected"*/
    protected:
    int IdProduit;
    std::string NomProduit;
    float Prix;
    int TVA;
    int Stock;
    //Details représente la description d'un produit
    std::string Details;
    std::string Category;
    public:
    //Constructeur Par Default

    Produit() : IdProduit(-9999), NomProduit("NullProduct"), Prix(-1.0), TVA(0), Stock(0), Details("N|A"), Category("None"){}
    
    //Constructeur Paramétrique
    Produit(int idP, std::string nomP, float p, int tva, int stck, std::string det, std::string ctg) : IdProduit(idP), NomProduit(nomP), Prix(p), 
     TVA(tva), Details(det), Category(ctg){
        //On vérifie que le stock ne sera jamais du valeur négative
        (stck < 0) ? Stock = -stck : Stock = stck ;
    }
    //Getters - Accesseurs

    int getIdProduit() const {return IdProduit;};
    std::string getNomProduit() const {return NomProduit;};
    float getPrix() const {return Prix;};
    int getTVA() const {return TVA;};
    int getStock() const {return Stock;};
    std::string getCategory() const {return Category;};
    std::string getDetails() const {return Details;};

    //Setters - Mutateurs

    void setIdProduit(int idP) {IdProduit = idP;};
    void setNomProduit(std::string nomP) {NomProduit = nomP;};
    void setPrix(float p) {Prix = p;};
    void setStock(int stck) {(stck < 0) ? Stock = -stck : Stock = stck;};
    void setDetails(std::string det) {Details = det;};
    void setCategory(std::string ctg) {Category = ctg;};

    /*Fonction du calculation du TVA pourcentage qui prend l'attribut
     qui est déjà initialisé en un des sous classes et le calculer.*/
    float calculerTVA() const {return (Prix * (TVA/100.0f));}

    /*Fonction de calculer le score de recommendation qui est pure virtuelle.
     Il doît retourner une valeur entre 0 et 10 où 10 est le plus recommendé pour le client.*/
    virtual double computerScore(Panier& Cart, std::vector<Commande>& History) const = 0;

    //Fonction d'affichage du produit qui fait une surchage d'opérateur du sortie.
    friend std::ostream& operator <<(std::ostream& os, Produit& pdt);
};

//Sous classe N°1 : Produits Electroniques.
class Electronique : public Produit {
    private:
    //Attribut du garantie ajoutée pour jouer un rôle dans le score de recommendation.
    int Guarantee;
    public:
    /*Constructeur Paramétrique qui accepte les valeurs d'un produit où
     la valeur TVA doît être initialisé en valeur du 20% et la catégorie
     est seléctionné en "Electronics".*/
    Electronique(int idP, std::string nomP, float p, int stck, std::string det, int warranty) 
    : Produit(idP, nomP, p, 20, stck, det, "Electronics"), Guarantee(warranty) {}

    //Accesseur et Mutateur.
    int getGuarantee() const {return Guarantee;}
    void setGuarantee(int warranty) {Guarantee = (warranty < 0) ? 0 : warranty;}

    //Fonction override du calculer le score de recommendation pour les produits électroniques.
    double computerScore (Panier& Cart, std::vector<Commande>& History) const override;
};

//Sous classe N°2 : Produits Alimentaires.
class Alimentaire : public Produit {
    private:
    //Attribut du durée d'expiration ajoutée pour jouer un rôle dans le score de recommendation.
    int DureeExp;
    public:
    /*Constructeur Paramétrique qui accepte les valeurs d'un produit où
     la valeur TVA doît être initialisé en valeur du 7% et la catégorie
     est seléctionné en "Food & Drinks".*/
    Alimentaire(int idP, std::string nomP, float p, int stck, std::string det, int exp) 
    : Produit(idP, nomP, p, 7, stck, det, "Food & Drinks"), DureeExp(exp) {}

    //Accesseur et Mutateur.
    int getExpiration() const {return DureeExp;}
    void setExpiration(int duree) {DureeExp = (duree < 0) ? 0 : duree;}

    //Fonction override du calculer le score de recommendation pour les produits alimentaires.
    double computerScore(Panier& Cart, std::vector<Commande>& History) const override;
};

//Sous classe N°3 : Vêtements.
class Vetements : public Produit {
    private:
    /*Attribut qui vérifie est ce que la pièce est du marque ou non,
     ajoutée pour jouer un rôle dans le score de recommendation.*/
    bool Branded;
    public:
    /*Constructeur Paramétrique qui accepte les valeurs d'un produit où
     la valeur TVA doît être initialisé en valeur du 20% et la catégorie
     est seléctionné en "Clothes".*/
    Vetements(int idP, std::string nomP, float p, int stck, std::string det, bool branded) 
    : Produit(idP, nomP, p, 20, stck, det, "Clothes"), Branded(branded) {}

    //Accesseur et Mutateur
    bool getBrand() const {return Branded;}
    void setBrand(bool branded) {Branded = branded;}

    //Fonction override du calculer le score de recommendation pour les Vêtements.
    double computerScore(Panier& Cart, std::vector<Commande>& History) const override;
};

//Sous classe N°4 : Livres.
class Livre : public Produit {
    private:
    //Attribut du nom d'auteur ajoutée pour jouer un rôle dans le score de recommendation.
    std::string Auteur;
    public:
    /*Constructeur Paramétrique qui accepte les valeurs d'un produit où
     la valeur TVA doît être initialisé en valeur du 10% et la catégorie
     est seléctionné en "Books & Articles".*/
    Livre(int idP, std::string nomP, float p, int stck, std::string det, std::string author) 
    : Produit(idP, nomP, p, 10, stck, det, "Books & Articles"), Auteur(author) {}

    //Accesseur et Mutateur
    string getAuthor() const {return Auteur;}
    void setAuthor(std::string author) {Auteur = author;}

    //Fonction override du calculer le score de recommendation pour les livres.
    double computerScore(Panier& Cart, std::vector<Commande>& History) const override;
};

//Sous classe N°5 : Meubles.
class Meuble : public Produit {
    public:
    /*Constructeur Paramétrique qui accepte les valeurs d'un produit où
     la valeur TVA doît être initialisé en valeur du 20% et la catégorie
     est seléctionné en "Furniture & Housing".*/
    Meuble(int idP, std::string nomP, float p, int stck, std::string det)
    : Produit(idP, nomP, p, 20, stck, det, "Furniture & Housing") {}

    //Fonction override du calculer le score de recommendation pour les meubles.
    double computerScore(Panier& Cart, std::vector<Commande>& History) const override;
};

//Sous classe N°6 : Produits Médicaux et Parapharmaceutiques.
class Parapharmacie : public Produit {
    public:
    /*Constructeur Paramétrique qui accepte les valeurs d'un produit où
     la valeur TVA doît être initialisé en valeur du 7% et la catégorie
     est seléctionné en "Medics & Pharmacy".*/
    Parapharmacie(int idP, std::string nomP, float p, int stck, std::string det) : Produit(idP, nomP, p, 7, stck, det, "Medics & Pharmacy") {}

    //Fonction override du calculer le score de recommendation pour les produits médicaux.
    double computerScore(Panier& Cart, std::vector<Commande>& History) const override;
};

#endif