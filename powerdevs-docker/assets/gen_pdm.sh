#!/bin/bash

echo "Verificando dependencias..."

# Comprobamos si tkinter se puede importar en python3
python3 -c "import tkinter" 2>/dev/null

# $? guarda el resultado del comando anterior (0 = éxito, 1 = error)
if [ $? -ne 0 ]; then    
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        sudo apt-get update
        sudo apt-get install -y python3-tk
    else
        echo "No se puede instalar tkinter automáticamente. Instala 'python3-tk' manualmente."
        exit 1
    fi
fi

python3 script_pdm_gen.py

