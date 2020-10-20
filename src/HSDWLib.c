/////////////////////////////////////////////////////////////////////////////////////
// Заголовочный файл
#include "HSDWLib.h"

// Макросы
#define DEFVARS		FILE *FI, *FO;\
					schar FNamOut[DEFSZ];
#define CLOSEFILES	fclose (FI); fclose (FO);

//////////////////////////////////////////////////////////////////////////
LC_API(sint) SHConvert (schar *FNam)
	{
	// Объявление
	DEFVARS
	sint i = 0,		// Переполнение допустимо
		Symb = 0;

	// Открытие
	if ((FI = fopen (FNam, "rb")) == NULL)
		return -1;

	FileNameOut (1, FNam, FNamOut);		// Создание имени выходного файла

	if ((FO = fopen (FNamOut, "wb")) == NULL)
		{
		fclose (FI);
		return -2;
		}

	// ---------------------------------------------------------------- 
	// КОНВЕРСИЯ
	while ((Symb = fgetc (FI)) != EOF)
		{
		fprintf (FO, "|%02X", Symb);

		if ((++i % 32) == 0)
			{
			fprintf (FO, "%c", 0x0A);
			i = 0;
			}
		}
	// ---------------------------------------------------------------- 

	CLOSEFILES
	return 0;
	}

//////////////////////////////////////////////////////////////////////////

LC_API(sint) HSConvert (schar *FNam)
	{
	// Объявление
	DEFVARS
	sint Symb = 0;

	// Открытие
	if ((FI = fopen (FNam, "rb")) == NULL)
		return -1;

	FileNameOut (2, FNam, FNamOut);

	if ((FO = fopen (FNamOut, "wb")) == NULL)
		{
		fclose (FI);
		return -2;
		}

	// ---------------------------------------------------------------- 
	// КОНВЕРСИЯ
	while (Symb != EOF)
		if ((Symb = fgetc (FI)) == '|')
			{
			fscanf (FI, "%X", &Symb);
			fprintf (FO, "%c", Symb);
			}
	// ---------------------------------------------------------------- 

	CLOSEFILES
	return 0;
	}

//////////////////////////////////////////////////////////////////////////

LC_API(sint) DWConvert (schar *FNam)
	{
	// Объявление
	DEFVARS
	uchar DWCnMs [256];
	sint i = 0, Symb = 0, msg = 0;

	// Открытие
	if ((FI = fopen (FNam, "rb")) == NULL)
		return -1;

	FileNameOut (3, FNam, FNamOut);

	if ((FO = fopen (FNamOut, "wb")) == NULL)
		{
		fclose (FI);
		return -2;
		}

	// ---------------------------------------------------------------- 
	// ЗАПОЛНЕНИЕ МАССИВА ДЛЯ ПРЕОБРАЗОВАНИЯ
	for (i = 0x00; i <= 0xFF; i++)
		DWCnMs [i] = 0x7F;	// Ненужные символы заменяются символом пустоты

	DWCnMs [0x09] = 0x09;
	DWCnMs [0x0A] = 0x0A;
	DWCnMs [0x0D] = 0x0D;							// Символы \t, \r и \n
	DWCnMs [0x14] = 0xB6;							// Знак абзаца
	DWCnMs [0x15] = 0xA7;							// Знак параграфа
	for (i = 0x20; i < 0x7F; i++)
		DWCnMs [i] = i;								// Базовые символы латиницы
	for (i = 0x80; i < 0xB0; i++)
		DWCnMs [i] = i + 0x40;						// Первая часть русского алфавита
	for (i = 0xE0; i < 0xF0; i++)
		DWCnMs [i] = i + 0x10;						// Вторая часть русского алфавита
	DWCnMs [0xF0] = 0xA8;							// Ё
	DWCnMs [0xF1] = 0xB8;							// ё
	DWCnMs [0xF8] = 0xB0;							// Знак градуса
	DWCnMs [0xF9] = 0x95;							// Большой маркер
	DWCnMs [0xFA] = 0xB7;							// Малый маркер
	DWCnMs [0xFC] = 0xB9;							// №

	// ---------------------------------------------------------------- 
	// ПРЕОБРАЗОВАНИЕ
	while (Symb != EOF)
		{
		Symb = fgetc (FI);
		
		if ((DWCnMs[Symb] == 0x7F) && (msg == 0))
			msg = 1;

		if (Symb != EOF)
			fprintf (FO, "%c", DWCnMs [Symb]);
		}
	// ---------------------------------------------------------------- 
	
	CLOSEFILES
	return msg;
	}

//////////////////////////////////////////////////////////////////////////

LC_API(sint) WDConvert (schar *FNam)
	{
	// Объявление
	DEFVARS
	schar *WDCnSMs[] = {"...", " Euro", " promille", " (tm) ",
					 " (c) ", " (r) ", " + - ", "micro"};		// Строки для замены
	uchar WDCnMs[256];
	sint i = 0, Symb = 0, msg = 0;

	// Открытие
	if ((FI = fopen (FNam, "rb")) == NULL)
		return -1;

	FileNameOut (4, FNam, FNamOut);

	if ((FO = fopen (FNamOut, "wb")) == NULL)
		{
		fclose (FI);
		return -2;
		}

	// ---------------------------------------------------------------- 
	// ЗАПОЛНЕНИЕ МАССИВА ДЛЯ ПРЕОБРАЗОВАНИЯ
	for (i = 0x00; i <= 0xFF; i++)
		WDCnMs [i] = 0xFE;							// Ненужные символы заменяются символом пустоты

	WDCnMs [0x09] = 0x09;
	WDCnMs [0x0A] = 0x0A;
	WDCnMs [0x0D] = 0x0D;							// Символы \t, \r и \n
	for (i = 0x20; i < 0x7F; i++)
		WDCnMs [i] = i;								// Базовые символы латиницы
	WDCnMs [0x84] = 0x22;							// Кавычки (нижние)
		WDCnMs [0x85] = 0x00;						// Троеточие (далее - в массив строк)
		WDCnMs [0x88] = 0x01;						// Евро
		WDCnMs [0x89] = 0x02;						// Промилле
	WDCnMs [0x93] = 0x22;							// Кавычки (верхние левые)
	WDCnMs [0x94] = 0x22;							// Кавычки (верхние правые)
	WDCnMs [0x95] = 0xF9;							// Большой маркер
	WDCnMs [0x96] = 0x2D;							// Малое тире
	WDCnMs [0x97] = 0x2D;							// Большое тире
		WDCnMs [0x99] = 0x03;						// Trademark
	WDCnMs [0xA0] = 0x20;							// Неразрывный пробел
	WDCnMs [0xA7] = 0x15;							// Знак параграфа
	WDCnMs [0xA8] = 0xF0;							// Ё
		WDCnMs [0xA9] = 0x04;						// Copyright
	WDCnMs [0xAB] = 0x22;							// Кавычки (скобочные левые)
	WDCnMs [0xAD] = 0x2D;							// Знак переноса
		WDCnMs [0xAE] = 0x05;						// Reserved
	WDCnMs [0xB0] = 0xF8;							// Знак градуса
		WDCnMs [0xB1] = 0x06;						// Плюс - минус
		WDCnMs [0xB5] = 0x07;						// Знак микро
	WDCnMs [0xB6] = 0x14;							// Знак абзаца
	WDCnMs [0xB7] = 0xFA;							// Малый маркер
	WDCnMs [0xB8] = 0xF1;							// ё
	WDCnMs [0xB9] = 0xFC;							// №
	WDCnMs [0xBB] = 0x22;							// Кавычки (скобочные правые)
	for (i = 0xC0; i <= 0xF0; i++)
		WDCnMs [i] = i - 0x40;						// Первая часть русского алфавита
	for (i = 0xF0; i <= 0xFF; i++)
		WDCnMs [i] = i - 0x10;						// Вторая часть русского алфавита

	// ---------------------------------------------------------------- 
	// ПРЕОБРАЗОВАНИЕ
	while (Symb != EOF)
		{
		Symb = fgetc (FI);

		if ((WDCnMs[Symb] == 0xFE) && (msg == 0))
			msg = 1;

		if (Symb != EOF)
			if (WDCnMs[Symb] < 0x08)
				fprintf (FO, "%s", WDCnSMs [WDCnMs [Symb]]);
			else
				fprintf (FO, "%c", WDCnMs [Symb]);
		}
	// ---------------------------------------------------------------- 
	
	CLOSEFILES
	return msg;
	}

//////////////////////////////////////////////////////////////////////////

LC_API(char*) REConvert (char *String)
	{
	// Переменные
	uint i;
	char RESm [256];
	static char StringOut [DEFSZ];

	// ---------------------------------------------------------------- 
	// ЗАПОЛНЕНИЕ МАССИВА ДЛЯ ПРЕОБРАЗОВАНИЯ
	for (i = 0x00; i <= 0x1F; i++)
		RESm [i] = 0x7F;							// Ненужные символы заменяются символом пустоты

	for (i = 0x20; i <= 0xFF; i++)
		RESm [i] = i;								// Общее заполнение

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
		RESm[i] = RESm[i - 0x20] + 0x20;

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
		StringOut[i] = RESm[String[i]];
	StringOut[i] = '\0';

	return StringOut;
	}

//////////////////////////////////////////////////////////////////////////

LC_API(char*) ERConvert (char *String)
	{
	// Объявление
	uint i;
	char RESm[256];
	static char StringOut[DEFSZ];

	// ---------------------------------------------------------------- 
	// ЗАПОЛНЕНИЕ МАССИВА ДЛЯ ПРЕОБРАЗОВАНИЯ
	for (i = 0x00; i <= 0x1F; i++)
		RESm [i] = 0x7F;							// Ненужные символы заменяются символом пустоты

	for (i = 0x20; i <= 0xFF; i++)
		RESm [i] = i;								// Общее заполнение

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
		RESm[i] = RESm[i - 0x20] + 0x20;

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
		StringOut[i] = RESm[String[i]];
	StringOut[i] = '\0';

	return StringOut;
	}

//////////////////////////////////////////////////////////////////////////

LC_API(void) FileNameOut (uchar Mode, schar *FNam, schar *FNamO)
	{
	uint i, s, j;

	_strrev (FNam);						// Разворот строки для поиска с конца
	i = (sint) strcspn (FNam, ".") + 1;	// Поиск точки - отделителя расширения
	s = (sint) strcspn (FNam, "\\");
	_strrev (FNam);

	if ((i - 1) > s)					// Файлы без расширения (с учётом возможного наличия точек в именах директорий)
		i = 0;

	for (j = 0; j < strlen (FNam) - i; j ++)
		FNamO[j] = FNam[j];
	FNamO[j] = '\0';					// Часть имени без расширения

	switch (Mode)
		{
		case 1:
			{strcat (FNamO, SH_EXT); break;}
		case 2:
			{strcat (FNamO, HS_EXT); break;}
		case 3:
			{strcat (FNamO, DW_EXT); break;}
		case 4:
			{strcat (FNamO, WD_EXT); break;}
		case 5:
			{strcat (FNamO, RE_EXT); break;}
		case 6:
			{strcat (FNamO, ER_EXT); break;}
		}
	}
