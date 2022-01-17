struct Stan_Klawisza {
	bool w_dole;
	bool zmiana;
};

// Enum to rodzaj danych, kt�ry zawiera ju� list� warto�ci, jak� mo�na nada� zmiennej w�asnego typu enum.

// lista mo�liwych przycisk�w do wci�ni�cia: 
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
	BUTTON_COUNT, //powinien by� ostatnim elementem
};

struct Input {
	Stan_Klawisza klawisze[BUTTON_COUNT];
};