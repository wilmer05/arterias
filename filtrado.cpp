#include<iostream>
#include<cstring>
#include<cstdio>
#include<vector>
#include<queue>
#include<string>
#define eps (1e-8)
using namespace std;


//Estructura auxiliar para procesar por ejes
struct aux{
	double val;
	string predicado;
	int x,y,z;
	aux(){}
	aux(double v, int xs, int ys, int zs, string pred) : val(v),x(xs),y(ys),z(zs),predicado(pred){}


	bool operator<(const aux &otr) const{
		return val - otr.val > eps;
	}

};

/*	Proceso que filtra los k datos mas probables dado un predicado

	Parametros:
		predicado --> predicado por el cual se procedera a filtrar
		archivo_entrada --> indica el nombre del archivo de entrada
		archivo_salida --> indica el nombre del archivo de salida
		k --> valor que representa cuantos valores mas probables se quiere en cada valor del eje analizado
		filtro --> doouble que indica que probabilidad se quiere que superen los datos filtrados
		fil --> indica si se quiere filtrar(true) o no (false)
*/

void procesar_predicado(char *predicado,char *archivo_entrada, char *archivo_salida, int k,double filtro,bool fil){

	priority_queue<aux>todos;
	FILE *file = fopen(archivo_entrada,"r");
	FILE *file2 = fopen(archivo_salida,"w");
	char auxiliar[200];
	char otro;
	double tmp;
	int tx,ty,tz;
	fseek(file,14,SEEK_SET);
	while(fscanf(file," '%[^(](%d,%d,%d)': %lf",auxiliar,&tx,&ty,&tz,&tmp)==5){
		aux nuev = aux(tmp,tx,ty,tz,string(auxiliar));
		int pos;
		if((!fil || tmp-filtro>eps) && !strcmp(auxiliar,predicado))
			todos.push(nuev);
		if(todos.size()>k) todos.pop();	

		fscanf(file,"%c",&otro);
		if(otro=='}') break;
	}
	
	fprintf(file2,"{");
	for(int j =0 ; j<k && todos.size();j++){
		aux tem = todos.top();
		fprintf(file2,"'%s(%d,%d,%d)': %lf",tem.predicado.c_str(),tem.x,tem.y,tem.z,tem.val);
		todos.pop();
		if(todos.size()) fprintf(file2,", ");
		else fprintf(file2,"}");
	}
	fprintf(file2,"\n");

	
	fclose(file);
	fclose(file2);

	
}



/*	Proceso que filtra los k datos mas probables para todos los posibles valores de ese eje en el rango [0,dim]
	
	Parametros:
		eje_x --> es un booleano que indica si el eje a analizar es el eje x
		eje_y --> es un booleano que indica si el eje a analizar es el eje y
		eje_z --> es un booleano que indica si el eje a analizar es el eje z
		dim --> es un entero que indica de que tamanio del eje a a anlizar
		archivo_entrada --> indica el nombre del archivo de entrada
		archivo_salida --> indica el nombre del archivo de salida
		k --> valor que representa cuantos valores mas probables se quiere en cada valor del eje analizado
	
*/

void procesar_eje(bool eje_x, bool eje_y, bool eje_z,int dim,char *archivo_entrada, char *archivo_salida, int k){

	if((eje_x && (eje_y||eje_z)) || ((eje_x || eje_y) && eje_z) || (eje_y && (eje_z && eje_x))) {
		fprintf(stderr,"Solo uno de los parametros booleanos de procesar_eje debe ser true\n");
		return ;
	}

	vector<priority_queue<aux> > todos(dim+1);
	FILE *file = fopen(archivo_entrada,"r");
	FILE *file2 = fopen(archivo_salida,"w");
	char auxiliar[200];
	char otro;
	double tmp;
	int tx,ty,tz;
	fseek(file,14,SEEK_SET);
	while(fscanf(file," '%[^(](%d,%d,%d)': %lf",auxiliar,&tx,&ty,&tz,&tmp)==5){
		aux nuev = aux(tmp,tx,ty,tz,string(auxiliar));
		int pos;
		if(eje_x) pos = tx;
		else if(eje_y) pos = ty;
		else pos = tz;
		todos[pos].push(nuev);
		if(todos[pos].size()>k) todos[pos].pop();	
		fscanf(file,"%c",&otro);
		if(otro=='}') break;
	}
	
	for(int i =0 ; i < dim+1;i++){
		fprintf(file2,"%d {",i);
		for(int j =0 ; j<k && todos[i].size();j++){
			aux tem = todos[i].top();
			fprintf(file2,"'%s(%d,%d,%d)': %lf",tem.predicado.c_str(),tem.x,tem.y,tem.z,tem.val);
			todos[i].pop();
			if(todos[i].size()) fprintf(file2,", ");
			else fprintf(file2,"}");
		}
		fprintf(file2,"}\n");
	}

	
	fclose(file);
	fclose(file2);

	
}


int main(){

	char in[200]="salida.infer";
	char out[200]="salida4";
	char pred[200]="center";
	procesar_eje(false,true,false,10,in,out,2);
	out[6]='5';
	procesar_predicado(pred,in,out,10,0.5,true);

	return 0;
}

