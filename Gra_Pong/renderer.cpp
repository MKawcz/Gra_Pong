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

global_variable float renderowanie_skali = 0.01f;

internal void
rysuj_prostokat(float x, float y, float polowa_rozmiaru_x, float polowa_rozmiaru_y, u32 color) {
	
	x *= stan_render.wysokosc*renderowanie_skali;
	y *= stan_render.wysokosc*renderowanie_skali;
	polowa_rozmiaru_x *= stan_render.wysokosc*renderowanie_skali;
	polowa_rozmiaru_y *= stan_render.wysokosc*renderowanie_skali;
	
	x += stan_render.szerokosc / 2.f;
	y += stan_render.wysokosc / 2.f;

	//zamiana na piksele
	int x0 = x - polowa_rozmiaru_x;
	int x1 = x + polowa_rozmiaru_x;
	int y0 = y - polowa_rozmiaru_y;
	int y1 = y + polowa_rozmiaru_y;


	rysuj_prostokat_w_pikselach(x0, y0, x1, y1, color);
}