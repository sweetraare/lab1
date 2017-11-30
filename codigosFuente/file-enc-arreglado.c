// FILE ENCRYPTION
 
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <openssl/des.h>
#include <sys/types.h>

void merror(char *msg) {
  perror(msg);
  exit(0);
}

char *Encriptar(char *Key, char *Msg, int size) {
  static char*    Res;
  int       n=0;
  DES_cblock      Key2;
  DES_key_schedule schedule;

  Res = ( char * ) malloc( size );

  memcpy( Key2, Key,8);
  DES_set_odd_parity( &Key2 );
  DES_set_key_checked( &Key2, &schedule );
  DES_ecb_encrypt((unsigned char *)Msg, (unsigned char *)Res,
         &schedule, DES_ENCRYPT);
   return (Res);
}

#define LINELEN 8

int main(int argc, char *argv[]) {
  int n, fdp, fdc;
  char key[]="password";
  unsigned char buf[LINELEN];

  if (argc < 3) {fprintf(stderr,"USO %s <fileP> <fileC>\n", argv[0]); exit(0);}

  if ((fdp = open (argv[1], O_RDONLY)) == -1)
    merror ("Open FDP");
  if ((fdc = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 00744)) == -1)
    merror ("Open FDC");

  char comando[100];
  sprintf(comando,"truncate -s %%8 %s",argv[1]); 
  //printf("%s",comando);
  system(comando); 

  while ((n = read(fdp, buf, LINELEN)) > 0) 
    write (fdc, Encriptar(key, buf, n), n);

  close (fdp);
  close (fdc);

  exit (0);
}
