#include <iostream>
#include "../include/client.h"
#include "../include/commande.h"

void Client::listerCommandes(){
    for (Commande* c : HistoryCmd){
        c->AfficheCommande();
    }
}

// Version console (cin/cout) — la version GUI equivalente
// se trouve dans main.cpp via l'interface Raylib.

Client* Client::InscriptionClient(std::vector<Client*> ClientsAccounts){
    Client* user = new Client();
    std::string strtemp;
    std::cout << "S'inscrire" << std::endl;
    std::cout << "Bienvenue à Mozart ! Pouvez-vous nous joindre?" << std::endl;
    std::cout << "Prenom :" << std::endl;
    std::cin >> strtemp;
    user->setSurname(strtemp);
    std::cout << "Nom :" << std::endl;
    std::cin >> strtemp;
    user->setName(strtemp);
    std::cout << "Email :" << std::endl;
    std::cin >> strtemp;
    user->setEmail(strtemp);
    std::cout << "Creer votre Mot de passe :" << std::endl;
    std::cin >> strtemp;
    user->setPassword(strtemp);
    while(user->Password.length() < 8){
        std::cout << "Mot de passe doit contenir 8 caracteres ou plus." << std::endl;
        std::cin >> strtemp;
        user->setPassword(strtemp);
    }
    std::cout << "Inscription validée. Veuillez profiter librement de nos services!" << std::endl;
    return user;
}

bool Client::SeConnecter(std::vector<Client*> ClientsAccounts, bool signin){
    std::string check1;
    std::string check2;
    std::cout << "Se Connecter" << std::endl;
    std::cout << "Content de vous revoir!" << std::endl;
    std::cout << "Email :" << std::endl;
    std::cin >> check1;
    std::cout << "Mot de passe :" << std::endl;
    std::cin >> check2;
    for(size_t i = 0; i < ClientsAccounts.size(); i++){
        while (check1 != ClientsAccounts[i]->getEmail() || check2 != ClientsAccounts[i]->getPassword()) {
            std::cout << "Votre Email ou mot de passe n'est pas valide! Veuillez réssayer." << std::endl;
            std::cout << "Email :" << std::endl;
            std::cin >> check1;
            std::cout << "Mot de passe :" << std::endl;
            std::cin >> check2;
        }
    }
    std::cout << "Inscription validée. Veuillez profiter librement de nos services!" << std::endl;
    signin = 1;
    return signin;
}