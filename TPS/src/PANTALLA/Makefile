
####################### GENERAL SETTINGS ############################

INCLUDE = ../include
INCLUDEW = ../includew
XERCES = /home/marcost/xerces-c_2_6_0-redhat_80-gcc_32/include

top_builddir = /usr/local
top_srcdir = /usr/local
EXTRALIBS = -pthread   -Wl,--version-script,$(top_builddir)/version-script -L/usr/lib -L/usr/X11R6/lib -lgtk -lgdk -rdynamic -lgmodule -lgthread -lglib -lpthread -ldl -lXi -lXext -lX11 -lm -lpng -ljpeg -ltiff -lz -ldl -lm 
LDLIBS = ${APPEXTRALIBS} ${top_builddir}/lib/libwx_gtkd-2.4.so.0.1.1 ${EXTRALIBS}

TOOLKIT = GTK
TOOLKITDIR = gtk

WXLIB = libwx_gtk-2.4.a
WXSHLIB = libwx_gtk-2.4.so.0.1.1
WXLIBBASE = wx_gtk

CXX         = c++
CC          = gcc


########################### Flags #################################

CPPFLAGS = ${APPEXTRACPPFLAGS} -I${INCLUDE} -I${INCLUDEW} -I${top_builddir}/lib/wx/include/gtk-2.4 -I/usr/include/gtk-2.0 -I/usr/include/glib-1.2 -I/usr/lib/glib/include -D_REENTRANT -I/usr/X11R6/include -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES $(EXTRADEFS) $(APPEXTRADEFS)  -D__WXGTK__ -DGTK_NO_CHECK_CASTS
CFLAGS = ${APPEXTRACFLAGS} ${CPPFLAGS} -O2 -MMD -pthread
CXXFLAGS = ${APPEXTRACXXFLAGS} ${CPPFLAGS} -ggdb -DINVEL_L  -Wno-deprecated -MMD -pthread
PICFLAGS = -fPIC


OBJECTS = AppMain.o CBrowse.o CFontMapper.o CGrid.o CIMain.o CInput.o CLabel.o CObject.o\
			CPanelBrowse.o CPanel.o GuiProperties.o IniEntity.o IniParser.o IPantalla.o\
			MainPanel.o MainThread.o PanelBrowse.o PanelCierre.o PanelCierreSubmedios.o\
         PanelCobro.o PanelFondoFijo.o PanelInput1.o PanelInput.o PanelMenu.o PanelPagos.o\
         PanelPresentacion.o PanelRetiros.o PanelStandby.o PanelVentas.o PanelCambioMedio.o\
         PanelPagosCaja.o XmlRpcClient.o XmlRpcDispatch.o XmlRpcServerConnection.o \
         XmlRpcServer.o XmlRpcServerMethod.o XmlRpcSocket.o	XmlRpcSource.o XmlRpcUtil.o\
         XmlRpcValue.o CFlag.o CIcon.o CColor.o CColumn.o CComponent.o CComponentRef.o \
         CDesktop.o CForms.o CImage.o CImageRef.o CImages.o\
         Components.o CRepository.o CSize.o CSkin.o CStates.o  CText.o CWindow.o\
         CWindows.o CScreencfg.o CFont.o PanelInicio.o PanelGrill.o CLang.o CMessage.o CForm.o\
         CInputDesc.o PanelPagosEsp.o PanelCobrosEsp.o PanelDeudaSocio.o CConfig.o PanelMedioReint.o

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $(APPEXTRADEFS)  $< -o $@
			
   
all:  $(OBJECTS)

