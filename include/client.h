#ifndef CLIENT_H
#define CLIENT_H
#include <string>
#include <vector>
#include "commande.h"

class Client {
    int const IdClient;
    std::string Name;
    std::string Surname;
    std::vector<Commande*> HistoryCmd;
public:
    Client(int id, std::string surname, std::string name, std::vector<Commande*> history={}) 
    : IdClient(id), Name(name), Surname(surname) , HistoryCmd(history) {}
    int getIdClient() {return IdClient;}
    std::string getName() {return Name;}
    std::string getSurname() {return Surname;}
    std::vector<Commande*> getHistoryCmd() {return HistoryCmd;}
    void ajouterCommande(Commande* cmd) { HistoryCmd.push_back(cmd); }
    void listerCommandes();
};

#endif