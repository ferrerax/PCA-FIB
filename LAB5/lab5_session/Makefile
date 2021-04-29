.SUFFIXES: .3 .2 .g3 .g2 .0 .g0 .pg .pg3 .pg2 .pca .0 .pg3s .pgs .3ni .g3ni

CC=${CROSS_COMPILE}gcc

PROGS_PG3 = ${PROGS:=.pg3}
PROGS_PG = ${PROGS:=.pg}
PROGS_G0 = ${PROGS:=.g0}
PROGS_G2 = ${PROGS:=.g2}
PROGS_G3 = ${PROGS:=.g3}
PROGS_3 = ${PROGS:=.3}
PROGS_3ni = ${PROGS:=.3ni}
PROGS_2 = ${PROGS:=.2}
PROGS_PG2 = ${PROGS:=.pg2}
PROGS_0 = ${PROGS:=.0}
PROGS_PCA = ${PROGS:=.pca}
PROGS_ALL = ${PROGS} ${PROGS_PG} ${PROGS_3} ${PROGS_0} ${PROGS_G3} ${PROGS_G2} ${PROGS_G0} ${PROGS_PG3} ${PROGS_PCA} ${PROGS_3ni}

all:${PROGS_ALL}

zip: 
	make cleanall; dir=`basename $(PWD)`; cd .. ; zip -r $$dir $$dir

.c.3:
	$(CC) $(CFLAGS) $< -O3 $(LDLIBS) -o $@

.c.2:
	$(CC) $(CFLAGS) $< -O2 $(LDLIBS) -o $@
.c.0:
	$(CC) $(CFLAGS) $< -O0 $(LDLIBS) -o $@

.c.pgs:
	$(CC) $(CFLAGS) $< -pg -g -static $(LDLIBS) -o $@

.c.3ni:
	$(CC) $(CFLAGS) $< -O3 -fno-inline $(LDLIBS) -o $@

.c.pg:
	$(CC) $(CFLAGS) $< -pg -g $(LDLIBS) -o $@

.c.pg3ni:
	$(CC) $(CFLAGS) $< -pg -g -O3 -fno-inline $(LDLIBS) -o $@

.c.pg3:
	$(CC) $(CFLAGS) $< -pg -g -O3 $(LDLIBS) -o $@

.c.pg3s:
	$(CC) $(CFLAGS) $< -pg -g -O3 -static $(LDLIBS) -o $@

.c.pg2:
	$(CC) $(CFLAGS) $< -pg -g -O2 -static $(LDLIBS) -o $@

.c.g3:
	$(CC) $(CFLAGS) $< -g -O3 $(LDLIBS) -o $@

.c.g2:
	$(CC) $(CFLAGS) $< -g -O3 $(LDLIBS) -o $@

.c.g0:
	$(CC) $(CFLAGS) $< -g $(LDLIBS) -o $@

.c.pca:
	$(CC) $(CFLAGS) $< -O3 -march=native -ffloat-store -Wl,-s,-O1 -lm  -o $@

clean:
	$(RM) -f $(PROGS_ALL) core.* gmon.out 

cleanall:
	@ for i in */. ; do  cd $$i; ${MAKE} clean; cd .. ; done
