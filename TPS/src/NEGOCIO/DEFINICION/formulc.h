#ifndef FORMULC_H
#define FORMULC_H
/* FORMULC.h     as of 12/13/94 (v2.2 definitive)*/
#define UCHAR unsigned char
#define MAXPAR 3
/* maximum number of parameters */
typedef struct
{
    UCHAR *code;
    double *ctable;
} formu;

typedef double ( *Func )( double );
typedef double ( *Func2 )( double, double );
typedef double ( *Func3 )( double, double, double );
typedef double ( *Func0 )( void );

formu translate( const char *source, const char *args, int *length, int *error );
void destrf( formu );
void make_empty( formu );
int fnot_empty( formu );
const char *fget_error( void );

double fval_at( formu function );
void make_var( char var, double value );
double fval( formu function, char *args, ... );
double f_x_val( formu function, double x );

int fnew( char *name, Func f, int n_of_pars, int varying );
int read_table( int i, char *name, int *n_of_pars, int *varying );
int where_table( char *name );
int fdel( char *name );

double rnd( void );
void rnd_init( void );
/* If MY_RND is defined, rnd() and rnd_init() must be defined by the user.*/
/* Otherwise, formulc.c uses the random-number generator r250  */
/* (written by W. L. Maier, S. Kirkpatrick and E. Stoll) */

/* rnd_init is used by formulc.c only if STAND_ALONE is defined.   */
/* If FORMULC is compiled without STAND_ALONE, it is the user's    */
/* responsibility to initialize her random-number generator.       */

#endif
