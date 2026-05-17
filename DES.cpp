#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include "DES.h"

//置换IP表

const static char ip_table[64] = {
	58, 50, 42, 34, 26, 18, 10, 2, 
	60, 52, 44, 36, 28, 20, 12, 4, 
	62, 54, 46, 38, 30, 22, 14, 6, 
	64, 56, 48, 40, 32, 24, 16, 8, 
	57, 49, 41, 33, 25, 17, 9, 1, 
	59, 51, 43, 35, 27, 19, 11, 3, 
	61, 53, 45, 37, 29, 21, 13, 5, 
	63, 55, 47, 39, 31, 23, 15, 7
};

//逆置换IP-1表
const static char ipr_table[64] = {
	40, 8, 48, 16, 56, 24, 64, 32, 
	39, 7, 47, 15, 55, 23, 63, 31, 
	38, 6, 46, 14, 54, 22, 62, 30, 
	37, 5, 45, 13, 53, 21, 61, 29, 
	36, 4, 44, 12, 52, 20, 60, 28, 
	35, 3, 43, 11,  51, 19, 59, 27, 
	34, 2, 42, 10, 50, 18, 58, 26, 
	33, 1, 41, 9, 49, 17, 57, 25
};

//E 位选择表
static const char e_table[] = {
	32, 1,  2,  3,  4,  5,
	4,  5,  6,  7,  8,  9,
	8,  9,  10, 11, 12, 13,
	12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21,
	20, 21, 22, 23, 24, 25,
	24, 25, 26, 27, 28, 29,
	28, 29, 30, 31, 32, 01
};

//P换位表
static const char p_table[] = {
	16, 7,  20, 21, 29, 12, 28, 17,
	1,  15, 23, 26, 5,  18, 31, 10,
	2,  8,  24, 14, 32, 27, 3,  9,
	19, 13, 30,  6, 22, 11, 4,  25
};

//pc1选位表
const static char pc1_table[56] = {
	57,49,41,33,25,17,9,1,
	58,50,42,34,26,18,10,2,
	59,51,43,35,27,19,11,3,
	60,52,44,36,63,55,47,39,
	31,23,15,7,62,54,46,38,
	30,22,14,6,61,53,45,37,
	29,21,13,5,28,20,12,4
};

//pc2选位表
const static char pc2_table[48] = {
	14,17,11,24,1,5,3,28,
	15,6,21,10,23,19,12,4,
	26,8,16,7,27,20,13,2,
	41,52,31,37,47,55,30,40,
	51,45,33,48,44,49,39,56,
	34,53,46,42,50,36,29,32
};

static const int leftshifts[] ={ 
	1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 
};

const static char s_box[8][4][16]={
	//s1
	14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
	0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
	4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
	15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13,
	//s2
	15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
	3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
	0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
	13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9,
	//s3
	10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
	13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
	13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
	1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12,
	//s4
	7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
	13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
	10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
	3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14,
	//s5
	2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
	14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
	4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
	11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3,
	//s6
	12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
	10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
	9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
	4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13,
	//s7
	4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
	13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
	1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
	6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12,
	//s8
	13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
	1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
	7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
	2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11
};

void f_func( bit in[32], const bit ki[48], DesFo* pfo )
{
	DesFo fo;
	ZeroMemory(&fo,sizeof(DesFo));
	memcpy( fo.FoR, in, 32 );	
	memcpy( fo.FoKey[0], &ki[0], 24 );
	memcpy( fo.FoKey[1], &ki[24], 24 );

	static bit mr[48];
	transform( mr, in, e_table, 48 );
	xor( mr, ki, 48 );
	memcpy( fo.FoSPBefore, mr, 48 );	
	s_func( in, mr );
	transform( in, in, p_table, 32 );
	memcpy( fo.FoSPLater, in, 32 );
	if( pfo != NULL )
	{
		(*pfo) = fo;
	}	
}

void s_func( bit out[32], const bit in[48] )
{
	for(char i = 0, j, k; i < 8; i++, in += 6, out += 4 )
	{
		j = (in[0]<<1) + in[5];
		k = (in[1]<<3) + (in[2]<<2) + (in[3]<<1) + in[4];
		//bytetobit( out, &s_box[i][j][k], 4 );
		char stmp = s_box[i][j][k];
		for( int i = 0; i < 4; i++ )
		{
			out[i] = ( stmp >> ( 3 - i % 4 ) ) & 1;
		}
	}
}

void transform( bit * out, bit * in, const char * table, int len )
{
	static bit tmp[256];
	for( int i = 0; i < len; i++ )
	{
		tmp[i] = in[ table[i] - 1 ];
	}
	memcpy( out, tmp, len );
}

void xor( bit * ina, const bit * inb, int len )
{
	for(int i=0;i<len;i++)
	{
		ina[i] ^= inb[i];
	}
}

void rotatel( bit *in, int len, int loop )
{
	static bit tmp[256];
	memcpy( tmp, in, loop );
	memcpy( in, in + loop, len - loop );
	memcpy( in + len - loop, tmp, loop );
}

void bytetobit( bit * out, const char * in, int bits )
{
	memset( out, 0, bits );
	for( int i = 0; i < bits; i++ )
	{
		if ( (unsigned)( i / 8 ) < (unsigned)strlen( in ) )
		{
			out[i] = ( in[i/8] >> ( 7 - i % 8 ) ) & 1;
		}		
	}
}

void bittobyte( char * out, const bit * in, int bits )
{
	memset( out, 0, (bits+7)/8 );
	for( int i = 0; i < bits; i++ )
	{
		out[i/8] |= in[i] << ( 7 - i % 8 );
	}
}

void bytetobit1( bool * out, const char * in, int bits )
{
	char m[16];
	memcpy( m, in, 16 * sizeof( char ) );		
	for( int i = 0; i < bits; i++ )
	{
		int pos = i / 4;
		if ( m[pos] >= '0' && m[pos] <= '9' )
		{
			m[pos] -= '0';
		}
		if ( m[pos] >= 'A' && m[pos] <= 'F' )
		{
			m[pos] = m[pos] - 'A' + 10;
		}
		out[i] = ( m[ i / 4 ] >> ( 3 - i % 4 ) ) & 1;
	}
}

/******************************************************************************/
//	名称：des_encipher
//	功能：DES加密函数
//  参数：out:密文; in:明文;
//	返回：无
/******************************************************************************/
static void des_encipher( bit * out, bit * in, DesEno * pEno = NULL )
{
	static bool m[64],tmp[32],*li=&m[0], *ri=&m[32];
	int i = 0;
	bit IPBeforeLR[2][32];
	bit IPLaterLR[2][32];
	bit FpBeforeLR[2][32];
	bit FPLaterLR[2][32];
	memcpy( m, in, 64 );	
	memcpy( IPBeforeLR[0], li, 32 );
	memcpy( IPBeforeLR[1], ri, 32 );
	transform( m, m, ip_table, 64 );
	memcpy( IPLaterLR[0], li, 32 );
	memcpy( IPLaterLR[1], ri, 32 );
	DesFo fo[17];
	DesEo eo;
	ZeroMemory( fo, sizeof( DesFo ) * 17 );

	for( i = 0; i < 16; ++i )
	{
		memcpy( eo.ItLR[i][0], li, 32 );
		memcpy( eo.ItLR[i][1], ri, 32 );
		memcpy( tmp, ri, 32 );
		f_func( ri, subkey[i], &fo[i] );
		xor( ri, li, 32 );
		memcpy( li, tmp, 32 );		
	}
	memcpy( tmp, ri, 32 );
	memcpy( ri, li, 32 );
	memcpy( li, tmp, 32 );
	memcpy( eo.ItLR[i], m, 64 );
	memcpy( FpBeforeLR, m, 64 );
	transform( m, m, ipr_table, 64 );
	memcpy( FPLaterLR, m, 64 );

	if( pEno != NULL )
	{
		memcpy( pEno->FPBeforeLR, FpBeforeLR, 64 );
		memcpy( pEno->FPLaterLR, FPLaterLR, 64 );
		memcpy( pEno->IPBeforeLR, IPBeforeLR, 64 );
		memcpy( pEno->IPLaterLR, IPLaterLR, 64 );
		memcpy( pEno->fo, fo, sizeof(DesFo) * 16 );
		pEno->eo = eo;
	}
	memcpy( out, m, 64 );
}

/******************************************************************************/
//	名称：des_decipher
//	功能：DES解密函数
//  参数：out:明文; in:密文;
//	返回：无
/******************************************************************************/
static void des_decipher( bit * out, bit * in )
{
	static bit m[64], tmp[32], *li=&m[0], *ri=&m[32];
	int i = 0;
	memcpy( m, in, 64 );	
	transform( m, m, ip_table, 64 );
	memcpy( tmp, ri, 32 );
	memcpy( ri, li, 32 );
	memcpy( li, tmp, 32 );	
	for( i = 15; i >= 0; i-- )
	{		
		memcpy( tmp, li, 32 );
		f_func( li, subkey[i] );
		xor( li, ri, 32 );
		memcpy( ri, tmp, 32 );
	}	
	transform( m, m, ipr_table, 64 );
	memcpy( out, m, 64 );
}


/******************************************************************************/
//	名称：des_key_setup
//	功能：产生子密钥；子密钥16轮循环
//	返回：无
/******************************************************************************/
void des_key_setup( const bit * key, DesKo * pK = NULL )
{
	bit KeyOPC1_Before[2][32];
	bit KeyOPC1_Later[2][28];
	bit KeyOPC2_Before[16][2][28];
	bit KeyOPC2_Later[16][2][24];	
	static bool k[64], *kl=&k[0], *kr=&k[28];

	memcpy( k, key, 64 );
	memcpy( KeyOPC1_Before, k, 64 );	

	transform( k, k, pc1_table, 56 );

	memcpy( KeyOPC1_Later[0], kl, 28 );
	memcpy( KeyOPC1_Later[1], kr, 28 );

	for(int i = 0; i < 16; i++ )
	{
		rotatel( kl, 28, leftshifts[i] );
		rotatel( kr, 28, leftshifts[i] );
		memcpy( KeyOPC2_Before[i][0], kl, 28 );
		memcpy( KeyOPC2_Before[i][1], kr, 28 );
		transform( subkey[i], k, pc2_table, 48 );
		memcpy( KeyOPC2_Later[i], subkey[i], 48 );
	}
	if( pK != NULL )
	{
		memcpy( pK->KeyOPC1_Before, KeyOPC1_Before, 64 );
		memcpy( pK->KeyOPC1_Later, KeyOPC1_Later, 64 );
		memcpy( pK->KeyOSiftLeft, leftshifts, 16 * sizeof( int ) );
		for(int i=0; i < 16; ++i)
		{
			memcpy( pK->KeyOPC2_Before[i], KeyOPC2_Before[i], 56 );
			memcpy( pK->KeyOPC2_Later[i], KeyOPC2_Later[i], 48 );			
		}
	}
}

/******************************************************************************/
//	名称：des_encrypt
//	功能：DES加密函数
//  参数：out:密文; in:明文; key:64位密文
//	返回：无
/******************************************************************************/
void DES_encrypt( bit * out, bit * in, const bit * key )
{
	des_key_setup( key );
	des_encipher( out, in );
}

/******************************************************************************/
//	名称：des_decrypt
//	功能：DES解密函数
//  参数：out:明文; in:密文; key:64位密文
//	返回：无
/******************************************************************************/
void DES_decrypt( bit * out, bit * in, const bit * key )
{
	des_key_setup( key );
	des_decipher( out, in );
}