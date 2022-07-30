#pragma pack (push,1) 
#ifndef TYPEACTI_H
#define TYPEACTI_H
/////////////////////////////////////////////////////////////////////
// "Flags" Constants (tipos acciones para FLAGS)
/////////////////////////////////////////////////////////////////////

//FAC,NC,TF,REM,GUIA
#define T_FLAGS_COMP_VTAS_AGREGAR   1
#define T_FLAGS_COMP_VTAS_BORRAR    2

//RAM_PRECIO_CREDITO
#define T_FLAGS_CRED_VTAS_AGREGAR   5
#define T_FLAGS_CRED_VTAS_BORRAR    6

//PEDIR CODIGO DE BARRAS O CODIGO INTERNO
#define T_FLAGS_CODE_VTAS_AGREGAR   9
#define T_FLAGS_CODE_VTAS_BORRAR    10

//TICKET EN ESPERA LOCAL
#define T_FLAGS_TKE_VTAS_AGREGAR    13
#define T_FLAGS_TKE_VTAS_BORRAR     14

#define T_FLAGS_DESCUENTO_AGREGAR   17
#define T_FLAGS_DESCUENTO_BORRAR    18

#define T_FLAGS_DESCUENTO_1_AGREGAR 21
#define T_FLAGS_DESCUENTO_2_AGREGAR 22
#define T_FLAGS_DESCUENTO_3_AGREGAR 23
#define T_FLAGS_DESCUENTO_4_AGREGAR 24
#define T_FLAGS_DESCUENTO_5_AGREGAR 25
#define T_FLAGS_DESCUENTO_1_BORRAR  26
#define T_FLAGS_DESCUENTO_2_BORRAR  27
#define T_FLAGS_DESCUENTO_3_BORRAR  28
#define T_FLAGS_DESCUENTO_4_BORRAR  29
#define T_FLAGS_DESCUENTO_5_BORRAR  30
#define T_FLAGS_DEVOLUCION_AGREGAR  31
#define T_FLAGS_DEVOLUCION_BORRAR   32
#define T_FLAGS_CAMBIO_AGREGAR      33
#define T_FLAGS_CAMBIO_BORRAR       34
#define T_FLAGS_BIEN_USO_AGREGAR    35
#define T_FLAGS_BIEN_USO_BORRAR     36
#define T_FLAGS_TIPO_DEV_AGREGAR    37
#define T_FLAGS_TIPO_DEV_BORRAR     38
#define T_FLAGS_MODO_ENT_AGREGAR    39
#define T_FLAGS_MODO_ENT_BORRAR     40
#define T_FLAGS_MODO_AGREGAR        41
#define T_FLAGS_MODO_BORRAR         42
#define T_FLAGS_ERROR_RED_AGREGAR   43
#define T_FLAGS_ERROR_RED_BORRAR    44
#define T_FLAGS_MEDIO_EXTR_AGREGAR  45
#define T_FLAGS_MEDIO_EXTR_BORRAR   46

//TICKET EN ESPERA REMOTO
#define T_FLAGS_TKE_REM_VTAS_AGREGAR    47
#define T_FLAGS_TKE_REM_VTAS_BORRAR     48

//MONEDERO ELECTRONICO
#define T_FLAGS_MONEDERO_BORRAR         49
#define T_FLAGS_MONEDERO_AGREGAR        50

#define T_FLAGS_DEV_ITEM_AGREGAR        51
#define T_FLAGS_DEV_ITEM_BORRAR         52

/////////////////////////////////////////////////////////////////////
// "Datos" Constants (tipos acciones para DATOS)
/////////////////////////////////////////////////////////////////////

#define T_DATOS_CLIENTE_AGREGAR     100
#define T_DATOS_CLIENTE_BORRAR      101

#define T_DATOS_TARJETA_AGREGAR     104
#define T_DATOS_TARJETA_BORRAR      105

#define T_DATOS_VENDEDOR_AGREGAR    107
#define T_DATOS_VENDEDOR_BORRAR     108

//AFINIDAD
#define T_DATOS_AFINIDAD_AGREGAR    109
#define T_DATOS_AFINIDAD_BORRAR     110

#define T_DESACTIVA_CUADRO_PETICION 111
#define T_NOVIOS                    112
#define T_CLIENTE_AFINIDAD          113
#define T_NRO_DOCUMENTO_CLIENTE     114
#define T_NRO_COMPROBANTE           115
#define T_FECHA_INICIAL             116
#define T_FECHA_FINAL               117
#define T_FLAGS_PRESENT_AGREGAR     118
#define T_FLAGS_PRESENT_BORRAR      119

//MUTUAL
#define T_DATOS_MUTUAL_AGREGAR         120
#define T_DATOS_MUTUAL_BORRAR          121

#define T_DATOS_DET_LIST_AGREGAR       122
#define T_DATOS_DET_LIST_BORRAR        123

//PERFIL
#define T_DATOS_PERFIL_AGREGAR         124
#define T_DATOS_PERFIL_BORRAR          125

//MOZO
#define T_DATOS_MOZO_AGREGAR           126
#define T_DATOS_MOZO_BORRAR            127

//INTERES
#define T_DATOS_INTERES_AGREGAR        128
#define T_DATOS_INTERES_BORRAR         129

//PANTALLA DE PAGO
#define T_DATOS_DISC_IVA_AGREGAR       130
#define T_DATOS_DISC_IVA_BORRAR        131

//PANTALLA DE PAGO
#define T_DATOS_AUTORIZACION_AGREGAR   132
#define T_DATOS_AUTORIZACION_BORRAR    133

//PANTALLA DE PAGO
#define T_DATOS_BONOS_AGREGAR          134
#define T_DATOS_BONOS_BORRAR           135

//PANTALLA DE PAGO
#define T_DATOS_NRO_FACTURA_AGREGAR    136
#define T_DATOS_NRO_FACTURA_BORRAR     137

#define T_DATOS_ENV_OFFLINE_AGREGAR    138
#define T_DATOS_ENV_OFFLINE_BORRAR     139

#define T_DATOS_RETENCION_DGR_AGREGAR  140
#define T_DATOS_RETENCION_DGR_BORRAR   141

#define T_DATOS_PERCEPCION_DGI_AGREGAR 142
#define T_DATOS_PERCEPCION_DGI_BORRAR  143

#define T_DATOS_PERCEPCION_212_AGREGAR 144
#define T_DATOS_PERCEPCION_212_BORRAR  145

#define T_DATOS_DIFERIMIENTO_AGREGAR   146
#define T_DATOS_DIFERIMIENTO_BORRAR    147

#define T_DATOS_CUPO_CHEQUE_AGREGAR    148
#define T_DATOS_CUPO_CHEQUE_BORRAR     149

#define T_DATOS_SALDO_AGREGAR          174
#define T_DATOS_SALDO_BORRAR           175

#define T_MEDIO_0_AGREGAR              172
#define T_MEDIO_0_BORRAR               173

#define T_MEDIO_1_AGREGAR              150
#define T_MEDIO_1_BORRAR               151

#define T_MEDIO_2_AGREGAR              152
#define T_MEDIO_2_BORRAR               153

#define T_MEDIO_3_AGREGAR              154
#define T_MEDIO_3_BORRAR               155

#define T_MEDIO_4_AGREGAR              156
#define T_MEDIO_4_BORRAR               157

#define T_MEDIO_5_AGREGAR              158
#define T_MEDIO_5_BORRAR               159

#define T_MEDIO_6_AGREGAR              160
#define T_MEDIO_6_BORRAR               161

#define T_MEDIO_7_AGREGAR              162
#define T_MEDIO_7_BORRAR               163

#define T_MEDIO_8_AGREGAR              164
#define T_MEDIO_8_BORRAR               165

#define T_MEDIO_9_AGREGAR              166
#define T_MEDIO_9_BORRAR               167

//bam error
#define T_MEDIO_10_AGREGAR             168
#define T_MEDIO_10_BORRAR              169

//luc
#define T_DATOS_TOTAL_ANT_AGREGAR      170
#define T_DATOS_TOTAL_ANT_BORRAR       171

#define T_DATOS_RETENCION_DGR_AGREGAR_PAGO  172
#define T_DATOS_RETENCION_DGR_BORRAR_PAGO   173

//tipos de flags para los comprobantes (iconos)
#define FLAG_FACA                0             //0: <FAC>
#define FLAG_FACB                1             //1: <FAC>
#define FLAG_FACC                2             //2: <FAC>
#define FLAG_TFA                 3             //3: <T F>
#define FLAG_TFB                 4             //4: <T F>
#define FLAG_TFC                 5             //5: <T F>
#define FLAG_NC                  6             //6: <NC>
#define FLAG_GUIA                7             //7: <GUIA>
#define FLAG_REM                 8             //8: <REM>
#define FLAG_CR                  9             //9: <CR>
#define FLAG_COD                 10            //10: <COD>
#define FLAG_BARR                11            //11: COD DE BARRAS
#define FLAG_TKE                 12            //12: T. ESPERA
#define FLAG_CLIENTE             13
#define FLAG_TARJETA             14
#define FLAG_VENDEDOR            15
#define FLAG_DESCUENTO_1         16
#define FLAG_DESCUENTO_2         17
#define FLAG_DESCUENTO_3         18
#define FLAG_DESCUENTO_4         19
#define FLAG_DESCUENTO_5         20
#define FLAG_AFINIDAD            21
#define FLAG_PETICION            22
#define FLAG_PRESENTACION        23
#define FLAG_MUTUAL              24
#define FLAG_DEVOLUCION          25
#define FLAG_CAMBIO              26
#define FLAG_BIEN_USO            27
#define FLAG_DIF                 28
#define FLAG_COMPROMISO          29
#define FLAG_BAJO                30
#define FLAG_ENTRENAMIENTO       31
#define FLAG_DET_LIST            32
#define FLAG_MODO_OPERACION      33
#define FLAG_MODO_ENTRENAMIENTO  34
#define FLAG_MODO_INVENTARIO     35
#define FLAG_MODO_ETIQUETAS      36
#define FLAG_MODO_TRANSFERENCIA  37
//bam agrege
#define FLAG_PERFIL              38
#define FLAG_MOZO                39
#define FLAG_CLIENTE_PP          40
#define FLAG_VENDEDOR_PP         41
#define FLAG_AFINIDAD_PP         42
#define FLAG_INTERES_PP          43
#define FLAG_DISC_IVA_PP         44
#define FLAG_AUTORIZACION_PP     45
#define FLAG_BONOS_PP            46
#define FLAG_NRO_FACTURA_PP      47
#define FLAG_ENV_OFFLINE_PP      48
#define FLAG_RETENCION_DGR_PP    49
#define FLAG_PERCEPCION_DGI_PP   50
#define FLAG_PERCEPCION_212_PP   51
#define FLAG_DIFERIMIENTO_PP     52
#define FLAG_CUPO_CHEQUE_PP      53
#define FLAG_CUPO_CHEQUE         54
#define FLAG_TARJETA_PP          55
#define FLAG_MEDIO_1_PP          56
#define FLAG_MEDIO_2_PP          57
#define FLAG_MEDIO_3_PP          58
#define FLAG_MEDIO_4_PP          59
#define FLAG_MEDIO_5_PP          60
#define FLAG_MEDIO_6_PP          61
#define FLAG_MEDIO_7_PP          62
#define FLAG_MEDIO_8_PP          63
#define FLAG_MEDIO_9_PP          64
#define FLAG_MEDIO_10_PP         65
#define FLAG_MEDIO_1_PFF         66
#define FLAG_MEDIO_2_PFF         67
#define FLAG_MEDIO_3_PFF         68
#define FLAG_MEDIO_4_PFF         69
#define FLAG_MEDIO_5_PFF         70
#define FLAG_MEDIO_6_PFF         71
#define FLAG_MEDIO_7_PFF         72
#define FLAG_MEDIO_8_PFF         73
#define FLAG_MEDIO_9_PFF         74
#define FLAG_MEDIO_10_PFF        75
#define FLAG_MEDIO_0_PR          142//
#define FLAG_MEDIO_1_PR          76
#define FLAG_MEDIO_2_PR          77
#define FLAG_MEDIO_3_PR          78
#define FLAG_MEDIO_4_PR          79
#define FLAG_MEDIO_5_PR          80
#define FLAG_MEDIO_6_PR          81
#define FLAG_MEDIO_7_PR          82
#define FLAG_MEDIO_8_PR          83
#define FLAG_MEDIO_9_PR          84
#define FLAG_MEDIO_10_PR         85

#define FLAG_MEDIO_1_PP_SEL      86
#define FLAG_MEDIO_2_PP_SEL      87
#define FLAG_MEDIO_3_PP_SEL      88
#define FLAG_MEDIO_4_PP_SEL      89
#define FLAG_MEDIO_5_PP_SEL      90
#define FLAG_MEDIO_6_PP_SEL      91
#define FLAG_MEDIO_7_PP_SEL      92
#define FLAG_MEDIO_8_PP_SEL      93
#define FLAG_MEDIO_9_PP_SEL      94
#define FLAG_MEDIO_10_PP_SEL     95
#define FLAG_MEDIO_1_PFF_SEL     96
#define FLAG_MEDIO_2_PFF_SEL     97
#define FLAG_MEDIO_3_PFF_SEL     98
#define FLAG_MEDIO_4_PFF_SEL     99
#define FLAG_MEDIO_5_PFF_SEL     100
#define FLAG_MEDIO_6_PFF_SEL     101
#define FLAG_MEDIO_7_PFF_SEL     102
#define FLAG_MEDIO_8_PFF_SEL     103
#define FLAG_MEDIO_9_PFF_SEL     104
#define FLAG_MEDIO_10_PFF_SEL    105
#define FLAG_MEDIO_0_PR_SEL      143
#define FLAG_MEDIO_1_PR_SEL      106
#define FLAG_MEDIO_2_PR_SEL      107
#define FLAG_MEDIO_3_PR_SEL      108
#define FLAG_MEDIO_4_PR_SEL      109
#define FLAG_MEDIO_5_PR_SEL      110
#define FLAG_MEDIO_6_PR_SEL      111
#define FLAG_MEDIO_7_PR_SEL      112
#define FLAG_MEDIO_8_PR_SEL      113
#define FLAG_MEDIO_9_PR_SEL      114
#define FLAG_MEDIO_10_PR_SEL     115
#define FLAG_TOT_ANT_PP          116
#define FLAG_MUTUAL_PP           117
#define FLAG_DET_LIST_PP         118
#define FLAG_PERFIL_PP           119
#define FLAG_ERROR_RED           120
#define FLAG_MEDIO_1_PMM         121
#define FLAG_MEDIO_2_PMM         122
#define FLAG_MEDIO_3_PMM         123
#define FLAG_MEDIO_4_PMM         124
#define FLAG_MEDIO_5_PMM         125
#define FLAG_MEDIO_6_PMM         126
#define FLAG_MEDIO_7_PMM         127
#define FLAG_MEDIO_8_PMM         128
#define FLAG_MEDIO_9_PMM         129
#define FLAG_MEDIO_10_PMM        130
#define FLAG_MEDIO_1_PMM_SEL     131
#define FLAG_MEDIO_2_PMM_SEL     132
#define FLAG_MEDIO_3_PMM_SEL     133
#define FLAG_MEDIO_4_PMM_SEL     134
#define FLAG_MEDIO_5_PMM_SEL     135
#define FLAG_MEDIO_6_PMM_SEL     136
#define FLAG_MEDIO_7_PMM_SEL     137
#define FLAG_MEDIO_8_PMM_SEL     138
#define FLAG_MEDIO_9_PMM_SEL     139
#define FLAG_MEDIO_10_PMM_SEL    140
#define FLAG_NC_REF              141
#define FLAG_MONEDERO            142
#define FLAG_CF                  143            //143: <CF>
#define FLAG_FCF                 144            //144: <FAC>
#define FLAG_CAMBIO_MAS          145 
#define FLAG_CAMBIO_MENOS        146
#define FLAG_CAMBIO_MEDIO        147
#define FLAG_COBRO				 148
#define FLAG_NC_A                149             //6: <NC_A>
#define FLAG_NC_B                150             //6: <NC_A>
#define FLAG_NC_A_REF            151
#define FLAG_NC_B_REF            152

//titulo para el cierre de caja
#define TIT_CIERRE  0   //C I E R R E   D E   C A J A
#define TIT_ARQUEO  1   //A R Q U E O   D E   C A J A

//tipos de cierres
#define T_CIERRE_MEDIOS     0
#define T_CIERRE_SUBMEDIOS  1

//bam
#define _COMO_FLAG                    0
#define _COMO_DATO                    1
#define _COMO_FLAG_Y_DATO             2
#define T_ICON_DEV_COBRO_AGREGAR      1
#define T_ICON_DEV_COBRO_BORRAR       2
#define T_ICON_COBRO_AGREGAR          3
#define T_ICON_COBRO_BORRAR           4
#define T_ICON_COBRO_COD_BARRA_AGREGAR          5
#define T_ICON_COBRO_COD_BARRA_BORRAR           6
#define T_ICON_COBRO_INPUT_COMPROB_AGREGAR      7
#define T_ICON_COBRO_INPUT_COMPROB_BORRAR       8
#define T_ICON_COBRO_INPUT_CUENTA_AGREGAR       9
#define T_ICON_COBRO_INPUT_CUENTA_BORRAR        10
#define T_ICON_COBRO_INPUT_IMPORTE_AGREGAR      11 
#define T_ICON_COBRO_INPUT_IMPORTE_BORRAR       12
#endif
#pragma pack (pop) 
