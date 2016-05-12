# NS3 - Simulation Examples :smile:

## Estado
- [x] Creación de la clase GNUPlot
- [x] Creación de la clase Simulation
- [x] Correción de errores de sintaxis
- [x] Correción de errores de ejecución
- [ ] Realización del primer test
- [ ] Métodos para generación de distintos tipos de tráfico

## Uso
Sincronizar este repositorio dentro de la carpeta scratch del simulador NS3
```sh
cd <path>/ns-3.24.1/scratch
git init .
git remote add -t \* -f origin <https://github.com/nextLVL/NS3-Simulations.git
git checkout master
```

**Antes de compilar y ejecutar, quitar la carpeta ejemplos de la carpeta scratch. Por ejemplo mover al escritorio.**

Compilar y ejecuar con:
```sh
./waf
./waf --run s3
```
