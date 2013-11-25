#include<iostream>
#include<cstring>
#include<cstdio>
using namespace std;

FILE *file,*file2;
char archivo_lectura[200]="spm00_256x256x768.raw";
char archivo_escritura[200]="salida2";
char prefijo[200]="pre";
char sufijo[200]="suf";
const char separador=',';
const char apertura_tupla='(';
const char cerrar_tupla=')';
const int dimx=256; //dimension en x
const int dimy=256; //dimension en y
const int dimz=427; // dimension en z

int main(){
	file = fopen(archivo_lectura,"rb");
	file2 = fopen(archivo_escritura,"w");
	unsigned short leido,aux;
	int tam =0;
	fseek(file,0,SEEK_END);
	tam = ftell(file);
	rewind(file);

	const int saltar = tam - dimx * dimy * dimz *2;

	char leer[2];
	
	fseek(file,saltar,SEEK_SET);
	for(int k=0;k<dimz;k++)
		for(int j =0;j<dimy;j++)
			for(int i =0;i<dimx;i++){
					fread(leer,2,1,file);
					leido=(unsigned short) leer[0];
					leido<<=8;
					leido+=(unsigned short) leer[1];
					fprintf(file2,"%s %c%d%c%d%c%d%c%hu%c\n%s\n",prefijo,apertura_tupla,i,separador,j,separador,k,separador,leido,cerrar_tupla,sufijo);
			}
		

	fclose(file);
	fclose(file2);
	return 0;
}
