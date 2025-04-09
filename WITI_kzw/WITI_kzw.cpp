#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <chrono>

using namespace std;

ofstream wyjscie;

struct zadanie
{
public:
    int p, w, d;    // p - czas trwania, w - waga, d - termin zakonczenia
    int id;
};

struct kolejnosc
{
public:
    int id;
    int set;
};

int C(int i, int n, zadanie* zad)
{
    int c = 0;
    int b = 1;
    for (int j = 0; j < n; j++)
    {
        if (i & b)
        {
            c += zad[j].p;
        }
        b *= 2;
    }

    return c;
}


void getPermutation(kolejnosc* kol, int i)
{
    if (kol[i].set < 0)
    {
        return;
    }
    else
    {
        getPermutation(kol, kol[i].set);
    }

    cout << kol[i].id << " ";
}

int main()
{
    int n;
    zadanie zad[100];
    ifstream wejscie;

    for (int k = 10; k < 21; k++)
    {
        string filename = "data." + to_string(k) + ".txt";
        wejscie.open(filename);
        wejscie >> n;
        for (int j = 0; j < n; j++)
        {
            zad[j].id = j + 1;
            wejscie >> zad[j].p >> zad[j].w >> zad[j].d;
        }
        wejscie.close();

        int N = 1 << n;
        int* F = new int[N];    // tablica z podproblemami 2^n
        F[0] = 0;               // z instrukcji, pusty zbiór

        kolejnosc *kol = new kolejnosc[N];
        kol[0].id = -1;
        kol[0].set = -1;

        int id_pom;
        int kolejnosc;

        int b = 1;

        auto start = chrono::steady_clock::now();
        long double mseconds = 0;

        // mierzenie czasu działania
        // powtarzane 100 razy, aby uzyskać widoczny wynik
        // w sprawozdaniu wynik jest dzielony na 100

        /*for (int a = 0; a < 100; a++)
        {
            for (int i = 1; i < N; i++)     //bez 0
            {
                F[i] = INT_MAX;
                b = 1;

                for (int j = 0; j < n; j++)
                {
                    if (i & b)
                    {
                        if (F[i - b] + zad[j].w * max(C(i, n, zad) - zad[j].d, 0) <= F[i])
                        {
                            kol[i].id = zad[j].id;
                            kol[i].set = i - b;
                        }
                        F[i] = min(F[i], F[i - b] + zad[j].w * max(C(i, n, zad) - zad[j].d, 0));
                    }
                    b *= 2;
                }
            }
        }*/

        for (int i = 1; i < N; i++)     //bez 0
        {
            F[i] = INT_MAX;
            b = 1;

            for (int j = 0; j < n; j++)
            {
                if (i & b)
                {
                    if (F[i - b] + zad[j].w * max(C(i, n, zad) - zad[j].d, 0) <= F[i])
                    {
                        kol[i].id = zad[j].id;
                        kol[i].set = i - b;
                    }
                    F[i] = min(F[i], F[i - b] + zad[j].w * max(C(i, n, zad) - zad[j].d, 0));
                }
                b *= 2;
            }
        }

        auto end = chrono::steady_clock::now();
        mseconds = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        
        // F[N-1] ostatni element jest opt
        cout << "dane." << k << ": " << endl;
        cout << "czas optymalny: " << F[N - 1] << endl;
        cout << "kolejnosc: ";
        getPermutation(kol, N - 1);
        //cout << endl << "total time: " << mseconds << " [ms]";
        cout << endl << endl;

        // zapis do pliku
        /*wyjscie.open("wyniki.txt");
        wyjscie << "dane." << k << ": " << endl;
        wyjscie << "czas optymalny: " << F[N - 1] << endl;
        wyjscie << "kolejnosc: ";
        //getPermutation_toFile(kol, N - 1);
        wyjscie << "total time: " << mseconds << " [ms]\n";
        wyjscie << endl << endl;*/
    }
    return 0;
}
