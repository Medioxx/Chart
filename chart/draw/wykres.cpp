#include "stdafx.h"
#include "wykres.h"

_Wykres::_Wykres() {



	x = 50;
	y = 500;

	wysokosc = 150;
	dlugosc = 700;

	probka_startowa = 80;

	wyswietlaj_a = true;
	wyswietlaj_v = true;
	wyswietlaj_s = true;
	wyswietlaj_k = true;

	czyNarysowane = false;

	mnoznik_a = 5.0;
	mnoznik_v = 1.0;
	mnoznik_s = 1.0;

	wybrany_mnoznik = &mnoznik_a;

}

//funkcja ktora wczytuje dane z pliku okreslonego sciezka
bool _Wykres::wczytajDane(string sciezka) {
	fstream fs;
	fs.open("outputSpring03.log", ios::in);

	if (!fs.is_open())		//sprawdzam czy poprawnie otworzono plik
		return false;
	double depozyt;
	int licznik = 0;
	while (!fs.eof()) {
		//wczytuje 3 liczby do jednej probki
		//for (int i = 0; i < 3; i++) {
		fs >> depozyt;
		fs >> depozyt;
		fs >> depozyt;
		fs >> syg_a[licznik][0];
		fs >> syg_a[licznik][1];
		fs >> syg_a[licznik][2];
		fs >> depozyt;
		fs >> depozyt;
		fs >> depozyt;
		fs >> depozyt;
		fs >> depozyt;
		fs >> depozyt;

		//przyspieszenie wczytuje bezposrednio do gotowych sygnalow
		dane[licznik][0] = syg_a[licznik][0];
		dane[licznik][1] = syg_a[licznik][1];
		dane[licznik][2] = syg_a[licznik][2];
		//}
		licznik++;
	}

	ilosc_probek = licznik;
	krok = (double)dlugosc / double(licznik - probka_startowa);
	dt = 1.0 / 25.0; //25Hz

	przetwarzajDane();

	fs.close();				//zamykam strumien
	return true;
}

//ta funkcja dokonuje calkowania przebiegow
void _Wykres::przetwarzajDane() {
	//mnoze wszystko przez mnoznik
	for (int i = probka_startowa; i < ilosc_probek; i++) {
		for (int j = 0; j < 3; j++)
			syg_a[i][j] = dane[i][j];
	}

	//najpierw calkuje 'a' aby dostac 'v'
	double *calka = new double[]{ 0.0, 0.0, 0.0 };
	for (int i = probka_startowa; i < ilosc_probek; i++) {
		//metoda trapezow

		for (int j = 0; j < 3; j++) {
			calka[j] += ((syg_a[i - 1][j] + syg_a[i][j]) / 2) * dt;
			syg_v[i][j] = calka[j];
		}
	}

	//najpierw calkuje 'v' aby dostac 's'
	calka = new double[]{ 0.0, 0.0, 0.0 };
	for (int i = probka_startowa; i < ilosc_probek; i++) {
		//metoda trapezow

		for (int j = 0; j < 3; j++) {
			calka[j] += ((syg_v[i - 1][j] + syg_v[i][j]) / 2) * dt;
			syg_s[i][j] = calka[j];
		}
	}

	//licze sygnal wypadkowy
	for (int i = probka_startowa; i < ilosc_probek; i++) {
		for (int j = 0; j < 3; j++) {
			//wypadkowe przyspieszenie
			syg_w[i][0] = sqrt(syg_a[i][0] * syg_a[i][0] + syg_a[i][1] * syg_a[i][1] + syg_a[i][2] * syg_a[i][2]);
			//wypadkowa predkosc
			syg_w[i][1] = sqrt(syg_v[i][0] * syg_v[i][0] + syg_v[i][1] * syg_v[i][1] + syg_v[i][2] * syg_v[i][2]);
			//wypadkowa droga
			syg_w[i][2] = sqrt(syg_s[i][0] * syg_s[i][0] + syg_s[i][1] * syg_s[i][1] + syg_s[i][2] * syg_s[i][2]);
		}
	}

	//teraz mnoze przez mnozniki, inaczej pomnozenie najpierw 'a' wplywa na v i s
	for (int i = probka_startowa; i < ilosc_probek; i++) {
		syg_w[i][0] *= mnoznik_a;
		syg_w[i][1] *= mnoznik_v;
		syg_w[i][2] *= mnoznik_s;
	}
}

void _Wykres::rysuj(Graphics &g, int ile) {
	//Rysowanie osi:
	Pen blackPen(Color::Black, 1);
	Font font(L"Times New Roman", 10, FontStyleBold, UnitPixel);

	krok = (double)dlugosc / double(ilosc_probek - probka_startowa);

	//OSIE:
	g.DrawLine(&blackPen, x, y, x + dlugosc, y);
	g.DrawLine(&blackPen, x, y + wysokosc, x, y - wysokosc);
	//OZNACZENIA OSI:
	g.DrawString(L"0", 1, &font, PointF(x - 10, y - 5), &SolidBrush(Color::Black));
	g.DrawString(L"t [s]", 5, &font, PointF(x + dlugosc + 5, y - 5), &SolidBrush(Color::Black));


	//rysuje podzialke na osi czasu
	double s_step = double(dlugosc) / (double(ilosc_probek) / 25.0);
	for (int i = 1; i < ilosc_probek / 25; i++) {

		g.DrawLine(&blackPen, x + i * s_step, y - 5, x + i * s_step, y + 5);

		wstring val = to_wstring(i);

		if (i % 5 == 0)
			g.DrawString(val.c_str(), 2, &font, PointF(x + i * s_step - 6, y + 10), &SolidBrush(Color::Black));
	}


	//teraz rysowanie poszczegolnych sygnalow na osi X
	//tutaj jezeli rysujemy wypadkwoe
	if (RYSUJ_WYPADKOWE) {
		//przyspieszenie
		if (wyswietlaj_a) {
			Pen pen_a(color_a, 1);
			int x0 = x, y0 = y;
			int x1, y1;
			for (int i = probka_startowa; i < ilosc_probek - 1; i++) {
				x1 = x + (i - probka_startowa)*krok;
				y1 = y - syg_w[i][0]; //minus ze wzgledu na to, ze oœ y jest skierowana w dó³ w WinAPI

				g.DrawLine(&pen_a, x0, y0, x1, y1);

				x0 = x1;
				y0 = y1;
			}
		}


		//predkosc
		if (wyswietlaj_v) {
			Pen pen_a(color_v, 1);
			int x0 = x, y0 = y;
			int x1, y1;
			for (int i = probka_startowa; i < ilosc_probek - 1; i++) {
				x1 = x + (i - probka_startowa)*krok;
				y1 = y - syg_w[i][1]; //minus ze wzgledu na to, ze oœ y jest skierowana w dó³ w WinAPI

				g.DrawLine(&pen_a, x0, y0, x1, y1);

				x0 = x1;
				y0 = y1;
			}
		}

		//droga
		if (wyswietlaj_s) {
			Pen pen_a(color_s, 1);
			int x0 = x, y0 = y;
			int x1, y1;
			for (int i = probka_startowa; i < ilosc_probek - 1; i++) {
				x1 = x + (i - probka_startowa)*krok;
				y1 = y - syg_w[i][2]; //minus ze wzgledu na to, ze oœ y jest skierowana w dó³ w WinAPI

				g.DrawLine(&pen_a, x0, y0, x1, y1);

				x0 = x1;
				y0 = y1;
			}
		}
	}
	//tutaj tylko na osi x
	else {
		//przyspieszenie
		if (wyswietlaj_a) {
			Pen pen_a(color_a, 1);
			int x0 = x, y0 = y;
			int x1, y1;
			for (int i = probka_startowa; i < ilosc_probek - 1; i++) {
				x1 = x + (i - probka_startowa)*krok;
				y1 = y - syg_a[i][0]; //minus ze wzgledu na to, ze oœ y jest skierowana w dó³ w WinAPI

				g.DrawLine(&pen_a, x0, y0, x1, y1);

				x0 = x1;
				y0 = y1;
			}
		}


		//predkosc
		if (wyswietlaj_v) {
			Pen pen_a(color_v, 1);
			int x0 = x, y0 = y;
			int x1, y1;
			for (int i = probka_startowa; i < ilosc_probek - 1; i++) {
				x1 = x + (i - probka_startowa)*krok;
				y1 = y - syg_v[i][0]; //minus ze wzgledu na to, ze oœ y jest skierowana w dó³ w WinAPI

				g.DrawLine(&pen_a, x0, y0, x1, y1);

				x0 = x1;
				y0 = y1;
			}
		}

		//droga
		if (wyswietlaj_s) {
			Pen pen_a(color_s, 1);
			int x0 = x, y0 = y;
			int x1, y1;
			for (int i = probka_startowa; i < ilosc_probek - 1; i++) {
				x1 = x + (i - probka_startowa)*krok;
				y1 = y - syg_s[i][0]; //minus ze wzgledu na to, ze oœ y jest skierowana w dó³ w WinAPI

				g.DrawLine(&pen_a, x0, y0, x1, y1);

				x0 = x1;
				y0 = y1;
			}
		}
	}
}

void _Wykres::wybierzMnoznik(int mnoznik) {
	switch (mnoznik) {
	case MNOZNIK_A: wybrany_mnoznik = &mnoznik_a; break;
	case MNOZNIK_V: wybrany_mnoznik = &mnoznik_v; break;
	case MNOZNIK_S: wybrany_mnoznik = &mnoznik_s; break;
	}
}

void _Wykres::modyfikujMnoznik(double diff) {
	*wybrany_mnoznik += diff;
	przetwarzajDane();

	if (*wybrany_mnoznik <= 0.1) {
		*wybrany_mnoznik -= diff;
	}
	else if (*wybrany_mnoznik >= 10.0)
		*wybrany_mnoznik -= diff;
}