#ifndef FACREM_H
#define FACREM_H
extern int _VERIFICAR_EVENTOS_DEL_COMPROBANTE( int tipo, int *cantidad_pagos , int *cant_items);
int PEDIR_FECHAS_LIMITE();
void GO_REMITOS( int modo, int inicio );
int REMITO_EN_FILTRO();
void DESMARCAR_REMITOS();
void ACTUALIZA_ACU_DEPTOS();
void PASA_TOTALES( struct _tot tot );
#endif
