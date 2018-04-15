#include<iostream>
#include<fstream>
#include<cstring>
#include <complex>
#include<algorithm>
#define PI 3.141592654
using namespace std;
typedef complex<double> comp;
int tam=0;

comp * transformadaFourier(int N,float *y){
	comp *resp;
	resp=new comp[N];
	comp s(0,-2*PI*(1.0/N));
	s=exp(s);
	for(int n=0;n<N;n++){
		comp sum(0,0);
		for(int k=0;k<N;k++){
			comp aux((double)y[k],0);
			sum+=pow(s,n*k)*aux;
		}
		comp Nc(N,0);
		resp[n]=sum/Nc;
	}
	return resp;
}
float polinomioDeLagrangeEnPunto(float p, float *x,int i, float *y){
	float prod=1;
	for(int j=0;j<tam;j++){
		if(j!=i){
			prod=prod*((p-x[j])/(x[i]-x[j]));
		}
	}
	return prod;
	}

float interpolacionEnPunto(float p, float *x,float *y){
	float sum=0;
	for(int i=0;i<tam;i++){
		sum=sum+y[i]*polinomioDeLagrangeEnPunto(p,x,i,y);
	}
	return sum;
	} 
	

int main(int argc, char **argv){
	float *x;
	float *y;
	string nom(argv[1]);
	ifstream datos(nom); 
	string fila;
	while(getline(datos,fila)){
		tam+=1;
	}
	cout<<tam<<endl;
	x=new float[tam];
	y=new float[tam];
	datos.clear();
	datos.seekg(0,ios::beg);
	int i=0;
	while(getline(datos,fila)){
		int pos=fila.find(",");
		string dax=fila.substr(0,pos);
		string day=fila.substr(pos+1);
		x[i]=stof(dax);
		y[i]=stof(day);
		i++;
	}
	datos.close();
	
	float Interx=x[tam-1]-x[0];
	float h=Interx/(tam-1);
	
	float *xequi;
	float *yequi;
	
	xequi=new float[tam];
	yequi=new float[tam];
	
	for(int k=0;k<tam;k++){
		xequi[k]=x[0]+k*h;
		yequi[k]=interpolacionEnPunto(xequi[k],x,y);
	}
	cout<<xequi[5]<<","<<yequi[5]<<endl;
	
	comp *trans;
	trans=new comp[tam];
	float *frec;
	frec=new float[tam];
	
	trans= transformadaFourier(tam,yequi);
        cout<<trans[3]<<endl;
	for(int i=0;i<tam;i++){
		frec[i]=i*2*PI/(tam*xequi[1]-xequi[0]);
	}
	
	
	ofstream salida("transformada.txt");
	
	for(int k=0;k<tam;k++){
		salida<<frec[k]<<","<<real(trans[k])<<","<<imag(trans[k])<<endl;
	}
	delete [] x;
	delete [] y;
	delete [] trans;
	delete [] frec;
	salida.close();
	
	return 0;
	
	}
