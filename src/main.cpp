#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <unistd.h>
#include "../lib/egIOUtility.h"
#include"../lib/egcolor.h"
using namespace std;

const string EGCEDULA = "1719529438";
const string EGNOMBRE= "ESTEBAN DAVID GOMEZ ROCHA";

struct egCoordenada
{
    int egCapacidadBelica;
    string egGeoLocalizacion;
    string egDetalleArsenal;
    struct egCoordenada *izq, *der;
};

typedef struct egCoordenada *ABB;
ABB arbol           = NULL;        
int arbolNroNodos   = 0;           
int arbolCapBelica  = 0;           
string arbolSecCarga= "";         
int const TKError   =-1;           
int const TKBomba   =-2;           
string const ALFA = "abc.";        

int mtBOMBIA_I[3][4]=    {
                         {1,         2,         TKError,     TKError},
                         {1,         2,         TKError,     TKError},
                         {TKError,   TKError,   2,           TKBomba}
                         }; 
int eggetIndexAlfabeto(char c)
{
    int index = ALFA.find(c);
    if(index < ALFA.length())
        return index;
    return TKError;    
}
string egaplicaBOMBIA_I(string tipoArsenal) 
{        
    int q=0, l=0;
    for (auto &&c : tipoArsenal+"." ) 
    {
          l = eggetIndexAlfabeto(c);
          q = mtBOMBIA_I[q][l];
          if(q == TKError || q > 3)
               return "";
          if(q == TKBomba)
               return "BOMBA_II";
    }
    return "";
}

ABB egcrearNodo(const string data)  
{
     try
     {
          vector<string> splitData;
          ppSplitStr(data,',',splitData);
          ABB nuevaCoor = new(struct egCoordenada);
          nuevaCoor->egCapacidadBelica    = stoi(splitData[0]); 
          nuevaCoor->egGeoLocalizacion    = ppTrim(splitData[1]);
          nuevaCoor->egDetalleArsenal     = ppTrim(splitData[2]);
          nuevaCoor->izq = NULL;
          nuevaCoor->der = NULL;
          return nuevaCoor;
     }
     catch(const exception& e)
     {
          setTextColor(textColorRed);
          cout << ">> Error: "<< data << " -> " << e.what() << endl;
          setTextColor(textColorGreen);
     }
     return NULL;
}
void eginsertar(ABB &arbol, ABB coord) 
{
     if(arbol==NULL)
     {
          arbol = coord; 
          arbolNroNodos++;
          arbolCapBelica += coord->egCapacidadBelica;
          arbolSecCarga  += to_string(coord->egCapacidadBelica) + " ";
     }
     else if(coord->egCapacidadBelica < arbol->egCapacidadBelica)
          eginsertar(arbol->izq, coord);
     else if(coord->egCapacidadBelica > arbol->egCapacidadBelica)
          eginsertar(arbol->der, coord);
}
void egpreOrden(ABB arbol)
{
     if(arbol!=NULL)
     {
          cout << arbol->egCapacidadBelica <<" ";
          egpreOrden(arbol->izq);
          egpreOrden(arbol->der);
     }
}
void egenOrden(ABB arbol)
{
     if(arbol!=NULL)
     {
          egenOrden(arbol->izq);
          cout << arbol->egCapacidadBelica << " ";
          egenOrden(arbol->der);
     }
}
void egpostOrden(ABB arbol)
{
     if(arbol!=NULL)
     {
          egpostOrden(arbol->izq);
          egpostOrden(arbol->der);
          cout << arbol->egCapacidadBelica << " ";
     }
}
void egverArbol(ABB arbol, int n)
{
     if(arbol==NULL)
          return;
     egverArbol(arbol->der, n+1);

     for(int i=0; i<n; i++)
         cout<<"\t";

     setTextColor(textColorBlue);
     cout << " "  << arbol->egGeoLocalizacion
          << "."  << arbol->egCapacidadBelica;
     setTextColor(textColorWhite);
     cout << " { " << arbol->egDetalleArsenal;
     setTextColor(textColorRed);
     cout << "  " << egaplicaBOMBIA_I(arbol->egDetalleArsenal);
     setTextColor(textColorWhite);
     cout << " }"<< endl;

     egverArbol(arbol->izq, n+1);
}

void egLoading(string str)
{ 
    string c= " \\|/-"; 
    for(int i=0; i<= 100; i++)
    {   
        cout<< "\r " << c[i % 4] << " " << i << "%   ";
        usleep(5000);
    }
    cout << str  << endl;
}

void eggetCoordenadas() 
{
    string str;
    fstream f;
    cout << "[+]Leyendo coordenadas ... " << endl;
    f.open("../egCoordenada.txt", ios_base::in); 
    if ( f.is_open() ) 
          while( !f.eof() )
          {
               getline( f, str );
               ABB coord  = egcrearNodo(str);
               if(coord != NULL)
               {
                    eginsertar(arbol , coord);
                    egLoading(str);
               }
          }
    else
          cout << "Error de abrir el archivo de coordenadas" << endl;
    f.close();
}

int main(void)
{
     setTextColor(textColorGreen);
     eggetCoordenadas();
     
     setTextColor(textColorCyan);
     cout << endl << "\n\n[+]Informacion Pila de capacidad belica Ucrania";
     cout << endl << "   Developer-Nombre : "  << EGNOMBRE
          << endl << "   Developer-Cedula : "  << EGCEDULA
          << endl << "   Capacidad Belica : "  << arbolCapBelica
          << endl << "   Coordenada-Total : "  << arbolNroNodos
          << endl << "   Coordenada-SecCarga: " << arbolSecCarga;
     setTextColor(textColorWhite);
}
