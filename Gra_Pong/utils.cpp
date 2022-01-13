//ZMIENNE DO U¯YTKU

typedef char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16; 
typedef int s32;
typedef unsigned int u32; 
typedef long long s64;
typedef unsigned long long u64; 

#define global_variable static		// static sprawia, ¿e zmienna klasowa jest wspólna dla wszystkich obiektów danej klasy
#define internal static				// zdefiniowanie zmiennych globalnych i wewnêtrznych
									// dziêki wstawieniu "global_variable" lub "internal" przed deklaracj¹ zmiennej, ³atwiej bêdzie rozró¿niæ, które s¹ byæ globalne, a które wewnêtrzne
inline int
zacisk(int min, int val, int max) {
	if (val < min) return min;
	if (val > max) return max;
	return val;
}