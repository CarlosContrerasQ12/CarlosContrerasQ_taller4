#include <cstring>
#include <cstdlib>
#include <fstream>
#include <complex>
#include <iostream>
#define PI 3.141592654
using namespace std;
typedef complex<double> comp;

float distanciaNormalizada(int x,int y,int x0,int y0){
	float d=sqrt((x-x0)*(x-x0)+(y-y0)*(y-y0));
	return d;
}
comp * transformadaFourier(int N,float *y){
	comp *resp;
	resp=(comp*)malloc(sizeof(comp)*N);
	comp s(0,-2*PI*(1.0/N));
	s=exp(s);
	for(int n=0;n<N;n++){
		comp sum(0,0);
		for(int k=0;k<N;k++){
			comp aux((double)y[k],0);
			sum+=pow(s,n*k)*aux;
		}
		//comp Nc(N,0);
		//resp[n]=sum/Nc;
		resp[n]=sum;
	}
	return resp;
}

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
		//comp Nc(N,0);
		//resp[n]=sum/Nc;
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

float ** convertirAReales(int alto, int ancho, comp **resp2){
	float **resp3;
	resp3=new float*[alto];
	comp a(alto*ancho,0);
	for(int i=0;i<alto;i++){
		resp3[i]=new float[ancho];
	}
	for(int i=0;i<ancho;i++){
		for(int j=0;j<alto;j++){
			resp3[j][i]=(float)sqrt(norm(resp2[j][i]*a));
		}
	}
	
	return resp3;
	
}

comp ** shiftTransformada(int alto, int ancho,comp **XY){
	comp **resp;
	resp=new comp*[alto];
	for(int i=0;i<alto;i++){
		resp[i]=new comp[ancho];
}

	int al=alto/2;
	int an=ancho/2;
	for(int j=0;j<alto;j++){
		for(int i=0;i<ancho;i++){
			if(j<al){
				if(i<an){
					resp[j+al][i+an]=XY[j][i];
				}
				else{
					resp[j+al][i-an]=XY[j][i];
				}		
			}
			else{
				if(i<an){
					resp[j-al][i+an]=XY[j][i];
				}
				else{
					resp[j-al][i-an]=XY[j][i];
				}	
			}
			
			
		}
		
	}

	return resp;
}

comp ** revertirShift(int alto, int ancho,comp **XY){
	comp **resp;
	resp=new comp*[alto];
	for(int i=0;i<alto;i++){
		resp[i]=new comp[ancho];
	}

	int al=(alto/2);
	int an=(ancho/2);
	for(int j=0;j<alto;j++){
		for(int i=0;i<ancho;i++){
			if(j<al){
				if(i<an){
					resp[j+al][i+an]=XY[j][i];
				}
				else{
					resp[j+al][i-an]=XY[j][i];
				}		
			}
			else{
				if(i<an){
					resp[j-al][i+an]=XY[j][i];
				}
				else{
					resp[j-al][i-an]=XY[j][i];
				}	
			}
			
			
		}
		
	}
	
	
	return resp;
	
	}
comp ** filtroPasaBajas(int alto,int ancho, float maxR,float w, comp **XY){
	comp **resp;
	resp=new comp*[alto];
	for(int i=0;i<alto;i++){
		resp[i]=new comp[ancho];
	}
	int al=alto/2;
	int an=ancho/2;
	for(int j=0;j<alto;j++){
		for(int i=0;i<ancho;i++){
			float p=distanciaNormalizada(i,j,al,an);
			if(p<(maxR-w)){
			resp[j][i]=XY[j][i];
			}
			else if(p>(maxR+w)){
			comp a(0,0);
			resp[j][i]=a;
			}
			else{
			comp a(0.5*(1-sin(PI*(p-w)/2*w)),0);
			resp[j][i]=a*XY[j][i];
			}
			
		
		}
		
	}
	return resp;

}

comp ** filtroPasaAltas(int alto,int ancho, float maxR,float w,comp **XY){
	comp **resp;
	resp=new comp*[alto];
	for(int i=0;i<alto;i++){
		resp[i]=new comp[ancho];
	}
	int al=alto/2;
	int an=ancho/2;
	for(int j=0;j<alto;j++){
		for(int i=0;i<ancho;i++){
			float p=distanciaNormalizada(i,j,al,an);
			if(p<(maxR-w)){		
			comp a(0,0);
			resp[j][i]=a;
			}
			else if(p>(maxR+w)){
			resp[j][i]=XY[j][i];
			}
			else{
			comp a(((0.5*(1-sin(PI*(p-w)/2*w)))*-1)+1,0);
			resp[j][i]=a*XY[j][i];
			}
			
		
		}
		
	}
	return resp;

}

comp ** convertirAComplejos(int alto, int ancho, float **xy){
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
	
	float ** red;
	red=new float*[alto];
	float ** green;
	green=new float*[alto];
	float ** blue;
	blue=new float*[alto];
	for (int i = 0; i < alto; i++) {
        red[i] = new float[ancho];
        green[i] = new float[ancho];
        blue[i] = new float[ancho];
    }
    for(int y = 0; y < alto; y++) {
		for(int x = 0; x < ancho; x++) {
			getline(archivored,vrojo,',');
			getline(archivogreen,vazul,',');
			getline(archivoblue,vverde,',');
			red[y][x]=static_cast<float>(stof(vrojo));
			green[y][x]=static_cast<float>(stof(vazul));
			blue[y][x]=static_cast<float>(stof(vverde));
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
    
    
	if(ancho%2==1){
		ancho=ancho-1;
		}
	if(alto%2==1){
		alto=alto-1;
		}
		
	cout<<"Calculando transformada de componente roja..."<<endl;
	transformadared=transformadaFourierBidimensional(alto,ancho,convertirAComplejos(alto,ancho,red));
	cout<<"   Calculada transformada"<<endl;
	
	cout<<"   Realizando Shift a la transformada"<<endl;
	transformadared=shiftTransformada(alto,ancho,transformadared);
	cout<<"   Shifteada transformada"<<endl;
	
	cout<<"	  Aplicando filtro"<<endl;
	string tipo;
	if(argv[2][0]=='a'){
	transformadared=filtroPasaAltas(alto,ancho,30.0,5.0,transformadared);
	tipo=" altas.png";
	}
	else{
	transformadared=filtroPasaBajas(alto,ancho,30.0,5.0,transformadared);
	tipo=" bajas.png";
	}
	
	cout<<"	  Aplicado filtro"<<endl;
	
	cout<<"   Revirtiendo shift a transformada"<<endl;
	transformadared=revertirShift(alto,ancho,transformadared);
	cout<<"   Revertida transformada"<<endl;
	
	cout<<"   Calculadando transformada inversa"<<endl;
	red=convertirAReales(alto,ancho,transformadaInversaFourierBidimensional(alto,ancho,transformadared));
	cout<<"   Calculada inversa"<<endl;
	
	
		
		
		
		
	cout<<"Calculando transformada de componente azul..."<<endl;
	transformadablue=transformadaFourierBidimensional(alto,ancho,convertirAComplejos(alto,ancho,blue));
	cout<<"   Calculada transformada"<<endl;
	
	cout<<"   Realizando Shift a la transformada"<<endl;
	transformadablue=shiftTransformada(alto,ancho,transformadablue);
	cout<<"   Shifteada transformada"<<endl;
	
	cout<<"	  Aplicando filtro"<<endl;
	if(argv[2][0]=='a'){
	transformadablue=filtroPasaAltas(alto,ancho,30.0,5.0,transformadablue);
	}
	else{
	transformadablue=filtroPasaBajas(alto,ancho,30.0,5.0,transformadablue);
	}
	
	cout<<"	  Aplicado filtro"<<endl;
	
	cout<<"   Revirtiendo shift a transformada"<<endl;
	transformadablue=revertirShift(alto,ancho,transformadablue);
	cout<<"   Revertida transformada"<<endl;
	
	cout<<"   Calculadando transformada inversa"<<endl;
	blue=convertirAReales(alto,ancho,transformadaInversaFourierBidimensional(alto,ancho,transformadablue));
	cout<<"   Calculada inversa"<<endl;
	
	
		
	
	cout<<"Calculando transformada de componente verde..."<<endl;
	transformadagreen=transformadaFourierBidimensional(alto,ancho,convertirAComplejos(alto,ancho,green));
	cout<<"   Calculada transformada"<<endl;
	
	cout<<"   Realizando Shift a la transformada"<<endl;
	transformadagreen=shiftTransformada(alto,ancho,transformadagreen);
	cout<<"   Shifteada transformada"<<endl;
	
	cout<<"	  Aplicando filtro"<<endl;
	if(argv[2][0]=='a'){
	transformadagreen=filtroPasaAltas(alto,ancho,30.0,5.0,transformadagreen);
	}
	else{
	transformadagreen=filtroPasaBajas(alto,ancho,30.0,5.0,transformadagreen);
	}
	
	cout<<"	  Aplicado filtro"<<endl;
	
	cout<<"   Revirtiendo shift a transformada"<<endl;
	transformadagreen=revertirShift(alto,ancho,transformadagreen);
	cout<<"   Revertida transformada"<<endl;
	
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
    system((py2+nombre+tipo).c_str());

	
	return 0;
}
