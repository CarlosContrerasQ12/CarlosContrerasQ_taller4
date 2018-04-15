#include <cstring>
#include <cstdlib>
#include <fstream>
#include <complex>
#include <iostream>
#define PI 3.1415926535897932384626433832795
using namespace std;
typedef complex<double> comp;



comp * transformadaFourierCompleja(int N,comp *y){
	comp *resp;
	resp=(comp*)malloc(sizeof(comp)*N);
	comp s(0,-2*PI*(1.0/N));
	s=exp(s);
	for(int n=0;n<N;n++){
		comp sum(0,0);
		for(int k=0;k<N;k++){
			sum+=pow(s,n*k)*y[k];
		}
		resp[n]=sum;
	}
	return resp;
}

comp ** transformadaFourierBidimensional(int alto, int ancho, comp **xy){
	comp **resp;
	resp=new comp*[alto];
	for(int i=0;i<alto;i++){
		resp[i]=new comp[ancho];
	}
	for(int i=0;i<alto;i++){
		resp[i]=transformadaFourierCompleja(ancho,xy[i]);
	}
	comp **aux;
	aux=new comp*[ancho];
	for(int i=0;i<ancho;i++){
		comp *col;
		col=(comp*)malloc(sizeof(comp)*alto);
		for(int j=0;j<alto;j++){
			col[j]=resp[j][i];
		}
		aux[i]=transformadaFourierCompleja(alto,col);
	}
	for(int j=0;j<alto;j++){
		for(int i=0;i<ancho;i++){
			resp[j][i]=aux[i][j];
		}
		
	}
	return resp;
	
	
	
}

comp * transformadaInversaFourier(int N,comp *Y){
	comp *resp;
	resp=(comp*)malloc(sizeof(comp)*N);
	comp s(0,2*PI*(1.0/N));
	s=exp(s);
	for(int n=0;n<N;n++){
		comp sum(0,0);
		for(int k=0;k<N;k++){
			sum+=pow(s,n*k)*Y[k];
		}
		resp[n]=sum;
	}
	return resp;
}

comp ** transformadaInversaFourierBidimensional(int alto,int ancho,comp **XY){
	
	comp **aux;
	aux=new comp*[ancho];
	for(int i=0;i<ancho;i++){
		comp *col;
		col=(comp*)malloc(sizeof(comp)*alto);
		for(int j=0;j<alto;j++){
			col[j]=XY[j][i];
		}
		aux[i]=transformadaInversaFourier(alto,col);
	}
	comp **aux2;
	aux2=new comp*[alto];
	for(int i=0;i<alto;i++){
		comp *fil;
		fil=(comp*)malloc(sizeof(comp)*ancho);
		for(int j=0;j<ancho;j++){
			fil[j]=aux[j][i];
		}
		aux2[i]=transformadaInversaFourier(ancho,fil);
	}
	return aux2;
}

double ** convertirAReales(int alto, int ancho, comp **resp2){
	double **resp3;
	comp a(alto*ancho,0);
	resp3=new double*[alto];
	for(int i=0;i<alto;i++){
		resp3[i]=new double[ancho];
	}
	for(int i=0;i<ancho;i++){
		for(int j=0;j<alto;j++){
			
			resp3[j][i]=sqrt(norm(resp2[j][i]*a));
		}
	}
	
	return resp3;
	
}





comp ** convertirAComplejos(int alto, int ancho, double **xy){
	comp **resp3;
	resp3=new comp*[alto];
	for(int i=0;i<alto;i++){
		resp3[i]=new comp[ancho];
	}
	for(int i=0;i<ancho;i++){
		for(int j=0;j<alto;j++){
			comp a(xy[j][i],0);
			resp3[j][i]=a;
		}
	}
	
	return resp3;
	

}

double * darArregloDeGaussiana(int tam,double sigma){
	double h=(2*sigma)/(tam-1);
	double *resp;
	resp=new double[tam];
	for(int i=0;i<tam;i++){
		double x=-1*sigma+(h*i);
		double b=1/(sqrt(sigma*PI*2));
		resp[i]=b*exp(-1*(1/(2*sigma))*x*x);
	}
	return resp;
	}


void generarKernel(int alto, int ancho, double **xy,double sigma){
	double *bu;
	bu=darArregloDeGaussiana(20,sigma);
	for(int i=0;i<ancho;i++){
		for(int j=0;j<alto;j++){
			if(i<20 && j<20){
				xy[j][i]=bu[i]*bu[j];
			}
			else{
				xy[j][i]=0.0;
			}		 			
		}
	}

	
}

comp ** convolucionarConKernel(int alto, int ancho, comp **XY,comp **xy){
	comp **resp;
	resp=new comp*[alto];
	for(int i=0;i<alto;i++){
		resp[i]=new comp[ancho];
	}
	
	for(int i=0;i<ancho;i++){
		for(int j=0;j<alto;j++){
			resp[j][i]=XY[j][i]*xy[j][i];
		}
	}
	return resp;
	
	}

int main(int argc, char **argv) {
	cout<<"Leyendo imagen con python"<<endl;

	string py="python LeerImagen.py ";
	string nombre(argv[1]);
	system((py+nombre).c_str());
	cout<<"Imagen leida"<<endl;
	ifstream archivored("rojo.txt");
	ifstream archivogreen("verde.txt");
	ifstream archivoblue("azul.txt");
	string vrojo;
	string vverde;
	string vazul;
	getline(archivored,vrojo,',');
	int alto=stoi(vrojo);
	getline(archivored,vrojo,',');
	int ancho=stoi(vrojo);
	cout<<"La imagen tiene un tamano de "<<alto<<','<<ancho<<endl;
	
	double ** red;
	red=new double*[alto];
	double ** green;
	green=new double*[alto];
	double ** blue;
	blue=new double*[alto];
	for (int i = 0; i < alto; i++) {
        red[i] = new double[ancho];
        green[i] = new double[ancho];
        blue[i] = new double[ancho];
    }
    for(int y = 0; y < alto; y++) {
		for(int x = 0; x < ancho; x++) {
			getline(archivored,vrojo,',');
			getline(archivogreen,vazul,',');
			getline(archivoblue,vverde,',');
			red[y][x]=static_cast<double>(stod(vrojo));
			green[y][x]=static_cast<double>(stod(vazul));
			blue[y][x]=static_cast<double>(stod(vverde));
		}
	}
	archivored.close();
	archivogreen.close();
	archivoblue.close();
	
	comp **transformadared;
	transformadared=new comp*[alto];
	comp **transformadagreen;
	transformadagreen=new comp*[alto];
	comp **transformadablue;
	transformadablue=new comp*[alto];
	
	for (int i = 0; i < alto; i++) {
        transformadared[i] = new comp[ancho];
        transformadagreen[i] = new comp[ancho];
        transformadablue[i] = new comp[ancho];
    }
    
    double **kernel;
    kernel =new double*[alto];
    comp ** transKernel;
    transKernel=new comp*[alto];
    
    for (int i = 0; i < alto; i++) {
        kernel[i] = new double[ancho];
        transKernel[i]=new comp[ancho];
    }
    
    string sig(argv[2]);
    generarKernel(alto,ancho,kernel,stod(sig));
   
    cout<<"Calculando transforada de Kernel"<<endl;
    transKernel=transformadaFourierBidimensional(alto,ancho,convertirAComplejos(alto,ancho,kernel));
    
	if(ancho%2==1){
		ancho=ancho-1;
		}
	if(alto%2==1){
		alto=alto-1;
		}
		
	cout<<"Calculando transformada de componente roja..."<<endl;
	transformadared=transformadaFourierBidimensional(alto,ancho,convertirAComplejos(alto,ancho,red));
	cout<<"   Calculada transformada"<<endl;
	
	cout<<"   Convolucionando con kernel"<<endl;
	transformadared=convolucionarConKernel(alto,ancho,transformadared,transKernel);
	cout<<"	  Convolucion Realizada"<<endl;
	
	cout<<"   Calculadando transformada inversa"<<endl;
	transformadared=transformadaInversaFourierBidimensional(alto,ancho,transformadared);
	cout<<transformadared[50][50]<<endl;
	red=convertirAReales(alto,ancho,transformadared);
	cout<<"   Calculada inversa"<<endl;
	
	
		
		
		
		
	cout<<"Calculando transformada de componente azul..."<<endl;
	transformadablue=transformadaFourierBidimensional(alto,ancho,convertirAComplejos(alto,ancho,blue));
	cout<<"   Calculada transformada"<<endl;
	
	
	cout<<"   Convolucionando con kernel"<<endl;
	transformadablue=convolucionarConKernel(alto,ancho,transformadablue,transKernel);
	cout<<"	  Convolucion Realizada"<<endl;

	cout<<"   Calculadando transformada inversa"<<endl;
	blue=convertirAReales(alto,ancho,transformadaInversaFourierBidimensional(alto,ancho,transformadablue));
	cout<<"   Calculada inversa"<<endl;
	
	
		
	
	cout<<"Calculando transformada de componente verde..."<<endl;
	transformadagreen=transformadaFourierBidimensional(alto,ancho,convertirAComplejos(alto,ancho,green));
	cout<<"   Calculada transformada"<<endl;
	
	cout<<"   Convolucionando con kernel"<<endl;
	transformadagreen=convolucionarConKernel(alto,ancho,transformadagreen,transKernel);
	cout<<"	  Convolucion Realizada"<<endl;
		
	cout<<"   Calculadando transformada inversa"<<endl;
	green=convertirAReales(alto,ancho,transformadaInversaFourierBidimensional(alto,ancho,transformadagreen));
	cout<<"   Calculada inversa"<<endl;
	
	
	for (int i = 0; i < alto; i++) {
        delete[] transformadared[i];
        delete[] transformadagreen[i];
        delete[] transformadablue[i];
    }
    delete[] transformadared;
    delete[] transformadagreen;
    delete[] transformadablue;
	
	
	ofstream rd("rojoc.txt");
	ofstream gr("verdec.txt");
	ofstream bu("bluec.txt");
	for(int y = 0; y < alto; y++) {
		for(int x = 0; x < ancho; x++) {
			rd<<red[y][x]<<",";
			gr<<green[y][x]<<",";
			bu<<blue[y][x]<<",";
		}
		rd<<endl;
		gr<<endl;
		bu<<endl;
	}
	rd.close();
	gr.close();
	bu.close();
	
	for (int i = 0; i < alto; i++) {
        delete[] red[i];
        delete[] green[i];
        delete[] blue[i];
    }
    delete[] red;
    delete[] green;
    delete[] blue;
	
	
	cout<<"Escribiendo imagen con python"<<endl;
	string py2="python PintarImagen.py ";
	string s=" sue";
    system((py2+nombre+s).c_str());

	
	return 0;
}
