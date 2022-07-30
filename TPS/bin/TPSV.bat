%dir%\xsltproc\xsltproc.exe  -v --output pom.txt --stringparam scope runtime %dir%\pom.xsl %dir%\pom.xml
@if errorlevel 10 goto ERROR_VERSION
@if errorlevel 4 goto ERROR_ARCHIVO
@if errorlevel 0 goto OK
goto ERROR_DESCONOCIDO
:OK
pskill W32MKDE
pskill NTMKDE
pskill TPSV.exe
pskill mplayer.exe
net stop "BTI Microkernel Engine"
net start "BTI Microkernel Engine"
rem
set path=c:\workcvs\TPV4MYSQ\TPVIV-uaa\bin\;c:\workcvs\TPV4MYSQ\TPVIV-uaa\bin\lib;
cd ..\..\TPVSIS\DATOS\TABLAS
c:\workcvs\TPV4MYSQ\TPVIV-uaa\bin\TPSV.exe path=. drive_virtual:C setcaja:9 no_glog_actions
EXIT
