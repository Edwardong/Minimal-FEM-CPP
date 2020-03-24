# Minimal-FEM-CPP
[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

This is a final project of CS180, an undergraduate-level Computer Graphics course taught by a talented genius Professor [Lingqi Yan](https://sites.cs.ucsb.edu/~lingqi/) at University of California, Santa Barbara.

This project has a python sibling: [minimal-FEM-simulation](https://github.com/Ara-yjx/minimal-FEM-simulation), which served as a prototype for validating the correctness of algorithm implementation, mainly developed by [Petry Ye](https://github.com/Ara-yjx), an incredibly smart student major in CCS computing at UCSB.

## Usage
### Code Compilation
* In the root directory where a Makefile exists, compile with 
```Bash
$ make all.
```
* Run the program with 
```Bash
$ ./main <path_to_obj_file>.
```
* For example, 
```Bash
$ ./main models/unit_cube.obj.
```
* There are currently two sample obj files, one unit cube and one rounded star, in models/ directory. After running the program, the output obj files will be in the /out folder.
* You can clean the project with 
```Bash
$ make clean.
```
There are several parameters defined in [formula.cpp](formula.cpp) and [main.cpp](main.cpp). For example:
```cpp
#define N_STEPS 3000 // number of iterations
#define stepPerFrame 20
```
These two parameters can significantly impact the running time of the program. You are free to change the parameters and play with them. However, we strongly recommend that you don't change
```cpp
#define DELTA_TIME 0.2
#define MASS 20.0
```
defined in [formula.cpp](formula.cpp). We made a lot of effort trying to find these parameters that fit the models we use under [models](models/).

## Reference:
* Sifakis, Eftychios D. “[FEM Simulation of 3D Deformable Solids](http://barbic.usc.edu/femdefo/sifakis-courseNotes-TheoryAndDiscretization.pdf): A practitioner’s guide to theory, discretization and model reduction. Part One: The classical FEM method and discretization methodology.”

* Si, Hang. “[TetGen, a Delaunay-Based Quality Tetrahedral Mesh Generator](https://dl.acm.org/doi/10.1145/2629697)”, ACM Transactions on Mathematical Software, February 2015


## NOTICE regarding academic integrity: 
This project is under **GPL 3.0** License. 

Simply copying & pasting and using it as your own course project would cause **serious** issues of academic integrity. 

