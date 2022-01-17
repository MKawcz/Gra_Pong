// PLIK PRZECHOWUJ�CY KOD DLA WARSTWY PLATFORMY WINDOWS

#include "utils.cpp"
#include <windows.h>

global_variable bool chodzi = true;  // zmienna okre�laj�ca, czy gra chodzi (dzia�a)
global_variable HWND window;

struct stan_renderowania {
	int wysokosc, szerokosc;	// wysoko�� i szeroko�� buforu
	void* pamiec;				// pami�� buforu

	BITMAPINFO bitmap_info;		//struktura, kt�ra zawiera informacje na temat wymiar�w i formatu koloru.
};

global_variable stan_renderowania stan_render;

#include "wspolna_platforma.cpp"
#include "renderer.cpp"
#include "gra.cpp"

// specyfikacja funkcji zwracaj�cej informacje o wszelkich akcjach na oknie, zgodna z dokumentacj�:
LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	/*
	hwnd - Uchwyt do procedury okna obs�uguj�cej komunikat
	msg - Komunikat
	wParam - Dodatkowe informacje specyficzne dla danego komunikatu. Warto�� tego argumentu zale�y od warto�ci argumentu msg
	lParam - Dodatkowe informacje specyficzne dla danego komunikatu. Warto�� tego argumentu zale�y od warto�ci argumentu msg
	*/

	LRESULT wynik = 0;
	switch (uMsg) {
	case WM_CLOSE:
	case WM_DESTROY: {
		chodzi = false;		// jezeli okno zostanie zniszczone gra przestanie chodzic
	} break;

	case WM_SIZE: {			// odbiera widomo�� dotycz�ce zmiany rozmiaru okna
		RECT prostokat;		// struktura zawieraj�ca rozmiary okna
		GetClientRect(hwnd, &prostokat);		// Pobiera wsp�rz�dne obszaru okna klienta
		stan_render.szerokosc = prostokat.right - prostokat.left;	// �eby wyznaczy� szeroko�� odejmujemy wsp�rz�dn� x lewego g�rnego rogu prostok�ta od x prawego
		stan_render.wysokosc = prostokat.bottom - prostokat.top;	// �eby wyznaczy� wysoko�� odejmujemy wsp�rz�dn� y lewego g�rnego rogu prostok�ta od y prawego

		// rozmiar buforu:
		int rozmiar = stan_render.szerokosc * stan_render.wysokosc * sizeof(unsigned int); 

		// je�eli jest ju� przechowywana jaka� warto�� pami�ci, zwalniamy j�, aby przydzieli� now�: 
		if (stan_render.pamiec) VirtualFree(stan_render.pamiec, 0, MEM_RELEASE);
		stan_render.pamiec = VirtualAlloc(0, rozmiar, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);  // funkcja powalaj�ca zarezerwowa� lub zmieni� alokacj� kodu w pami�ci
		/* Parametry:
		 0 - adres pocz�tkowy regionu do przydzielenia
		 rozmiar - rozmiar regionu w bajtach
		 MEM_COMMIT | MEM_RESERVE - rezerwuje i zatwierdza zakres wirtualnej przestrzeni adresowej
		 PAGE_READWRITE - Ochrona pami�ci dla regionu stron, kt�re maj� zosta� przydzielone
		*/

		// struktura bmiHeader zawiera informacje o wymiarach i formacie koloru mapy bitowej niezale�nej od urz�dzenia:
		stan_render.bitmap_info.bmiHeader.biSize = sizeof(stan_render.bitmap_info.bmiHeader);		// biSize - Okre�la liczb� bajt�w wymaganych przez struktur�
		stan_render.bitmap_info.bmiHeader.biWidth = stan_render.szerokosc;							// biWidth - Okre�la szeroko�� mapy bitowej w pikselach
		stan_render.bitmap_info.bmiHeader.biHeight = stan_render.wysokosc;							// biHeight - Okre�la wysoko�� mapy bitowej w pikselach
		stan_render.bitmap_info.bmiHeader.biPlanes = 1;												// biPlanes - Okre�la liczb� p�aszczyzn dla urz�dzenia docelowego. Ta warto�� musi by� ustawiona na 1
		stan_render.bitmap_info.bmiHeader.biBitCount = 32;											// biBitCount - Okre�la liczb� bit�w na piksel
		stan_render.bitmap_info.bmiHeader.biCompression = BI_RGB;									// biCompression = BI_RGB - oznacza nieskompresowany format RGB

	} break;

	default: {
		wynik = DefWindowProc(hwnd, uMsg, wParam, lParam); // Funkcja zapewnia domy�ln� obs�ug� komunikat�w, kt�rych program nie obs�uguje
	}
	}

	return wynik;
}

//'punkt wejscia' (int main()) dla graficznych aplikacji okienkowych systemu Windows :
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int  nShowCmd) {

	/*
	hInstance - Uchwyt do obecnej instancji programu.
	hPrevInstance - Uchwyt do poprzedniej instancji programu. Warto�� tego argumentu wynosi zawsze NULL.
	lpCmdLine - Argumenty podane w wierszu polecenia dla programu (bez nazwy programu).
	nCmdShow - Okre�la, w jaki spos�b okno zostanie wy�wietlone
	*/

	ShowCursor(FALSE);			// chowamy kursor


	//Stworzenie Klasy Okna:
	WNDCLASS window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW;		// rysowanie okna poziomo i pionowo
	window_class.lpszClassName = L"Game Window Class";	// nazwa klasy okna
	window_class.lpfnWndProc = window_callback;			//funkcja zwracaj�ca informacje o wszelkich akcjach na oknie 

	//Zarejestrowanie Klasy:
	RegisterClass(&window_class);	// Rejestruje klas� okna do p�niejszego u�ycia w wywo�aniach funkcji CreateWindow

	//Tworzenie Okna
	window = CreateWindow(window_class.lpszClassName, L"Gra Pong", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
	{
		/* Argumenty CreateWindow():
		- klasa okna
		- nazwa okna
		- style okreslajace wyglad okna (u nas nak�adajace si� i widoczne)
		- pocz�tkowa pozioma pozycja okna (u nas domy�lnie)
		- pocz�tkowa pionowa pozycja okna (u nas domy�lnie)
		- szeroko�� okna
		- wysoko�� okna
		- uchwyt okna nadrz�dengo (niepotrzebne)
		- uchwyt do menu dla zwyk�ych okien (niepotrzebne)
		- uchwyt do instalacji modu�u, powi�zanego z oknem (przekazujemy zmienn�, kt�r� otrzymali�my w WinMain
		- wska�nik na warto�� przekazywan� jako element lpCreateParams struktury CREATESTRUCT, w komunikacie WM_CREATE (niepotrzebne)
		*/

		//ustawiamy pelny ekran:
		SetWindowLong(window, GWL_STYLE, GetWindowLong(window, GWL_STYLE) & ~WS_OVERLAPPEDWINDOW);
		MONITORINFO mi = { sizeof(mi) };
		GetMonitorInfo(MonitorFromWindow(window, MONITOR_DEFAULTTOPRIMARY), &mi);		// pobieramy informacje o monitorze u�ytkownika
		SetWindowPos(window, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);		//ustawiamy okno na g�rze, jako now� szeroko�� i wysoko�� przyjmujemy dane monitora
	}


	HDC hdc = GetDC(window);	// pobiera uchwyt do kontekstu urz�dzenia dla obszaru klienta okre�lonego okna lub ca�ego ekranu

	Input input = {};			// struktura wej�cia

	float delta_time = 0.016666f;		//ustalamy ile czasu up�yne�o w jednej klatce  (60 fps)
	LARGE_INTEGER frame_begin_time;
	QueryPerformanceCounter(&frame_begin_time);		// Pobiera bie��c� warto�� licznika wydajno�ci, kt�ry jest sygnatur� czasow� o wysokiej rozdzielczo�ci, kt�ra mo�e by� u�ywana do pomiar�w odst�p�w czasu
													// Funkcja b�dzie sprawdza� ile czasu zaj�o procesorowi przetworzenie programu (po zako�czeniu p�tli while, odejmuje pocz�tkow� od ko�cowej warto�ci i przypisuje j� do delta_time
	float performance_frequency;					// funkcja sprawdzaj�ca ile cykli zosta�o wykonanych w jednej sekundzie
	{
		LARGE_INTEGER perf;
		QueryPerformanceFrequency(&perf);
		performance_frequency = (float)perf.QuadPart;
	}

	while (chodzi) {
		//wejscie
		MSG wiadomosc;		// struktura odbieraj�ca informacje o wiadomo�ciach

		for (int i = 0; i < BUTTON_COUNT; i++) {		// p�tla resetuj�ca zmian�, na pocz�tku ka�dej klatki
			input.klawisze[i].zmiana = false;			// gdy zaczyna si� nowa klatka, przechodzimy przez wszystkie przyciski i resetujemy zmian�
		}


		while (PeekMessage(&wiadomosc, window, 0, 0, PM_REMOVE)) {		// Przsy�a przychodz�ce wys�ane wiadomo�ci, sprawdza kolejk� wiadomo�ci w�tku pod k�tem opublikowanej wiadomo�ci i pobiera wiadomo�� (je�li istnieje)
																		// argument PM_REMOVE zapewinia to, �e funkcja przeczyta wiadomo�� tylko raz i j� usunie
			switch (wiadomosc.message) {
			case WM_KEYUP:					// odbiera wiadomo��, o tym, �e przcisk zosta� puszczony
			case WM_KEYDOWN: {				// odbiera wiadomo��, o tym, �e przcisk zosta� wci�ni�ty
				u32 vk_code = (u32)wiadomosc.wParam;		// wysy�a wiadomo�� o tym, kt�ry przycisk zosta� wci�ni�ty
				bool w_dole = ((wiadomosc.lParam & (1 << 31)) == 0);		// wysy�a wiadomo��, czy przycisk ten jest wci�ni�ty, czy puszczony

#define klawisz_w_procesie(b, vk)\
case vk: {\
input.klawisze[b].zmiana = w_dole != input.klawisze[b].w_dole;\
input.klawisze[b].w_dole = w_dole;\
} break;

				switch (vk_code) {							// kody kluczy wirtualnch przypisane odpowiednim przyciskom:
					klawisz_w_procesie(BUTTON_UP, VK_UP);
					klawisz_w_procesie(BUTTON_DOWN, VK_DOWN);
					klawisz_w_procesie(BUTTON_W, 'W');
					klawisz_w_procesie(BUTTON_S, 'S');
					klawisz_w_procesie(BUTTON_LEFT, VK_LEFT);
					klawisz_w_procesie(BUTTON_RIGHT, VK_RIGHT);
					klawisz_w_procesie(BUTTON_ENTER, VK_RETURN);
					klawisz_w_procesie(BUTTON_ESC, VK_ESCAPE);
				}
			}break;

			default: {
				TranslateMessage(&wiadomosc);		// T�umaczy komunikaty wirtualnego klucza na komunikaty znakowe. Komunikaty znakowe s� publikowane w kolejce komunikat�w w�tku wywo�uj�cego, aby zosta�y odczytane przy nast�pnym wywo�aniu funkcji PeekMessage
				DispatchMessage(&wiadomosc);		// Wysy�a komunikat do procedury okna (PeekMessage)
			}
			}

		}

		//symulacja
		symuluj_gre(&input, delta_time);

		//renderowanie
		// Funkcja StretchDIBits kopiuje dane koloru prostok�ta pikseli w obrazie DIB, JPEG lub PNG do okre�lonego prostok�ta docelowego oraz dostosowuje rozmiar prostok�tu �r�d�owego do prostok�tu docelowego
		StretchDIBits(hdc, 0, 0, stan_render.szerokosc, stan_render.wysokosc, 0, 0, stan_render.szerokosc, stan_render.wysokosc, stan_render.pamiec, &stan_render.bitmap_info, DIB_RGB_COLORS, SRCCOPY);

		LARGE_INTEGER frame_end_time;
		QueryPerformanceCounter(&frame_end_time);

		delta_time = (float)(frame_end_time.QuadPart - frame_begin_time.QuadPart) / performance_frequency;			// dzielimy przez pf, aby dosta� wynik w sekundach
		frame_begin_time = frame_end_time;			// przypisujemy koniec do pocz�tku, aby rozpocz�� now� klatk�
	}

}