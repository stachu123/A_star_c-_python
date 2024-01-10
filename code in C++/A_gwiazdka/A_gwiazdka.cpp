#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

int r_s = 19;
int c_s = 0;
int r_c = 0;
int c_c = 19;

/* Funkcje potrzebne do wczytania tablicy z pliku*/

void wczytajDane(double**& G, const string& nazwap, int wym1, int wym2) {
	std::ifstream plik(nazwap.c_str());
	if (!plik.is_open()) {
		cerr << "Nie mozna otworzyc pliku" << endl;
		return;
	}

	for (unsigned int i = 0; i <= wym2 - 1; i++) {
		for (unsigned int j = 0; j <= wym1 - 1; j++) {
			plik >> G[i][j];
		}
	}
	plik.close();
};

void zwolnijPamiec(double** G, int wym1, int wym2) {
	for (int i = 0; i <= wym2; i++) {
		delete[] G[i];
	}
	delete[] G;
};

/*Funkcje do Algorytmu*/

//Tworzenie struktury odnoszącej sie do punktu w macierzy
struct Punkt {

	int r, c;
	Punkt(int r, int c) : r(r), c(c) {}

	bool operator==(const Punkt& other) const {
		return r == other.r && c == other.c;
	}

};

//tworzenie listy zamknietej i otwartej oraz Tworzenie macierzy parent (na podstawie vectora)
//przechowujacego  odwolanie do rodzica kazdego punktu na macierzy,
//domyslnie wszystkie punkty wypelnione wspolrzednymi -1, -1*/

vector<Punkt> lista_otwarta;
vector<Punkt> lista_zamknieta;
vector<vector<Punkt>> parent(20, vector<Punkt>(20, Punkt(-1, -1)));



/* Funkcja oblicz g przyjmuje jako argument punkt i oblicza dla danego miejsca w macierzy
   F_g + H_g, F_g liczone jest przez obliczenie kroków do startu dzieki macierzy parent,
   na podstawie której tworzona jest trasa*/

double oblicz_g(Punkt punkt) {
	int r = punkt.r;
	int c = punkt.c;

	vector<Punkt> trasa;
	Punkt aktualny(r, c);

	while (aktualny.r != r_s || aktualny.c != c_s) {
		trasa.push_back(aktualny);
		aktualny = parent[aktualny.r][aktualny.c];
	}
	trasa.push_back(Punkt(r_s, c_s));

	int f_g = trasa.size();
	double h_g = sqrt(pow(r - r_c, 2) + pow(c - c_c, 2));
	return f_g + h_g;

}

//Funkcja wybierz_do_zamknietej przyjmuje jako orgument vector z Punktami (liste otwarta)
// funkcja zwraca indeks w podane liscie punktu z najmniejszym G

int wybierz_do_zamknietej(const std::vector<Punkt>& lista) {
	double najmniejsza_wartosc = oblicz_g(lista[0]);
	int indeks_najmniejszej = 0;

	for (size_t i = 1; i < lista.size(); ++i) {
		double aktualna_wartosc = oblicz_g(lista[i]);
		if (aktualna_wartosc <= najmniejsza_wartosc) {
			najmniejsza_wartosc = aktualna_wartosc;
			indeks_najmniejszej = i;
		}
	}
	return indeks_najmniejszej;
}


int main(void) {
	cout << "Wczytywanie danych z pliku \n";
	string nazwap = "grid.txt";
	int wym1 = 20;
	int wym2 = 20;
	int rows = wym2 ;
	double** G;
	G = new double* [rows];
	while (rows--) G[rows] = new double[wym1];

	cout << "Wczytywanie danych z pliku \n";
	wczytajDane(G, nazwap, wym1, wym2);

	cout << "Wyswietlam tablice \n";
	for (int i = 0; i <= wym2-1; i++) {
		for (int j = 0; j <= wym1-1; j++) {
			cout << " " << G[i][j];
		}
		cout << "\n";
	}



	int r = r_s, c = c_s;

	while (r != r_c || c != c_c) {

		if (r - 1 >= 0 && G[r - 1][c] != 5 && find(lista_zamknieta.begin(), lista_zamknieta.end(), Punkt(r - 1, c)) == lista_zamknieta.end()) {
			if (find(lista_otwarta.begin(), lista_otwarta.end(), Punkt(r - 1, c)) == lista_otwarta.end()) {
				lista_otwarta.push_back(Punkt(r - 1, c));
				parent[r - 1][c] = Punkt(r, c);
			}
		}
		if (r + 1 < 20 && G[r + 1][c] != 5 && find(lista_zamknieta.begin(), lista_zamknieta.end(), Punkt(r + 1, c)) == lista_zamknieta.end()) {
			if (find(lista_otwarta.begin(), lista_otwarta.end(), Punkt(r + 1, c)) == lista_otwarta.end()) {
				lista_otwarta.push_back(Punkt(r + 1, c));
				parent[r + 1][c] = Punkt(r, c);
			}
		}
		if (c - 1 >= 0 && G[r][c - 1] != 5 && find(lista_zamknieta.begin(), lista_zamknieta.end(), Punkt(r, c - 1)) == lista_zamknieta.end()) {
			if (find(lista_otwarta.begin(), lista_otwarta.end(), Punkt(r, c - 1)) == lista_otwarta.end()) {
				lista_otwarta.push_back(Punkt(r, c - 1));
				parent[r][c - 1] = Punkt(r, c);
			}
		}
		if (c + 1 < 20 && G[r][c + 1] != 5 && find(lista_zamknieta.begin(), lista_zamknieta.end(), Punkt(r, c + 1)) == lista_zamknieta.end()) {
			if (find(lista_otwarta.begin(), lista_otwarta.end(), Punkt(r, c + 1)) == lista_otwarta.end()) {
				lista_otwarta.push_back(Punkt(r, c + 1));
				parent[r][c + 1] = Punkt(r, c);
			}
		}

		if (lista_otwarta.empty()) {
			cout << "Nie mozna znalezc trasy \n";
			return 0;
		}

		int nastepny = wybierz_do_zamknietej(lista_otwarta);
		lista_zamknieta.push_back(lista_otwarta[nastepny]);
		r = lista_otwarta[nastepny].r;
		c = lista_otwarta[nastepny].c;
		lista_otwarta.erase(lista_otwarta.begin() + nastepny);	

	}

	vector<Punkt> sciezka;
	Punkt aktualny(r_c, c_c);

	while (aktualny.r != r_s || aktualny.c != c_s) {
		sciezka.push_back(aktualny);
		aktualny = parent[aktualny.r][aktualny.c];
	}
	sciezka.push_back(Punkt(r_s, c_s));

	for (const auto& point : sciezka) {
		G[point.r][point.c] = 3;
	}


	cout << "Wyswietlam wypelniona tablice \n";
	for (int i = 0; i < 20; ++i) {
		for (int j = 0; j < 20; ++j) {
			cout << " " << G[i][j] ;
		}
		cout << endl;  
	}



	zwolnijPamiec(G, wym1, wym2);


	return 0;
}
