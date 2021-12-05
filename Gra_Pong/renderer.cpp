internal void 
render_background() {
	unsigned int* piksel = (u32*)stan_render.pamiec;
	for (int y = 0; y < stan_render.wysokosc; y++) {
		for (int x = 0; x < stan_render.szerokosc; x++) {
			*piksel++ = 0x0066ff*x;
		}
	}
}

internal void 
czysc_ekran(u32 color) {
	unsigned int* piksel = (u32*)stan_render.pamiec;
	for (int y = 0; y < stan_render.wysokosc; y++) {
		for (int x = 0; x < stan_render.szerokosc; x++) {
			*piksel++ = color;
		}
	}
}


internal void 
rysuj_prostokat_w_pikselach(int x0, int y0, int x1, int y1, u32 color) {
	
	x0 = zacisk(0, x0, stan_render.szerokosc);
	x1 = zacisk(0, x1, stan_render.szerokosc);
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

internal void
rysuj_obramowanie_areny(float arena_x, float arena_y, u32 color) {
	arena_x *= stan_render.wysokosc * skala_renderowania;
	arena_y *= stan_render.wysokosc * skala_renderowania;

	int x0 = (int)((float)stan_render.szerokosc * .5f - arena_x);
	int x1 = (int)((float)stan_render.szerokosc * .5f + arena_x);
	int y0 = (int)((float)stan_render.wysokosc * .5f - arena_y);
	int y1 = (int)((float)stan_render.wysokosc * .5f + arena_y);

	rysuj_prostokat_w_pikselach(0, 0, stan_render.szerokosc, y0, color);
	rysuj_prostokat_w_pikselach(0, y1, x1, stan_render.wysokosc, color);
	rysuj_prostokat_w_pikselach(0, y0, x0, y1, color);
	rysuj_prostokat_w_pikselach(x1, y0, stan_render.szerokosc, stan_render.wysokosc, color);
}

internal void
rysuj_prostokat(float x, float y, float polowa_rozmiaru_x, float polowa_rozmiaru_y, u32 color) {
	
	x *= stan_render.wysokosc*skala_renderowania;
	y *= stan_render.wysokosc*skala_renderowania;
	polowa_rozmiaru_x *= stan_render.wysokosc*skala_renderowania;
	polowa_rozmiaru_y *= stan_render.wysokosc*skala_renderowania;
	
	x += stan_render.szerokosc / 2.f;
	y += stan_render.wysokosc / 2.f;

	//zamiana na piksele
	int x0 = x - polowa_rozmiaru_x;
	int x1 = x + polowa_rozmiaru_x;
	int y0 = y - polowa_rozmiaru_y;
	int y1 = y + polowa_rozmiaru_y;


	rysuj_prostokat_w_pikselach(x0, y0, x1, y1, color);
}

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

internal void
rysuj_tekst(const char *tekst, float x, float y, float rozmiar, u32 color) {
	float polowa_rozmiaru = rozmiar * .5f;
	float oryginalny_y = y;

	while (*tekst) {
		if (*tekst != 32) {
			const char** litera;
			if (*tekst == 47) litera = litery[27];
			else if (*tekst == 46) litera = litery[26]; 
			else litera = litery[*tekst - 'A'];
			float oryginalny_x = x;

			for (int i = 0; i < 7; i++) {
				const char* wiersz = litera[i];
				while (*wiersz) {
					if (*wiersz == '0') {
						rysuj_prostokat(x, y, polowa_rozmiaru, polowa_rozmiaru, color);
					}
					x += rozmiar;
					wiersz++;
				}
				y -= rozmiar;
				x = oryginalny_x;
			}
		}
		tekst++;
		x += rozmiar * 6.f;
		y = oryginalny_y;
	}
}

internal void
rysuj_liczby(int liczba, float x, float y, float rozmiar, u32 color) {
	float polowa_rozmiaru = rozmiar * .5f;
	
	bool rysuj_liczby = false;
	while (liczba || !rysuj_liczby) {
		rysuj_liczby = true;
		int cyfra = liczba % 10;
		liczba = liczba / 10;

		switch (cyfra) {
		case 0: {
			rysuj_prostokat(x - rozmiar, y, polowa_rozmiaru, 2.5f * rozmiar, color);
			rysuj_prostokat(x + rozmiar, y, polowa_rozmiaru, 2.5f * rozmiar, color);
			rysuj_prostokat(x, y + rozmiar * 2.f, polowa_rozmiaru, polowa_rozmiaru, color);
			rysuj_prostokat(x, y - rozmiar * 2.f, polowa_rozmiaru, polowa_rozmiaru, color);
			x -= rozmiar * 4.f;
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