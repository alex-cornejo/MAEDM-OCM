#!/bin/bash

# --------------------------------------------------
# Default build
# --------------------------------------------------

BUILD_TYPE="Release"
REBUILD_ONLY=false

# --------------------------------------------------
# Arguments -d for debug, -r for rebuild only
# --------------------------------------------------

while getopts "dr" opt; do
    case $opt in
        d)
            BUILD_TYPE="Debug"
            ;;
        r)
            REBUILD_ONLY=true
            ;;
        *)
            echo "Uso: ./build.sh [-d] [-r]"
            exit 1
            ;;
    esac
done

# --------------------------------------------------
# Compilation
# --------------------------------------------------

echo "Build type: $BUILD_TYPE"

if [ "$REBUILD_ONLY" = true ]; then
    # Si se usa -r, solo ejecutar make en la carpeta build
    if [ ! -d "build" ]; then
        echo "Error: La carpeta 'build' no existe. Ejecuta primero el script sin opciones o con -d."
        exit 1
    fi
    cd build/ || exit
    make -j$(nproc)
    cd ..
else
    # Si no se usa -r, compilar todo desde cero
    rm -rf build/
    mkdir build/
    cd build/ || exit
    cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE ..
    make -j$(nproc)
    cd ..
fi