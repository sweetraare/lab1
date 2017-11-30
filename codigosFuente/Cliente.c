/*
CLIENTE ECHO
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <openssl/des.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void merror(char *msg) {
  perror(msg);
  exit(1);
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
  DES_cfb64_encrypt((unsigned char *)Msg, (unsigned char *)Res,
         size, &schedule, &Key2, &n, DES_ENCRYPT );
  return (Res);
}

int main(int argc, char *argv[]) {
  int sd, portno, n, i;
  char key[]="password";
  struct sockaddr_in svr_addr;
  struct hostent *svr;
  char buffer[256];
  unsigned char *decriptado;
  unsigned char *encriptado;

  if (argc < 3) {fprintf(stderr,"USO %s host puerto\n", argv[0]); exit(0);}

  portno = atoi(argv[2]);
  if ((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    merror("ERROR al abrir socket");

  if ((svr = gethostbyname(argv[1])) == NULL)
    merror("ERROR, no existe el host");

  bzero((char *) &svr_addr, sizeof(svr_addr));
  svr_addr.sin_family = AF_INET;
  bcopy((char *)svr->h_addr, (char *)&svr_addr.sin_addr.s_addr, svr->h_length);
  svr_addr.sin_port = htons(portno);

  if (connect(sd,(struct sockaddr *) &svr_addr,sizeof(svr_addr)) < 0) 
    merror("ERROR conectando");


  bzero(buffer,256);
  while (fgets (buffer, sizeof(buffer), stdin)) {
    encriptado=malloc(strlen(buffer));
    memcpy(encriptado,Encriptar(key, buffer, strlen(buffer)), strlen(buffer));
//    printf("Texto cifrado\t : %s \n",encriptado);
//printf ("ENC: %d, %d\n", sizeof(encriptado), strlen(encriptado));
    for (i=0; i<strlen(buffer); i++) 
      printf ("%2x ", (unsigned char)encriptado[i]);
    printf ("\n");
  
    if ((n = write(sd, encriptado, strlen(buffer))) < 0)
      merror("ERROR escribiendo en el socket");
    bzero(buffer, 256);
    if ((n = read(sd, buffer, 255)) < 0)
      merror("ERROR leyendo del socket");
    fputs (buffer, stdout);
    free (encriptado);
    bzero(buffer,256);
  }
  if (close(sd) < 0)
    merror("ERROR cerrando el socket");
  exit (0);
}
