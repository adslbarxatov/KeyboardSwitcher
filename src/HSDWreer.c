/////////////////////////////////////////////////////////////////////////////////////
// Заголовочный файл
#include "HSDWLib.h"

/////////////////////////////////////////////////////////////////////////////////////
// Макросы
#define _PAUSE(p)		Sleep (p);
#define _EXITONERROR_	/*_getch ();*/ return -1;
#define _EXEC_			system (Buff);
#define _MOVETO(x,y)	mouse_event (MOUSEEVENTF_MOVE, -500, -500, 0, 0);	\
						mouse_event (MOUSEEVENTF_MOVE, x, y, 0, 0);
#define _CLICK_			mouse_event (MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);		\
						mouse_event (MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
#define _ONEKEY(k)		keybd_event (k, 0, 0, 0);							\
						keybd_event (k, 0, KEYEVENTF_KEYUP, 0);
#define _DBLKEY(k1,k2)	keybd_event (k1, 0, 0, 0);							\
						keybd_event (k2, 0, 0, 0);							\
						keybd_event (k1, 0, KEYEVENTF_KEYUP, 0);			\
						keybd_event (k2, 0, KEYEVENTF_KEYUP, 0);

/////////////////////////////////////////////////////////////////////////////////////
// Переменные
char String[DEFSZ], *StringOut;
HGLOBAL hglb;
HANDLE hnd;

/////////////////////////////////////////////////////////////////////////////////////
// Главная функция
int main (int argc, char* argv[])
	{
	// Выбор варианта использования
	uchar useCopyComb = 0;
	if (argc > 1)
		{
		if (((argv[1][0] == '-') || (argv[1][0] == '/')) &&
			((argv[1][1] == 'F') || (argv[1][1] == 'f')))
			{
			useCopyComb = 1;
			}
		}

	// Переход в целевое окно
	if (useCopyComb)
		{
		// Alt + Tab
		_PAUSE (200)
		_DBLKEY (VK_MENU, VK_TAB)
		_PAUSE (200)

		// Ctrl + Ins
		_DBLKEY (VK_CONTROL, VK_INSERT)
		_PAUSE (200)
		}

	// Копирование текста из буфера обмена
	OpenClipboard (NULL);
	hnd = GetClipboardData (CF_TEXT);
	sprintf (String, "%s", hnd);

	// Обработка
	if (String[0] < 0x80)		// Английский текст
		StringOut = ERConvert (String);
	else						// Русский текст
		StringOut = REConvert (String);

	// Очистка буфера и загрузка строки
	hglb = GlobalAlloc (GMEM_FIXED, strlen (String) + 1);
	strcpy_s ((char *)hglb, strlen (StringOut) + 1, StringOut);

	EmptyClipboard ();
	SetClipboardData (CF_TEXT, hglb);

	if (useCopyComb)
		{
		// Shift + Ins
		CloseClipboard ();
		_DBLKEY (VK_SHIFT, VK_INSERT)
		}

	return 0;
	}
