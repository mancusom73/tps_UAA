#pragma pack (1) 

//#include "ftypes.h"
//#include "loglib.h"
#ifdef INVEL_L
#include "cr.h"
#include "path.h"
#endif
#ifdef INVEL_W
#include <cr_disco.h>
#endif //INVEL_W

#include "logrecov.h"
#include <string.h>
#include "etpv.h"
#include "ini.h"
#include <motor.h>

#pragma pack (1)

extern int eventFinish;

//#ifdef MODULO_PROMOCIONES
/*
 * Variables Globales del Motor de Promociones
 */
#include "interpr.h"

//extern promo pr, gpr;
//extern states ss;
//extern recover_info state_venta, state_pago;

//#endif //MODULO_PROMOCIONES


/* 
 * MODULO DE RECUPERACION A PARTIR DE UN LOG
 */

/*
 * Carga los datos y eventos registrados en el log, 
 * en ls y qs respectivamente, para su posterior
 * uso en la recuperacion.
 */
extern int      logPanelVentas( void );
extern int      logPanelPagos( void );
extern int      logPanelPosPago( void );

int loadLog( FILE *log, eventQueue *qs, dataList *ls, benefList *bs )
{
    logReg r = NULL;

    if( !ls || !qs || !bs ) {
        return 1;
    }
    if( *ls ) {
        freeDataList( *ls );
        *ls = NULL;
    }
    if( *bs ) {
        freeBenefList( *bs );
        *bs = NULL;
    }
    if( *qs ) {
        freeEventQueue( *qs );
        *qs = NULL;
    }
    *qs = newEventQueue();

    while( ( r = getLogEntry( log ) ) != NULL ) {
        switch( r->type ) {
            case InitValue:
            case InitSubValue:
                *ls = dataListCons( *ls, r ); 
                //freeLogReg( r ); 	//revisar 17/07/08
                break;
            case CountValue:
            case CountSubValue:
                *ls = dataListCons( *ls, r ); 
                free( r ); 			//revisar 17/07/08
                //freeLogReg( r ); 	//revisar 17/07/08
                break;

            case AddArticle:
            case RemoveArticle:
            case UpdateArticle:
            case AddExternalDiscount:
                *qs = eventEnQueue( *qs, r->type, ( char* )r->data, (double) r->count, r->depart, r->precio, r->promoc, r->porImporte );
                //*qs = eventEnQueue( *qs, r->type, ( identifier )r->data, r->count );
                free( r );			//revisar 17/07/08
                //freeLogReg( r );	//revisar 17/07/08
                break;

            case PanelVentas:
            case PanelPagos:
            case PanelPosPago:
                *qs = eventEnQueue( *qs, r->type, NULL, 0, 0, 0, 0, 0 );
                free( r );			//revisar 17/07/08
                //freeLogReg( r );	//revisar 17/07/08
                break;

            case CashdeskState:
                *qs = eventEnQueue( *qs, r->type, ( identifier )r->data, r->count, 0, 0, 0, 0 );
                free( r );			//revisar 17/07/08
                //freeLogReg( r );	//revisar 17/07/08
                break;

            case Credit:
                *bs = benefListCons( *bs, r->type, ( identifier )r->data, NULL, r->count, 0, NULL );
                /**/fprintf( stderr, "loadLog: CREDIT(%s,%lf)\n", ( char* )r->data, r->count );
                freeLogReg( r );
                break;

            case Distribute:
                *bs = benefListCons( *bs, r->type, ( identifier )r->data, NULL, r->count,
                                     *( number * )( ( int )( r->data ) + strlen( ( char* )r->data )
                                                  + 1 ), NULL );
                /**/fprintf( stderr, "loadLog: DISTRIBUTE(%s,%lf,%lf)\n", ( char* )r->data,
                             r->count,
                             *( number * )( ( int )( r->data ) + strlen( r->data ) + 1 ) );
                freeLogReg( r );
                break;

            case IssueBonus:
                {
                    char *target = NULL;
                    char *type = NULL;
                    char *label = NULL;
                    int tar_size,type_size,lab_size;
                    number value;

                    target = ( char* )( r->data );
                    tar_size = strlen( target ) + 1;

                    type = ( char* )( ( int )( r->data ) + tar_size );
                    type_size = strlen( type ) + 1;

                    value = *( number * )( ( int )( r->data ) + tar_size + type_size );

                    label = ( char* )( ( int )( r->data ) + tar_size + type_size
                                     + sizeof( number ) );
                    lab_size = strlen( label );


                    *bs = benefListCons( *bs, r->type, target, type, r->count, value, label );

                    /**/fprintf( stderr, "loadLog: ISSUE_BONUS(%s,%s,%lf,%lf,%s)\n", target, type,
                                 r->count, value, label );

                    freeLogReg( r );
                }
                break;

            case GivePoints:
                *bs = benefListCons( *bs, r->type, ( identifier )r->data, NULL, r->count, 0, NULL );
                fprintf( stderr, "loadLog: GIVE_POINTS(%s,%lf)\n", ( char* )r->data, r->count );
                freeLogReg( r );
                break;

            default:
                fprintf( stderr, "loadLog: registro no valido en log.\n" );
                //freeLogReg( r ); //revisar 17/07/08
                break;
        }
    }

    return 0;
}

/*
 * Toma una cola de eventos, y los reproduce para lograr generar
 * un estado igual al que se tenia al momento de producirse la falla.
 */ 
int recoverState( eventQueue *qs )
{
    logtype e;
    while( !isEmptyQueue( *qs ) ) {
        char id[256];
        number count;
		long depart;
		double precio;
		char promocionable;
		char por_importe;
        void *buff;

        e = currentEvent( *qs );

        if( currentIdentifier( *qs ) != NULL ) {
            if( e == CashdeskState ) {
                if( ( buff = malloc( ( int )currentCount( *qs ) ) ) == NULL ) {
                    glog(
                    "recoverState: Error reproduciendo evento CashDeskState: falta de memoria\n",LOG_VARIOS,2);
                    //return -1;
                }
                memcpy( buff, ( void* )currentIdentifier( *qs ), ( int )currentCount( *qs ) );
            }
            else {
                strcpy( id, currentIdentifier( *qs ) );
            }
        }
        else {
            id[0] = '\0';
        }
        count = currentCount( *qs );
        depart = currentDepart( *qs );
        precio = currentPrecio( *qs );
		promocionable = currentPromocionable( *qs );
		por_importe = currentPorImporte( *qs );
        *qs = eventDeQueue( *qs );

        // se indica si se leyo el ultimo evento de la cola, o si aun
        // quedan eventos por recuperar.
        eventFinish = isEmptyQueue( *qs );

        switch( e ) {
	case AddArticle:
	  {
	    long codInt = 0;
	    char codBar[17] = "";
	    double valor = 0;
	    long codPresentacion = 0;
	    
	    switch ( getCodsArt( id, &codInt, codBar, &valor, &codPresentacion) ) {
					case COD_INTERNO:
	    case COD_INTERNO_PRECIO:
	    case COD_BARRA:
	    case COD_BARRA_PRECIO:
	    case COD_INTERNO_BARRA:
	    case COD_INTERNO_BARRA_PRECIO:
	    case COD_INTERNO_BARRA_PRESENTACION:
	    case COD_INTERNO_BARRA_PRESENTACION_PRECIO:
						add_articles( codInt, ( strlen( codBar ) == 0 ? NULL : codBar ), codPresentacion, count, precio, depart, promocionable, !FUNCION_PIDE_PRECIO && por_importe, pr, &ss );
	      break;
	    case COD_ERROR:
	      /**/glog( "PROMO: recoverState: AddArticle: El tipo de articulo no esta soportado o presenta un error. No se aplica el beneficio.", LOG_MOTOR_PROMO,3);
	    break;
	    default:
	      /**/glog( "PROMO: recoverState: AddArticle: Tipo de articulo desconocido. No se aplica el beneficio." , LOG_MOTOR_PROMO,3);
	    break;
	    
	    }
	  }
/* ANTIGUA IMPLEMENTACION PARA ARTICULOS SOLO POR COD_INTERNO					
                {
					
					long codint = 0;
					char * codbar = NULL;
                    //double precio = 0;
                    if( id[0] == 'j' ) {
                        sscanf( id, "j%li$%lf", &codint, &precio );
                    }
                    else if( id[0] == 'x' ) {
						codbar = (char*) malloc(256);
                        sscanf( id, "x%li_%s", &codint, codbar );
                    }
                    else {
                        sscanf( id, "i%li", &codint );
                    }




					if ( BUSCAR_PROMOCION_POR_CODIGO_INTE ) {
						add_articles( codint, NULL, count, precio, depart, promocionable, pr, &ss );
						//add_articles( ITEM_COD_ESPECIAL, NULL, RAM_CANTIDAD, ITEM_PRECIO, ITEM_DEPTO, ITEM_HABILITADO_EN_PROMOCION, pr, &ss );
					} else {
						add_articles( codint, codbar, count, precio, depart, promocionable, pr, &ss );
						//add_articles( ITEM_COD_ESPECIAL, ITEM_COD_BARRA, RAM_CANTIDAD, ITEM_PRECIO, ITEM_DEPTO, ITEM_HABILITADO_EN_PROMOCION, pr, &ss );
					}
                    //add_articles( codint, NULL, count, precio, ITEM_DEPTO, pr, &ss );
                }

                //fprintf(stderr,"recoverState: ADD(%s,%lf)\n",id,count);
*/
	  break;
	case RemoveArticle:
	  {
		long codInt = 0;
		char codBar[17] = "";
		double valor = 0;
		long codPresentacion = 0;
		
		switch ( getCodsArt( id, &codInt, codBar, &valor, &codPresentacion) ) {
		case COD_INTERNO:
		case COD_INTERNO_PRECIO:
		case COD_BARRA:
		case COD_BARRA_PRECIO:
		case COD_INTERNO_BARRA:
		case COD_INTERNO_BARRA_PRECIO:
		case COD_INTERNO_BARRA_PRESENTACION:
		case COD_INTERNO_BARRA_PRESENTACION_PRECIO:
			remove_articles( codInt, ( strlen( codBar ) == 0 ? NULL : codBar ), codPresentacion, count, precio, depart, promocionable, !FUNCION_PIDE_PRECIO && por_importe, pr, &ss );
			break;
		case COD_ERROR:
			/**/glog( "PROMO: recoverState: RemoveArticle: El tipo de art�culo no est� soportado o presenta un error. No se aplic� el beneficio.", LOG_MOTOR_PROMO,3);
			break;
		default:
			/**/glog( "PROMO: recoverState: RemoveArticle: Tipo de art�culo desconocido. No se aplic� el beneficio." , LOG_MOTOR_PROMO,3);
			break;

		}
	}

/* ANTIGUA IMPLEMENTACION PARA ARTICULOS SOLO POR COD_INTERNO					
                {
                    long codint = 0;
					char * codbar = NULL;
                    //double precio = 0;
                    if( id[0] == 'j' ) {
                        sscanf( id, "j%li$%lf", &codint, &precio );
                    }
                    else if( id[0] == 'x' ) {
						codbar = (char*) malloc(256);
                        sscanf( id, "x%li_%s", &codint, codbar );
                    }
                    else {
                        sscanf( id, "i%li", &codint );
                    }

                    
					remove_articles( codint, codbar, count, precio, depart, promocionable, pr, &ss );
					//remove_articles( codint, NULL, count, precio, -1, pr, &ss );
                }

                //fprintf(stderr,"recoverState: DEL(%s,%lf)\n",id,count);
*/
                break;
            case UpdateArticle:
				{
					long codInt = 0;
					char codBar[17] = "";
					double valor = 0;
					long codPresentacion = 0;
					
					switch ( getCodsArt( id, &codInt, codBar, &valor, &codPresentacion) ) {
					case COD_INTERNO:
					case COD_INTERNO_PRECIO:
					case COD_BARRA:
					case COD_BARRA_PRECIO:
					case COD_INTERNO_BARRA:
					case COD_INTERNO_BARRA_PRECIO:
					case COD_INTERNO_BARRA_PRESENTACION:
					case COD_INTERNO_BARRA_PRESENTACION_PRECIO:
						update_articles( codInt, ( strlen( codBar ) == 0 ? NULL : codBar ), codPresentacion, count, precio, depart, promocionable, pr, &ss );
						break;
					case COD_ERROR:
						/**/glog( "PROMO: recoverState: UpdateArticle: El tipo de art�culo no est� soportado o presenta un error. No se aplic� el beneficio." , LOG_MOTOR_PROMO,3);
						break;
					default:
						/**/glog( "PROMO: recoverState: UpdateArticle: Tipo de art�culo desconocido. No se aplic� el beneficio." , LOG_MOTOR_PROMO,3);
						break;

					}
				}

                break;

            case AddExternalDiscount:
				add_external_discount(precio, pr, &ss );
				/**/glog( "PROMO: recoverState: AddExternalDiscount: Se recupero y aplico un descuento externo." , LOG_MOTOR_PROMO,4);
                break;

            case PanelVentas:
                //fprintf(stderr,"recoverState: PANEL VENTAS\n");
                glog( "recoverState: PANEL VENTAS" , LOG_PANTALLAS,5);




                if( state_venta != NULL ) {
                    // en este caso ya se evaluaron las promos prepago,
                    // en cuyo caso hay que revertir el estado para poder
                    // volver a ventas con el estado en el que habia quedado.
                    ss = recover( state_venta, ss );
                    state_venta = free_recover_info( state_venta );
                    state_pago = free_recover_info( state_pago );
                }

                logPanelVentas();


                break;

            case PanelPagos:
                //fprintf(stderr,"recoverState: PANEL PAGOS\n");
                glog( "recoverState: PANEL PAGOS",LOG_PAGOS,4);

                //MENSAJE("Evaluando PROMOs");
                /**/glog( "Evaluando PROMOS PREPAGO",LOG_MOTOR_PROMO,3);

                // se guarda el estado actual de la caja y de las
                // variables de las promos para permitir revertir
                // los efectos de la evaluacion prepago.
                state_venta = actual_info( ss );

                if( eval( PREPAGO, pr, &ss ) != NORM ) {
					glog( "PROMO: LOGRECOV.C: eval(PREPAGO): ERROR in PROMOS" ,LOG_MOTOR_PROMO,3);
                }

                logPanelPagos();

                /**/glog( "Evaluacion de promociones OK.",LOG_MOTOR_PROMO,4);
                //MENSAJE("PROMO Evaluada correctamente");

                break;
            case PanelPosPago:
                //fprintf(stderr,"recoverState: PANEL POSPAGO\n");
                glog( "recoverState: PANEL POSPAGO" ,LOG_MOTOR_PROMO,4);

                //MENSAJE("Evaluando PROMOs POSTPAGO");
                /**/glog( "Evaluando PROMOS POSTPAGO" ,LOG_MOTOR_PROMO,4);

                // se guarda el estado actual de la caja y de las
                // variables de las promos para permitir revertir
                // los efectos de la evaluacion postpago.
                state_pago = actual_info( ss );
				{ 
					//veo si hau medios requeridos
					int cant =1;
					if( RAM_P_PAGO > 0)  {//esta en pagos y si recupera estado habra promo postpago?
						int cant1 = CANTIDAD_REGISTROS_REC(ARCH_REC_REQUERIR_MEDIO, sizeof(struct requerir_medios));
						if( cant1 == 0 && RAM_P_PAGO > 0)
							cant = 0;
					}
		
								

					if( cant > 0 && eval( POSTPAGO, pr, &ss ) != NORM ) {
					glog( "PROMO: LOGRECOV.C: eval(PREPAGO): ERROR in PROMOS",LOG_MOTOR_PROMO,3);
	                }
				}

                logPanelPosPago();

                /**/glog( "PROMOs PostPago Evaluadas correctamente",LOG_MOTOR_PROMO,4);
                //MENSAJE("PROMOs Evaluadas correctamente");

                break;

            case CashdeskState:
                //fprintf(stderr,"recoverState: PANEL POSPAGO\n");
                glog( "recoverState: CASHDESK_STATE",LOG_MOTOR_PROMO,4);
                if( globalCashdeskState != NULL ) {
                    free( globalCashdeskState );
                }
                globalCashdeskState = buff;

                break;
            default:
                //fprintf(stderr,"recoverState: evento no valido\n");
                glog( "recoverState: evento no valido",LOG_MOTOR_PROMO,3);
                break;
        }
    }
    return 0;
}
