/*
 * main.cpp — Raylib UI for Mini Amazon Store
 *
 * Compile (from project root):
 *   g++ -std=c++17 src/main.cpp src/produit.cpp src/panier.cpp \
 *       src/Commande.cpp src/Client.cpp src/recommandation.cpp \
 *       -lraylib -lm -o storeTest
 */

#include <raylib.h>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>

#include "../include/produit.h"
#include "../include/commande.h"
#include "../include/panier.h"
#include "../include/client.h"

// ─── Forward declaration ──────────────────────────────────────────────────────
Produit* recommanderProduit(
    std::vector<Produit*> catalogue,
    Panier& panier,
    std::vector<Commande>& historique
);

// ─── Capture helpers ──────────────────────────────────────────────────────────
std::string produitToString(Produit& p) {
    std::ostringstream oss;
    oss << p;
    return oss.str();
}

std::string commandeToString(Commande& cmd) {
    std::ostringstream oss;
    std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
    cmd.AfficheCommande();
    std::cout.rdbuf(old);
    return oss.str();
}

std::string panierToString(Panier& panier) {
    std::ostringstream oss;
    auto& produits  = panier.getProduits();
    auto& quantites = panier.getQuantity();
    if (produits.empty()) {
        oss << "(panier vide)";
    } else {
        for (size_t i = 0; i < produits.size(); i++) {
            oss << "- " << produits[i]->getNomProduit()
                << "  x" << quantites[i]
                << "  (" << produits[i]->getCategory() << ")"
                << "  " << produits[i]->getPrix() * quantites[i] << " DH\n";
        }
        oss << "\nTotal HT  : " << panier.calculTotalHT()  << " DH\n";
        oss << "Total TVA : " << panier.calculTVA()        << " DH\n";
        oss << "Total TTC : " << panier.calculTotalTTC()   << " DH";
    }
    return oss.str();
}

// ─── Drawing helpers ──────────────────────────────────────────────────────────

// DrawMultilineText with custom font
int DrawMultilineTextF(Font font, const std::string& text, int x, int y, int fontSize, Color color) {
    std::istringstream stream(text);
    std::string line;
    int lineHeight = fontSize + 5;
    while (std::getline(stream, line)) {
        DrawTextEx(font, line.c_str(), { (float)x, (float)y }, (float)fontSize, 1.0f, color);
        y += lineHeight;
    }
    return y;
}

// Panel with custom font
int DrawPanelF(Font font, const char* title, int x, int y, int w, int h, Color bg, Color border) {
    DrawRectangle(x, y, w, h, bg);
    DrawRectangleLines(x, y, w, h, border);
    DrawTextEx(font, title, { (float)(x + 10), (float)(y + 8) }, 16, 1.0f, border);
    DrawLine(x, y + 28, x + w, y + 28, border);
    return y + 36;
}

// Mini tab button — returns true if active
void DrawMiniTab(Font font, const char* label, Rectangle rect, bool active, Color accent, Color tabOff, Color border) {
    DrawRectangleRec(rect, active ? accent : tabOff);
    DrawRectangleLinesEx(rect, 1, active ? accent : border);
    int tw = (int)MeasureTextEx(font, label, 15, 1.0f).x;
    DrawTextEx(font, label,
               { rect.x + (rect.width  - tw) / 2.0f,
                 rect.y + (rect.height - 15) / 2.0f },
               15, 1.0f, active ? WHITE : BLACK);
}

// ─── Screens & sub-tabs ───────────────────────────────────────────────────────
enum Screen    { AUTH, CATALOGUE, PANIER, PROFILE, PARAMETRES };
enum AuthMode  { SIGNUP, LOGIN };
enum ProfileTab{ INFOS, HISTORIQUE };
int logoutBtnY = 0;

// ─── Main ─────────────────────────────────────────────────────────────────────
int main() {

    // ── Catalogue ─────────────────────────────────────────────────────────────
    Electronique  laptop     (1,  "Laptop Pro 15",          8999.99f, 12, "PC Portable avec 16GB RAM",         24);
    Electronique  headphones (2,  "Headphones BT",           450.00f,  3, "Sans-fil avec réduction active du bruit",        12);
    Alimentaire   coffee     (3,  "Arabica Cafe 1kg",       85.00f, 50, "Cafe du Haute Qualite",           180);
    Alimentaire   milk       (4,  "Lait Centrale Danone 1L",            12.50f,  2, "Lait entier",                     5);
    Vetements     jacket     (5,  "T-Shirt POLO",            145.00f,  8, "Coton doux, unisex, BLeu",             true);
    Vetements     tshirt     (6,  "Basic T-Shirt",             79.00f, 20, "100% cotton, Blanc",            false);
    Livre         book1      (7,  "Clean Code",               210.00f,  6, "By Robert C. Martin",   "Robert C. Martin");
    Livre         book2      (8,  "The Pragmatic Programmer", 195.00f,  4, "Classic software engineering",  "David Thomas");
    Meuble        desk       (9,  "Bureau assis-debout",           3200.00f,  2, "Bureau electrique a hauteur reglable");
    Parapharmacie vitamins   (10, "Vitamin C 1000mg",          55.00f, 30, "Complément pour le soutien immunitaire");

    std::vector<Produit*> catalogue = {
        &laptop, &headphones, &coffee, &milk,
        &jacket, &tshirt, &book1, &book2, &desk, &vitamins
    };

    // ── Accounts & session ────────────────────────────────────────────────────
    std::vector<Client*> clientsAccounts;
    Client* loggedClient = nullptr;

    // ── Cart & orders ─────────────────────────────────────────────────────────
    Panier panier;
    std::vector<Commande*> allCommandes;
    std::vector<Commande>  historique;

    // ── Raylib init ───────────────────────────────────────────────────────────
    const int SW = 1280, SH = 720;
    InitWindow(SW, SH, "Mozart! Votre E-boutique Ici");
    SetTargetFPS(60);

    // ── Font loading ──────────────────────────────────────────────────────────
    // Put your font files in assets/fonts/ — adjust filenames to match yours
    Font fontMain  = LoadFontEx("../assets/fonts/Chillax-Medium.ttf",   32, nullptr, 0);
    Font fontBold  = LoadFontEx("../assets/fonts/Chillax-Bold.ttf",     32, nullptr, 0);
    Font fontLight = LoadFontEx("../assets/fonts/Chillax-Medium.ttf",    32, nullptr, 0);

    // Fallback: if any font fails to load, GetFontDefault() is used automatically
    // by Raylib — so the app won't crash, it'll just use the default font

    // ── UI state ──────────────────────────────────────────────────────────────
    Screen     currentScreen  = AUTH;
    AuthMode   authMode       = SIGNUP;
    ProfileTab profileTab     = INFOS;
    int        activeField    = 0;
    int        catalogueScroll = 0;
    int        selectedProduct = -1;

    // ── Auth buffers ──────────────────────────────────────────────────────────
    char inputPrenom  [64] = "";
    char inputNom     [64] = "";
    char inputEmail   [64] = "";
    char inputPassword[64] = "";
    std::string authMessage = "";

    // ── Cached display strings ────────────────────────────────────────────────
    std::string panierStr = panierToString(panier);
    std::string recoStr   = "";

    // ── Tab layout ────────────────────────────────────────────────────────────
    const int TAB_Y = 12, TAB_H = 36, TAB_W = 170;
    Rectangle tabs[4] = {
        {  10.0f, (float)TAB_Y, (float)TAB_W, (float)TAB_H },
        { 700.0f, (float)TAB_Y, (float)TAB_W, (float)TAB_H },
        { 880.0f, (float)TAB_Y, (float)TAB_W, (float)TAB_H },
        {1060.0f, (float)TAB_Y, (float)TAB_W, (float)TAB_H },
    };
    const char* tabLabels[4] = { "Catalogue", "Panier", "Profile", "Parametres" };

    // Profile sub-tabs
    Rectangle profileTabs[2] = {
        {  10.0f, 60.0f, 150.0f, 30.0f },
        { 168.0f, 60.0f, 150.0f, 30.0f },
    };
    //const char* profileTabLabels[2] = { "Infos", "Historique" };

    // ── Theme colors — 3 variants ─────────────────────────────────────────────
    // [0]=orange  [1]=blue  [2]=green
    Color ACCENTS[3]    = {{ 199,108, 35,255}, { 35,108,199,255}, { 35,199,108,255}};
    Color HIGHLIGHTS[3] = {{ 255,235,220,255}, {220,235,255,255}, {220,255,235,255}};
    Color BORDERS[3]    = {{ 210,190,180,255}, {180,190,210,255}, {180,210,190,255}};
    Color TABOFFS[3]    = {{ 225,215,210,255}, {210,215,225,255}, {210,225,215,255}};
    Color TEXTDIMS[3]   = {{ 140,125,120,255}, {120,125,140,255}, {120,140,125,255}};

    int   themeIndex = 0;   // 0=orange 1=blue 2=green — changed in Parametres

    // Fixed colors
    Color BG       = { 245, 245, 250, 255 };
    Color PANEL_BG = { 255, 255, 255, 255 };
    Color TEXT_MAIN= {  30,  30,  30, 255 };
    Color SUCCESS  = {  34, 139,  34, 255 };
    Color DANGER   = { 199,  40,  40, 255 };

    // ── Main loop ─────────────────────────────────────────────────────────────
    while (!WindowShouldClose()) {

        // Resolve theme colors once per frame
        Color ACCENT    = ACCENTS   [themeIndex];
        Color HIGHLIGHT = HIGHLIGHTS[themeIndex];
        Color BORDER    = BORDERS   [themeIndex];
        Color TAB_OFF   = TABOFFS   [themeIndex];
        Color TEXT_DIM  = TEXTDIMS  [themeIndex];

        Vector2 mouse = GetMousePosition();

        // ══ AUTH input ════════════════════════════════════════════════════════
        if (currentScreen == AUTH) {
            Rectangle btnSignup = {  20.0f, 100.0f, 150.0f, 34.0f };
            Rectangle btnLogin  = { 180.0f, 100.0f, 150.0f, 34.0f };
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (CheckCollisionPointRec(mouse, btnSignup)) { authMode = SIGNUP; authMessage = ""; activeField = 0; }
                if (CheckCollisionPointRec(mouse, btnLogin))  { authMode = LOGIN;  authMessage = ""; activeField = 0; }
            }

            int numFields = (authMode == SIGNUP) ? 4 : 2;
            Rectangle fieldRects[4] = {
                { 20, 180, 400, 36 },
                { 20, 250, 400, 36 },
                { 20, 320, 400, 36 },
                { 20, 390, 400, 36 },
            };
            char* signupBuffers[4] = { inputPrenom, inputNom, inputEmail, inputPassword };
            char* loginBuffers [2] = { inputEmail, inputPassword };
            char** buffers = (authMode == SIGNUP) ? signupBuffers : (char**)loginBuffers;

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                for (int i = 0; i < numFields; i++)
                    if (CheckCollisionPointRec(mouse, fieldRects[i]))
                        activeField = i;
            if (activeField >= numFields) activeField = 0;

            int ch = GetCharPressed();
            while (ch > 0) {
                int len = strlen(buffers[activeField]);
                if (ch >= 32 && len < 63) {
                    buffers[activeField][len]     = (char)ch;
                    buffers[activeField][len + 1] = '\0';
                }
                ch = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE)) {
                int len = strlen(buffers[activeField]);
                if (len > 0) buffers[activeField][len - 1] = '\0';
            }
            if (IsKeyPressed(KEY_TAB))
                activeField = (activeField + 1) % numFields;

            Rectangle btnSubmit = { 20.0f, (float)(authMode == SIGNUP ? 450 : 390), 180.0f, 38.0f };
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, btnSubmit)) {
                if (authMode == SIGNUP) {
                    if (strlen(inputPassword) < 8) {
                        authMessage = "Mot de passe: 8 caracteres minimum.";
                    } else {
                        Client* newUser = new Client((int)clientsAccounts.size() + 1, inputNom, inputPrenom);
                        newUser->setEmail(inputEmail);
                        newUser->setPassword(inputPassword);
                        clientsAccounts.push_back(newUser);
                        authMessage = "Inscription validee! Vous pouvez vous connecter.";
                        authMode = LOGIN;
                        memset(inputPrenom, 0, 64); memset(inputNom, 0, 64);
                        memset(inputEmail,  0, 64); memset(inputPassword, 0, 64);
                        activeField = 0;
                    }
                } else {
                    bool found = false;
                    for (auto* c : clientsAccounts) {
                        if (std::string(inputEmail) == c->getEmail() &&
                            std::string(inputPassword) == c->getPassword()) {
                            loggedClient = c;
                            found = true;
                            break;
                        }
                    }
                    if (found) {
                        authMessage = "";
                        currentScreen = CATALOGUE;
                        memset(inputEmail, 0, 64); memset(inputPassword, 0, 64);
                    } else {
                        authMessage = "Email ou mot de passe invalide.";
                    }
                }
            }
        }

        // ══ Logged-in input ═══════════════════════════════════════════════════
        if (currentScreen != AUTH) {

            // Main tab switching
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                for (int i = 0; i < 4; i++)
                    if (CheckCollisionPointRec(mouse, tabs[i]))
                        currentScreen = (Screen)(i + 1);
            }

            // ── CATALOGUE input ───────────────────────────────────────────────
            if (currentScreen == CATALOGUE) {
                // Scroll
                catalogueScroll -= (int)GetMouseWheelMove() * 30;
                catalogueScroll  = std::max(0, catalogueScroll);

                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    // Catalogue list — left 3/4 of screen
                    int listW = (int)(SW * 0.72f);
                    int listY = 56;
                    for (int i = 0; i < (int)catalogue.size(); i++) {
                        int y = listY + i * 90 - catalogueScroll;

                        // "Ajouter" button — right side of each row
                        Rectangle btnAdd = { (float)(listW - 130), (float)(y + 28), 120.0f, 32.0f };
                        if (CheckCollisionPointRec(mouse, btnAdd)) {
                            selectedProduct = i;
                            panier.ajouterProduit(catalogue[i], 1);
                            panierStr = panierToString(panier);
                            // Refresh reco
                            Produit* rec = recommanderProduit(catalogue, panier, historique);
                            recoStr = rec
                                ? ("Recommande pour " + loggedClient->getSurname()
                                   + " " + loggedClient->getName() + ":\n\n"
                                   + produitToString(*rec))
                                : "Aucune recommandation disponible.";
                        }
                    }
                }
            }

            // ── PANIER input ──────────────────────────────────────────────────
            if (currentScreen == PANIER && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Rectangle btnValider = { SW - 230.0f, SH - 60.0f, 210.0f, 40.0f };
                if (CheckCollisionPointRec(mouse, btnValider) && !panier.getProduits().empty()) {
                    static int cmdId = 2000;
                    Commande* newCmd = new Commande(cmdId++, panier);
                    allCommandes.push_back(newCmd);
                    loggedClient->ajouterCommande(newCmd);
                    historique.push_back(*newCmd);
                    panier.vider();
                    panierStr = panierToString(panier);
                    currentScreen = PROFILE;
                    profileTab    = HISTORIQUE;
                }
            }

            // ── PROFILE sub-tab input ─────────────────────────────────────────
            if (currentScreen == PROFILE && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                // sub-tab switching
                if (CheckCollisionPointRec(mouse, profileTabs[0])) profileTab = INFOS;
                if (CheckCollisionPointRec(mouse, profileTabs[1])) profileTab = HISTORIQUE;

                // logout — checked independently, not nested inside profileTab check
                Rectangle btnLogout = { 30.0f, 316.0f, 180.0f, 40.0f };
                if (CheckCollisionPointRec(mouse, btnLogout)) {
                    loggedClient  = nullptr;
                    currentScreen = AUTH;
                    authMode      = SIGNUP;
                    authMessage   = "";
                    panier.vider();
                    panierStr = panierToString(panier);
                    recoStr   = "";
                }
            }

            // ── PARAMETRES input ──────────────────────────────────────────────
            if (currentScreen == PARAMETRES && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Rectangle btnOrange = { 40.0f,  140.0f, 160.0f, 50.0f };
                Rectangle btnBlue   = { 40.0f,  210.0f, 160.0f, 50.0f };
                Rectangle btnGreen  = { 40.0f,  280.0f, 160.0f, 50.0f };
                if (CheckCollisionPointRec(mouse, btnOrange)) themeIndex = 0;
                if (CheckCollisionPointRec(mouse, btnBlue))   themeIndex = 1;
                if (CheckCollisionPointRec(mouse, btnGreen))  themeIndex = 2;
            }
        }

        // ══ Draw ══════════════════════════════════════════════════════════════
        BeginDrawing();
        ClearBackground(BG);

        // ── AUTH ──────────────────────────────────────────────────────────────
        if (currentScreen == AUTH) {
            // Title
            const char* title = "MOZART";
            int tw = (int)MeasureTextEx(fontBold, title, 28, 1.0f).x;
            DrawTextEx(fontBold, title, { (float)(SW/2 - tw/2), 20 }, 28, 1.0f, ACCENT);
            const char* sub = "Bienvenue!";
            int sw2 = (int)MeasureTextEx(fontLight, sub, 16, 1.0f).x;
            DrawTextEx(fontLight, sub, { (float)(SW/2 - sw2/2), 58 }, 16, 1.0f, TEXT_DIM);

            // Mode toggle
            Rectangle btnSignup = {  20.0f, 100.0f, 150.0f, 34.0f };
            Rectangle btnLogin  = { 180.0f, 100.0f, 150.0f, 34.0f };
            DrawRectangleRec(btnSignup, authMode == SIGNUP ? ACCENT : TAB_OFF);
            DrawRectangleRec(btnLogin,  authMode == LOGIN  ? ACCENT : TAB_OFF);
            DrawTextEx(fontMain, "S'inscrire",   { 35,  111 }, 15, 1.0f, authMode == SIGNUP ? WHITE : TEXT_MAIN);
            DrawTextEx(fontMain, "Se connecter", { 195, 111 }, 15, 1.0f, authMode == LOGIN  ? WHITE : TEXT_MAIN);

            int numFields = (authMode == SIGNUP) ? 4 : 2;
            const char* signupLabels[4] = { "Prenom", "Nom", "Email", "Mot de passe" };
            const char* loginLabels [2] = { "Email", "Mot de passe" };
            const char** labels = (authMode == SIGNUP) ? signupLabels : loginLabels;
            char* signupBuffers[4] = { inputPrenom, inputNom, inputEmail, inputPassword };
            char* loginBuffers [2] = { inputEmail, inputPassword };
            char** buffers = (authMode == SIGNUP) ? signupBuffers : (char**)loginBuffers;
            Rectangle fieldRects[4] = {
                { 20, 180, 400, 36 }, { 20, 250, 400, 36 },
                { 20, 320, 400, 36 }, { 20, 390, 400, 36 },
            };
            bool isPasswordMask[4]  = { false, false, false, true };
            bool loginIsPassword[2] = { false, true };
            bool* passwordMask = (authMode == SIGNUP) ? isPasswordMask : loginIsPassword;

            for (int i = 0; i < numFields; i++) {
                DrawTextEx(fontLight, labels[i], { 20, fieldRects[i].y - 20 }, 14, 1.0f, TEXT_DIM);
                DrawRectangleRec(fieldRects[i], PANEL_BG);
                DrawRectangleLinesEx(fieldRects[i], activeField == i ? 2 : 1,
                                     activeField == i ? ACCENT : BORDER);
                std::string displayed = passwordMask[i]
                    ? std::string(strlen(buffers[i]), '*')
                    : std::string(buffers[i]);
                DrawTextEx(fontMain, displayed.c_str(), { 30, fieldRects[i].y + 10 }, 16, 1.0f, TEXT_MAIN);
                if (activeField == i && (int)(GetTime() * 2) % 2 == 0) {
                    float cx = 30 + MeasureTextEx(fontMain, displayed.c_str(), 16, 1.0f).x;
                    DrawTextEx(fontMain, "|", { cx, fieldRects[i].y + 10 }, 16, 1.0f, ACCENT);
                }
            }

            int submitY = (authMode == SIGNUP) ? 450 : 390;
            Rectangle btnSubmit = { 20.0f, (float)submitY, 180.0f, 38.0f };
            DrawRectangleRec(btnSubmit, ACCENT);
            DrawTextEx(fontBold, authMode == SIGNUP ? "S'inscrire" : "Se connecter",
                       { 30, (float)(submitY + 11) }, 16, 1.0f, WHITE);

            if (!authMessage.empty()) {
                bool isError = authMessage.find("invalide") != std::string::npos
                            || authMessage.find("minimum") != std::string::npos;
                DrawTextEx(fontMain, authMessage.c_str(), { 20, (float)(submitY + 52) },
                           14, 1.0f, isError ? DANGER : SUCCESS);
            }
        }

        // ── Logged-in chrome (tabs + logout + welcome) ────────────────────────
        if (currentScreen != AUTH) {

            if (loggedClient) {
                std::string welcome = "Bonjour, " + loggedClient->getSurname() + "!";
                float wx = SW - 120 - MeasureTextEx(fontMain, welcome.c_str(), 14, 1.0f).x - 10;
                DrawTextEx(fontMain, welcome.c_str(), { wx, (float)(TAB_Y + 11) }, 14, 1.0f, TEXT_DIM);
            }

            for (int i = 0; i < 4; i++) {
                bool active = (currentScreen == (Screen)(i + 1));
                DrawRectangleRec(tabs[i], active ? ACCENT : TAB_OFF);
                DrawRectangleLinesEx(tabs[i], 1, active ? ACCENT : BORDER);
                float tw2 = MeasureTextEx(fontBold, tabLabels[i], 15, 1.0f).x;
                DrawTextEx(active ? fontBold : fontMain, tabLabels[i],
                           { tabs[i].x + (tabs[i].width  - tw2) / 2.0f,
                             tabs[i].y + (tabs[i].height - 15)  / 2.0f },
                           15, 1.0f, active ? WHITE : TEXT_MAIN);
            }

            // ── CATALOGUE ─────────────────────────────────────────────────────
            if (currentScreen == CATALOGUE) {
                // Layout: left list + right reco panel
                int listW   = (int)(SW * 0.72f);   // ~920px
                int recoX   = listW + 10;
                int recoW   = SW - recoX - 10;
                int listY   = 56;
                int contentH = SH - listY - 10;

                // Right panel — recommendation
                DrawRectangle(recoX, listY, recoW, contentH, PANEL_BG);
                DrawRectangleLines(recoX, listY, recoW, contentH, BORDER);
                DrawTextEx(fontBold, "Recommandation",
                           { (float)(recoX + 10), (float)(listY + 8) }, 15, 1.0f, ACCENT);
                DrawLine(recoX, listY + 28, recoX + recoW, listY + 28, BORDER);
                if (recoStr.empty()) {
                    DrawTextEx(fontLight, "Ajoutez un produit\npour une suggestion.",
                               { (float)(recoX + 10), (float)(listY + 38) }, 13, 1.0f, TEXT_DIM);
                } else {
                    DrawMultilineTextF(fontMain, recoStr, recoX + 10, listY + 38, 13, TEXT_MAIN);
                }

                // Left list
                DrawTextEx(fontLight, "Cliquez sur 'Ajouter' pour mettre au panier.",
                           { 10, (float)(listY - 16) }, 12, 1.0f, TEXT_DIM);

                BeginScissorMode(0, listY, listW, contentH);
                for (int i = 0; i < (int)catalogue.size(); i++) {
                    int y   = listY + i * 90 - catalogueScroll;
                    bool sel = (i == selectedProduct);

                    DrawRectangle(10, y, listW - 20, 86, sel ? HIGHLIGHT : PANEL_BG);
                    DrawRectangleLines(10, y, listW - 20, 86, sel ? ACCENT : BORDER);

                    // Category badge
                    std::string cat = catalogue[i]->getCategory();
                    float bw = MeasureTextEx(fontLight, cat.c_str(), 12, 1.0f).x + 10;
                    DrawRectangle(listW - (int)bw - 145, y + 8, (int)bw, 20, ACCENT);
                    DrawTextEx(fontLight, cat.c_str(),
                               { (float)(listW - (int)bw - 140), (float)(y + 11) }, 12, 1.0f, WHITE);

                    // Name + price
                    DrawTextEx(fontBold,  catalogue[i]->getNomProduit().c_str(), { 20, (float)(y + 8)  }, 17, 1.0f, TEXT_MAIN);
                    std::string priceStr = std::to_string((int)catalogue[i]->getPrix()) + " DH";
                    DrawTextEx(fontBold,  priceStr.c_str(),                      { 20, (float)(y + 32) }, 15, 1.0f, ACCENT);

                    // Stock + details
                    std::string stock = "Stock: " + std::to_string(catalogue[i]->getStock());
                    DrawTextEx(fontLight, stock.c_str(),                          { 20,  (float)(y + 56) }, 12, 1.0f, TEXT_DIM);
                    DrawTextEx(fontLight, catalogue[i]->getDetails().c_str(),     { 110, (float)(y + 56) }, 12, 1.0f, TEXT_DIM);

                    // "Ajouter" button
                    Rectangle btnAdd = { (float)(listW - 145), (float)(y + 27), 120.0f, 32.0f };
                    DrawRectangleRec(btnAdd, ACCENT);
                    DrawTextEx(fontBold, "+ Ajouter", { btnAdd.x + 12, btnAdd.y + 8 }, 14, 1.0f, WHITE);
                }
                EndScissorMode();

                if ((int)catalogue.size() * 90 > contentH)
                    DrawTextEx(fontLight, "molette pour defiler",
                               { (float)(listW - 170), (float)(SH - 18) }, 12, 1.0f, TEXT_DIM);
            }

            // ── PANIER ────────────────────────────────────────────────────────
            if (currentScreen == PANIER) {
                int contentY = DrawPanelF(fontBold, "Panier", 10, 56, SW - 20, SH - 66, PANEL_BG, ACCENT);
                DrawMultilineTextF(fontMain, panierStr, 20, contentY, 17, TEXT_MAIN);
                if (!panier.getProduits().empty()) {
                    Rectangle btnValider = { SW - 230.0f, SH - 60.0f, 210.0f, 40.0f };
                    DrawRectangleRec(btnValider, ACCENT);
                    DrawTextEx(fontBold, "Valider la commande",
                               { (float)(SW - 220), (float)(SH - 48) }, 15, 1.0f, WHITE);
                }
            }

            // ── PROFILE ───────────────────────────────────────────────────────
            if (currentScreen == PROFILE) {
                // Sub-tabs
                DrawMiniTab(fontMain, "Infos",       profileTabs[0], profileTab == INFOS,      ACCENT, TAB_OFF, BORDER);
                DrawMiniTab(fontMain, "Historique",  profileTabs[1], profileTab == HISTORIQUE,  ACCENT, TAB_OFF, BORDER);

                int contentY = 100;
                logoutBtnY = contentY + 10;

                if (profileTab == INFOS && loggedClient) {
                    DrawRectangle(10, contentY, SW - 20, SH - contentY - 10, PANEL_BG);
                    DrawRectangleLines(10, contentY, SW - 20, SH - contentY - 10, BORDER);
                    contentY += 16;

                    // Info rows
                    auto row = [&](const char* label, const std::string& value) {
                        DrawTextEx(fontLight, label,        { 30,  (float)contentY }, 14, 1.0f, TEXT_DIM);
                        DrawTextEx(fontBold,  value.c_str(),{ 200, (float)contentY }, 16, 1.0f, TEXT_MAIN);
                        DrawLine(30, contentY + 24, SW - 30, contentY + 24, BORDER);
                        contentY += 38;
                    };

                    row("Prenom",          loggedClient->getSurname());
                    row("Nom",             loggedClient->getName());
                    row("Email",           loggedClient->getEmail());
                    row("ID Client",       std::to_string(loggedClient->getIdClient()));
                    row("Commandes",       std::to_string(loggedClient->getHistoryCmd().size()));

                    contentY += 10; // a little breathing room
                    Rectangle btnLogout = { 30.0f, 316.0f, 180.0f, 40.0f };
                    DrawRectangleRec(btnLogout, DANGER);
                    DrawTextEx(fontBold, "Deconnexion", { 50.0f, 327.0f }, 16, 1.0f, WHITE);
                }

                if (profileTab == HISTORIQUE && loggedClient) {
                    DrawRectangle(10, contentY, SW - 20, SH - contentY - 10, PANEL_BG);
                    DrawRectangleLines(10, contentY, SW - 20, SH - contentY - 10, BORDER);
                    contentY += 16;

                    auto cmds = loggedClient->getHistoryCmd();
                    if (cmds.empty()) {
                        DrawTextEx(fontLight, "Aucune commande dans l'historique.",
                                   { 30, (float)contentY }, 15, 1.0f, TEXT_DIM);
                    } else {
                        for (auto* cmd : cmds) {
                            std::string cmdStr = commandeToString(*cmd);
                            contentY = DrawMultilineTextF(fontMain, cmdStr, 20, contentY, 15, TEXT_MAIN);
                            DrawLine(20, contentY + 4, SW - 20, contentY + 4, BORDER);
                            contentY += 14;
                        }
                    }
                }
            }

            // ── PARAMETRES ────────────────────────────────────────────────────
            if (currentScreen == PARAMETRES) {
                int contentY = DrawPanelF(fontBold, "Parametres", 10, 56, SW - 20, SH - 66, PANEL_BG, ACCENT);

                DrawTextEx(fontBold, "Couleur du theme", { 30, (float)contentY }, 18, 1.0f, TEXT_MAIN);
                contentY += 36;

                struct ThemeBtn { Rectangle rect; Color color; const char* label; };
                ThemeBtn themeBtns[3] = {
                    {{ 40, (float)contentY,       200, 46 }, ACCENTS[0], "Orange"},
                    {{ 40, (float)contentY + 60,  200, 46 }, ACCENTS[1], "Bleu"  },
                    {{ 40, (float)contentY + 120, 200, 46 }, ACCENTS[2], "Vert"  },
                };

                for (int i = 0; i < 3; i++) {
                    bool selected = (themeIndex == i);
                    DrawRectangleRec(themeBtns[i].rect, themeBtns[i].color);
                    if (selected) DrawRectangleLinesEx(themeBtns[i].rect, 3, WHITE);
                    DrawTextEx(fontBold, themeBtns[i].label,
                               { themeBtns[i].rect.x + 14, themeBtns[i].rect.y + 14 },
                               16, 1.0f, WHITE);
                    if (selected)
                        DrawTextEx(fontBold, "  <x>",
                                   { themeBtns[i].rect.x + 120, themeBtns[i].rect.y + 14 },
                                   16, 1.0f, WHITE);
                }
            }
        }

        EndDrawing();
    }

    // ── Cleanup ───────────────────────────────────────────────────────────────
    UnloadFont(fontMain);
    UnloadFont(fontBold);
    UnloadFont(fontLight);
    for (auto* c : clientsAccounts) delete c;
    for (auto* c : allCommandes)    delete c;

    CloseWindow();
    return 0;
}