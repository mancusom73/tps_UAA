
#include "CScreencfg.h"
#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <list>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <ini.h>
#define ELOOP                   WSAELOOP

extern "C" {

#define LOG_INCONDICIONAL   24  //si lo queremos loguear si o si
void glog( char *men, int origen, int nivel );
void SET_VALORES_CONFIG_TPS(struct _config_tps auxi);

}



void  CConfigTPS::GetConfig_tps( struct _config_tps *auxi )
{
   	int h = 0;

	auxi->activarTFpermanente = auxconfig.activarTFpermanente;
	auxi->activarLecturaClienteBarra = auxconfig.activarLecturaClienteBarra;
	strncpy(auxi->NombreCliente, auxconfig.NombreCliente,sizeof(auxconfig.NombreCliente)) ;
	strncpy(auxi->DniCliente, auxconfig.DniCliente,sizeof(auxconfig.DniCliente)) ;
	strncpy(auxi->Prefijo_scanner_cliente, auxconfig.Prefijo_scanner_cliente,sizeof(auxconfig.Prefijo_scanner_cliente));
	auxi->encabezadoTF = auxconfig.encabezadoTF;
	auxi->versionxml = auxconfig.versionxml;
	auxi->ControlarMediosDePago = auxconfig.ControlarMediosDePago;
	if( auxconfig.ControlarMediosDePago == 1 ) {
		auxi->AgruparMediosEnImpresion = auxconfig.AgruparMediosEnImpresion;
		auxi->CantMediosPermitidos = auxconfig.CantMediosPermitidos;
		auxi->InformarMediosExcedidos = auxconfig.InformarMediosExcedidos;
	}
	if( auxconfig.ClienteTarjetaEspecial == 1 ) {
		auxi->ClienteTarjetaEspecial  = auxconfig.ClienteTarjetaEspecial ;
		auxi->Codigo_seguridad_cliente = auxconfig.Codigo_seguridad_cliente;
		auxi->Perfil_cliente_especial = auxconfig.Perfil_cliente_especial;
	}
	if(auxconfig.ActivarCashPlus == 1){
		auxi->ActivarCashPlus = auxconfig.ActivarCashPlus;
		auxi->ValidacionCambioMediosCash = auxconfig.ValidacionCambioMediosCash;
		auxi->MinimoaHabilitar = auxconfig.MinimoaHabilitar;
		auxi->HabilitarCashPlusSolo = auxconfig.HabilitarCashPlusSolo;
		memcpy(auxi->MontosValidos , auxconfig.MontosValidos, 10*sizeof(float));
	}
	if( auxconfig.CambioMedioAutomatico == 1 ) {
		auxi->CambioMedioAutomatico = auxconfig.CambioMedioAutomatico;
		auxi->PedirAutorizacionCambioMedioAutomatico = auxconfig.PedirAutorizacionCambioMedioAutomatico;
		auxi->ValidacionCambioMedioAutomatico = auxconfig.ValidacionCambioMedioAutomatico;
		auxi->PedirAutorizacionEnCierreConCambioMedio = auxconfig.PedirAutorizacionEnCierreConCambioMedio;
		for( h = 0; h < 10; h++ ) {
			if( auxconfig.det_CambioMedio[h].MedioEntrante
				&& auxconfig.det_CambioMedio[h].MedioSaliente ) {
				auxi->det_CambioMedio[h].MedioEntrante = auxconfig.det_CambioMedio[h].MedioEntrante;
				auxi->det_CambioMedio[h].SubmedioEntrante = auxconfig.det_CambioMedio[h].SubmedioEntrante;
				auxi->det_CambioMedio[h].MedioSaliente = auxconfig.det_CambioMedio[h].MedioSaliente;
				auxi->det_CambioMedio[h].SubmedioSaliente = auxconfig.det_CambioMedio[h].SubmedioSaliente;
			}
		}
	}
	if( auxconfig.PadronesPorFTP ==1){ 
		strncpy(auxi->RutaFtp, auxconfig.RutaFtp,sizeof(auxconfig.RutaFtp));
		auxi->PadronesPorFTP = auxconfig.PadronesPorFTP;
	}
	auxi->AlicuotasEnArticulo = auxconfig.AlicuotasEnArticulo;
	if( auxi->AlicuotasEnArticulo == 1 ) {
		auxi->ModificarAlicuotaDeArticuloConsFinal= auxconfig.ModificarAlicuotaDeArticuloConsFinal;
	}
	SET_VALORES_CONFIG_TPS(*auxi); 
	
}
CConfigTPS::CConfigTPS( string xmlname )
{
   
  struct stat fileStatus;
  errno = 0;
  int seguirprocesando = 1;
  char cadena2[50];

  memset(&auxconfig,0, sizeof(struct _config_tps2));
  memset(cadena2,0, sizeof(cadena2));
 
  if(stat(xmlname.c_str(), &fileStatus) == -1) // ==0 ok; ==-1 error
  {
	  try {      
			  if( errno == ENOENT )      // errno declared by include file errno.h
				// throw ( std::runtime_error("Path file_name does not exist, or path is an empty string.") );
				throw ( "Path file_name does not exist, or path is an empty string.");
			  else if( errno == ENOTDIR )
				 throw ( "A component of the path is not a directory.");
			  else if( errno == ELOOP )
				 throw ( "Too many symbolic links encountered while traversing the path.");
			  else if( errno == EACCES )
				 throw ( "Permission denied.");
			  else if( errno == ENAMETOOLONG )
				 throw ( "File can not be read\n");
	  }catch( const std::exception& e) {
		  seguirprocesando = 0;
		  sglog("ERROR conftps.xml") ;
		  throw 'X'; // valor de tipo char
	}
  }
  // Configure DOM parser.
  if( seguirprocesando == 0)
	  return;
	try {
        XMLPlatformUtils::Initialize();
    }
    catch( const XMLException &toCatch ) {
        char *message = XMLString::transcode( toCatch.getMessage() );
        cout << "Error during initialization! :\n" << message << "\n";
        XMLString::release( &message );
        error = INITIALIZE_ERR;
        return;
    }

	TAG_root        = XMLString::transcode("root");
	TAG_VersionXml = XMLString::transcode("VersionXml");
    TAG_SeteosTPS = XMLString::transcode("SeteosTPS");
	TAG_Funcionalidades = XMLString::transcode("Funcionalidades");
	TAG_TicketFacturaPermanente = XMLString::transcode("TicketFacturaPermanente");
	TAG_LecturaClienteBarra= XMLString::transcode("LecturaClienteBarra");
	TAG_ControlarMediosDePago = XMLString::transcode("ControlarMediosDePago");
	TAG_ClienteTarjetaEspecial = XMLString::transcode("ClienteTarjetaEspecial");
	TAG_ActivarCashPlus = XMLString::transcode("ActivarCashPlus");
	TAG_CambioMedioAutomatico = XMLString::transcode("CambioMedioAutomatico");
	TAG_AlicuotasEnArticulo= XMLString::transcode("AlicuotasEnArticulo");
	TAG_PadronesPorFTP = XMLString::transcode("PadronesPorFTP");
	
	ATTR_Version = XMLString::transcode("Version");
	ATTR_TicketFacturaPermanente = XMLString::transcode("TicketFacturaPermanente");
	ATTR_LecturaClienteBarra= XMLString::transcode("LecturaClienteBarra");
	ATTR_NombreCliente = XMLString::transcode("NombreCliente");
    ATTR_DniCliente = XMLString::transcode("DniCliente");
	ATTR_Prefijo_scanner_cliente = XMLString::transcode("Prefijo_scanner_cliente");
	ATTR_EncabezadoTF = XMLString::transcode("EncabezadoTF");
	ATTR_AgruparMediosEnImpresion = XMLString::transcode("AgruparMediosEnImpresion");
	ATTR_CantMediosPermitidos = XMLString::transcode("CantMediosPermitidos");
	ATTR_InformarMediosExcedidos = XMLString::transcode("InformarMediosExcedidos");
	ATTR_Codigo_seguridad_cliente = XMLString::transcode("Codigo_seguridad_cliente");
	ATTR_Perfil_cliente_especial = XMLString::transcode("Perfil_cliente_especial");
	ATTR_ValidacionCambioMediosCash = XMLString::transcode("ValidacionCambioMediosCash");
	ATTR_MinimoaHabilitar = XMLString::transcode("MinimoaHabilitar");
	ATTR_HabilitarCashPlusSolo = XMLString::transcode("HabilitarCashPlusSolo");
	ATTR_MontosValidos = XMLString::transcode("MontosValidos");
	ATTR_PedirAutorizacionCambioMedioAutomatico = XMLString::transcode("PedirAutorizacionCambioMedioAutomatico");
	ATTR_ValidacionCambioMedioAutomatico = XMLString::transcode("ValidacionCambioMedioAutomatico");
	ATTR_PedirAutorizacionEnCierreConCambioMedio = XMLString::transcode("PedirAutorizacionEnCierreConCambioMedio");	
	ATTR_ModificarAlicuotaDeArticuloConsFinal= XMLString::transcode("ModificarAlicuotaDeArticuloConsFinal");
	ATTR_MedioSubmedioEntrante = XMLString::transcode("MedioSubmedioEntrante");	
	ATTR_MedioSubmedioSaliente = XMLString::transcode("MedioSubmedioSaliente");	
	ATTR_RutaFtp = XMLString::transcode("RutaFtp");	
    
    parser = new XercesDOMParser();
    parser->setValidationScheme( XercesDOMParser::Val_Always );    // optional.
    parser->setDoNamespaces( true );    // optional

    errHandler = ( ErrorHandler * ) new HandlerBase();
    parser->setErrorHandler( errHandler );

// m_ConfigFileParser->setLoadExternalDTD( false );
  try
  {
     parser->parse( xmlname.c_str() );
	 sglog("Cargado de conftps.xml OK");
     // no need to free this pointer - owned by the parent parser object
     xmlDoc = parser->getDocument();
     // Get the top-level element: NAme is "root". No attributes for "root"
     DOMElement* elementRoot = xmlDoc->getDocumentElement();

     if( !elementRoot ) throw(std::runtime_error( "empty XML document" ));
     // Parse XML file for tags of interest: "SeteosTPS"
     // Look one level nested within "root". (child of root)
     DOMNodeList*      children = elementRoot->getChildNodes();
     const  XMLSize_t nodeCount = children->getLength();
    // For all nodes, children of "root" in the XML tree.
     for( XMLSize_t xx = 0; xx < nodeCount; ++xx )
     {
        DOMNode* currentNode = children->item(xx);
        if( currentNode->getNodeType() &&  // true is not NULL
            currentNode->getNodeType() == DOMNode::ELEMENT_NODE ) // is element
        {
           // Found node which is an Element. Re-cast node as element
          DOMElement* currentElement
                       = dynamic_cast< xercesc::DOMElement* >( currentNode );
		  if( XMLString::equals(currentElement->getTagName(), TAG_VersionXml))
           {
              // Already tested node as type element and of name "ApplicationSettings".
              // Read attributes of element "ApplicationSettings".
              const XMLCh* xmlch_OptionA = currentElement->getAttribute(ATTR_Version);
              m_OptionA = XMLString::transcode(xmlch_OptionA);
			  if( m_OptionA != NULL){ 
				strncpy(cadena2, m_OptionA,50); 
				cadena2[sizeof(cadena2) - 1] = 0;
			    auxconfig.versionxml= atoi(cadena2);
			  }
		  }
			if( XMLString::equals(currentElement->getTagName(), TAG_SeteosTPS))
           {
              // Already tested node as type element and of name "ApplicationSettings".
              // Read attributes of element "ApplicationSettings".
              const XMLCh* xmlch_OptionA = currentElement->getAttribute(ATTR_TicketFacturaPermanente);
              m_OptionA = XMLString::transcode(xmlch_OptionA);
			  auxconfig.activarTFpermanente = (!strcmp( m_OptionA, "SI")) ? 1 : 0;
              const XMLCh* xmlch_OptionB = currentElement->getAttribute(ATTR_LecturaClienteBarra);
              m_OptionB = XMLString::transcode(xmlch_OptionB);
			  auxconfig.activarLecturaClienteBarra = (!strcmp( m_OptionA, "SI")) ? 1 : 0;
			  const XMLCh* xmlch_OptionLL = currentElement->getAttribute(TAG_ControlarMediosDePago);
              m_OptionA = XMLString::transcode(xmlch_OptionLL);
			  auxconfig.ControlarMediosDePago = (!strcmp( m_OptionA, "SI")) ? 1 : 0;
			  xmlch_OptionLL = currentElement->getAttribute(TAG_ClienteTarjetaEspecial);
              m_OptionA = XMLString::transcode(xmlch_OptionLL);
			  auxconfig.ClienteTarjetaEspecial = (!strcmp( m_OptionA, "SI")) ? 1 : 0;
			  
			  xmlch_OptionLL = currentElement->getAttribute(TAG_ActivarCashPlus);
			  m_OptionA = XMLString::transcode(xmlch_OptionLL);
			  auxconfig.ActivarCashPlus = (!strcmp( m_OptionA, "SI")) ? 1 : 0;

			  xmlch_OptionA = currentElement->getAttribute(TAG_CambioMedioAutomatico);
			  m_OptionA = XMLString::transcode(xmlch_OptionA);
			  auxconfig.CambioMedioAutomatico = (!strcmp( m_OptionA, "SI")) ? 1 : 0;
			 
			  xmlch_OptionA = currentElement->getAttribute(TAG_PadronesPorFTP);
			  m_OptionA = XMLString::transcode(xmlch_OptionA);
			  auxconfig.PadronesPorFTP = (!strcmp( m_OptionA, "SI")) ? 1 : 0;
const XMLCh* xmlch_OptionR = currentElement->getAttribute(TAG_AlicuotasEnArticulo);
              m_OptionA = XMLString::transcode(xmlch_OptionR);
			  auxconfig.AlicuotasEnArticulo = (!strcmp( m_OptionA, "SI")) ? 1 : 0;	
   //           break;  // Data found. No need to look at other elements in tree.
		   } 
			if( XMLString::equals(currentElement->getTagName(), TAG_Funcionalidades))
				{
				 //veamos si tiene hijos
					DOMNodeList*      childrenF = currentElement->getChildNodes();
					const  XMLSize_t nodeCountF = childrenF->getLength();
					for( XMLSize_t xx = 0; xx < nodeCountF; ++xx )
					{
						DOMNode* currentNodeF = childrenF->item(xx);
						if( currentNodeF->getNodeType() &&  // true is not NULL
								currentNodeF->getNodeType() == DOMNode::ELEMENT_NODE ) // is element
						{
							// Found node which is an Element. Re-cast node as element
							DOMElement* currentElement
											= dynamic_cast< xercesc::DOMElement* >( currentNodeF );
							if( XMLString::equals(currentElement->getTagName(), TAG_TicketFacturaPermanente))
							{
								const XMLCh* xmlch_OptionA = currentElement->getAttribute(ATTR_NombreCliente);
								m_OptionA = XMLString::transcode(xmlch_OptionA);
								if( m_OptionA != NULL){ 
									strncpy(cadena2, m_OptionA,50); 
									cadena2[sizeof(cadena2) - 1] = 0;
									strncpy(auxconfig.NombreCliente,cadena2,16);
									auxconfig.NombreCliente[16]=0;
								}

								const XMLCh* xmlch_OptionB = currentElement->getAttribute(ATTR_DniCliente);
								m_OptionB = XMLString::transcode(xmlch_OptionB);
								 if( m_OptionA != NULL){ 
									strncpy(cadena2, m_OptionB,50); 
									cadena2[sizeof(cadena2) - 1] = 0;
									strncpy(auxconfig.DniCliente,cadena2,14);
									auxconfig.DniCliente[14]=0;
								}
							    const XMLCh* xmlch_OptionC = currentElement->getAttribute(ATTR_EncabezadoTF);
								m_OptionC = XMLString::transcode(xmlch_OptionC);
								if( m_OptionC != NULL){ 
									strncpy(cadena2, m_OptionC,50); 
									cadena2[sizeof(cadena2) - 1] = 0;
									auxconfig.encabezadoTF= atoi(cadena2);
								}
			  				}
							if( XMLString::equals(currentElement->getTagName(),TAG_LecturaClienteBarra))
							{
								const XMLCh* xmlch_OptionA = currentElement->getAttribute(ATTR_Prefijo_scanner_cliente);
								m_OptionA = XMLString::transcode(xmlch_OptionA);
								 if( m_OptionA != NULL){ 
									strncpy(cadena2, m_OptionA,50); 
									cadena2[sizeof(cadena2) - 1] = 0;
									strncpy(auxconfig.Prefijo_scanner_cliente,cadena2,4/*5*/);
									auxconfig.Prefijo_scanner_cliente[4]=0;
								}
								
							}
							//control de medios
							if( XMLString::equals(currentElement->getTagName(), TAG_ControlarMediosDePago))
							{
 							    const XMLCh* xmlch_OptionA = currentElement->getAttribute(ATTR_AgruparMediosEnImpresion);
								m_OptionA = XMLString::transcode(xmlch_OptionA);
								if( m_OptionA != NULL){
									strncpy(cadena2, m_OptionA,50);
									cadena2[sizeof(cadena2) - 1] = 0;
									auxconfig.AgruparMediosEnImpresion = atoi(cadena2);
								}
								const XMLCh* xmlch_OptionB = currentElement->getAttribute(ATTR_CantMediosPermitidos);
								m_OptionB = XMLString::transcode(xmlch_OptionB);
								if( m_OptionA != NULL){
									strncpy(cadena2, m_OptionB,50);
									cadena2[sizeof(cadena2) - 1] = 0;
									auxconfig.CantMediosPermitidos = atoi(cadena2);
								}
								const XMLCh* xmlch_OptionC = currentElement->getAttribute(ATTR_InformarMediosExcedidos);
								m_OptionC = XMLString::transcode(xmlch_OptionC);
								if( m_OptionC != NULL){
									strncpy(cadena2, m_OptionC,50);
									cadena2[sizeof(cadena2) - 1] = 0;
									auxconfig.InformarMediosExcedidos = atoi(cadena2);
								}
			  				}
							//fin control de medios
							//inicio cliente especial
							if( XMLString::equals(currentElement->getTagName(), TAG_ClienteTarjetaEspecial))
							{
 							    const XMLCh* xmlch_OptionA = currentElement->getAttribute(ATTR_Codigo_seguridad_cliente);
								m_OptionA = XMLString::transcode(xmlch_OptionA);
								if( m_OptionA != NULL){
									strncpy(cadena2, m_OptionA,50);
									cadena2[sizeof(cadena2) - 1] = 0;
									auxconfig.Codigo_seguridad_cliente = atoi(cadena2);
								}
								const XMLCh* xmlch_OptionB = currentElement->getAttribute(ATTR_Perfil_cliente_especial);
								m_OptionB = XMLString::transcode(xmlch_OptionB);
								if( m_OptionA != NULL){
									strncpy(cadena2, m_OptionB,50);
									cadena2[sizeof(cadena2) - 1] = 0;
									auxconfig.Perfil_cliente_especial = atoi(cadena2);
								}
			  				}
							//fin cliente especial
							if( XMLString::equals(currentElement->getTagName(), TAG_ActivarCashPlus))
							{
 							    char *ptr;
								const XMLCh* xmlch_OptionA = currentElement->getAttribute(ATTR_ValidacionCambioMediosCash);
								m_OptionA = XMLString::transcode(xmlch_OptionA);
								if( m_OptionA != NULL){
									strncpy(cadena2, m_OptionA,50);
									cadena2[sizeof(cadena2) - 1] = 0;
									auxconfig.ValidacionCambioMediosCash = atoi(cadena2);
								}
								const XMLCh* xmlch_OptionB = currentElement->getAttribute(ATTR_MinimoaHabilitar);
								m_OptionB = XMLString::transcode(xmlch_OptionB);
								 if( m_OptionB != NULL){ 
									strncpy(cadena2, m_OptionB,50); 
									cadena2[sizeof(cadena2) - 1] = 0;
									auxconfig.MinimoaHabilitar= atoi(cadena2);
								}
							    const XMLCh* xmlch_OptionC = currentElement->getAttribute(ATTR_HabilitarCashPlusSolo);
								m_OptionC = XMLString::transcode(xmlch_OptionC);
								if( m_OptionC != NULL){ 
									strncpy(cadena2, m_OptionC,50); 
									cadena2[sizeof(cadena2) - 1] = 0;
									auxconfig.HabilitarCashPlusSolo= atoi(cadena2);
								}
								xmlch_OptionC = currentElement->getAttribute(ATTR_MontosValidos);
								m_OptionC = XMLString::transcode(xmlch_OptionC);
								if( m_OptionC != NULL){ 
									ptr = strtok(m_OptionC ,";");
									int ii =0;
									for(int jj=0; jj< 10;jj++){
										auxconfig.MontosValidos[jj] = 0;
									}

									while(ptr != NULL)
									{
										
										//strncpy(cadena2, ptr,sizeof(ptr));
										strncpy(cadena2, ptr,strlen (ptr));
										
										cadena2[sizeof(cadena2) - 1] = 0;
										auxconfig.MontosValidos[ii] = atof(cadena2);
										ii++;
										ptr = strtok(NULL, ";");
									}
								}
			  				} //fin cashplus

							if( XMLString::equals(currentElement->getTagName(), TAG_CambioMedioAutomatico))
							{
								char *token, *str;
								char seps[] = ";";
								char _MedioEntrante[5],_SubmedioEntrante[5], str_aux[50];
								char _MedioSaliente[5],_SubmedioSaliente[5];
								int i = 0, j = 0, h = 0;
	
								memset( _MedioEntrante, 0, sizeof( _MedioEntrante ) );
								memset( _SubmedioEntrante, 0, sizeof( _SubmedioEntrante ) );
								memset( _MedioSaliente, 0, sizeof( _MedioSaliente ) );
								memset( _SubmedioSaliente, 0, sizeof( _SubmedioSaliente ) );
								memset( str_aux, 0, sizeof( str_aux ) );
								
								const XMLCh* xmlch_OptionA = currentElement->getAttribute(ATTR_PedirAutorizacionCambioMedioAutomatico);
								m_OptionA = XMLString::transcode(xmlch_OptionA);
								 if( m_OptionA != NULL){
									strncpy(cadena2, m_OptionA,sizeof(cadena2));
									cadena2[sizeof(cadena2) - 1] = 0;
									auxconfig.PedirAutorizacionCambioMedioAutomatico = atof(cadena2);
								}
								xmlch_OptionA = currentElement->getAttribute(ATTR_ValidacionCambioMedioAutomatico);
								m_OptionA= XMLString::transcode(xmlch_OptionA);
								if( m_OptionA != NULL){
									strncpy(cadena2, m_OptionA,sizeof(cadena2));
									cadena2[sizeof(cadena2) - 1] = 0;
									auxconfig.ValidacionCambioMedioAutomatico = atoi(cadena2);
								}
								xmlch_OptionA = currentElement->getAttribute(ATTR_PedirAutorizacionEnCierreConCambioMedio);
								m_OptionA= XMLString::transcode(xmlch_OptionA);
								if( m_OptionA != NULL){
									strncpy(cadena2, m_OptionA,sizeof(cadena2));
									cadena2[sizeof(cadena2) - 1] = 0;
									auxconfig.PedirAutorizacionEnCierreConCambioMedio = atoi(cadena2);
								}

								/**********************************************************************/	
								xmlch_OptionA = currentElement->getAttribute(ATTR_MedioSubmedioEntrante);
								m_OptionA = XMLString::transcode(xmlch_OptionA);
								memset(cadena2,0, sizeof(cadena2));
								if( m_OptionA != NULL) {
									i = 0, j = 0, h = 0;
									strncpy(cadena2, m_OptionA,sizeof(cadena2));
									cadena2[sizeof(cadena2) - 1] = 0;

									str = (char*)malloc(strlen(cadena2)+1);
									strncpy(str,cadena2,strlen(cadena2)+1);
									token = strtok( str, seps );
									strncpy(str_aux,token,strlen(token)+1);
									while( ( token != NULL ) && ( i < 10 ) )
									{
										/* While there are tokens in "string" */
										for(j=0;j<strlen(str_aux) && strncmp( &str_aux[j], "-",1 ) != 0;j++) {
											_MedioEntrante[j] = str_aux[j];
										}
										for(j=j+1;j<strlen(str_aux);j++,h++) {
											_SubmedioEntrante[h] = str_aux[j];
										}
										auxconfig.det_CambioMedio[i].MedioEntrante = atof(_MedioEntrante);
										auxconfig.det_CambioMedio[i].SubmedioEntrante = atof(_SubmedioEntrante);
										
										//Si MedioEntrante = 0 si o si MedioEntrante = 1 
										if( auxconfig.det_CambioMedio[i].MedioEntrante == 0 )
											auxconfig.det_CambioMedio[i].MedioEntrante = 1;
										//Si MedioEntrante = 1 (efectivo) si o si SubmedioEntrante = 0 
										//Si SubmedioEntrante = 0 y MedioEntrante > 1  entonces esta habilitado para todos los submedios del MedioEntrante configurado
										if( auxconfig.det_CambioMedio[i].MedioEntrante == 1 )//Efectivo
											auxconfig.det_CambioMedio[i].SubmedioEntrante = 0;

										memset( _MedioEntrante, 0, sizeof( _MedioEntrante ) );
										memset( _SubmedioEntrante, 0, sizeof( _SubmedioEntrante ) );

										/* Get next tokqen: */
										token = strtok( NULL, seps ); 
										if( token != NULL )
											strncpy(str_aux,token,strlen(token)+1);
										i++;
										h = 0;
									}
									if(str)
										free (str);
								}

								xmlch_OptionA = currentElement->getAttribute(ATTR_MedioSubmedioSaliente);
								m_OptionA = XMLString::transcode(xmlch_OptionA);
								memset(cadena2,0, sizeof(cadena2));
								if( m_OptionA != NULL) {
									i = 0, j = 0, h = 0;
									strncpy(cadena2, m_OptionA,sizeof(cadena2));
									cadena2[sizeof(cadena2) - 1] = 0;

									str = (char*)malloc(strlen(cadena2)+1);
									strncpy(str,cadena2,strlen(cadena2)+1);
									token = strtok( str, seps );
									strncpy(str_aux,token,strlen(token)+1);
									while( ( token != NULL ) && ( i < 10 ) )
									{
										/* While there are tokens in "string" */
										for(j=0;j<strlen(str_aux) && strncmp( &str_aux[j], "-",1 ) != 0;j++) {
											_MedioSaliente[j] = str_aux[j];
										}
										for(j=j+1;j<strlen(str_aux);j++,h++) {
											_SubmedioSaliente[h] = str_aux[j];
										}
										auxconfig.det_CambioMedio[i].MedioSaliente = atof(_MedioSaliente);
										auxconfig.det_CambioMedio[i].SubmedioSaliente = atof(_SubmedioSaliente);
										
										//Si MedioSaliente = 0 si o si MedioSaliente = 1 
										if( auxconfig.det_CambioMedio[i].MedioSaliente  == 0 )
											auxconfig.det_CambioMedio[i].MedioSaliente  = 1;
										//Si MedioSaliente = 1 (efectivo) si o si SubmedioSaliente = 0 
										//Si SubmedioSaliente  = 0 y MedioSaliente  > 1  entonces esta habilitado para todos los submedios del MedioSaliente configurado
										if( auxconfig.det_CambioMedio[i].MedioSaliente == 1 )//Efectivo
											auxconfig.det_CambioMedio[i].SubmedioSaliente = 0;

										memset( _MedioSaliente, 0, sizeof( _MedioSaliente ) );
										memset( _SubmedioSaliente, 0, sizeof( _SubmedioSaliente ) );
										/* Get next tokqen: */
										token = strtok( NULL, seps ); 
										if( token != NULL )
											strncpy(str_aux,token,strlen(token)+1);
										i++;
										h = 0;
									}
									if(str)
										free (str);
								}
			  				}//fin cambio de medio automatico
if( XMLString::equals(currentElement->getTagName(), TAG_AlicuotasEnArticulo)){
								const XMLCh* xmlch_OptionA = currentElement->getAttribute(ATTR_ModificarAlicuotaDeArticuloConsFinal);
								m_OptionA = XMLString::transcode(xmlch_OptionA);
								if( m_OptionA != NULL){
									strncpy(cadena2, m_OptionA,sizeof(cadena2));
									cadena2[sizeof(cadena2) - 1] = 0;
									auxconfig.ModificarAlicuotaDeArticuloConsFinal= atoi(cadena2);
								}												
							}//FIN TAG_AlicuotasEnArticulo
							//inicio PadronesPorFTP
							if( XMLString::equals(currentElement->getTagName(), TAG_PadronesPorFTP))
							{
 							    const XMLCh* xmlch_OptionA = currentElement->getAttribute(ATTR_RutaFtp);
								m_OptionA = XMLString::transcode(xmlch_OptionA);
								if( m_OptionA != NULL){
									strncpy(cadena2, m_OptionA,50); 
									cadena2[sizeof(cadena2) - 1] = 0;
									strncpy(auxconfig.RutaFtp,cadena2,sizeof(cadena2));
									auxconfig.RutaFtp[49]=0;
								}
							}//fin PadronesxFtp
						}
					}
			}
		} else {//veamos si es la version

		}
	 }
	 GetConfig_tps(&config_tps/* _config_tps auxi*/ ) ;
}

  catch( xercesc::XMLException& e )
  {
     char* message = xercesc::XMLString::transcode( e.getMessage() );
     ostringstream errBuf;
     errBuf << "Error parsing file: " << message << flush;
     XMLString::release( &message );
  }
}