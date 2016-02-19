#!/bin/bash


mpiexec -n 1 ./ejer3 < entrada
mpiexec -n 2 ./ejer3 < entrada
mpiexec -n 4 ./ejer3 < entrada
mpiexec -n 8 ./ejer3 < entrada

#mpiexec -n 1 ./ejer4 < entrada
#mpiexec -n 2 ./ejer4 < entrada
#mpiexec -n 4 ./ejer4 < entrada
#mpiexec -n 8 ./ejer4 < entrada