# Teoria de PCA
## 1. Introducció
Els compiladors poden optimitzar el codi amb limitacions
 - El programa pot estar escrit de diverses maneres
 - El compilador és conservador: No farà modificacions que puguin afectar al resultat.

**Què podem fer?**
 - Facilitar la tasca del compilador
    - Propagació de constants, inlining, unrolling. En alguns casos aquestes millores ja les aplica el compilador
 - Aplicant optimitzacions de bloc com blocking, buffering o memoization
 - Detectar codi que podem accelerar en una FPGA.
 - Aplicar coneixement de l'arquitectura com caracteístiques concretes o explotar la jerarquia de memòria.
 
 ### 1.1 Metodologia
 1. **Obtenir codi font.**
 2. **Obtenir el golden output** Per comprovar la correctesa del codi
 3. **Analitzar el codi.**
    1. Hi ha tot un seguit de mètriques que podem usar:
        - Elapsed time: Temps que triga el programa
        - CPU time: temps de user + system
        - CPI, IPC
        - MIPS
        - TFLOPS
        - EDP: Per mesurar eficiència energètica.
    2. Hi ha un seguit de tècniques que podem aplicar.
        - **Accounting:** Mostrar es recursos necessaris per a executar el programa
        - **Profiling:** Instrumentar el codi per analizar a nivell de funció o de línea de codi o assembler.
        - **Tracing:** Obtenir informació extreta al llarg de l'execució del programa.
 4. **Millorar** 
    1. Aplicar una optimització considerada
    2. Assegurar la correctesa del codi
    3. Analitzar la performance un altre cop
 
 5. Decidir si es possible millorar més questions i tornar al punt 3.
 
 ## 2. Eines
 ## 2.1 Compilació i linkat.
 Flags del gcc per a PCA:
 - `-S` Genera codi assembler
 - `-o` output name
 - `-g` afegeix info de profiling
 - `-O0,O1,O2,O3`
 - `-march=type` Compilar per a X arquitectura --> cross compiling
 - `-v` Verbose
 
 Flags del linker (a posar també a la comanda gcc)
 - `-L` Afegeix una llibreria al path
 - `-lname` Linka la llibreria anomenada libname.a
 - `-static`
 
 ## 2.2 Eines per a executables
- **`objdump`** dissassembly amb opció `-d`.
- **`ldd`** mostra les llibreries que necessita l'executable.
- **`nm`** Mostra els símbols de l'executable.
 
 ## 2.3 Eines per a mesurar
 Veure que aquestes eines acostumen a treure l'output per stderr cosa que permet redirigir la sortida.
 ### 2.3.1 Accounting
 - **`time`** Hi ha diferents versions, la que implementa cada shell i la GNU que es troba normalment a _/usr/bin/time_
   - `-a` append
   - `-f` modificar format de output
   - `-p` format posix 
- **`perf stat`** Mostra info treta d'alguns contadors hw. Eina molt interessant per a veure els fallos de cache, TLB, etc.
  - `-o`
  - `r` Repetir la comanda i fer mitjana
  - `d` Detailed, dona més detalls. És bo posar 3 d: `-d -d -d` 
 ###2.3.2 Profiling
 - **`taskset`** Permet posat paràmetres a l'execució com ara el nombre de processadors a usar o quins en concret usar. Es fa a a través d'una mascara.
   - Usage: `taskset [options] mask command [args]`
 - **`gprof`** Standard Linux Profiler. Cal compilar amb el flag `-g` i `-pg`.
   - Mostra info profiling d'una execució que ja s'ha fet. **CAL EXECUTAR EL PROGRAMA ABANS**. Mostra el CPU time i el nombre de crides a funcions, a llibreries dinàmiques. No mostra info de codi. 
   - Genera un report en raw out i un callgraph .ps molt cheto que es pot veure amb el programa ghostview.
   - Té molta presició però pot haver biaix en el pes de cada subr/linea. 
   - Coses a tenir en compte del seu funcionament:
     - El flag `-pg` fa que el compilador inserti linies de codi a l'inici de cada func.
     - Afegeix interrupcions per fer fer sampling
     - Guardarà el resultat de l'execució del codi a un fitxer out.gmon
   - **Usage:**
     - Compilar amb `-g` i `-pg`
     - `-p` flat profile.
     - `-q` callgraph.
     - `-b` Treure la verbosity pesada.
     - `-l` Veure el profiling a nivell de linea de codi.
