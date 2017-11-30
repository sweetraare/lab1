/* SERVIDOR ECHO
 *
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <openssl/des.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void merror(char *msg) {
  perror(msg);
  exit(1);
}

char * Decriptar( char *Key, char *Msg, int size) {
  static char*    Res;
  int       n=0;
  DES_cblock      Key2;
  DES_key_schedule schedule;

  Res = ( char * ) malloc( size );

  /* Preparar la llave */
  memcpy( Key2, Key,8);
  DES_set_odd_parity( &Key2 );
  DES_set_key_checked( &Key2, &schedule );

  /* Desencriptar */
  DES_cfb64_encrypt((unsigned char *)Msg, (unsigned char *)Res,
         size, &schedule, &Key2, &n, DES_DECRYPT );
  return (Res);
}

int main(int argc, char *argv[]) {
  int sd, nsd, portno, cltl;
  char buffer[256];
  struct sockaddr_in svr_addr, clt_addr;
  int i, n;
  char *decriptado;
  char key[]="password";

  if (argc != 2)
    merror("ERROR, no se ha ingresado el puerto");
  
  if ((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    merror("ERROR abrindo socket");

  bzero((char *) &svr_addr, sizeof(svr_addr));
  portno = atoi(argv[1]);
  svr_addr.sin_family = AF_INET;
  svr_addr.sin_addr.s_addr = INADDR_ANY;
  svr_addr.sin_port = htons(portno);

  if (bind(sd, (struct sockaddr *) &svr_addr, sizeof(svr_addr)) < 0) 
    merror("ERROR en binding");
  listen(sd,5);
  cltl = sizeof(clt_addr);
  if ((nsd = accept(sd, (struct sockaddr *) &clt_addr, &cltl)) < 0)
    merror("ERROR en accept");

  bzero(buffer,256);
  while (n = read(nsd, buffer, 255)) {
    if (n < 0)
      merror("ERROR leyendo del socket");
    decriptado = malloc(n);
    memcpy(decriptado, Decriptar(key, buffer, n), n);
//    fputs (buffer, stdout);
    for (i=0; i<n; i++) 
      printf ("%2x ", (unsigned char)buffer[i]);
    printf ("\n");

    fputs (decriptado, stdout);
    if ((n = write(nsd, decriptado, n)) < 0)
      merror("ERROR escribiendo en el socket");
    free (decriptado);
    bzero(buffer,256);
  }
  return 0; 
}
