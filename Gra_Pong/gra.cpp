#define w_dole(b) input->klawisze[b].w_dole
#define wcisniety(b) (input->klawisze[b].w_dole && input->klawisze[b].zmiana)
#define puszczony(b) (!input->klawisze[b].w_dole && input->klawisze[b].zmiana)


float pozycja_1_gracza, dpozycja_1_gracza, pozycja_2_gracza, dpozycja_2_gracza;
float polowa_rozmiaru_areny_x = 85, polowa_rozmiaru_areny_y = 45;
float polowa_rozmiaru_gracza_x = 2.5, polowa_rozmiaru_gracza_y = 12;
float pozycja_pilki_x, pozycja_pilki_y, dpozycja_pilki_x = 130, dpozycja_pilki_y, polowa_rozmiaru_pilki = 1;

int punkt_gracza_1, punkt_gracza_2;

internal bool
aabb_vs_aabb(float p1x, float p1y, float hs1x, float hs1y,		//sprawdza, czy nast¹pi³a kolizja pi³ki ze œcian¹
	float p2x, float p2y, float hs2x, float hs2y) {
	return (p1x + hs1x > p2x - hs2x &&
		p1x - hs1x < p2x + hs2x &&
		p1y + hs1y > p2y - hs2y &&
		p1y + hs1y < p2y + hs2y);
}
// lista trybów gry:
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
	ddp -= *dp * 10.f;			// zapobiega przed "lataniem" obiektu, sprawia, ¿e ruch koñczy siê w bardziej kontrolowany sposób

	*p = *p + *dp * dt + ddp * dt * dt * .5f;			// ustalamy pozycjê na podstawie pochodnej od pozycji (dp, prêdkoœæ) i pochodnej od pochodnej (ddp, przyspieszenie)
	*dp = *dp + ddp * dt;								// ustalamy prêdkoœæ

	if (*p + polowa_rozmiaru_gracza_y > polowa_rozmiaru_areny_y) {			// obs³uga kolizji gracza ze œcian¹
		*p = polowa_rozmiaru_areny_y - polowa_rozmiaru_gracza_y;			// zatrzymujemy gracza
		*dp = 0;															// musimy te¿ wyzrowaæ prêdkoœæ, poniewa¿ zatrzymaliœmy gracza
	}
	else if (*p - polowa_rozmiaru_gracza_y < -polowa_rozmiaru_areny_y) {
		*p = -polowa_rozmiaru_areny_y + polowa_rozmiaru_gracza_y;
		*dp = 0;
	}
}

internal void
symuluj_gre(Input* input, float dt) {
	rysuj_prostokat(0, 0, polowa_rozmiaru_areny_x, polowa_rozmiaru_areny_y, 0xffaa33);			// rysujemy arenê
	rysuj_obramowanie_areny(polowa_rozmiaru_areny_x, polowa_rozmiaru_areny_y, 0x0066ff);		// zamiast rysowania kolejnego prostok¹ta, który s³u¿y³by jako t³o areny, rysujemy tylko jej obramowanie, aby unikn¹æ przerysowania pikseli

	// tryb rozgrywki: 
	if (obecny_tryb == GM_GAMEPLAY) {
		float ddpozycja_1_gracza = 0.f;
		if (!przeciwnik_komputer) {										// tryb multieplayer
			if (w_dole(BUTTON_UP)) ddpozycja_1_gracza += 1300;			// ustalamy odpowiednie przyspieszenie dla obiektu gracza 1
			if (w_dole(BUTTON_DOWN)) ddpozycja_1_gracza -= 1300;		// tak jak d³ugo przycisk jest wciœniêty, tak d³ugo obiekt gracza 1 bêdzie siê porusza³
		}
		else {																		//tryb singleplayer
			ddpozycja_1_gracza = (pozycja_pilki_y - pozycja_1_gracza) * 100;		// ustalamy przyspieszenie dla AI, który nie bedzie przyspiesza³ do maksymalnej prêdkoœci, gdy pi³ka bêdzie blisko, dziêki temu unikniemy "trzêsienia siê" AI
			if (ddpozycja_1_gracza > 1300) ddpozycja_1_gracza = 1300;				// ustalamy limity prêdkoœci AI, dziêki czemu bêdzie mo¿liwe do pokonania			
			if (ddpozycja_1_gracza < -1300) ddpozycja_1_gracza = -1300;
		}
															// ustalamy odpowiednie przyspieszenie dla obiektu gracza 2, który bêdzie zale¿ny od u¿ytkownika w obu trybach
		float ddpozycja_2_gracza = 0.f;						// pochodna od pochodnej pozycji gracza, która odpowiada za przyspieszenie
		if (w_dole(BUTTON_W)) ddpozycja_2_gracza += 2000;	// je¿eli przycisk jest wciœniêty obiekt bêdzie zwiêksza³ przyspieszenie
		if (w_dole(BUTTON_S)) ddpozycja_2_gracza -= 2000;

		symuluj_gracza(&pozycja_1_gracza, &dpozycja_1_gracza, ddpozycja_1_gracza, dt);
		symuluj_gracza(&pozycja_2_gracza, &dpozycja_2_gracza, ddpozycja_2_gracza, dt);


		//symulacja pilki
		{
			pozycja_pilki_x += dpozycja_pilki_x * dt;			// do pozycji pi³ki dodajemy jej prêdkoœæ
			pozycja_pilki_y += dpozycja_pilki_y * dt;			// dt to ma³a jednoplikowa biblioteka C++ do pomiaru wydajnoœci

			// warunki kolizji pi³ki z graczem:
			if (aabb_vs_aabb(pozycja_pilki_x, pozycja_pilki_y, polowa_rozmiaru_pilki, polowa_rozmiaru_pilki, 80, pozycja_1_gracza, polowa_rozmiaru_gracza_x, polowa_rozmiaru_gracza_y)) {
				pozycja_pilki_x = 80 - polowa_rozmiaru_gracza_x - polowa_rozmiaru_pilki;	// zatrzymujemy pi³kê
				dpozycja_pilki_x *= -1;														// i sprawiamy, ¿e leci w drug¹ stronê	
				dpozycja_pilki_y = (pozycja_pilki_y - pozycja_1_gracza) * 2 + dpozycja_1_gracza * .75f;		// pi³ka bêdzie lecia³a w kierunku zale¿nym od tego w jakim miejscu odbije j¹ gracz (np. odbije pi³kê gór¹ obiektu, pi³ka leci w górê, plus zmnienia siê jej prêdkoœæ)
			}
			else if (aabb_vs_aabb(pozycja_pilki_x, pozycja_pilki_y, polowa_rozmiaru_pilki, polowa_rozmiaru_pilki, -80, pozycja_2_gracza, polowa_rozmiaru_gracza_x, polowa_rozmiaru_gracza_y)) {
				pozycja_pilki_x = -80 + polowa_rozmiaru_gracza_x + polowa_rozmiaru_pilki;
				dpozycja_pilki_x *= -1;
				dpozycja_pilki_y = (pozycja_pilki_y - pozycja_2_gracza) * 2 + dpozycja_2_gracza * .75f;
			}

			// warunki kolizji pi³ki ze œcianami:
			if (pozycja_pilki_y + polowa_rozmiaru_pilki > polowa_rozmiaru_areny_y) {
				pozycja_pilki_y = polowa_rozmiaru_areny_y - polowa_rozmiaru_pilki;
				dpozycja_pilki_y *= -1;
			}
			else if (pozycja_pilki_y - polowa_rozmiaru_pilki < -polowa_rozmiaru_areny_y) {
				pozycja_pilki_y = -polowa_rozmiaru_areny_y + polowa_rozmiaru_pilki;
				dpozycja_pilki_y *= -1;
			}

			// warunki przejscia pi³ki za gracza
			if (pozycja_pilki_x + polowa_rozmiaru_pilki > polowa_rozmiaru_areny_x) {
				dpozycja_pilki_x *= -1;		// kiedy gracz przepuœci pi³kê, jej pozycja siê resetuje i pi³ka zmienia kierunek
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

		// po zdobyciu 10 pkt przez jednego z graczy, przenosimy siê do ekranu koñca rozgrywki:
		if (punkt_gracza_1 == 10 || punkt_gracza_2 == 10)
		{
			obecny_tryb = GM_END;	
		}
		// jeœli gracz wciœnie przycisk ESC, przenosimy siê do ekranu pauzy 
		if (wcisniety(BUTTON_ESC)) {
			obecny_tryb = GM_PAUSE;
		}

		//renderowanie
		rysuj_prostokat(pozycja_pilki_x, pozycja_pilki_y, polowa_rozmiaru_pilki, polowa_rozmiaru_pilki, 0xffffff);


		rysuj_prostokat(80, pozycja_1_gracza, polowa_rozmiaru_gracza_x, polowa_rozmiaru_gracza_y, 0xff0000);
		rysuj_prostokat(-80, pozycja_2_gracza, polowa_rozmiaru_gracza_x, polowa_rozmiaru_gracza_y, 0xff0000);

	} else if (obecny_tryb == GM_PAUSE) {				// EKRAN PAUZY:

		// umo¿liwiamy wybór przycisku za pomoc¹ strza³ek, zmieniaj¹c wartoœæ "hot_button":
		if (wcisniety(BUTTON_LEFT) || wcisniety(BUTTON_RIGHT)) {
			hot_button = !hot_button;
		}

		// w zale¿noœci od wybranego przycisku, po naciœniêciu enter u¿ytkownik zostaje przeniesiony do odpowiedniego trybu gry:
		if (wcisniety(BUTTON_ENTER)) {
			if (hot_button == 0) {
				obecny_tryb = GM_GAMEPLAY;
			}
			else {
				obecny_tryb = GM_MENU;

				// po przejsciu do MENU, resetujemy wynik, pozycje graczy oraz pi³ki:
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
		// zmieniamy kolor przycisków w zale¿noœci od tego, który wybraliœmy:
		if (hot_button == 0) {

			rysuj_tekst("RESUME", -65, -10, 1, 0xff0000);
			rysuj_tekst("EXIT TO MENU", 10, -10, 1, 0xaaaaaa);
		}
		else {

			rysuj_tekst("RESUME", -65, -10, 1, 0xaaaaaa);
			rysuj_tekst("EXIT TO MENU", 10, -10, 1, 0xff0000);
		}
		// tytu³ ekranu:
		rysuj_tekst("PAUSE", -30, 30, 2, 0xfffffff);

	} else if (obecny_tryb == GM_END) {				// EKRAN KOÑCA ROZGRYWKI:
		
		// umo¿liwiamy wybór przycisku za pomoc¹ strza³ek, zmieniaj¹c wartoœæ "hot_button":
		if (wcisniety(BUTTON_LEFT) || wcisniety(BUTTON_RIGHT)) {
			hot_button = !hot_button;
		}
		
		// w zale¿noœci od wybranego przycisku, po naciœniêciu enter u¿ytkownik zostaje przeniesiony do odpowiedniego trybu gry:
		if (wcisniety(BUTTON_ENTER)) {
			if (hot_button == 0) {
				obecny_tryb = GM_GAMEPLAY;
			}
			else {
				obecny_tryb = GM_MENU;
			}

			// po zakoñczonej rozgrywce, resetujemy wynik i pozycje graczy:
			punkt_gracza_1 = 0;
			punkt_gracza_2 = 0;

			pozycja_1_gracza = 0.f;
			dpozycja_1_gracza = 0.f;
			pozycja_2_gracza = 0.f;
			dpozycja_2_gracza = 0.f;

			
		}
		
		// zmieniamy kolor przycisków w zale¿noœci od tego, który wybraliœmy:
		if (hot_button == 0) {

			rysuj_tekst("PLAY AGAIN", -80, -10, 1, 0xff0000);
			rysuj_tekst("EXIT TO MENU", 10, -10, 1, 0xaaaaaa);
		}
		else {

			rysuj_tekst("PLAY AGAIN", -80, -10, 1, 0xaaaaaa);
			rysuj_tekst("EXIT TO MENU", 10, -10, 1, 0xff0000);
		}
		
		// tytu³ ekranu, zale¿ny od tego, który gracz wygra³:
		if (punkt_gracza_1 == 10) {
			rysuj_tekst("PLAYER ONE WON", -63, 30, 1.5, 0xffffff);
		}
		else if (punkt_gracza_2 == 10) {
			rysuj_tekst("PLAYER TWO WON", -63, 30, 1.5, 0xffffff);

		}
		
	} else {			// MENU:
		
		// umo¿liwiamy wybór przycisku za pomoc¹ strza³ek, zmieniaj¹c wartoœæ "hot_button":
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

		// w zale¿noœci od wybranego przycisku, po naciœniêciu enter u¿ytkownik zostaje przeniesiony do odpowiedniego trybu gry
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
				DestroyWindow(window);			// zamkniêcie programu
			}
		}
		
		// zmieniamy kolor przycisków w zale¿noœci od tego, który wybraliœmy:
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

		// tytu³ menu:
		rysuj_tekst("PONG GAME", -50, 30, 2, 0xfffffff);

	}
}