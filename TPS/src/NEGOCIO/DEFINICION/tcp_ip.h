#ifndef TCP_IP_H
#define TCP_IP_H
int _ENVIAR_PAQUETE_TCP_IP( char *paquete, int l_paquete, char *rta, int l_rta, int timeout_nodo,
                            char espera_rta, int canal );
int INIT_DRV_TCP_IP();
int CERRAR_CANAL_TCP_IP(int canal);
int ABRIR_CANAL_TCP_IP( int canal, char *ip, int formato_mensaje );
int PING();
int PING_ABRIR_SOCKET( char *ip_addres );
int PING_CLOSE_SOCKET();
unsigned TCP_canal_puerto( int canal, int puerto );
#endif
