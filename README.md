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
 ### 2.1 Compilació i linkat.
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
 
 ### 2.2 Eines per a executables
- **`objdump`** dissassembly amb opció `-d`.
- **`ldd`** mostra les llibreries que necessita l'executable.
- **`nm`** Mostra els símbols de l'executable.
 
 ### 2.3 Eines per a mesurar
 Veure que aquestes eines acostumen a treure l'output per stderr cosa que permet redirigir la sortida.
 #### 2.3.1 Accounting
 - **`time`** Hi ha diferents versions, la que implementa cada shell i la GNU que es troba normalment a _/usr/bin/time_
   - `-a` append
   - `-f` modificar format de output
   - `-p` format posix 
- **`perf stat`** Mostra info treta d'alguns contadors hw. Eina molt interessant per a veure els fallos de cache, TLB, etc.
  - `-o`
  - `r` Repetir la comanda i fer mitjana
  - `d` Detailed, dona més detalls. És bo posar 3 d: `-d -d -d` 
 #### 2.3.2 Profiling
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
 - **`valgrind`**
   - No necessita flags de compilació.
   - Mostra els resultats en una UI prou intuitiva
   - **Usage:**
     - `valgrind --tool=callgrind ./prog [args]` Genera la informació.
     - `kcachegrind callgrind.out-pid` Ens permet veure'l.
     - `callgrind_annotate --auto=yes` També ens permet veure'l.
 - **Oprofile** Són un seguit d'eines que permeten usar el compts HW per coses com el nombre d'instruccions executades, Hits/Misses en els diferents nivells de memòria, Hits/Misses en pedicció.
   - No mostra les crides a funció.
   - Inclou info de llibreria i OS.
   - Pot incloure activitat d'altres processos.
   - **Usage:**
     - `ophelp` per veure els diferents contados HW que tenim.
     - `operf --event=<event>:<repeticions_per_sample> ./pi.O0.g` per capturar la info
     - `opreport -l` Veure la info.
     - `opanotate --source <prog>` ens permet veure la info a nivell de linea de codi.
     - `ocount` ens permet fer simplent acounting d'events.
 - **Perf** Fa també un contatge de comptadors HW.
   - No mostra el nombre de crides a cada funcions.
   - **Usage:**
     - `perf list` llista els possibles events.
     - `perf record -e <event> -F <events_per_sample> command [args]`
     - `perf report` Veure el report en una UI de termninal molt cheta.
     - `perf annotate` Com el opanotate.
 - **Pintools** És un seguit d'eines. La que tractiem en questió és *insmix*. Es útil perquè ens conta quines instruccions s'estan executant encada rutina i les conta. Va bé per entendre optimitzacions.
   - Usage:
     - `insmix_total command [args]` per fer el profiling. Veure el fitxer _insmix.out_ al acabar.
 - **PAPI** Llibreria que permet instrumentació manual del codi.
   - Per a mes info veure transpas pag 60 tema 2.  --> no hem tocat PAPI al lab.
 #### 2.3.3 Tracing
 - **`strace`** Crides a sistema
   - Usage:
     - `-c` Mostra el numero de crides a sistema
     - `-e trace=<crida>` Monitoritzar només un tipus de crida.
     - `-e read=3` Monitoritzar crides a sistema que llegeixen d'un canal en concret.
     - `-t` Afegir un timestamp.
 - **`ltrace`** Mostra les invocacions a llibreries dinàmiques. Mostra la funció, l'input i els paràmetres.
   - Funciona igual que `strace`.

Aquí unes preguntes útils que ens va fer en Dani a classe:

![imatge](https://user-images.githubusercontent.com/47460533/114274068-05627600-9a1d-11eb-9bdb-0417a59ca545.png)


## 3. Operacions de llarga latència.

Les operacions de llarga latència depen molt de l'arquitectura però normalment són:
 - Divisions i mòduls
 - Multiplicacions
 - Algunes Crides a sistema.
 - Càlculs repetits.
 - Ifs que generen moltes fallades de predicció.
 -  

### 3.1 Bithacks.
Canviar les operacions de llarga latència per jocs amb bits. Molts cops ja els fa el compilador.
 - **Potències de 2 i divisió entre potències de 2** Es pot canviar per un shift esquerra o dreta.
   - Ojo amb els negatius perquè és fa malament l'arrodoniment!!
     - Podem fer workarround amb la seguent expressió: `X/2^n = (X + (X>>32) & (Y-1)) >> n`
 - **Moduls entre potències de 2** `n % 2^(n) = n & ( 2^(n) - 1 )`
 - **Valor absolut** `abs = (n ^ (n>>31)) - (n>>31);`
 - **Màxim de dos enters** `max = n - ((n-m) & ((n-m)>>31));` o `max = n - ((n-m) & -(n<m));` Per evitar salts innecessaris i possibles fallades de predicció.
   - Cal valorar el cost del if vs el cost del bithack des d'un punt de vista de mitjana aritmàtica de costos. 
 - **Mirar que si els bits N i M estan actius** `((n & ((1<<N)|(1<<M))) != 0)` o `((n & ((1<<N)|(1<<M))) == ((1<<N)|(1<<M)))`
 - **Obtenir bit N** `(x>>N)&0x1`
 - **Setejar bit N** `x | (1<<N)`
 - **Posar el bit N a 0** ` x & (~(1<<N))`
 - **Aconseguir el bit N d'un vector d'enters** `(vector[N/32]>>(N%32))&0x1)
 - **És X potència de 2?** `((x & (x-1))==0) & (x!=0)`

### 3.2 Expressions aritmètiques
 - Tenir en compte que a vegades el compilador no sap que certs nombres són constants i podem fer la propagació manualment.
 - Canviar operacions de llarga latència per altres. `a*3 = a + a + a`
   - Atenció que en el cas de la coma flotant podríem obtenir resultats diferents per la questió de la presició. Pot no importar-nos. 
 - Cal detectar calculs doblats (càlcul de resultats previament calculats).
 - Detectar codi que genera resultats no usats.
 - Càlculs linears a la variable d'inducció poden calcular-se sumant a la variable:
   - `for (i=0; i<N; i++){...  i*5 + 2 ...}`  --> `for(i=0, i5=2; i<N; i++, i5+=5){...  i5 ...}`
     - Molt donat en casos així: `for (i=0; i<N; i++){...  a[i] ...}` 
 - Reconeixement de patrons --> Podem aplicar _specialization_.
### 3.3 Memoization
Per a reduir la computació que es fa sempre amb les mateixes dades precalculem els resultats i els guardem en un vector/taula de hash.
 - nslookups
 - Funcions factorials
 - Trigonometria.
### 3.4 Specialization
Hi ha rutines que s'han fet genèriques/parametritzables però que poden ser més eficiens si les fem específiques.
  - El compilador pot fer-ho si veu que hi ha constants o inlining.
### 3.5 Buffering
La idea és reduir el nombre de crides a sistema que poden bloquejar l'execució. Hi ha un gran overhead al canviar el mode d'execució i canvi de context.
 - És interessant veure el temps que passem en system mode.
 - Analitzem el nombre de crides a sistema que tenim.
 - **SOLUCIÓ** Compactem les crides a sistema usant buffers. P.E. fer les escriptures en buffers més grans.

## 4. Control de fluxe
La idea vindria a ser ajudar al predictor de salts a encertar i/o reduir els salts en processadors segmentats per poder augmentar l'IPC.

Predicció de salts:
 - **Per Hardware:** Hi ha un autòmata que prediu els salts
 - **Per Software:** Podem fer us de macros per ajudar al predictor:
  ```
  #define likely(x)      __builtin_expect(!!(x), 1)
  #define unlikely(x)    __builtin_expect(!!(x), 0)
  ```
  ```
  if (unlikely(pmd_bad(*pmd))) { ...  }
  ```
Podem facilment estudiar el comportament dels branches d'un programa amb el `perf stat` o usant l'event _branches_: `perf record --event branches -F 25000 ./pi.g 10000 >/dev/null`

### 4.1 Tècniques de control de fluxe
L'objectiu és reduir els salts que realitza el programa.
#### 4.1.1 Inlinig
Substituir la crida pel codi de la funció. Ens estalviem moltes instruccions (còpia de paràmetres, enllaç dinàmic, etc).
**Qüestions a tenir en compte:**
 - Al augmentar el codi estem explotant pitjor la jerarquia de memòria.
 - Veure que fer unrolling podria fer que el compilador no ens afegís l'inlining desitjat.
 - Ens podem assegurar de que el processador farà inlining si el tenim en una macro.
 - Cal posar parèntesis entre els paràmetres d'una macro:
 ```
 #define right_shift_ok(_x,_y) ((_x) << (_y))
 #define right_shift_ko(_x,_y) (_x << _y)
 ```

#### 4.1.2 Code hoisting
Veure càlculs que poden fer-se fora del bucle i no cal calcular a cada iteració.

#### 4.1.3 Unrolling
Ens permet tenir més instruccions útils per iteració. Cosa que ajuda al hardware a explotar millor l'execució fora d'ordre.
**Qüestions a tenir en compte:**
 - Els compiladors poden fer unrolling però són molt conservadors. GCC ho fa al nivell O3.
 - Pot caldre afegir un epílog al final del bucle al que s'ha fet unrolling.
 - Fer molt d'unrolling pot fer que el compilador no faci inlinig per no augmentar els fallos en cache d'instruccions.
 - Es poden acabar els registres i guardar variables a la pila no millora la performance.
 - Cal anar en compte no estiguem creant dependències de dades. Podem fer ús d'acumuladors per a que no sigui així:
   ```
   Aquest codi desenrollat té dependències de dades:
   
   for(i=0; i<3000; i+=3) {
    acum += b[i]   + c[i];
    acum += b[i+1] + c[i+1];
    acum += b[i+2] + c[i+2];
   }
   
   Cal arreglar:
   
   for(i=0; i<3000; i+=3) {
    acum0 += b[i]   + c[i];
    acum1 += b``[i+1] + c[i+1];
    acum2 += b[i+2] + c[i+2];
   }
   acum = acum0 + acum1 + acum2;
   ```
  #### 4.1.4 Loop Collapsing
  Convertir dos bucles imbrincats en un de sol:
  - Codi original:
  ```
  int a[100][300];
  for (i = 0; i < 300; i++)
   for (j = 0; j < 100; j++)
     a[j][i] = 0;
  ```
  - Codi millorat:
  ```
  int *p = &a[0][0];
   for (i = 0; i < 300*100; i++)
     *p++ = 0;
  ``` 
 **Qüestions a tenir en compte:**
  - Els compiladors no ho acostumen a fer
 
 #### 4.1.5 Loop Fusion
 Ajuntar dos bucles que poden tenir un comportament semblant i sense dependències. És una optimització que normalment no aplica el compilador.
 
 #### 4.1.6 Ordre d'evaluació de la condició
 Aprofitar la _Lazy Evaluation_ de C. El compilador no pot aplicar aquesta optimització. Podem considerar els costs de les diferents condicions per ordenar-les correctament intentant impedir que s'hagin de comprovar les més costoses.
 
 #### 4.1.7 Eliminar els salts de difícil predicció.
 Podem fer-ho per mitjà de bithacks.
 
 ## 5. Optimitzacions conscients de la memòria.
 Eines d'anàlisi
  - **`/proc/cpuinfo`**
  - **`/proc/meminfo`**
  - **`lscpu`**
  - _Perf_ i _Operf_
  - **Valgrind:** Ens pot donar una visió acurada de la gestió del HEAP.
  ```
  Per grabar:
  > valgrind --tool=massif prog
  
  Per veure:
  > ms_print massif.out.PID
  ```
  
  ### 5.1 Tècniques d'explotació de la jerarquia de memòria.
  #### 5.1.1 Evitar l'alisaing
  Evitar que el compilador forci accessos a memoria innecessaris per tal d'evitar el possible aliasing. Per exemple, en el codi següent, el compilador no sap si x i y estaran sempre apuntant a la mateixa dada i per tant ha d'accedir a memòria a cada iteració:
  ```
  void multiply(int *x, int *y, int n){ 
   int i;
   for (i=0; i<n; i++)
    *x = *x + *y;
  }
  ```
  Cosa que pot arreglar-se perque el valor d'x i y es guardi sempre en registres:
  ```
  void multiply(int *x, int *y, int n){ 
   int i, tmp_x = *x, tmp_y = *y;
   for (i=0; i<n; i++)
    tmp_x = tmp_x + tmp_y;
   *x = tmp_x;
  }
  ```
#### 5.1.2 Alineament de dades.
Tenir les dades correctament alineades a memòria ens permet aprofitar molt més els accessos i per tant tenir una molt millor explotació de l'ample de banda.
 - **Alineament estàtic:** Generalment la memòria es troba alineada. El complador inclou _padding_ per a tenir en compte aquesta questió. Podem dir al compilador que no afegeixi padding als structs amb `__atribute__((__packed__))` o `#pragma pack(1)`
 - **Alineament dinàmic:** Els mallocs alineen a int. Podem alinear a altres tamanys amb `int posix_memalign(void **ptr, sizet align, sizetsize);`
 
 #### 5.1.3 Aprofitar l'ample de banda
  - Eliminar paddings estúpids
  - Usant tamanys de dades petits. Ser conseqüent amb els tamanys de les dades.
  - Fer bufferings per no llegir i escriure dades a memoria una a una sinó aprofitant l'ample de banda.
  - Reorganitzar els structs.

#### 5.1.4 Explotar la localitat de les dades
- Accedir a les matrius per files i no per columnes
- Fusió de vectors per no tenir problemes de cache
- Fer tractament de matrius en blocs per poder fer blocking.
   
