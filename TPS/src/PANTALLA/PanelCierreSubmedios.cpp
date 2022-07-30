#include "PanelCierreSubmedios.h"

PanelCierreSubmedios::PanelCierreSubmedios( const wxString &title, const wxPoint &pos,
                                            const wxSize &size, long style ) : CPanel( title, pos,
                                                                                       size, style )
{
    medios_title = new CLabel();
    sub_medios_title = new CLabel();
    label_mensaje = new CLabel();
    screenXml = new CScreencfg( "cierresubmedio", "default", "default" );
    #ifdef INVEL_W
    dummy = new CInput( this, 200, 200, 100, 12, false );
    #else
    dummy = new CInput( this, 600, 600, 100, 12, false );
    #endif

    for( int i = 0;i < 10;i++ ) {
        LabelSubmedios[i] = new CLabel();
        LabelSubmediosImporte[i] = new CLabel();
        LabelMediosImporteInf[i] = new CLabel();
        LabelMediosImporteDif[i] = new CLabel();
		LabelSubmediosImporteDif[i] = new CLabel();/**/
    }
}

void PanelCierreSubmedios::createLabels()
{
    medio_selected = -1;
    submedio_selected = -1;

    medios1.setParent( this );
    medios2.setParent( this );
    medios3.setParent( this );
    medios4.setParent( this );
    medios5.setParent( this );
    medios6.setParent( this );
    medios7.setParent( this );
    medios8.setParent( this );
    medios9.setParent( this );
    medios10.setParent( this );
    title.setParent( this );
    medios_title->setParent( this );
    sub_medios_title->setParent( this );
    label_mensaje->setParent( this );

    this->AddObject( &medios1 );
    this->AddObject( &medios2 );
    this->AddObject( &medios3 );
    this->AddObject( &medios4 );
    this->AddObject( &medios5 );
    this->AddObject( &medios6 );
    this->AddObject( &medios7 );
    this->AddObject( &medios8 );
    this->AddObject( &medios9 );
    this->AddObject( &medios10 );

    this->AddObject( &title );
    this->AddObject( medios_title );
    this->AddObject( sub_medios_title );
    this->AddObject( label_mensaje );

    SetBackGround( screenXml->getBackgroundXML().c_str() );

    createLabelXML();
    createInputXML();
    createIconXML();

    create();
}


void PanelCierreSubmedios::createLabelXML()
{
    char aux[100];
    screenXml->setLabelXML( label_mensaje, "Mensaje" );
    screenXml->setLabelXML( medios_title, "mediosTitle" );
    screenXml->setLabelXML( sub_medios_title, "submediosTitle" );
    medios_title->setVisible( true );
    sub_medios_title->setVisible( true );

    for( int i = 0;i < 10;i++ ) {
        memset( aux, 0, 100 );
        LabelSubmedios[i]->setParent( this );
        this->AddObject( LabelSubmedios[i] );
        sprintf( aux, "LabelSubmedio%d", i + 1 );
        screenXml->setLabelXML( LabelSubmedios[i], aux );

        memset( aux, 0, 100 );
        LabelSubmediosImporte[i]->setParent( this );
        this->AddObject( LabelSubmediosImporte[i] );
        sprintf( aux, "LabelSubmedioImporte%d", i + 1 );
        screenXml->setLabelXML( LabelSubmediosImporte[i], aux );

        memset( aux, 0, 100 );
        LabelSubmediosImporteDif[i]->setParent( this );
        this->AddObject( LabelSubmediosImporteDif[i] );
        sprintf( aux, "LabelSubmedioImporteDif%d", i + 1 );
        screenXml->setLabelXML( LabelSubmediosImporteDif[i], aux );

        memset( aux, 0, 100 );
        LabelMediosImporteInf[i]->setParent( this );
        this->AddObject( LabelMediosImporteInf[i] );
        sprintf( aux, "LabelMedioImporteInf%d", i + 1 );
        screenXml->setLabelXML( LabelMediosImporteInf[i], aux );

        memset( aux, 0, 100 );
        LabelMediosImporteDif[i]->setParent( this );
        this->AddObject( LabelMediosImporteDif[i] );
        sprintf( aux, "LabelMedioImporteDif%d", i + 1 );
        screenXml->setLabelXML( LabelMediosImporteDif[i], aux );
    }
}

void PanelCierreSubmedios::createInputXML()
{
    char aux[100];

    for( int i = 0;i < 10;i++ ) {
        memset( aux, 0, 100 );
        sprintf( aux, "InputSubmedio%d", i + 1 );
        screenXml->setInputXML( InputSubmedios[i], this, aux );
        InputSubmedios[i]->SetNotVisible();

        memset( aux, 0, 100 );
        sprintf( aux, "InputMedio%d", i + 1 );
        screenXml->setInputXML( InputMedios[i], this, aux );
        InputMedios[i]->SetNotVisible();
    }

    #ifdef INVEL_W
    dummy->SetNotVisible();
    #endif
}

void PanelCierreSubmedios::createIconXML()
{
    //   int x, y;
    //   CTLabel* label;

    screenXml->setIconXML( &medios1, "mediosPagos1" );
    screenXml->setIconXML( &medios2, "mediosPagos2" );
    screenXml->setIconXML( &medios3, "mediosPagos3" );
    screenXml->setIconXML( &medios4, "mediosPagos4" );
    screenXml->setIconXML( &medios5, "mediosPagos5" );
    screenXml->setIconXML( &medios6, "mediosPagos6" );
    screenXml->setIconXML( &medios7, "mediosPagos7" );
    screenXml->setIconXML( &medios8, "mediosPagos8" );
    screenXml->setIconXML( &medios9, "mediosPagos9" );
    screenXml->setIconXML( &medios10, "mediosPagos10" );

    medios1.setUseText( true );
    medios2.setUseText( true );
    medios3.setUseText( true );
    medios4.setUseText( true );
    medios5.setUseText( true );
    medios6.setUseText( true );
    medios7.setUseText( true );
    medios8.setUseText( true );
    medios9.setUseText( true );
    medios10.setUseText( true );


    //screenXml->setIconXML(&title, "ModoSubmedio");
    title.setVisible( true );
}

void PanelCierreSubmedios::create()
{
}


void PanelCierreSubmedios::init( const string &s )
{
    //   int i;

    cerearSubmedios();
    cerearMedios();

    med_index = 0;
    sub_med_index = 0;
}


void PanelCierreSubmedios::cerearSubmedios()
{
    //char buf[300];
	string buf;

    //sprintf( buf, "                       " );
    MutexGuiEnter();
    for( int i = 0;i <= 9;i++ ) {
        if( LabelSubmedios[i] && LabelSubmediosImporte[i] ) {
            LabelSubmedios[i]->setText( buf );
            LabelSubmedios[i]->setVisible( true );
            LabelSubmediosImporte[i]->setText( buf );
            LabelSubmediosImporte[i]->setVisible( true );
			LabelSubmediosImporteDif[i]->setText( buf );
            LabelSubmediosImporteDif[i]->setVisible( true );
        }
    }
	//Refresh();
    MutexGuiLeave();
}


void PanelCierreSubmedios::cerearMedios()
{
    char buf[10];

    sprintf( buf, " " );
    MutexGuiEnter();
    for( int i = 0;i <= 9;i++ ) {
        LabelMediosImporteInf[i]->setText( buf );
        LabelMediosImporteInf[i]->setVisible( true );
        LabelMediosImporteDif[i]->setText( buf );
        LabelMediosImporteDif[i]->setVisible( true );
    }
    //Refresh();
    MutexGuiLeave();
}

void PanelCierreSubmedios::mostrarImporteMedioInf( const string &s, const string &s1, int type )
{
    MutexGuiEnter();

    if( ( medio_selected >= 0 ) && ( medio_selected <= 9 ) ) {
        LabelMediosImporteInf[medio_selected]->setText( s1 );
        LabelMediosImporteInf[medio_selected]->setVisible( true );
        InputMedios[medio_selected]->SetNotVisible();
        //medio_selected = -1;
    }
    MutexGuiLeave();
}

void PanelCierreSubmedios::mostrarImporteMedioDif( const string &s, const string &s1, int type )
{
    MutexGuiEnter();

    if( ( medio_selected >= 0 ) && ( medio_selected <= 9 ) ) {
        LabelMediosImporteDif[medio_selected]->setText( s1 );
        LabelMediosImporteDif[medio_selected]->setVisible( true );
        InputMedios[medio_selected]->SetNotVisible();
        medio_selected = -1;
    }
    MutexGuiLeave();
}


void PanelCierreSubmedios::mostrarItemRendicion( const string &s, const string &s1, int type )
{
    char buf[512];

    MutexGuiEnter();
    if( ( medio_selected >= 0 ) && ( medio_selected <= 9 ) ) {
        InputMedios[medio_selected]->SetNotVisible();
        //medio_selected = -1;
    }

    if( type != 9 ) {
        sprintf( buf, "%d    %s", type + 1, s.c_str() );
    }
    else {
        sprintf( buf, "%d    %s", 0, s.c_str() );
    }
    LabelSubmedios[type]->setVisible( false );
    LabelSubmedios[type]->setText( buf );
    LabelSubmedios[type]->setVisible( true );
    MutexGuiLeave();
}


void PanelCierreSubmedios::resaltarMedioElegido( const string &s, int type )
{
    int step = 30;
    MutexGuiEnter();
    if( ( type >= 0 ) && ( type <= 9 ) ) {
        LabelMediosImporteInf[type]->setVisible( false );
        //LabelMediosImporteDif[type]->setVisible( false );
        InputMedios[type]->SetVisible();
        medio_selected = type;
    }
	//Refresh();
    MutexGuiLeave();
}


void PanelCierreSubmedios::resaltarSubmedioElegido( const string &s, int type )
{
    MutexGuiEnter();
    if( ( medio_selected >= 0 ) && ( medio_selected <= 9 ) ) {
        InputMedios[medio_selected]->SetNotVisible();
        //medio_selected = -1;
    }
    LabelSubmediosImporte[type]->setVisible( false );
    InputSubmedios[type]->SetVisible();
    submedio_selected = type;
    //Refresh();
    MutexGuiLeave();
}


void PanelCierreSubmedios::mostrarImporteSubmedio( const string &s, const string &s1, int type )
{
    MutexGuiEnter();
    InputSubmedios[submedio_selected]->SetNotVisible();
    LabelSubmediosImporte[type]->setText( s1 );
    LabelSubmediosImporte[type]->setVisible( true );
    //Refresh();
    //submedio_selected = -1;
    MutexGuiLeave();
}


void PanelCierreSubmedios::mostrarImporteSubmedioDif( const string &s, const string &s1, int type )
{ 
	MutexGuiEnter();
	InputSubmedios[submedio_selected]->SetNotVisible();
	LabelSubmediosImporteDif[type]->setText( s1 );
	LabelSubmediosImporteDif[type]->setVisible( true );	
    //Refresh();
    submedio_selected = -1;
	MutexGuiLeave();
}

void PanelCierreSubmedios::borrarImporteSubmedioDif()
{
	int i;
	MutexGuiEnter();
	for ( i = 0 ; i < 10 ; i++ )
		LabelSubmediosImporteDif[i]->setNotVisible();
	MutexGuiLeave();
}

void PanelCierreSubmedios::ocultarImporteSubmedioDif( int type )
{	
	MutexGuiEnter();
	LabelSubmediosImporteDif[type]->setNotVisible();
	//Refresh();
	MutexGuiLeave();
}

void PanelCierreSubmedios::mostrarSumaCierre( const string &s, const string &s1, int type, int x )
{ int i;
  i = 0;
  //cargo en la diferencia para que muestre
 
}
void PanelCierreSubmedios::mostrarSumaParcialMedio( const string &s, const string &s1, int type, int x )
{ int i;
  i = 0;
  //cargo en la diferencia para que muestre
  LabelMediosImporteDif[type]->setText( s1 );
  LabelMediosImporteDif[type]->setVisible( true );	
}
void PanelCierreSubmedios::mostrarSumaParcialSubmedio( const string &s, const string &s1, int type, int x )
{ int i;
  i = 0;
  LabelSubmediosImporteDif[type]->setText( s1 );
  LabelSubmediosImporteDif[type]->setVisible( true );	
}


void PanelCierreSubmedios::mostrarMsgPaginaCierre( int type )
{
    int i;
    i = 0;
}


void PanelCierreSubmedios::agregarMedioCierreSub( const string &s )
{
    //   char buf[1000];
    int step = 30;
    MutexGuiEnter();

    switch( med_index ) {
        case 0:
            medios1.setText( s );
            medios1.setVisible( true );
            break;
        case 1:
            medios2.setText( s );
            medios2.setVisible( true );
            break;
        case 2:
            medios3.setText( s );
            medios3.setVisible( true );
            break;
        case 3:
            medios4.setText( s );
            medios4.setVisible( true );
            break;
        case 4:
            medios5.setText( s );
            medios5.setVisible( true );
            break;
        case 5:
            medios6.setText( s );
            medios6.setVisible( true );
            break;
        case 6:
            medios7.setText( s );
            medios7.setVisible( true );
            break;
        case 7:
            medios8.setText( s );
            medios8.setVisible( true );
            break;
        case 8:
            medios9.setText( s );
            medios9.setVisible( true );
            break;
        case 9:
            medios10.setText( s );
            medios10.setVisible( true );
            break;
    }
    //Refresh();
    MutexGuiLeave();
    med_index++;
}


CInput * PanelCierreSubmedios::getFocus()
{
    if( submedio_selected != -1 ) {
        return InputSubmedios[submedio_selected];
    }
    else if( medio_selected != -1 ) {
        return InputMedios[medio_selected];
    }
    else {
        return dummy;
    }
}
