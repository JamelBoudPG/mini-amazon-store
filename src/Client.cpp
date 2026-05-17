class Client {
    int id;
    string name;
    vector<Commande> orders;
public:
    Client(int i, string n) : id(i), name(n) {}
    void addCmd(Commande c) { orders.push_back(c); }
    void listOrders() {
        cout << "\nHistorique de " << name << ":" << endl;
        for (auto& o : orders) o.print();
    }
};
