// PLIK OBS£UGUJ¥CY RENDEROWANIE OBIEKTÓW

internal void 
render_background() {									// rysujemy t³o
	unsigned int* piksel = (u32*)stan_render.pamiec;
	for (int y = 0; y < stan_render.wysokosc; y++) {
		for (int x = 0; x < stan_render.szerokosc; x++) {
			*piksel++ = 0x0066ff*x;						// ustalamy kolor pikseli, dodajemy inkrementacjê, aby rysowaæ kolejne
		}
	}
}

internal void 
czysc_ekran(u32 color) {								// umo¿liwia czyszczenie ekranu
	unsigned int* piksel = (u32*)stan_render.pamiec;
	for (int y = 0; y < stan_render.wysokosc; y++) {
		for (int x = 0; x < stan_render.szerokosc; x++) {
			*piksel++ = color;
		}
	}
}


internal void 
rysuj_prostokat_w_pikselach(int x0, int y0, int x1, int y1, u32 color) {		// umo¿liwia rysowanie prostok¹tów renderuj¹c odpowiednie piksele
	
	x0 = zacisk(0, x0, stan_render.szerokosc);			// zacisk zwraca najwiêksz¹ wartoœæ z podanych jako argumenty, dziêki temu porgam siê nie zepsuje podczas zmiany rozmiaru okna
	x1 = zacisk(0, x1, stan_render.szerokosc);			// (rozmiar prostok¹ta nie przekroczy dozwolonego rozmiaru)
	y0 = zacisk(0, y0, stan_render.wysokosc);
	y1 = zacisk(0, y1, stan_render.wysokosc);

	
	for (int y = y0; y < y1; y++) {
		u32* piksel = (u32*)stan_render.pamiec + x0 + y*stan_render.szerokosc;
		for (int x = x0; x < x1; x++) {
			*piksel++ = color;
		}
	}
}

global_variable float skala_renderowania = 0.01f;		

// funkcja rysuj¹ca obramowanie areny s³u¿¹ce jako t³o:
internal void
rysuj_obramowanie_areny(float arena_x, float arena_y, u32 color) {
	arena_x *= stan_render.wysokosc * skala_renderowania;
	arena_y *= stan_render.wysokosc * skala_renderowania;

	int x0 = (int)((float)stan_render.szerokosc * .5f - arena_x);
	int x1 = (int)((float)stan_render.szerokosc * .5f + arena_x);
	int y0 = (int)((float)stan_render.wysokosc * .5f - arena_y);
	int y1 = (int)((float)stan_render.wysokosc * .5f + arena_y);

	// rysujemy 4 prostok¹ty, które maj¹ pokryæ pust¹ przestrzeñ: 
	rysuj_prostokat_w_pikselach(0, 0, stan_render.szerokosc, y0, color);
	rysuj_prostokat_w_pikselach(0, y1, x1, stan_render.wysokosc, color);
	rysuj_prostokat_w_pikselach(0, y0, x0, y1, color);
	rysuj_prostokat_w_pikselach(x1, y0, stan_render.szerokosc, stan_render.wysokosc, color);
}

internal void
rysuj_prostokat(float x, float y, float polowa_rozmiaru_x, float polowa_rozmiaru_y, u32 color) {			//u¿ywa poprzedniej funkcji, aby rysowaæ obiekty, które bêd¹ dostosowywaæ swój rozmiar do rozmiaru okna
	
	// zamieniamy piksele na rozmiar obiektu, np. jeœli rozmiar obiektu to 0.2, to zajmie 20% ekranu
	x *= stan_render.wysokosc*skala_renderowania;		// dziêki temu, ¿e mno¿ymy przez wysokoœæ, to gdy u¿ytkownik zmieni szerokoœæ okna, rozmiar gracza siê nie zmieni, przy równoczesnym zwiêkszeniu pola widzenia
	y *= stan_render.wysokosc*skala_renderowania;		// je¿eli jednak zmienimy wysokoœæ okna, obiekt gracza dostosuje siê
	polowa_rozmiaru_x *= stan_render.wysokosc*skala_renderowania;
	polowa_rozmiaru_y *= stan_render.wysokosc*skala_renderowania;
	
	x += stan_render.szerokosc / 2.f;			// dodajemy po³owê szerkoœci i wyskoœci okna, dziêki czemu wartoœæ 0 podana w pozycji obiektu bêdzie oznacza³a œrodek (odpowiednio wysokoœci i szerokoœci)
	y += stan_render.wysokosc / 2.f;

	//zamiana na piksele
	int x0 = x - polowa_rozmiaru_x;
	int x1 = x + polowa_rozmiaru_x;
	int y0 = y - polowa_rozmiaru_y;
	int y1 = y + polowa_rozmiaru_y;


	rysuj_prostokat_w_pikselach(x0, y0, x1, y1, color);
}

// tworzymy tabelê przechowuj¹c¹ mapê kafelkow¹ liter:
const char* litery[][7] = {			
" 00",
"0  0",
"0  0",
"0000",
"0  0",
"0  0",
"0  0",

	"000",
	"0  0",
	"0  0",
	"000",
	"0  0",
	"0  0",
	"000",

	" 000",
	"0",
	"0",
	"0",
	"0",
	"0",
	" 000",

	"000",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"000",

	"0000",
	"0",
	"0",
	"000",
	"0",
	"0",
	"0000",

	"0000",
	"0",
	"0",
	"000",
	"0",
	"0",
	"0",

	" 000",
	"0",
	"0",
	"0 00",
	"0  0",
	"0  0",
	" 000",

	"0  0",
	"0  0",
	"0  0",
	"0000",
	"0  0",
	"0  0",
	"0  0",

	"000",
	" 0",
	" 0",
	" 0",
	" 0",
	" 0",
	"000",

	" 000",
	"   0",
	"   0",
	"   0",
	"0  0",
	"0  0",
	" 000",

	"0  0",
	"0  0",
	"0 0",
	"00",
	"0 0",
	"0  0",
	"0  0",

	"0",
	"0",
	"0",
	"0",
	"0",
	"0",
	"0000",

	"00 00",
	"0 0 0",
	"0 0 0",
	"0   0",
	"0   0",
	"0   0",
	"0   0",

	"00  0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	"0  00",

	"0000",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0000",

	" 000",
	"0  0",
	"0  0",
	"000",
	"0",
	"0",
	"0",

	" 000 ",
	"0   0",
	"0   0",
	"0   0",
	"0 0 0",
	"0  0 ",
	" 00 0",

	"000",
	"0  0",
	"0  0",
	"000",
	"0  0",
	"0  0",
	"0  0",

	" 000",
	"0",
	"0 ",
	" 00",
	"   0",
	"   0",
	"000 ",

	"000",
	" 0",
	" 0",
	" 0",
	" 0",
	" 0",
	" 0",

	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	"0  0",
	" 00",

	"0   0",
	"0   0",
	"0   0",
	"0   0",
	"0   0",
	" 0 0",
	"  0",

	"0   0 ",
	"0   0",
	"0   0",
	"0 0 0",
	"0 0 0",
	"0 0 0",
	" 0 0 ",

	"0   0",
	"0   0",
	" 0 0",
	"  0",
	" 0 0",
	"0   0",
	"0   0",

	"0   0",
	"0   0",
	" 0 0",
	"  0",
	"  0",
	"  0",
	"  0",

	"0000",
	"   0",
	"  0",
	" 0",
	"0",
	"0",
	"0000",

	"",
	"",
	"",
	"",
	"",
	"",
	"0",

	"   0",
	"  0",
	"  0",
	" 0",
	" 0",
	"0",
	"0",
};

// funkcja rysuj¹ca tekst (w skrócie pracuje tak, ¿e dla ka¿dej litery przechodzi przez ka¿dy wiersz i jeœli napotka zero ma narysowaæ prostok¹t)
internal void
rysuj_tekst(const char *tekst, float x, float y, float rozmiar, u32 color) {
	float polowa_rozmiaru = rozmiar * .5f;
	float oryginalny_y = y;

	while (*tekst) {									// przechodzimy przez ka¿dy znak podanego tekstu
		if (*tekst != 32) {								// 32 to odpowiednik spacji, jeœli napotkamy spacjê, pomijamy literê
			const char** litera;
			if (*tekst == 47) litera = litery[27];		// rysujemy slash
			else if (*tekst == 46) litera = litery[26]; // rysujemy kropkê
			else litera = litery[*tekst - 'A'];			// znak 'A' odpowiada wartoœci 65 wed³ug tabeli ASCII, dziêki takiemu dzia³aniu 'A' bêdzie przypisana do 0, 'B' do 1 itd.
			float oryginalny_x = x;

			for (int i = 0; i < 7; i++) {				// poniewa¿ ka¿da litera ma 7 wierszy
				const char* wiersz = litera[i];
				while (*wiersz) {						// dopóki mamy znajomy znak w wierszu
					if (*wiersz == '0') {				// sprawdzamy, czy jest to zero
						rysuj_prostokat(x, y, polowa_rozmiaru, polowa_rozmiaru, color);		// jeœli tak to rysujemy prostok¹t
					}
					x += rozmiar;						// przesuwamy pozycje x o rozmiar prostok¹ta
					wiersz++;							// zmieniamy wiersz
				}
				y -= rozmiar;							// po narysowaniu wiersza, zmieniamy pozycjê y
				x = oryginalny_x;						// resetujemy pozycjê x
			}
		}
		tekst++;										// przechodzimy do kolejnej litery
		x += rozmiar * 6.f;								// przesuwamy pozycjê x, aby zrobiæ odstêpy miêdzy literami
		y = oryginalny_y;								// resetujemy pozycjê y dla nowej litery
	}
}

// funkcja rysuj¹ca liczby:
internal void
rysuj_liczby(int liczba, float x, float y, float rozmiar, u32 color) {			//funkcja umo¿liwiaj¹ca rysowaæ wynik graczy
	float polowa_rozmiaru = rozmiar * .5f;										// x i y znajduj¹ siê w centrum narysowanej liczby
																				
	bool rysuj_liczby = false;
	while (liczba || !rysuj_liczby) {	// wchodzimy do pêtli, gdy cyfr¹ nie jest zero, lub jeszcze nie narysowaliœmy ¿adnej liczby
		rysuj_liczby = true;
		int cyfra = liczba % 10;		//wyci¹gamy resztê z dzielenia przez 10 z podanej liczby, dziêki temu nie musimy rysowaæ oddzielnie np. 10, tylko narysujemy 1 i 0 obok siebie
		liczba = liczba / 10;			//now¹ liczb¹ bêdzie liczba podzielona przez 10, dziêki temu na pocz¹tku bêdzie rysowana odpowiednia cyfra reprezentuj¹ca dziesi¹tki

		switch (cyfra) {		// ka¿dy przypadek to rysowanie innej cyfry za pomoc¹ prostk¹tów
		case 0: {
			rysuj_prostokat(x - rozmiar, y, polowa_rozmiaru, 2.5f * rozmiar, color);
			rysuj_prostokat(x + rozmiar, y, polowa_rozmiaru, 2.5f * rozmiar, color);
			rysuj_prostokat(x, y + rozmiar * 2.f, polowa_rozmiaru, polowa_rozmiaru, color);			// .f  mówi kompilatorowi, aby interpretowa³ dan¹ liczbê jako float
			rysuj_prostokat(x, y - rozmiar * 2.f, polowa_rozmiaru, polowa_rozmiaru, color);
			x -= rozmiar * 4.f;				// musimy przesun¹æ pozycjê x za ka¿dym razem kiedy narysujemy cyfrê
		}break;

		case 1: {
			rysuj_prostokat(x + rozmiar, y, polowa_rozmiaru, 2.5f * rozmiar, color);
			x -= rozmiar * 2.f;
		} break;

		case 2: {
			rysuj_prostokat(x, y + rozmiar * 2.f, 1.5f * rozmiar, polowa_rozmiaru, color);
			rysuj_prostokat(x, y, 1.5f * rozmiar, polowa_rozmiaru, color);
			rysuj_prostokat(x, y - rozmiar * 2.f, 1.5f * rozmiar, polowa_rozmiaru, color);
			rysuj_prostokat(x + rozmiar, y + rozmiar, polowa_rozmiaru, polowa_rozmiaru, color);
			rysuj_prostokat(x - rozmiar, y - rozmiar, polowa_rozmiaru, polowa_rozmiaru, color);
			x -= rozmiar * 4.f;
		} break;

		case 3: {
			rysuj_prostokat(x - polowa_rozmiaru, y + rozmiar * 2.f, rozmiar, polowa_rozmiaru, color);
			rysuj_prostokat(x - polowa_rozmiaru, y, rozmiar, polowa_rozmiaru, color);
			rysuj_prostokat(x - polowa_rozmiaru, y - rozmiar * 2.f, rozmiar, polowa_rozmiaru, color);
			rysuj_prostokat(x + rozmiar, y, polowa_rozmiaru, 2.5f * rozmiar, color);
			x -= rozmiar * 4.f;
		} break;

		case 4: {
			rysuj_prostokat(x + rozmiar, y, polowa_rozmiaru, 2.5f * rozmiar, color);
			rysuj_prostokat(x - rozmiar, y + rozmiar, polowa_rozmiaru, 1.5f * rozmiar, color);
			rysuj_prostokat(x, y, polowa_rozmiaru, polowa_rozmiaru, color);
			x -= rozmiar * 4.f;
		} break;

		case 5: {
			rysuj_prostokat(x, y + rozmiar * 2.f, 1.5f * rozmiar, polowa_rozmiaru, color);
			rysuj_prostokat(x, y, 1.5f * rozmiar, polowa_rozmiaru, color);
			rysuj_prostokat(x, y - rozmiar * 2.f, 1.5f * rozmiar, polowa_rozmiaru, color);
			rysuj_prostokat(x - rozmiar, y + rozmiar, polowa_rozmiaru, polowa_rozmiaru, color);
			rysuj_prostokat(x + rozmiar, y - rozmiar, polowa_rozmiaru, polowa_rozmiaru, color);
			x -= rozmiar * 4.f;
		} break;

		case 6: {
			rysuj_prostokat(x + polowa_rozmiaru, y + rozmiar * 2.f, rozmiar, polowa_rozmiaru, color);
			rysuj_prostokat(x + polowa_rozmiaru, y, rozmiar, polowa_rozmiaru, color);
			rysuj_prostokat(x + polowa_rozmiaru, y - rozmiar * 2.f, rozmiar, polowa_rozmiaru, color);
			rysuj_prostokat(x - rozmiar, y, polowa_rozmiaru, 2.5f * rozmiar, color);
			rysuj_prostokat(x + rozmiar, y - rozmiar, polowa_rozmiaru, polowa_rozmiaru, color);
			x -= rozmiar * 4.f;
		} break;

		case 7: {
			rysuj_prostokat(x + rozmiar, y, polowa_rozmiaru, 2.5f * rozmiar, color);
			rysuj_prostokat(x - polowa_rozmiaru, y + rozmiar * 2.f, rozmiar, polowa_rozmiaru, color);
			x -= rozmiar * 4.f;
		} break;

		case 8: {
			rysuj_prostokat(x - rozmiar, y, polowa_rozmiaru, 2.5f * rozmiar, color);
			rysuj_prostokat(x + rozmiar, y, polowa_rozmiaru, 2.5f * rozmiar, color);
			rysuj_prostokat(x, y + rozmiar * 2.f, polowa_rozmiaru, polowa_rozmiaru, color);
			rysuj_prostokat(x, y - rozmiar * 2.f, polowa_rozmiaru, polowa_rozmiaru, color);
			rysuj_prostokat(x, y, polowa_rozmiaru, polowa_rozmiaru, color);
			x -= rozmiar * 4.f;
		} break;

		case 9: {
			rysuj_prostokat(x - polowa_rozmiaru, y + rozmiar * 2.f, rozmiar, polowa_rozmiaru, color);
			rysuj_prostokat(x - polowa_rozmiaru, y, rozmiar, polowa_rozmiaru, color);
			rysuj_prostokat(x - polowa_rozmiaru, y - rozmiar * 2.f, rozmiar, polowa_rozmiaru, color);
			rysuj_prostokat(x + rozmiar, y, polowa_rozmiaru, 2.5f * rozmiar, color);
			rysuj_prostokat(x - rozmiar, y + rozmiar, polowa_rozmiaru, polowa_rozmiaru, color);
			x -= rozmiar * 4.f;
		} break;
		}
	}
}