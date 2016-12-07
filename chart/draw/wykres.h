#pragma once
#include "stdafx.h"
#include <vector>
#include <iostream>
#include <string>
#include <fstream>

#define SCIEZKA "outputSpring02.log"
#define MNOZNIK_A	0xD0
#define MNOZNIK_V	0xD1
#define MNOZNIK_S	0xD2
#define RYSUJ_WYPADKOWE true

using namespace std;

class _Wykres {
public:
	_Wykres();

	int dlugosc, wysokosc;		//rozmiar wykresu
	double dt, krok;			//dt	- odstep okreslajacy czas miedzy kolejnymi probkami
	//krok	- odstep w pikselach od kolejnych probek na wykresie

	double mnoznik_a, mnoznik_v, mnoznik_s;
	//odpowiada za rozciagniecie wykresu w pionie (sam wykres jest bardzo ciasny)

	int probka_startowa;		//iles tam probek jest wadliwych, wiec tyle pomijamy
	int ilosc_probek;			//ile probek wczytalismy. okreslane podczas dzialania wczytajDane()

	double dane[2000][3];		//miejsce dla tysiaca probek, kazda probka ma 3 elementy w wierszu (z pliku outputSpring03.log)
	double syg_a[2000][3];		//kolejne wartosci a_x, a_y, a_z	
	double syg_v[2000][3];		//kolejne wartosci v_x, v_y, v_z po scalkowaniu przebiegow 'a'
	double syg_s[2000][3];		//kolejne wartosci s_x, s_y, s_z po scalkowaniu przebiegow 'v'
	double syg_w[2000][3];		//sygna wypadkowy, kolejno a,v,s ([0],[1],[2]);

	int x, y;					//wpolrzedne punktu zaczepienia wykresu

	bool wczytajDane(string sciezka);

	bool czyNarysowane;			//rysujemy tylko raz, dziala to na zasadzie 'zatrzasku'; tak zeby nie rysowac co kazdy przebieg funkcji MyOnPaint;

	bool wyswietlaj_a, wyswietlaj_v, wyswietlaj_s, wyswietlaj_k;
	//kolejne zmienne logiczne mowiace czy wyswietlac konkretne skladowe sygnalu

	double *wybrany_mnoznik;		//w interfejsie 'wybieramy' ktory wykres chcemy rozszerzac/zwezac w pionie, ten wskaznik bedzie wskazywal na mnoznik ktory bedziemy modyfikowali

	const Color color_a = Color::Red,
		color_v = Color::Blue,
		color_s = Color::Green,
		color_k = Color::Black;
	//kolory poszczegolnych lini na wykresie

	void przetwarzajDane();		//calkowanie przebiegu przyspieszenia zeby uzyskac predkosci i polozenie
	void rysuj(Graphics &g, int ile);
	//rysuje 'ile' probek wykresu, mozna uzyc do animacji albo od razu podac calkowita ilosc probek i wyswietlic wszystko

	double double_abs(double d);
	void wybierzMnoznik(int);
	void modyfikujMnoznik(double diff);

};