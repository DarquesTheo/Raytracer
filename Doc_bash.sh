#!/bin/bash

# filepath: /home/nolan/delivery/semestre-4/OOP/B-OOP-400-TLS-4-1-raytracer-antonin.veau/run_doxygen.sh

# Vérifie si Doxygen est installé
if ! command -v doxygen &> /dev/null; then
    echo "Doxygen n'est pas installé. Veuillez l'installer avec : sudo apt install doxygen"
    exit 1
fi

# Exécute Doxygen avec le fichier Doxyfile
echo "Génération de la documentation avec Doxygen..."
doxygen Doxyfile

# Vérifie si la génération a réussi
if [ $? -ne 0 ]; then
    echo "Erreur lors de la génération de la documentation."
    exit 1
fi

# Chemin vers le fichier index.html généré
DOC_PATH="docs/html/index.html"

# Vérifie si le fichier index.html existe
if [ ! -f "$DOC_PATH" ]; then
    echo "Le fichier $DOC_PATH n'a pas été trouvé. Vérifiez la configuration de votre Doxyfile."
    exit 1
fi

# Ouvre la documentation dans le navigateur par défaut
echo "Ouverture de la documentation dans le navigateur..."
xdg-open "$DOC_PATH"