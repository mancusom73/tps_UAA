#ifndef BAND_HOR_H
#define BAND_HOR_H
int VERIFICAR_BANDA_HORARIA();
int BANDA_VIGENTE( struct _bandas_ventas *bandas_sic );
int ABRE_ARCHIVOS_BANDAS( char *bandas_sic, char *band_art, char *band_clas );
void CIERRA_ARCHIVOS_BANDAS();
int ARTICULO_EN_BAND_ART( int cod_banda, struct _band_acu_art *art );
int BAND_ACU_ART( struct _band_acu_art *acu_art, int l );
int BANDAS_RECORRER_CLASIF( struct _clasif1 *clasif, struct _datos_clasif *datos );
int CLASIFICACION_EN_BANDA( int cod_banda, struct _band_acu_art *art );
#endif
