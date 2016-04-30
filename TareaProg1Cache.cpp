/* Andres Sanchez Delgado
   B05832
   ansanc7@gmail.com 88140943

   $   ./mi_cache   <asociatividad>   <tamaño   del   caché>   <tamaño   del bloque>
   Compilar en linux con g++: g++ TP1_CMESEN.cpp -o cache
   Archivo Necesario: prueba.txt
   Ejecutar en el bash: ./cache 0 256 32  
   Asociatividad 0: Directamente mapeada
                 1: 4 way-associative
								 2: 2 way-associative*/
			

#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <bitset>
#include <limits>
#include <vector>
#include <sstream>
#include <ctime>

using namespace std;

/*
implementar funciones
int LeerCache();
void EscribirCache();
*/

	//Variables Externas
	string sBits_byte_offset, sBits_index, sBits_tag;
	string dirHex, sBits_direccion, sLine; // Strings que contienen la direccion hexadecimal del dato leido del archivo (dirHex), 
//los bits en binario de la direccion leida del archivo y la linea leida del archivo (sLine)

	string HexStringABinString (string dirHex);

	void BinStringAMultipleString (string sBits_direccion, int tam_index, int tam_tag, int tam_byte_offset);

int main(int argc, char *argv[]){

//Variables
	int Asociatividad;
	int TamCache;
	int TamBloque;
	int TamTag;
	int TamIndice;
	int LongCad = 9;
	int LongPalabra = 32;
  int LE=0;
	char Direccion[LongCad];
	char DirTemp[LongCad];
  char LecturaEscritura[LongCad];
	string TipoCache;
	int NumBloques;
	int ByteOffset;
	int Filas;
	int Columnas;
	double HitRatio = 0.0, MissRatio = 0.0;
  // Valor decimal del tag,index y byte offset del la linea leida y cantidad de misses y hits en el cache
  unsigned long decimal_tag, decimal_index, decimal_byte_offset, misses = 0, hits = 0, Cont = 0; 


//Objeto de tipo ofstream que permite leer el archivo 
  ifstream archivo;

//Valida que la cantidad de parámetros ingresados al programa sea la necesaria
	if(argc!=4){
  	cout << "\nError! debe ingresar 3 parámetros...\n" << endl;
    return -1;
   }
	else{//1
		Asociatividad = atoi(argv[1]);
		TamCache = atoi(argv[2]);
    TamBloque = atoi(argv[3]);
		NumBloques = TamCache/TamBloque;
		ByteOffset = log2(TamBloque);
		TamTag=LongPalabra-(Asociatividad+ByteOffset);
		cout << "\nAsociatividad:" << argv[1] << endl;
		cout << "Tamaño de la cache:" << argv[2] << endl;
		cout << "Tamaño del bloque:" << argv[3] << "\n" << endl;

//Dependiendo de la asociatividad elije la estructura de datos para la cache; vector o matriz
		switch(Asociatividad){//2	
    	case(0):{
				TipoCache = "Directo.";
				Filas = NumBloques;
				Columnas = 2;
				TamIndice = log(NumBloques)/log(2);
				cout << "BytesIndex:" << TamIndice << "\n" << endl;
			}
			break; 
			case(1):{
				TipoCache = "4-way associative.";
				Filas = NumBloques/4;
				Columnas = 8;
				TamIndice = log(NumBloques/4)/log(2);
			}
			break;
			case(2):{
				TipoCache = "2-way associative.";
				Filas = NumBloques/2;
				Columnas = 4;
				TamIndice = TamIndice = log(NumBloques/2)/log(2);
			}
			break;
			default:
				TipoCache = "No implementado. Error!.";
    }//2

		if(TipoCache == "No implementado. Error!."){
			cout << "\nCache de tipo: " << TipoCache << "\n" << endl;
  		exit(-2);
			}
		else{//3
			cout << "\nCache de tipo: " << TipoCache << endl;
			cout << "Bloques en la Cache: " << NumBloques << endl;
			cout << "ByteOffset: " << ByteOffset << endl;

			TamTag=LongPalabra-(TamIndice+ByteOffset);
    
			//Define la estructura de datos de la cache.
			//Es una matriz de longitud NumBloques X Sets + bit de válido 
			unsigned long Cache[Filas][Columnas]; 
			cout << "\nMatriz de: " << Filas << "X" << Columnas << "\n" << endl;
				
			//Setea el valor del bit de válido en cero, cache vacia.
			for(int i=0;i<Filas;i++){
				for(int j=0;j<Columnas;j++)
					Cache[i][j] = 0;
			}

			cout << "'Cache Vacia:'\n"<< endl;
			for(int i=0;i<Filas;i++){
				for(int j=0;j<Columnas;j++){
					cout << Cache[i][j] << " ";
				}
					cout << endl;
			}

  		//Apertura del archivo para su lectura
			archivo.open("prueba1.txt",ifstream::in);

  		//Verifica que el archivo se haya abierto correctamente
			if (archivo.fail()){
				cout << "\nError al abrir el archivo\n" << endl;
				return -3;
			}
			else{//4
				cout << "\nArchivo abierto con éxito\n" << endl;

      int indice0 = 0;
			int indice1 = 0;
				//Inicio del ciclo de lectura	
				//Obtiene la línea de dirección
			while(archivo.getline(Direccion,LongCad,' ')){
				Cont++;				
   			sBits_byte_offset = ""; // Inicializacion de los strings.
	 			sBits_index = "";
  			sBits_tag = ""; 
 				LE=0; 
	
				//Obtiene la línea de lectura/escritura
				archivo.getline(LecturaEscritura,LongCad,'\n');

				cout << "Dirección: " << Direccion << endl;
				cout << "Acción: " << LecturaEscritura << endl;

				//Convierte la dirección en hexadecimal a binario
				
				sBits_direccion = HexStringABinString (Direccion);
						
				cout << "Direccion binaria: " << sBits_direccion << endl;

				//Separa la dirección en Tag, Index y Offset
				BinStringAMultipleString (sBits_direccion, TamIndice, TamTag, ByteOffset);

				cout << "Tag: " << sBits_tag << endl;
				cout << "Index: " << sBits_index << endl;
				cout << "Offset: " << sBits_byte_offset << endl;

    		/*----------------------------------------------------------------------
 				*  Conversion de bits a decimal index, BO y tag
     		* -------------------------------------------------------------------*/
    		// Convierte el string que contiene  los bits del tag a un numero decimal.
				decimal_tag = bitset<numeric_limits<unsigned long>::digits>(sBits_tag).to_ulong(); 
	
				// Convierte el string que contiene  los bits del índice a un numero decimal.
				decimal_index = bitset<numeric_limits<unsigned long>::digits>(sBits_index).to_ulong();
	
		   	// Convierte el string que contiene  los bits del offset a un numero decimal.
				decimal_byte_offset = bitset<numeric_limits<unsigned long>::digits>(sBits_byte_offset).to_ulong();
		
				cout << "Decimal Tag: " << decimal_tag << endl;
				cout << "Decimal Index: " << decimal_index << endl;
				cout << "Decimal Offset: " << decimal_byte_offset << endl;

				//Define si se hace una escritura o lectura comparando esa bandera
				for(int i=0;i<LongCad-1;i++){			
					if(LecturaEscritura[i]=='S')
							LE=1;				
				}

		  	if(LE != 0){//1
					//Escritura de la cache
					cout << "'Escritura a la cache'\n"<< endl; 
					switch(Asociatividad){	
    				case(0):{//Cache de mapeo directo
							if(Cache[decimal_index][0] == 0){
								//cout << "No esta en la cache" << endl;
								Cache[decimal_index][1] = decimal_tag;
								Cache[decimal_index][0] = 1;	
								misses++;						
							}
							else{
								//cout << "esta en la cache" << endl;
								if(Cache[decimal_index][1] == decimal_tag)
									hits++;
							  else{
									//cout << "el bloque esta ocupado y lo debe cambiar" << endl;
	           	    Cache[decimal_index][1] = decimal_tag;
									Cache[decimal_index][0] = 1;
									misses++;
								}
							}
						}
						break;
						case(1):{
							int EnCache = 0;
							cout << "cache de 4 set-asociativo" << endl;
							for(int i=0;i<4;i++){
								if(Cache[decimal_index][i] == 1 & Cache[decimal_index][i+4] == decimal_tag){
									EnCache = 1;
									cout << "Esta en la cache" << endl;
								}
							}
							cout << "Indice0:" << indice0 << endl;
							cout << "Indice1:" << indice1 << endl;
							if(!EnCache){//No esta en la cache, lo va a escribir
								misses++;																		
								if(decimal_index == 0){//Si el indice es 0									
									if(indice0==4){
										indice0 = 0;
										Cache[decimal_index][indice0] = 1; 
										Cache[decimal_index][indice0+4] = decimal_tag; 
										indice0++;												
										cout << "No esta en la cache, indice 0" << endl;
									}	
									else{
										Cache[decimal_index][indice0] = 1; 
										Cache[decimal_index][indice0+4] = decimal_tag; 
										indice0++;													
										cout << "No esta en la cache, indice i" << endl;
									}	
								}
								else	if(decimal_index == 1){//Si el indice es 1 
												if(indice1==4){
													indice1 = 0;
													Cache[decimal_index][indice1] = 1; 
													Cache[decimal_index][indice1+4] = decimal_tag; 
													indice1++;												
													cout << "No esta en la cache, indice0" << endl;
											  	}	
												else{
											 		Cache[decimal_index][indice1] = 1; 
													Cache[decimal_index][indice1+4] = decimal_tag; 
													indice1++;													
													cout << "No esta en la cache,indicei" << endl;
												}
											}			
							 				else{//Entonces no hay set vacios, asignelo por random
												srand(time(NULL));
												int aleatorio = rand() % 4;
             						Cache[decimal_index][aleatorio] = 1; 
												Cache[decimal_index][aleatorio+4] = decimal_tag; 											
												cout << "No esta en la cache,reemplazo" << endl;	
								 		 }
							}
							else{
								hits++;
								cout << "Esta en la cache" << endl;
							}
						}
						break;
						case(2):{
							cout << "cache de 2 set-asociativo" << endl;
						 	if(Cache[decimal_index][0] == 1 & Cache[decimal_index][2] == decimal_tag ){//hay dato válido en seti, se compara etiqueta
								//El dato ya esta en la cache
								hits++;	
								cout << "esta en la cache" << endl;							
							}
							else if(Cache[decimal_index][1] == 1 & Cache[decimal_index][3] == decimal_tag){//hay dato válido y etiqueta en seti, se 
											hits++;
											cout << "esta en la cache" << endl;							
										}
									 else if(Cache[decimal_index][0] == 0){
									 				Cache[decimal_index][0] = 1; 
													Cache[decimal_index][2] = decimal_tag; 											
											    misses++;	
													cout << "No esta en la cache" << endl;
												}
												else if(Cache[decimal_index][1] == 0){
									 						Cache[decimal_index][1] = 1; 
															Cache[decimal_index][3] = decimal_tag; 											
											    		misses++;	
															cout << "No esta en la cache" << endl;	
												}
														 else{
													 	 	srand(time(NULL));
													   	int aleatorio = rand() % 2;
               	     			 	 	Cache[decimal_index][aleatorio] = 1; 
													   	Cache[decimal_index][aleatorio+2] = decimal_tag; 											
											     	 	misses++;	
													 		cout << "No esta en la cache, reemplazo" << endl;	
															}	
		        	}
							break;
							default:
								cout << "No esta en la cache" << endl;
					}
				}

					//Lectura de La cache
				else{
					cout << "'Lectura a la cache'\n"<< endl;
					switch(Asociatividad){	
   					case(0):{//Directamente mapeado
							if(Cache[decimal_index][0] == 0){//Si el bit de válido esta en 1, el dato en cache
							//out << "No es un dato válido" << endl;
								misses++;
							}
							else{							
								if(Cache[decimal_index][1] == decimal_tag){
									hits++;
									//cout << "Esta en la cache" << endl;
								}
								else{
									//cout << "No esta en la cache" << endl;
									misses++;
							  }	
							}
						}
						break;
						case(1):{//Asociativo de 4 vias
							cout << "Asociativo de 4 vias" << endl;
							int EnCache = 0;
							for(int i=0;i<4;i++){
								if(Cache[decimal_index][i] == 1){//hay dato válido en seti(tiene 4 sets o vías), se compara etiqueta
									if(Cache[decimal_index][i+4] == decimal_tag)//El dato ya esta en la cache
										EnCache = 1;
								}
							}
							if(EnCache){
								hits++;
								cout << "esta en la cache" << endl;
								}		
							else{//No esta en la cache										
								misses++;	
								cout << "No esta en la cache" << endl;	
							}	
						}
						break;
						case(2):{//Asociativo de 2 vias
							cout << "Asociativo de 2 vias" << endl;
							if(Cache[decimal_index][0] == 1 & Cache[decimal_index][2] == decimal_tag){//hay dato válido y etqueta en seti
								hits++;
								cout << "Esta en la cache" << endl;
							}
							else if(Cache[decimal_index][1] == 1 & Cache[decimal_index][3] == decimal_tag){//hay dato válido y etqueta en seti
										hits++;
										cout << "Esta en la cache" << endl;
										}
									 else{
										 cout << "No esta en la cache" << endl;
										 misses++;
									 }
						}
						break;
						default:	
							cout << "No es un dato válido" << endl;
					}
				}
		
				cout << "'Estado de la Cache:'\n"<< endl;
		
				for(int i=0;i<Filas;i++){
					for(int j=0;j<Columnas;j++){
						cout << Cache[i][j] << " ";
					}
						cout << endl;
				}			
		
				cout << "\nHit:" << hits << endl;
				cout << "Miss:" << misses << "\n" << endl;
				/*HitRatio = 100.0*(double(hits)/double(Cont));
				MissRatio = 100.0*(double(misses)/double(Cont));
				cout << "\nDatos Analizados:" << Cont << endl;
				cout << "Hit Ratio:" << HitRatio << "%" << endl;
				cout << "Miss Rato:" << MissRatio << "%\n" << endl;*/
			}
		}	
	}
	//Cierre del archivo

	cout << "\nHit:" << hits << endl;
	cout << "Miss:" << misses << "\n" << endl;
	HitRatio = 100.0*double(hits)/double(Cont);
	MissRatio = 100.0*double(misses)/double(Cont);
	cout << "\nDatos Analizados:" << Cont << endl;
	cout << "Hit Ratio:" << HitRatio << "%" << endl;
	cout << "Miss Rato:" << MissRatio << "%\n" << endl;

	archivo.close();

}
  cout << "Cierra archivo y finaliza correctamente" << endl;
  return 0;
}


//Funciones de conversion

//Funcion que convierte un string en hexadecimal a un string en binario 
string HexStringABinString (string dirHex) 
	{
		string sReturn = "";
		for (int i = 0; i < dirHex.length (); ++i)
			{
				switch (dirHex [i])
					{
						case '0': sReturn.append ("0000"); break;
						case '1': sReturn.append ("0001"); break;
						case '2': sReturn.append ("0010"); break;
						case '3': sReturn.append ("0011"); break;
						case '4': sReturn.append ("0100"); break;
						case '5': sReturn.append ("0101"); break;
						case '6': sReturn.append ("0110"); break;
						case '7': sReturn.append ("0111"); break;
						case '8': sReturn.append ("1000"); break;
						case '9': sReturn.append ("1001"); break;
						case 'a': sReturn.append ("1010"); break;
						case 'b': sReturn.append ("1011"); break;
						case 'c': sReturn.append ("1100"); break;
						case 'd': sReturn.append ("1101"); break;
						case 'e': sReturn.append ("1110"); break;
						case 'f': sReturn.append ("1111"); break;
					}
			}
		return sReturn;
	}

// Funcion que  convierte el string de direccion binaria a multiples strings binarios, correspondientes a los strings del index, tag y byte offset.
void BinStringAMultipleString (string sBits_direccion, int tam_index, int tam_tag, int tam_byte_offset) 
	{
		for (int j = 0; j < sBits_direccion.length (); ++j)
			{
				if ( tam_tag != 0 ) // Coviert primero el tag.
					{
						switch (sBits_direccion [j])
							{
								case '0': sBits_tag.append ("0"); break;
								case '1': sBits_tag.append ("1"); break;
							}
						tam_tag--;
					}
				else if ( tam_index != 0 ) // Luego del tag, convierte el index.
					{
						switch (sBits_direccion [j])
							{
								case '0': sBits_index.append ("0"); break;
								case '1': sBits_index.append ("1"); break;
							}
						tam_index--;
					}
				else if ( tam_byte_offset != 0) // Termina de convertir con el byte offset.
					{
						switch (sBits_direccion [j])
							{
								case '0': sBits_byte_offset.append ("0"); break;
								case '1': sBits_byte_offset.append ("1"); break;
							}
						tam_byte_offset--;
					}
			}
	}

