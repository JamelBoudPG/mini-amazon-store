#ifndef CLIENT_H
#define CLIENT_H
#include <string>
#include <vector>
#include "commande.h"

class Client {
private:
    int const IdClient;
    std::string Name;
    std::string Surname;
    std::string email;
    std::string Password;
    std::vector<Commande*> HistoryCmd;
public:

    Client(std::vector<Commande*> history={}) 
    : IdClient(-99), Name("na"), Surname("na") , HistoryCmd(history) {}

    Client(int id, std::string surname, std::string name, std::vector<Commande*> history={}) 
    : IdClient(id), Name(name), Surname(surname) , HistoryCmd(history) {}
    
    Client* InscriptionClient(std::vector<Client*> ClientsAccounts);
    bool SeConnecter(std::vector<Client*> ClientsAccounts, bool signin=0);

    int getIdClient() {return IdClient;}
    std::string getName() {return Name;}
    std::string getSurname() {return Surname;}
    std::string getEmail() {return email;}
    std::string getPassword() {return Password;}

    void setName(std::string name) {Name = name;}
    void setSurname(std::string surname) {Surname = surname;}
    void setEmail(std::string mail) {email = mail;}
    void setPassword(std::string pswd) {Password = pswd;}

    std::vector<Commande*> getHistoryCmd() {return HistoryCmd;}
    void ajouterCommande(Commande* cmd) { HistoryCmd.push_back(cmd); }
    void listerCommandes();
};

#endif