# Mozart v1.0

"Mozart" est officiellement ici!!!
Mozart est une application du e-commerce développée en C++ avec une GUI Raylib.

# Exigences

- MSYS2 (https://www.msys2.org/)
- Ouvrir le Terminal UCRT64 et écrire:

    pacman -S mingw-w64-ucrt-x86_64-gcc
    pacman -S mingw-w64-ucrt-x86_64-cmake

# Build et Exécution

    git clone --recurse-submodules https://github.com/JamelBoudPG/mini-amazon-store.git
    cd mini-amazon-store
    mkdir build && cd build
    cmake .. -G "MinGW Makefiles"
    cmake --build .
    ./Mozart.exe
## Remarque (Très Important)
    Cette application Importe et utilise la bibliothèque installé Raylib;
    La première Compilation (build) prend plus de temps que d'habitude (environ 2~3 minutes) pour installer et compiler Raylib.
    Les compilations suivantes sont instantanées.
