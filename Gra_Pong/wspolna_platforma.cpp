struct Stan_Klawisza {
	bool w_dole;
	bool zmiana;
};

// Enum to rodzaj danych, który zawiera ju¿ listê wartoœci, jak¹ mo¿na nadaæ zmiennej w³asnego typu enum.

// lista mo¿liwych przycisków do wciœniêcia: 
enum {
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_W,
	BUTTON_S,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_ENTER,
	BUTTON_ESC,

	//licznik przyciskow:
	BUTTON_COUNT, //powinien byæ ostatnim elementem
};

struct Input {
	Stan_Klawisza klawisze[BUTTON_COUNT];
};