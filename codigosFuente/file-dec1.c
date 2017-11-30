/*RYPTION
 */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <openssl/des.h>
#include <sys/types.h>

void merror(char *msg) {
  perror(msg);
  exit(0);
}

char *Decriptar(char *Key, char *Msg, int size) {
  static char*    Res;
  int       n=0;
  DES_cblock      Key2;
  DES_key_schedule schedule;

  Res = ( char * ) malloc( size );

  memcpy( Key2, Key,8);
  DES_set_odd_parity( &Key2 );
  DES_set_key_checked( &Key2, &schedule );
  DES_cfb64_encrypt((unsigned char *)Msg, (unsigned char *)Res,
         size, &schedule, &Key2, &n, DES_DECRYPT );
   return (Res);
}

#define LINELEN 8

int main(int argc, char *argv[]) {
  int n, fdp, fdc;
  char key[]="password";
  unsigned char buf[LINELEN];

  if (argc < 3) {fprintf(stderr,"USO %s <fileC> <fileP>\n", argv[0]); exit(0);}

  if ((fdc = open (argv[1], O_RDONLY)) == -1)
    merror ("Open FDP");
  if ((fdp = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 00744)) == -1)
    merror ("Open FDC");

  while ((n = read(fdc, buf, LINELEN)) > 0) 
    write (fdp, Decriptar(key, buf, n), n);

  close (fdp);
  close (fdc);

  exit (0);
}
