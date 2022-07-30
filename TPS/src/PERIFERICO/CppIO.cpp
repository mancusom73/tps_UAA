#include <iostream>
#include <fstream>

using namespace std;

extern "C" {
	void cio_Write( char *buffer, int cant, char *filename ){
		ofstream file(filename,ios_base::app);
		file.write(buffer,cant);
		file.close();
	}
}