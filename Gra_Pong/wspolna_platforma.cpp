struct Stan_Klawisza {
	bool w_dole;
	bool zmiana;
};

enum {
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_W,
	BUTTON_S,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_ENTER,
	BUTTON_ESC,

	BUTTON_COUNT, //powinien byæ ostatnim elementem
};

struct Input {
	Stan_Klawisza klawisze[BUTTON_COUNT];
};