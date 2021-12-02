#define w_dole(b) input->klawisze[b].w_dole
#define wcisniety(b) (input->klawisze[b].w_dole && input->klawisze[b].zmiana)
#define puszczony(b) (!input->klawisze[b].w_dole && input->klawisze[b].zmiana)

float pozycja_gracza_x = 0.f;
float pozycja_gracza_y = 0.f;


internal void
symuluj_gre(Input* input, float dt) {
	czysc_ekran(0x0066ff);
	float predkosc = 50.f; // jednostek na sekunde
	if (w_dole(BUTTON_UP)) pozycja_gracza_y += predkosc*dt;
	if (w_dole(BUTTON_DOWN)) pozycja_gracza_y -= predkosc * dt;
	if (w_dole(BUTTON_RIGHT)) pozycja_gracza_x += predkosc * dt;
	if (w_dole(BUTTON_LEFT)) pozycja_gracza_x -= predkosc * dt;



	rysuj_prostokat(pozycja_gracza_x, pozycja_gracza_y, 1, 1, 0xff0000);
}