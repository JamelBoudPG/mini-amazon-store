#ifndef PRODUIT_H
#define PRODUIT_H
#include <string>
#include <vector>
#include "../include/panier.h"

// Forward declaration — breaks the circular include with commande.h
class Commande;

class Produit {
    protected:
    int IdProduit;
    std::string NomProduit;
    float Prix;
    int TVA;
    int Stock;
    std::string Details;
    std::string Category;
    public:
    Produit() : IdProduit(-9999), NomProduit("NullProduct"), Prix(-1.0), TVA(0), Stock(0), Details("N|A"), Category("None"){}
    
    Produit(int idP, std::string nomP, float p, int tva, int stck, std::string det, std::string ctg) : IdProduit(idP), NomProduit(nomP), Prix(p), 
     TVA(tva), Details(det), Category(ctg){
        (stck < 0) ? Stock = -stck : Stock = stck ;
    }

    int getIdProduit() const {return IdProduit;};
    std::string getNomProduit() const {return NomProduit;};
    float getPrix() const {return Prix;};
    int getTVA() const {return TVA;};
    int getStock() const {return Stock;};
    std::string getCategory() const {return Category;};
    std::string getDetails() const {return Details;};

    void setIdProduit(int idP) {IdProduit = idP;};
    void setNomProduit(std::string nomP) {NomProduit = nomP;};
    void setPrix(float p) {Prix = p;};
    void setStock(int stck) {(stck < 0) ? Stock = -stck : Stock = stck;};
    void setDetails(std::string det) {Details = det;};
    void setCategory(std::string ctg) {Category = ctg;};

    float calculerTVA() const {return (Prix * (TVA/100.0f));}

    virtual double computerScore(Panier& Cart, std::vector<Commande>& History) const = 0;

    friend std::ostream& operator <<(std::ostream& os, Produit& pdt);
};

class Electronique : public Produit {
    private:
    int Guarantee;
    public:
    Electronique(int idP, std::string nomP, float p, int stck, std::string det, int warranty) 
    : Produit(idP, nomP, p, 20, stck, det, "Electronics"), Guarantee(warranty) {}

    int getGuarantee() const {return Guarantee;}
    void setGuarantee(int warranty) {Guarantee = (warranty < 0) ? 0 : warranty;}

    double computerScore(Panier& Cart, std::vector<Commande>& History) const override;
};

class Alimentaire : public Produit {
    private:
    int DureeExp;
    public:
    Alimentaire(int idP, std::string nomP, float p, int stck, std::string det, int exp) 
    : Produit(idP, nomP, p, 7, stck, det, "Food & Drinks"), DureeExp(exp) {}

    int getExpiration() const {return DureeExp;}
    void setExpiration(int duree) {DureeExp = (duree < 0) ? 0 : duree;}

    double computerScore(Panier& Cart, std::vector<Commande>& History) const override;
};

class Vetements : public Produit {
    private:
    bool Branded;
    public:
    Vetements(int idP, std::string nomP, float p, int stck, std::string det, bool branded) 
    : Produit(idP, nomP, p, 20, stck, det, "Clothes"), Branded(branded) {}

    bool getBrand() const {return Branded;}
    void setBrand(bool branded) {Branded = branded;}

    double computerScore(Panier& Cart, std::vector<Commande>& History) const override;
};

class Livre : public Produit {
    private:
    std::string Auteur;
    public:
    Livre(int idP, std::string nomP, float p, int stck, std::string det, std::string author) 
    : Produit(idP, nomP, p, 10, stck, det, "Books & Articles"), Auteur(author) {}

    std::string getAuthor() const {return Auteur;}
    void setAuthor(std::string author) {Auteur = author;}

    double computerScore(Panier& Cart, std::vector<Commande>& History) const override;
};

class Meuble : public Produit {
    public:
    Meuble(int idP, std::string nomP, float p, int stck, std::string det)
    : Produit(idP, nomP, p, 20, stck, det, "Furniture & Housing") {}

    double computerScore(Panier& Cart, std::vector<Commande>& History) const override;
};

class Parapharmacie : public Produit {
    public:
    Parapharmacie(int idP, std::string nomP, float p, int stck, std::string det) : Produit(idP, nomP, p, 7, stck, det, "Medics & Pharmacy") {}

    double computerScore(Panier& Cart, std::vector<Commande>& History) const override;
};

#endif
