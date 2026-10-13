#include <iostream>
#include <fstream>

using namespace std;

int main (void) {

cout<<"Wczytywanie danych z pliku\n";

string nazwap="grid.txt";
int wym2=20;
int wym1=20;

//teraz deklarujemy dynamicznie tablice do, której wczytamyu nasz plik,
int rows = wym2;
double **G;
G = new double*[rows];
while(rows--) G[rows] = new double[wym1];

cout<<"\n\nNacisnij ENTER aby wczytac tablice o nazwie "<< nazwap;
getchar();

std::ifstream plik(nazwap.c_str());

for ( unsigned int i=0;i<wym2;i++)
    {
        for ( unsigned int j=0;j<wym1;j++)
        {
            plik >> G[i][j];
        }
    }
plik.close();














cout<<"\nWypisujemy na ekran\n\n";
for(int i=wym2-1;i>=0;i--)
    {
        if (i<10){
            cout<<" "<<i<<" "<<char(179)<<" ";
        }else{
            cout<<i<<" "<<char(179)<<" ";
        }

        for(int j=0;j<wym1;j++)
        {
            cout<<" "<<G[i][j];
        }
        cout<<"\n";
    }
cout<<char(196)<<char(196)<<char(196)<<char(197);
for(int i=0;i<42;i++){
    cout<<char(196);
}
cout<<"\n";
cout<<"   "<<char(179)<<"  0 1 2 3 4 5 6 7 8 9 O 1 2 3 4 5 6 7 8 9";


cout<<"\n\n\n\n [0,0] to "<<G[0][0];


















//na koniec czyscimy pamiec po naszej tablicy
for(int i=0;i<wym2;i++)
{delete[] G[i];}//czyscimy wiersze
delete[] G;//zwalniamy tablice wskaznikow do wierszy

cout<<"\n\nNacisnij ENTER aby zakonczyc";
getchar();

return 0;
}
