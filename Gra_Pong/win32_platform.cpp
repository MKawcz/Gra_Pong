#include "utils.cpp"
#include <windows.h>
 
global_variable bool chodzi = true;

struct stan_renderowania {
	int wysokosc, szerokosc;
	void* pamiec;

	BITMAPINFO bitmap_info;
};

global_variable stan_renderowania stan_render;

#include "wspolna_platforma.cpp"
#include "renderer.cpp"
#include "gra.cpp"

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

	ShowCursor(FALSE);


	//Stworzenie Klasy Okna
	WNDCLASS window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW;		// rysowanie okna poziomo i pionowo
	window_class.lpszClassName = L"Game Window Class";
	window_class.lpfnWndProc = window_callback;			//funkcja zwracaj¹ca informacje o wszelkich akcjach na oknie 
	//Zarejestrowanie Klasy
	RegisterClass(&window_class);

	//Stworzenie Okna
	HWND window = CreateWindow(window_class.lpszClassName, L"Gra Pong", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
	{
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
		MSG wiadomosc;

		for (int i = 0; i < BUTTON_COUNT; i++) {
			input.klawisze[i].zmiana = false;
		}


		while (PeekMessage(&wiadomosc, window, 0, 0, PM_REMOVE)) {
			
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

					}
				}break;

				default: {
					TranslateMessage(&wiadomosc);
					DispatchMessage(&wiadomosc);
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