//ZMIENNE DO U�YTKU

typedef char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16; 
typedef int s32;
typedef unsigned int u32; 
typedef long long s64;
typedef unsigned long long u64; 

#define global_variable static		// static sprawia, �e zmienna klasowa jest wsp�lna dla wszystkich obiekt�w danej klasy
#define internal static				// zdefiniowanie zmiennych globalnych i wewn�trznych
									// dzi�ki wstawieniu "global_variable" lub "internal" przed deklaracj� zmiennej, �atwiej b�dzie rozr�ni�, kt�re s� by� globalne, a kt�re wewn�trzne
inline int
zacisk(int min, int val, int max) {
	if (val < min) return min;
	if (val > max) return max;
	return val;
}