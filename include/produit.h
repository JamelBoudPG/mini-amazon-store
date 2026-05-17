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
    int Stock;
    std::string Details;
    std::string Category;
    public:
    //Constructeur Par Default pour 
    Produit() : IdProduit(-9999), NomProduit("NullProduct"), Prix(-1.0), Stock(0), Details("N|A"), Category("None"){}
    Produit(int idP, std::string nomP, float p, int stck, std::string det, std::string ctg) : IdProduit(idP), NomProduit(nomP), Prix(p),
     Details(det), Category(ctg){
        (stck < 0) ? Stock = -stck : Stock = stck ;
    }
    //Getters
    int getIdProduit() const {return IdProduit;};
    std::string getNomProduit() const {return NomProduit;};
    float getPrix() const {return Prix;};
    int getStock() const {return Stock;};
    std::string getCategory() const {return Category;};
    std::string getDetails() const {return Details;};
    //Setters
    void setIdProduit(int idP) {IdProduit = idP;};
    void setNomProduit(std::string nomP) {NomProduit = nomP;};
    void setPrix(float p) {Prix = p;};
    void setStock(int stck) {(stck < 0) ? Stock = -stck : Stock = stck;};
    void setDetails(std::string det) {Details = det;};
    void setCategory(std::string ctg) {Category = ctg;};
    float calculerTVA(int percentage = 20) const {return (Prix * (percentage/100.0f));}
    virtual double computerScore(Panier& Cart, std::vector<Commande>& History) const = 0;
    friend std::ostream& operator <<(std::ostream& os, Produit& pdt);
};
class Electronique : public Produit {
    private:
    int Guarantee;
    public:
    Electronique(int idP, std::string nomP, float p, int stck, std::string det, int warranty) : Produit(idP, nomP, p, stck, det, "Electronics"), Guarantee(warranty) {}
    int getGuarantee() const {return Guarantee;}
    void setGuarantee(int warranty) {Guarantee = (warranty < 0) ? 0 : warranty;}
    double computerScore (Panier& Cart, std::vector<Commande>& History) const override;
};
class Alimentaire : public Produit {
    private:
    int DureeExp;
    public:
    Alimentaire(int idP, std::string nomP, float p, int stck, std::string det, int exp) : Produit(idP, nomP, p, stck, det, "Food & Drinks"), DureeExp(exp) {}
    int getExpiration() const {return DureeExp;}
    void setExpiration(int duree) {DureeExp = (duree < 0) ? 0 : duree;}
    double computerScore(Panier& Cart, std::vector<Commande>& History) const override;
};
class Vetements : public Produit {
    private:
    bool Branded;
    public:
    Vetements(int idP, std::string nomP, float p, int stck, std::string det, bool branded) : Produit(idP, nomP, p, stck, det, "Clothes"), Branded(branded) {}
    bool getBrand() const {return Branded;}
    void setBrand(bool branded) {Branded = branded;}
    double computerScore(Panier& Cart, std::vector<Commande>& History) const override;
};
class Articles : public Produit {
    private:
    std::string Auteur;
    public:
    Articles(int idP, std::string nomP, float p, int stck, std::string det, std::string author) : Produit(idP, nomP, p, stck, det, "Books & Articles"), Auteur(author) {}
    string getAuthor() const {return Auteur;}
    void setAuthor(std::string author) {Auteur = author;}
    double computerScore(Panier& Cart, std::vector<Commande>& History) const override;
};
class Meubles : public Produit {
    public:
    Meubles(int idP, std::string nomP, float p, int stck, std::string det) : Produit(idP, nomP, p, stck, det, "Furniture & Housing") {}
    double computerScore(Panier& Cart, std::vector<Commande>& History) const override;
};
class Parapharmacie : public Produit {
    public:
    Parapharmacie(int idP, std::string nomP, float p, int stck, std::string det) : Produit(idP, nomP, p, stck, det, "Medics & Pharmacy") {}
    double computerScore(Panier& Cart, std::vector<Commande>& History) const override;
};

#endif