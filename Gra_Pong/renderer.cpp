void render_background() {
	unsigned int* piksel = (unsigned int*)stan_render.pamiec;
	for (int y = 0; y < stan_render.wysokosc; y++) {
		for (int x = 0; x < stan_render.szerokosc; x++) {
			*piksel++ = 0x0066ff*x;
		}
	}
}

void czysc_ekran(unsigned int color) {
	unsigned int* piksel = (unsigned int*)stan_render.pamiec;
	for (int y = 0; y < stan_render.wysokosc; y++) {
		for (int x = 0; x < stan_render.szerokosc; x++) {
			*piksel++ = color;
		}
	}
}

void rysuj_prostokat(int x0, int y0, int x1, int y1, unsigned int color) {
	for (int y = y0; y < y1; y++) {
		unsigned int* piksel = (unsigned int*)stan_render.pamiec + x0 + y*stan_render.szerokosc;
		for (int x = x0; x < x1; x++) {
			*piksel++ = color;
		}
	}
}