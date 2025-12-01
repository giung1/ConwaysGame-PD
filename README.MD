# Proyecto de simulación de Conway's Game of Life

Simulación del Juego de la Vida de Conway utilizando **PowerDEVS** para el modelo DEVS y **Scilab** para la visualización.

---

## Requisitos

- PowerDEVS instalado
- Scilab instalado
- Bash (para ejecutar el script `gen_pdm.sh`)

---

## Instrucciones de uso

### 1. Copiar el proyecto en la carpeta de PowerDEVS

Copiar esta carpeta del proyecto dentro del directorio donde está instalado **PowerDEVS**.  

- Se creará una carpeta `assets` que contiene el script de inicialización del PDM (`gen_pdm.sh`) y archivos necesarios para su ejecución
- El archivo `gameoflife_visualization.sce` dentro de `output/`

- Los módulos en las carpetas `VECTOR` y `DISCRETE`

---


### 2. Generar el archivo `.pdm`

Moverse a la carpeta `assets` y ejecutar el script de generación:

```bash
cd assets
bash gen_pdm.sh
```

Luego 
- En Power-Devs cargar el pdm generado en `examples/gameoflife/` con nombre `gameoflife_generated.pdm` 
- Ejecutar la simulación 
- Realizar la visualizacion de la simulación en Scilab usando el archivo `gameoflife_visualization.sce`
