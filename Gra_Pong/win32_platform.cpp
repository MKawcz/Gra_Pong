

#include <windows.h>

bool chodzi = true;

struct stan_renderowania {
	int wysokosc, szerokosc;
	void* pamiec;

	BITMAPINFO bitmap_info;
};

stan_renderowania stan_render;

#include "renderer.cpp"

LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	LRESULT wynik = 0;
	switch (uMsg){
		case WM_CLOSE:
		case WM_DESTROY: {
			chodzi = false;
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
			wynik = DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
	
	return wynik;
} // specyfikacja funkcji zwrotnej zgodna z dokumentacj¹



int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int  nShowCmd) {   //'punkt wejscia' dla graficznych aplikacji okienkowych systemu Windows
	//Stworzenie Klasy Okna
	WNDCLASS window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW;		// rysowanie okna poziomo i pionowo
	window_class.lpszClassName = L"Game Window Class";
	window_class.lpfnWndProc = window_callback;			//funkcja zwracaj¹ca informacje o wszelkich akcjach na oknie 
	//Zarejestrowanie Klasy
	RegisterClass(&window_class);

	//Stworzenie Okna
	HWND window = CreateWindow(window_class.lpszClassName, L"Gra Pong", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
	HDC hdc = GetDC(window);

	while (chodzi) {
		//wejscie
		MSG wiadomosc;
		while (PeekMessage(&wiadomosc, window, 0, 0, PM_REMOVE)) {
			TranslateMessage(&wiadomosc);
			DispatchMessage(&wiadomosc);
		}

		//symulacja
		czysc_ekran(0x0066ff);
		rysuj_prostokat(50, 50, 200, 500, 0xff0000);

		//renderowanie
		StretchDIBits(hdc, 0, 0, stan_render.szerokosc, stan_render.wysokosc, 0, 0, stan_render.szerokosc, stan_render.wysokosc, stan_render.pamiec, &stan_render.bitmap_info, DIB_RGB_COLORS, SRCCOPY);
	}
	
}