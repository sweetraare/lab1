# Makefile para Aplicacion C/S ECHO

SVRSRC= Servidor.c
SVROBJ= Servidor.o
CLTSRC= Cliente.c
CLTOBJ= Cliente.o


all:	Cliente Servidor file-enc file-dec file-enc1 file-dec1 file-enc-arreglado

Cliente:	$(CLTOBJ)
	cc -o Cliente $(CLTOBJ) -lcrypto

Servidor:	$(SVROBJ)
	cc -o Servidor $(SVROBJ) -lcrypto

file-enc:	file-enc.o
	cc -o file-enc file-enc.o -lcrypto

file-dec:	file-dec.o
	cc -o file-dec file-dec.o -lcrypto

file-enc1:	file-enc1.o
	cc -o file-enc1 file-enc1.o -lcrypto

file-dec1:	file-dec1.o
	cc -o file-dec1 file-dec1.o -lcrypto
file-enc-arreglado: file-enc-arreglado.o
	cc -o file-enc-arreglado file-enc-arreglado.o -lcrypto
