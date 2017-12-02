/////////////////////////////////////////////////////////////////////////////////////
// Заголовочный файл
#include "HSDWLib.h"

//////////////////////////////////////////////////////////////////////////

__declspec (dllexport) char *REConvert (char *String)
	{
	// Переменные
	unsigned int i;
	char RESm [256];
	static char StringOut [DEFSZ];

	// ---------------------------------------------------------------- 
	// ЗАПОЛНЕНИЕ МАССИВА ДЛЯ ПРЕОБРАЗОВАНИЯ
	for (i = 0x00; i <= 0x1F; i++)
		{
		RESm [i] = 0x7F;							// Ненужные символы заменяются символом пустоты
		}
	for (i = 0x20; i <= 0xFF; i++)
		{
		RESm [i] = i;								// Общее заполнение
		}
	RESm [0x09] = 0x09;
	RESm [0x0A] = 0x0A;
	RESm [0x0D] = 0x0D;
	RESm [0xAD] = 0x7F;
	
	RESm ['А'] = 'F';
	RESm ['В'] = 'D';
	RESm ['Г'] = 'U';
	RESm ['Д'] = 'L';
	RESm ['Е'] = 'T';
	RESm ['З'] = 'P';
	RESm ['И'] = 'B';
	RESm ['Й'] = 'Q';
	RESm ['К'] = 'R';
	RESm ['Л'] = 'K';
	RESm ['М'] = 'V';
	RESm ['Н'] = 'Y';
	RESm ['О'] = 'J';
	RESm ['П'] = 'G';
	RESm ['Р'] = 'H';
	RESm ['С'] = 'C';
	RESm ['Т'] = 'N';
	RESm ['У'] = 'E';
	RESm ['Ф'] = 'A';
	RESm ['Ц'] = 'W';
	RESm ['Ч'] = 'X';
	RESm ['Ш'] = 'I';
	RESm ['Щ'] = 'O';
	RESm ['Ы'] = 'S';
	RESm ['Ь'] = 'M';
	RESm ['Я'] = 'Z';

	for (i = 'а'; i <= 'я'; i++)
		{
		RESm[i] = RESm[i - 0x20] + 0x20;
		}

	RESm ['Б'] = '<';
	RESm ['Ж'] = ':';
	RESm ['Х'] = '{';
	RESm ['Ъ'] = '}';
	RESm ['Э'] = '"';
	RESm ['Ю'] = '>';
	RESm ['Ё'] = '~';

	RESm ['б'] = ',';
	RESm ['ж'] = ';';
	RESm ['х'] = '[';
	RESm ['ъ'] = ']';
	RESm ['э'] = '\'';
	RESm ['ю'] = '.';
	RESm ['ё'] = '`';

	RESm ['"'] = '@';
	RESm ['№'] = '#';
	RESm [';'] = '$';
	RESm [':'] = '^';
	RESm ['?'] = '&';
	RESm ['/'] = '|';
	RESm [','] = '?';
	RESm ['.'] = '/';

	// ---------------------------------------------------------------- 
	// ПРЕОБРАЗОВАНИЕ

	for (i = 0; i < strlen (String); i++)
		{
		StringOut[i] = RESm[String[i]];
		}
	StringOut[i] = '\0';

	return StringOut;
	}

//////////////////////////////////////////////////////////////////////////

__declspec (dllexport) char *ERConvert (char *String)
	{
	// Объявление
	unsigned int i;
	char RESm[256];
	static char StringOut[DEFSZ];

	// ---------------------------------------------------------------- 
	// ЗАПОЛНЕНИЕ МАССИВА ДЛЯ ПРЕОБРАЗОВАНИЯ
	for (i = 0x00; i <= 0x1F; i++)
		{
		RESm [i] = 0x7F;							// Ненужные символы заменяются символом пустоты
		}
	for (i = 0x20; i <= 0xFF; i++)
		{
		RESm [i] = i;								// Общее заполнение
		}
	RESm [0x09] = 0x09;
	RESm [0x0A] = 0x0A;
	RESm [0x0D] = 0x0D;
	RESm [0xAD] = 0x7F;
	
	RESm ['A'] = 'Ф';
	RESm ['B'] = 'И';
	RESm ['C'] = 'С';
	RESm ['D'] = 'В';
	RESm ['E'] = 'У';
	RESm ['F'] = 'А';
	RESm ['G'] = 'П';
	RESm ['H'] = 'Р';
	RESm ['I'] = 'Ш';
	RESm ['J'] = 'О';
	RESm ['K'] = 'Л';
	RESm ['L'] = 'Д';
	RESm ['M'] = 'Ь';
	RESm ['N'] = 'Т';
	RESm ['O'] = 'Щ';
	RESm ['P'] = 'З';
	RESm ['Q'] = 'Й';
	RESm ['R'] = 'К';
	RESm ['S'] = 'Ы';
	RESm ['T'] = 'Е';
	RESm ['U'] = 'Г';
	RESm ['V'] = 'М';
	RESm ['W'] = 'Ц';
	RESm ['X'] = 'Ч';
	RESm ['W'] = 'Ц';
	RESm ['X'] = 'Ч';
	RESm ['Y'] = 'Н';
	RESm ['Z'] = 'Я';

	for (i = 'a'; i <= 'z'; i++)
		{
		RESm[i] = RESm[i - 0x20] + 0x20;
		}

	RESm ['<'] = 'Б';
	RESm [':'] = 'Ж';
	RESm ['{'] = 'Х';
	RESm ['}'] = 'Ъ';
	RESm ['"'] = 'Э';
	RESm ['>'] = 'Ю';
	RESm ['~'] = 'Ё';

	RESm [','] = 'б';
	RESm [';'] = 'ж';
	RESm ['['] = 'х';
	RESm [']'] = 'ъ';
	RESm ['\''] = 'э';
	RESm ['.'] = 'ю';
	RESm ['`'] = 'ё';

	RESm ['@'] = '"';
	RESm ['#'] = '№';
	RESm ['$'] = ';';
	RESm ['^'] = ':';
	RESm ['&'] = '?';
	RESm ['|'] = '/';
	RESm ['?'] = ',';
	RESm ['/'] = '.';

	// ---------------------------------------------------------------- 
	// ПРЕОБРАЗОВАНИЕ

	for (i = 0; i < strlen (String); i++)
		{
		StringOut[i] = RESm[String[i]];
		}
	StringOut[i] = '\0';

	return StringOut;
	}

//////////////////////////////////////////////////////////////////////////
