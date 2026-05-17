typedef bool			bit;

typedef struct {
	bit k0246[16][24], k1357[16][24];
	bit iv0[32], iv1[32];
} DESContext;

static bool subkey[16][48];//16»¶◊”√‹‘ø

struct DesKo
{	
	bit		KeyOPC1_Before[2][32];
	bit		KeyOPC1_Later[2][28];
	bit		KeyOPC2_Before[17][2][28];
	bit		KeyOPC2_Later[17][2][24];	
	int		KeyOSiftLeft[16];
};

struct DesEo
{
	bit		ItLR[17][2][32];
};

struct DesFo
{
	bit FoR[32];
	bit FoKey[2][24];
	bit FoSPBefore[2][24];
	bit FoSPLater[32];
};

struct DesEno
{
	bit IPBeforeLR[2][32];
	bit IPLaterLR[2][32];
	bit FPBeforeLR[2][32];
	bit FPLaterLR[2][32];
	DesEo eo;
	DesFo fo[17];
	DesKo keyInfo;
};

void f_func( bit in[32], const bit ki[48], DesFo* pfo = NULL );
void s_func( bit out[32], const bit in[48] );
void transform( bit * out, bit * in, const char * table, int len );
void xor( bit * ina, const bit * inb, int len );
void rotatel( bit * in, int len, int loop );
void bytetobit( bit * out, const char * in, int bits );
void bytetobit1( bool * out, const char * in, int bits );
void bittobyte( char * out, const bit * in, int bits );

void DES_encrypt( bit * out, bit * in, const bit * key );
void DES_decrypt( bit * out, bit * in, const bit * key );