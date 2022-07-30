#include "params.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>

int getNumParam ( char* params, char* param, double* res ) {
  int r = 0;
  char* s = NULL;

  r = getStrParam( params, param, &s );
  if (!r) {
    *res = atof(s);
    //fprintf(stdout,"getNumParam(%s,%s) = %f\n",params,param,*res);
    if (s) free(s);
  }
  return r;
  
}

// En caso que la funcion finalice correctamente (devolviendo 0),
// se deberá hacer el free del buffer devuelto, una vez que ya 
// no sea necesario.
int getStrParam ( char* params, char* param, char** res ) {
  char* buf = NULL, *s = NULL;
  int n = 0;
  assert(params && param && res);

  n = strlen(params);
  buf = (char*)malloc(n+1);
  strncpy(buf,params,n+1);

  s = strstr(buf,param);
  if (s) {
    char* t = NULL;
    //fprintf(stdout,"parametro encontrado: [%s]\n",s);
    t = strchr(s,';');
    if ( t ) {
      t[0]='\0';
      //fprintf(stdout,"parametro encontrado: [%s]\n",s);
    } else {
      //último parámetro de la lista.
    }

    t = strchr(s,'=');
    if ( t ) {
      s = &t[1];
      //fprintf(stdout,"parametro encontrado: [%s]\n",s);
    } else {
      //parámetro mal definido.
      *res = NULL;
      if (buf) free(buf);
      return -1;
    }

    *res = (char*)malloc(strlen(s)+1);
    strncpy(*res,s,strlen(s)+1);
    if (buf) free(buf);
    return 0;
    
    
  } else {
    // el parametro no se encontro en las definiciones.
    *res = NULL;
  }

  if (buf) free(buf);
  return -1;

}
