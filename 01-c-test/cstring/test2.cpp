#include <atlstr.h>       //CString

#define print(str)	printf( "%s\n", str )

int main( )
{
	CString strText1 = "ABCDE";
	CString strText2 = "abcde";
	CString strText3 = "a1b2c3d4e";

	strText1.MakeLower( );
	print( strText1 );		//abcde

	strText2.MakeUpper( );
	print( strText2 );		//ABCDE

	strText3.MakeUpper( );
	print( strText3 );		//A1B2C3D4E

	return 0;
}
