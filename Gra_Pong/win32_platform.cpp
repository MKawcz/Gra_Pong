// PLIK PRZECHOWUJ¥CY KOD DLA WARSTWY PLATFORMY WINDOWS

#include "utils.cpp"
#include <windows.h>
 
global_variable bool chodzi = true;  // zmienna okreslajaca, czy gra chodzi (dziala)

struct stan_renderowania {
	int wysokosc, szerokosc;
	void* pamiec;

	BITMAPINFO bitmap_info;
};

global_variable stan_renderowania stan_render;

#include "wspolna_platforma.cpp"
#include "renderer.cpp"
#include "gra.cpp"

// specyfikacja funkcji zwracaj¹cej informacje o wszelkich akcjach na oknie, zgodna z dokumentacj¹:
LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	
	/*
	hwnd - Uchwyt do procedury okna obs³uguj¹cej komunikat.
	msg - Komunikat.
	wParam - Dodatkowe informacje specyficzne dla danego komunikatu. Wartoœæ tego argumentu zale¿y od wartoœci argumentu msg.
	lParam - Dodatkowe informacje specyficzne dla danego komunikatu. Wartoœæ tego argumentu zale¿y od wartoœci argumentu msg.
	*/
	
	LRESULT wynik = 0;
	switch (uMsg){
		case WM_CLOSE:
		case WM_DESTROY: {
			chodzi = false;		// jezeli okno zostanie zniszczone gra przestanie chodzic
		} break;
		
		case WM_SIZE: {
			RECT prostokat;
			GetClientRect(hwnd, &prostokat);
			stan_render.szerokosc = prostokat.right - prostokat.left;
			stan_render.wysokosc = prostokat.bottom - prostokat.top;
		
			int rozmiar = stan_render.szerokosc * stan_render.wysokosc * sizeof(unsigned int);
			
			if (stan_render.pamiec) VirtualFree(stan_render.pamiec, 0, MEM_RELEASE);
			stan_render.pamiec = VirtualAlloc(0, rozmiar, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		
			stan_render.bitmap_info.bmiHeader.biSize = sizeof(stan_render.bitmap_info.bmiHeader);
			stan_render.bitmap_info.bmiHeader.biWidth = stan_render.szerokosc;
			stan_render.bitmap_info.bmiHeader.biHeight = stan_render.wysokosc;
			stan_render.bitmap_info.bmiHeader.biPlanes = 1;
			stan_render.bitmap_info.bmiHeader.biBitCount = 32;
			stan_render.bitmap_info.bmiHeader.biCompression = BI_RGB;
			
		} break;

		default: {
			wynik = DefWindowProc(hwnd, uMsg, wParam, lParam); // Funkcja zapewnia domyœln¹ obs³ugê komunikatów, których program nie obs³uguje
		}
	}
	
	return wynik; 
} 

 //'punkt wejscia' (int main()) dla graficznych aplikacji okienkowych systemu Windows :
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int  nShowCmd) {     

	/*
	hInstance - Uchwyt do obecnej instancji programu.
	hPrevInstance - Uchwyt do poprzedniej instancji programu. Wartoœæ tego argumentu wynosi zawsze NULL.
	lpCmdLine - Argumenty podane w wierszu polecenia dla programu (bez nazwy programu).
	nCmdShow - Okreœla, w jaki sposób okno zostanie wyœwietlone
	*/

	ShowCursor(FALSE);


	//Stworzenie Klasy Okna
	WNDCLASS window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW;		// rysowanie okna poziomo i pionowo
	window_class.lpszClassName = L"Game Window Class";
	window_class.lpfnWndProc = window_callback;			//funkcja zwracaj¹ca informacje o wszelkich akcjach na oknie 

	//Zarejestrowanie Klasy
	RegisterClass(&window_class);	// Rejestruje klasê okna do póŸniejszego u¿ycia w wywo³aniach funkcji CreateWindow

	//Stworzenie Okna (uchwytu okna)
	HWND window = CreateWindow(window_class.lpszClassName, L"Gra Pong", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
	{
		/* Argumenty CreateWindow():
		- klasa okna
		- nazwa okna
		- style okreslajace wyglad okna (u nas nakladajace sie i widoczne)
		- poczatkowa pozioma pozycja okna (u nas domyslnie)
		- poczatkowa pionowa pozycja okna (u nas domyslnie)
		- szerokosc okna
		- wysokosc okna
		- uchwyt okna nadrzêdengo (niepotrzebne)
		- uchwyt do menu dla zwyklych okien (niepotrzebne)
		- uchwyt do instalacji modulu, powiazanego z oknem (przekazujemy zmienna, ktora otrzymalismy w WinMain
		- wskaŸnik na wartoœæ przekazywan¹ jako element lpCreateParams struktury CREATESTRUCT, w komunikacie WM_CREATE (niepotrzebne)
		*/

		//Pelny ekran
		SetWindowLong(window, GWL_STYLE, GetWindowLong(window, GWL_STYLE) & ~WS_OVERLAPPEDWINDOW);
		MONITORINFO mi = { sizeof(mi) };
		GetMonitorInfo(MonitorFromWindow(window, MONITOR_DEFAULTTOPRIMARY), &mi);
		SetWindowPos(window, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}
	
	
	HDC hdc = GetDC(window);

	Input input = {};

	float delta_time = 0.016666f;
	LARGE_INTEGER frame_begin_time;
	QueryPerformanceCounter(&frame_begin_time);

	float performance_frequency;
	{
		LARGE_INTEGER perf;
		QueryPerformanceFrequency(&perf);
		performance_frequency = (float)perf.QuadPart;
	}

	while (chodzi) {
		//wejscie
		MSG wiadomosc;		// struktura odbieraj¹ca informacje o wiadomosciach

		for (int i = 0; i < BUTTON_COUNT; i++) {
			input.klawisze[i].zmiana = false;
		}


		while (PeekMessage(&wiadomosc, window, 0, 0, PM_REMOVE)) {		// Przsy³a przychodz¹ce wys³ane wiadomoœci, sprawdza kolejkê wiadomoœci w¹tku pod k¹tem opublikowanej wiadomoœci i pobiera wiadomoœæ (jeœli istnieje)
																		// argument PM_REMOVE zapewinia to, ¿e funkcja przeczyta wiadomoœæ tylko raz i j¹ usunie
			switch (wiadomosc.message) {
				case WM_KEYUP:
				case WM_KEYDOWN: {
					u32 vk_code = (u32)wiadomosc.wParam;
					bool w_dole = ((wiadomosc.lParam & (1 << 31)) == 0);
				
#define klawisz_w_procesie(b, vk)\
case vk: {\
input.klawisze[b].zmiana = w_dole != input.klawisze[b].w_dole;\
input.klawisze[b].w_dole = w_dole;\
} break;

					switch (vk_code) {
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
					TranslateMessage(&wiadomosc);		// T³umaczy komunikaty wirtualnego klucza na komunikaty znakowe. Komunikaty znakowe s¹ publikowane w kolejce komunikatów w¹tku wywo³uj¹cego, aby zosta³y odczytane przy nastêpnym wywo³aniu funkcji PeekMessage
					DispatchMessage(&wiadomosc);		// Wysy³a komunikat do procedury okna (PeekMessage)
				}
			}

		}

		//symulacja
		symuluj_gre(&input, delta_time);

		//renderowanie
		StretchDIBits(hdc, 0, 0, stan_render.szerokosc, stan_render.wysokosc, 0, 0, stan_render.szerokosc, stan_render.wysokosc, stan_render.pamiec, &stan_render.bitmap_info, DIB_RGB_COLORS, SRCCOPY);
	
		LARGE_INTEGER frame_end_time;
		QueryPerformanceCounter(&frame_end_time);
		delta_time = (float)(frame_end_time.QuadPart - frame_begin_time.QuadPart) / performance_frequency;
		frame_begin_time = frame_end_time;
	}
	
}