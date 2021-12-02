struct Stan_Klawisza {
	bool w_dole;
	bool zmiana;
};

enum {
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_LEFT,
	BUTTON_RIGHT,

	BUTTON_COUNT, //powinien byæ ostatnim elementem
};

struct Input {
	Stan_Klawisza klawisze[BUTTON_COUNT];
};