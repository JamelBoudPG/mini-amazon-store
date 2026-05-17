class Commande {
    int id_cmd;
    vector<Produit*> items;
    double total_final;

public:
    Commande(int num, vector<Produit*> p) : id_cmd(num), items(p), total_final(0) {}

    void calc() {
        double ht = 0;
        for (auto p : items) ht += p->getPrix();
        // calcul avec tva et remise
        total_final = (ht * 1.20) - 50; 
    }

    void print() {
        cout << "\n--- Facture N: " << id_cmd << " ---" << endl;
        for (auto p : items) p->show();
        cout << "Total a payer: " << total_final << " DH" << endl;
    }
};


