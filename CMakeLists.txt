cmake_minimum_required(VERSION 3.16)
project(MiniAmazonStore)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── Download Raylib automatically ─────────────────────────────────────────────
include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG        5.5
)
FetchContent_MakeAvailable(raylib)

# ── Sources ───────────────────────────────────────────────────────────────────
set(SOURCES
    src/main.cpp
    src/produit.cpp
    src/panier.cpp
    src/Commande.cpp
    src/Client.cpp
    src/recommandation.cpp
)

# ── WIN32 hides the console window on Windows ─────────────────────────────────
if(WIN32)
    add_executable(Mozart WIN32 ${SOURCES})
else()
    add_executable(Mozart ${SOURCES})
endif()

# ── Link raylib ───────────────────────────────────────────────────────────────
target_link_libraries(Mozart PRIVATE raylib)

# ── Compiler flags ────────────────────────────────────────────────────────────
target_compile_options(Mozart PRIVATE -O1 -Wall)