#define w_dole(b) input->klawisze[b].w_dole
#define wcisniety(b) (input->klawisze[b].w_dole && input->klawisze[b].zmiana)
#define puszczony(b) (!input->klawisze[b].w_dole && input->klawisze[b].zmiana)


float pozycja_1_gracza, dpozycja_1_gracza, pozycja_2_gracza, dpozycja_2_gracza;
float polowa_rozmiaru_areny_x = 85, polowa_rozmiaru_areny_y = 45;
float polowa_rozmiaru_gracza_x = 2.5, polowa_rozmiaru_gracza_y = 12;
float pozycja_pilki_x, pozycja_pilki_y, dpozycja_pilki_x = 130, dpozycja_pilki_y, polowa_rozmiaru_pilki = 1;

int punkt_gracza_1, punkt_gracza_2;

internal bool
aabb_vs_aabb(float p1x, float p1y, float hs1x, float hs1y,		//sprawdza, czy nast�pi�a kolizja pi�ki ze �cian�
	float p2x, float p2y, float hs2x, float hs2y) {
	return (p1x + hs1x > p2x - hs2x &&
		p1x - hs1x < p2x + hs2x &&
		p1y + hs1y > p2y - hs2y &&
		p1y + hs1y < p2y + hs2y);
}
// lista tryb�w gry:
enum Tryb_Gry {
	GM_MENU,
	GM_GAMEPLAY,
	GM_END,
	GM_PAUSE,
};

Tryb_Gry obecny_tryb;
int hot_button;
bool przeciwnik_komputer;

internal void
symuluj_gracza(float *p, float *dp, float ddp, float dt) {
	ddp -= *dp * 10.f;			// zapobiega przed "lataniem" obiektu, sprawia, �e ruch ko�czy si� w bardziej kontrolowany spos�b

	*p = *p + *dp * dt + ddp * dt * dt * .5f;			// ustalamy pozycj� na podstawie pochodnej od pozycji (dp, pr�dko��) i pochodnej od pochodnej (ddp, przyspieszenie)
	*dp = *dp + ddp * dt;								// ustalamy pr�dko��

	if (*p + polowa_rozmiaru_gracza_y > polowa_rozmiaru_areny_y) {			// obs�uga kolizji gracza ze �cian�
		*p = polowa_rozmiaru_areny_y - polowa_rozmiaru_gracza_y;			// zatrzymujemy gracza
		*dp = 0;															// musimy te� wyzrowa� pr�dko��, poniewa� zatrzymali�my gracza
	}
	else if (*p - polowa_rozmiaru_gracza_y < -polowa_rozmiaru_areny_y) {
		*p = -polowa_rozmiaru_areny_y + polowa_rozmiaru_gracza_y;
		*dp = 0;
	}
}

internal void
symuluj_gre(Input* input, float dt) {
	rysuj_prostokat(0, 0, polowa_rozmiaru_areny_x, polowa_rozmiaru_areny_y, 0xffaa33);			// rysujemy aren�
	rysuj_obramowanie_areny(polowa_rozmiaru_areny_x, polowa_rozmiaru_areny_y, 0x0066ff);		// zamiast rysowania kolejnego prostok�ta, kt�ry s�u�y�by jako t�o areny, rysujemy tylko jej obramowanie, aby unikn�� przerysowania pikseli

	// tryb rozgrywki: 
	if (obecny_tryb == GM_GAMEPLAY) {
		float ddpozycja_1_gracza = 0.f;
		if (!przeciwnik_komputer) {										// tryb multieplayer
			if (w_dole(BUTTON_UP)) ddpozycja_1_gracza += 1300;			// ustalamy odpowiednie przyspieszenie dla obiektu gracza 1
			if (w_dole(BUTTON_DOWN)) ddpozycja_1_gracza -= 1300;		// tak jak d�ugo przycisk jest wci�ni�ty, tak d�ugo obiekt gracza 1 b�dzie si� porusza�
		}
		else {																		//tryb singleplayer
			ddpozycja_1_gracza = (pozycja_pilki_y - pozycja_1_gracza) * 100;		// ustalamy przyspieszenie dla AI, kt�ry nie bedzie przyspiesza� do maksymalnej pr�dko�ci, gdy pi�ka b�dzie blisko, dzi�ki temu unikniemy "trz�sienia si�" AI
			if (ddpozycja_1_gracza > 1300) ddpozycja_1_gracza = 1300;				// ustalamy limity pr�dko�ci AI, dzi�ki czemu b�dzie mo�liwe do pokonania			
			if (ddpozycja_1_gracza < -1300) ddpozycja_1_gracza = -1300;
		}
															// ustalamy odpowiednie przyspieszenie dla obiektu gracza 2, kt�ry b�dzie zale�ny od u�ytkownika w obu trybach
		float ddpozycja_2_gracza = 0.f;						// pochodna od pochodnej pozycji gracza, kt�ra odpowiada za przyspieszenie
		if (w_dole(BUTTON_W)) ddpozycja_2_gracza += 2000;	// je�eli przycisk jest wci�ni�ty obiekt b�dzie zwi�ksza� przyspieszenie
		if (w_dole(BUTTON_S)) ddpozycja_2_gracza -= 2000;

		symuluj_gracza(&pozycja_1_gracza, &dpozycja_1_gracza, ddpozycja_1_gracza, dt);
		symuluj_gracza(&pozycja_2_gracza, &dpozycja_2_gracza, ddpozycja_2_gracza, dt);


		//symulacja pilki
		{
			pozycja_pilki_x += dpozycja_pilki_x * dt;			// do pozycji pi�ki dodajemy jej pr�dko��
			pozycja_pilki_y += dpozycja_pilki_y * dt;			// dt to ma�a jednoplikowa biblioteka C++ do pomiaru wydajno�ci

			// warunki kolizji pi�ki z graczem:
			if (aabb_vs_aabb(pozycja_pilki_x, pozycja_pilki_y, polowa_rozmiaru_pilki, polowa_rozmiaru_pilki, 80, pozycja_1_gracza, polowa_rozmiaru_gracza_x, polowa_rozmiaru_gracza_y)) {
				pozycja_pilki_x = 80 - polowa_rozmiaru_gracza_x - polowa_rozmiaru_pilki;	// zatrzymujemy pi�k�
				dpozycja_pilki_x *= -1;														// i sprawiamy, �e leci w drug� stron�	
				dpozycja_pilki_y = (pozycja_pilki_y - pozycja_1_gracza) * 2 + dpozycja_1_gracza * .75f;		// pi�ka b�dzie lecia�a w kierunku zale�nym od tego w jakim miejscu odbije j� gracz (np. odbije pi�k� g�r� obiektu, pi�ka leci w g�r�, plus zmnienia si� jej pr�dko��)
			}
			else if (aabb_vs_aabb(pozycja_pilki_x, pozycja_pilki_y, polowa_rozmiaru_pilki, polowa_rozmiaru_pilki, -80, pozycja_2_gracza, polowa_rozmiaru_gracza_x, polowa_rozmiaru_gracza_y)) {
				pozycja_pilki_x = -80 + polowa_rozmiaru_gracza_x + polowa_rozmiaru_pilki;
				dpozycja_pilki_x *= -1;
				dpozycja_pilki_y = (pozycja_pilki_y - pozycja_2_gracza) * 2 + dpozycja_2_gracza * .75f;
			}

			// warunki kolizji pi�ki ze �cianami:
			if (pozycja_pilki_y + polowa_rozmiaru_pilki > polowa_rozmiaru_areny_y) {
				pozycja_pilki_y = polowa_rozmiaru_areny_y - polowa_rozmiaru_pilki;
				dpozycja_pilki_y *= -1;
			}
			else if (pozycja_pilki_y - polowa_rozmiaru_pilki < -polowa_rozmiaru_areny_y) {
				pozycja_pilki_y = -polowa_rozmiaru_areny_y + polowa_rozmiaru_pilki;
				dpozycja_pilki_y *= -1;
			}

			// warunki przejscia pi�ki za gracza
			if (pozycja_pilki_x + polowa_rozmiaru_pilki > polowa_rozmiaru_areny_x) {
				dpozycja_pilki_x *= -1;		// kiedy gracz przepu�ci pi�k�, jej pozycja si� resetuje i pi�ka zmienia kierunek
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
		// rysujemy wyniki graczy:
		rysuj_liczby(punkt_gracza_1, -10, 40, 1.f, 0xbbffbb);
		rysuj_liczby(punkt_gracza_2, 10, 40, 1.f, 0xbbffbb);

		// po zdobyciu 10 pkt przez jednego z graczy, przenosimy si� do ekranu ko�ca rozgrywki:
		if (punkt_gracza_1 == 10 || punkt_gracza_2 == 10)
		{
			obecny_tryb = GM_END;	
		}
		// je�li gracz wci�nie przycisk ESC, przenosimy si� do ekranu pauzy 
		if (wcisniety(BUTTON_ESC)) {
			obecny_tryb = GM_PAUSE;
		}

		//renderowanie
		rysuj_prostokat(pozycja_pilki_x, pozycja_pilki_y, polowa_rozmiaru_pilki, polowa_rozmiaru_pilki, 0xffffff);


		rysuj_prostokat(80, pozycja_1_gracza, polowa_rozmiaru_gracza_x, polowa_rozmiaru_gracza_y, 0xff0000);
		rysuj_prostokat(-80, pozycja_2_gracza, polowa_rozmiaru_gracza_x, polowa_rozmiaru_gracza_y, 0xff0000);

	} else if (obecny_tryb == GM_PAUSE) {				// EKRAN PAUZY:

		// umo�liwiamy wyb�r przycisku za pomoc� strza�ek, zmieniaj�c warto�� "hot_button":
		if (wcisniety(BUTTON_LEFT) || wcisniety(BUTTON_RIGHT)) {
			hot_button = !hot_button;
		}

		// w zale�no�ci od wybranego przycisku, po naci�ni�ciu enter u�ytkownik zostaje przeniesiony do odpowiedniego trybu gry:
		if (wcisniety(BUTTON_ENTER)) {
			if (hot_button == 0) {
				obecny_tryb = GM_GAMEPLAY;
			}
			else {
				obecny_tryb = GM_MENU;

				// po przejsciu do MENU, resetujemy wynik, pozycje graczy oraz pi�ki:
				punkt_gracza_1 = 0;
				punkt_gracza_2 = 0;

				pozycja_1_gracza = 0.f;
				dpozycja_1_gracza = 0.f;
				pozycja_2_gracza = 0.f;
				dpozycja_2_gracza = 0.f;

				
				pozycja_pilki_x = 0;
				pozycja_pilki_y = 0;
				dpozycja_pilki_x = 130;
				dpozycja_pilki_y = 0;
				
			}
		}
		// zmieniamy kolor przycisk�w w zale�no�ci od tego, kt�ry wybrali�my:
		if (hot_button == 0) {

			rysuj_tekst("RESUME", -65, -10, 1, 0xff0000);
			rysuj_tekst("EXIT TO MENU", 10, -10, 1, 0xaaaaaa);
		}
		else {

			rysuj_tekst("RESUME", -65, -10, 1, 0xaaaaaa);
			rysuj_tekst("EXIT TO MENU", 10, -10, 1, 0xff0000);
		}
		// tytu� ekranu:
		rysuj_tekst("PAUSE", -30, 30, 2, 0xfffffff);

	} else if (obecny_tryb == GM_END) {				// EKRAN KO�CA ROZGRYWKI:
		
		// umo�liwiamy wyb�r przycisku za pomoc� strza�ek, zmieniaj�c warto�� "hot_button":
		if (wcisniety(BUTTON_LEFT) || wcisniety(BUTTON_RIGHT)) {
			hot_button = !hot_button;
		}
		
		// w zale�no�ci od wybranego przycisku, po naci�ni�ciu enter u�ytkownik zostaje przeniesiony do odpowiedniego trybu gry:
		if (wcisniety(BUTTON_ENTER)) {
			if (hot_button == 0) {
				obecny_tryb = GM_GAMEPLAY;
			}
			else {
				obecny_tryb = GM_MENU;
			}

			// po zako�czonej rozgrywce, resetujemy wynik i pozycje graczy:
			punkt_gracza_1 = 0;
			punkt_gracza_2 = 0;

			pozycja_1_gracza = 0.f;
			dpozycja_1_gracza = 0.f;
			pozycja_2_gracza = 0.f;
			dpozycja_2_gracza = 0.f;

			
		}
		
		// zmieniamy kolor przycisk�w w zale�no�ci od tego, kt�ry wybrali�my:
		if (hot_button == 0) {

			rysuj_tekst("PLAY AGAIN", -80, -10, 1, 0xff0000);
			rysuj_tekst("EXIT TO MENU", 10, -10, 1, 0xaaaaaa);
		}
		else {

			rysuj_tekst("PLAY AGAIN", -80, -10, 1, 0xaaaaaa);
			rysuj_tekst("EXIT TO MENU", 10, -10, 1, 0xff0000);
		}
		
		// tytu� ekranu, zale�ny od tego, kt�ry gracz wygra�:
		if (punkt_gracza_1 == 10) {
			rysuj_tekst("PLAYER ONE WON", -63, 30, 1.5, 0xffffff);
		}
		else if (punkt_gracza_2 == 10) {
			rysuj_tekst("PLAYER TWO WON", -63, 30, 1.5, 0xffffff);

		}
		
	} else {			// MENU:
		
		// umo�liwiamy wyb�r przycisku za pomoc� strza�ek, zmieniaj�c warto�� "hot_button":
		if (wcisniety(BUTTON_RIGHT)) {		 
			hot_button++;
			if (hot_button > 2)
			{
				hot_button = 0;
			}
		}
		else if (wcisniety(BUTTON_LEFT)) {
			hot_button--;
			if (hot_button < 0)
			{
				hot_button = 2;
			}
		}

		// w zale�no�ci od wybranego przycisku, po naci�ni�ciu enter u�ytkownik zostaje przeniesiony do odpowiedniego trybu gry
		if (wcisniety(BUTTON_ENTER)) {
			if (hot_button == 0) {
				obecny_tryb = GM_GAMEPLAY;
				przeciwnik_komputer = 1;		// tryb singleplayer
			}
			else if (hot_button == 1) {
				obecny_tryb = GM_GAMEPLAY;
				przeciwnik_komputer = 0;		// tryb multiplayer
			}
			else if (hot_button == 2) {
				DestroyWindow(window);			// zamkni�cie programu
			}
		}
		
		// zmieniamy kolor przycisk�w w zale�no�ci od tego, kt�ry wybrali�my:
		if (hot_button == 0) {
			rysuj_tekst("SINGLE PLAYER", -80, -10, 1, 0xff0000);
			rysuj_tekst("MULTIPLAYER", 20, -10, 1, 0xaaaaaa);
			rysuj_tekst("EXIT PONG", -30, -30, 1, 0xaaaaaa);
		}
		else if (hot_button == 1) {
			rysuj_tekst("SINGLE PLAYER", -80, -10, 1, 0xaaaaaa);
			rysuj_tekst("MULTIPLAYER", 20, -10, 1, 0xff0000);
			rysuj_tekst("EXIT PONG", -30, -30, 1, 0xaaaaaa); 
		}
		else {
			rysuj_tekst("SINGLE PLAYER", -80, -10, 1, 0xaaaaaa);
			rysuj_tekst("MULTIPLAYER", 20, -10, 1, 0xaaaaaa);
			rysuj_tekst("EXIT PONG", -30, -30, 1, 0xff0000);
		}

		// tytu� menu:
		rysuj_tekst("PONG GAME", -50, 30, 2, 0xfffffff);

	}
}