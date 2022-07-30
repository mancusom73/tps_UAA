#ifndef PENDIENT_H
#define PENDIENT_H
void PEDIR_VENTA_PENDIENTE();
void TRAER_VENTA_PENDIENTE( long nro_venta );
void AGREGA_ARTICULO_AL_TICKET( long codigo, double precio_unit, double cantidad, int anulacion );
#endif
