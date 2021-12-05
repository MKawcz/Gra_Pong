#define w_dole(b) input->klawisze[b].w_dole
#define wcisniety(b) (input->klawisze[b].w_dole && input->klawisze[b].zmiana)
#define puszczony(b) (!input->klawisze[b].w_dole && input->klawisze[b].zmiana)

float pozycja_1_gracza, dpozycja_1_gracza, pozycja_2_gracza, dpozycja_2_gracza;
float polowa_rozmiaru_areny_x = 85, polowa_rozmiaru_areny_y = 45;
float polowa_rozmiaru_gracza_x = 2.5, polowa_rozmiaru_gracza_y = 12;
float pozycja_pilki_x, pozycja_pilki_y, dpozycja_pilki_x = 130, dpozycja_pilki_y, polowa_rozmiaru_pilki = 1;

int punkt_gracza_1, punkt_gracza_2;

internal bool
aabb_vs_aabb(float p1x, float p1y, float hs1x, float hs1y,
	float p2x, float p2y, float hs2x, float hs2y) {
	return (p1x + hs1x > p2x - hs2x &&
		p1x - hs1x < p2x + hs2x &&
		p1y + hs1y > p2y - hs2y &&
		p1y + hs1y < p2y + hs2y);
}

enum Tryb_Gry {
	GM_MENU,
	GM_GAMEPLAY,
};

Tryb_Gry obecny_tryb;
int hot_button;
bool przeciwnik_komputer;

internal void
symuluj_gracza(float *p, float *dp, float ddp, float dt) {
	ddp -= *dp * 10.f;

	*p = *p + *dp * dt + ddp * dt * dt * .5f;
	*dp = *dp + ddp * dt;

	if (*p + polowa_rozmiaru_gracza_y > polowa_rozmiaru_areny_y) {
		*p = polowa_rozmiaru_areny_y - polowa_rozmiaru_gracza_y;
		*dp = 0;
	}
	else if (*p - polowa_rozmiaru_gracza_y < -polowa_rozmiaru_areny_y) {
		*p = -polowa_rozmiaru_areny_y + polowa_rozmiaru_gracza_y;
		*dp = 0;
	}
}

internal void
symuluj_gre(Input* input, float dt) {
	rysuj_prostokat(0, 0, polowa_rozmiaru_areny_x, polowa_rozmiaru_areny_y, 0xffaa33);
	rysuj_obramowanie_areny(polowa_rozmiaru_areny_x, polowa_rozmiaru_areny_y, 0x0066ff);


	if (obecny_tryb == GM_GAMEPLAY) {
		float ddpozycja_1_gracza = 0.f;
		if (!przeciwnik_komputer) {
			if (w_dole(BUTTON_UP)) ddpozycja_1_gracza += 1300;
			if (w_dole(BUTTON_DOWN)) ddpozycja_1_gracza -= 1300;
		}
		else {
			//if (pozycja_pilki_y > pozycja_1_gracza+2.f) ddpozycja_1_gracza += 2000;
			//if (pozycja_pilki_y < pozycja_1_gracza-2.f) ddpozycja_1_gracza -= 2000;
			ddpozycja_1_gracza = (pozycja_pilki_y - pozycja_1_gracza) * 100;
			if (ddpozycja_1_gracza > 1300) ddpozycja_1_gracza = 1300;
			if (ddpozycja_1_gracza < -1300) ddpozycja_1_gracza = -1300;
		}

		float ddpozycja_2_gracza = 0.f;
		if (w_dole(BUTTON_W)) ddpozycja_2_gracza += 2000;
		if (w_dole(BUTTON_S)) ddpozycja_2_gracza -= 2000;

		symuluj_gracza(&pozycja_1_gracza, &dpozycja_1_gracza, ddpozycja_1_gracza, dt);
		symuluj_gracza(&pozycja_2_gracza, &dpozycja_2_gracza, ddpozycja_2_gracza, dt);


		//symulacja pilki
		{
			pozycja_pilki_x += dpozycja_pilki_x * dt;
			pozycja_pilki_y += dpozycja_pilki_y * dt;

			if (aabb_vs_aabb(pozycja_pilki_x, pozycja_pilki_y, polowa_rozmiaru_pilki, polowa_rozmiaru_pilki, 80, pozycja_1_gracza, polowa_rozmiaru_gracza_x, polowa_rozmiaru_gracza_y)) {
				pozycja_pilki_x = 80 - polowa_rozmiaru_gracza_x - polowa_rozmiaru_pilki;
				dpozycja_pilki_x *= -1;
				dpozycja_pilki_y = (pozycja_pilki_y - pozycja_1_gracza) * 2 + dpozycja_1_gracza * .75f;
			}
			else if (aabb_vs_aabb(pozycja_pilki_x, pozycja_pilki_y, polowa_rozmiaru_pilki, polowa_rozmiaru_pilki, -80, pozycja_2_gracza, polowa_rozmiaru_gracza_x, polowa_rozmiaru_gracza_y)) {
				pozycja_pilki_x = -80 + polowa_rozmiaru_gracza_x + polowa_rozmiaru_pilki;
				dpozycja_pilki_x *= -1;
				dpozycja_pilki_y = (pozycja_pilki_y - pozycja_2_gracza) * 2 + dpozycja_2_gracza * .75f;
			}

			if (pozycja_pilki_y + polowa_rozmiaru_pilki > polowa_rozmiaru_areny_y) {
				pozycja_pilki_y = polowa_rozmiaru_areny_y - polowa_rozmiaru_pilki;
				dpozycja_pilki_y *= -1;
			}
			else if (pozycja_pilki_y - polowa_rozmiaru_pilki < -polowa_rozmiaru_areny_y) {
				pozycja_pilki_y = -polowa_rozmiaru_areny_y + polowa_rozmiaru_pilki;
				dpozycja_pilki_y *= -1;
			}

			if (pozycja_pilki_x + polowa_rozmiaru_pilki > polowa_rozmiaru_areny_x) {
				dpozycja_pilki_x *= -1;
				dpozycja_pilki_y = 0;
				pozycja_pilki_x = 0;
				pozycja_pilki_y = 0;
				punkt_gracza_1++;
			}
			else if (pozycja_pilki_x - polowa_rozmiaru_pilki < -polowa_rozmiaru_areny_x) {
				dpozycja_pilki_x *= -1;
				dpozycja_pilki_y = 0;
				pozycja_pilki_x = 0;
				pozycja_pilki_y = 0;
				punkt_gracza_2++;
			}
		}

		rysuj_liczby(punkt_gracza_1, -10, 40, 1.f, 0xbbffbb);
		rysuj_liczby(punkt_gracza_2, 10, 40, 1.f, 0xbbffbb);


		//renderowanie
		rysuj_prostokat(pozycja_pilki_x, pozycja_pilki_y, polowa_rozmiaru_pilki, polowa_rozmiaru_pilki, 0xffffff);


		rysuj_prostokat(80, pozycja_1_gracza, polowa_rozmiaru_gracza_x, polowa_rozmiaru_gracza_y, 0xff0000);
		rysuj_prostokat(-80, pozycja_2_gracza, polowa_rozmiaru_gracza_x, polowa_rozmiaru_gracza_y, 0xff0000);

	} else {
		if (wcisniety(BUTTON_LEFT) || wcisniety(BUTTON_RIGHT)) {
			hot_button = !hot_button;
		}


		if (wcisniety(BUTTON_ENTER)) {
			obecny_tryb = GM_GAMEPLAY;
			przeciwnik_komputer = hot_button ? 0 : 1;
		}

		if (hot_button == 0) {
			rysuj_tekst("SINGLE PLAYER", -80, -10, 1, 0xff0000);
			rysuj_tekst("MULTIPLAYER", 20, -10, 1, 0xaaaaaa);
		}
		else {
			rysuj_tekst("SINGLE PLAYER", -80, -10, 1, 0xaaaaaa);
			rysuj_tekst("MULTIPLAYER", 20, -10, 1, 0xff0000);
		}

		rysuj_tekst("PONG GAME", -45, 30, 2, 0xfffffff);

	}
}