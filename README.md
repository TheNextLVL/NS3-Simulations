# NS3 - Simulation Examples :smile:

## Estado
- [x] Creaci�n de la clase GNUPlot
- [x] Creaci�n de la clase Simulation
- [x] Correci�n de errores de sintaxis
- [x] Correci�n de errores de ejecuci�n
- [ ] Realizaci�n del primer test
- [ ] M�todos para generaci�n de distintos tipos de tr�fico

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
