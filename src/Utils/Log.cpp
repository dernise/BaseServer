#include "Log.hpp"

void Logging::Initialize(const char* fileName, int logLevel, ...){
	if(!fileName || !logLevel)
		return;

	m_logLevel = logLevel;
	m_file = new std::ofstream;
	m_file->open(fileName, std::ios_base::app);
}

void Logging::SetColor(bool stdout_stream, Color color)
{
#ifdef WIN32
    static WORD WinColorFG[Color_count] =
    {
        0,                                                  // BLACK
        FOREGROUND_RED,                                     // RED
        FOREGROUND_GREEN,                                   // GREEN
        FOREGROUND_RED | FOREGROUND_GREEN,                  // BROWN
        FOREGROUND_BLUE,                                    // BLUE
        FOREGROUND_RED |                    FOREGROUND_BLUE,// MAGENTA
        FOREGROUND_GREEN | FOREGROUND_BLUE,                 // CYAN
        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,// WHITE
        // YELLOW
        FOREGROUND_RED | FOREGROUND_GREEN |                   FOREGROUND_INTENSITY,
        // RED_BOLD
        FOREGROUND_RED |                                      FOREGROUND_INTENSITY,
        // GREEN_BOLD
        FOREGROUND_GREEN |                   FOREGROUND_INTENSITY,
        FOREGROUND_BLUE | FOREGROUND_INTENSITY,             // BLUE_BOLD
        // MAGENTA_BOLD
        FOREGROUND_RED |                    FOREGROUND_BLUE | FOREGROUND_INTENSITY,
        // CYAN_BOLD
        FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
        // WHITE_BOLD
        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
    };

    HANDLE hConsole = GetStdHandle(stdout_stream ? STD_OUTPUT_HANDLE : STD_ERROR_HANDLE);
    SetConsoleTextAttribute(hConsole, WinColorFG[color]);
#else

    enum ANSITextAttr
    {
        TA_NORMAL = 0,
        TA_BOLD = 1,
        TA_BLINK = 5,
        TA_REVERSE = 7
    };

    enum ANSIFgTextAttr
    {
        FG_BLACK = 30, FG_RED,  FG_GREEN, FG_BROWN, FG_BLUE,
        FG_MAGENTA,  FG_CYAN, FG_WHITE, FG_YELLOW
    };

    enum ANSIBgTextAttr
    {
        BG_BLACK = 40, BG_RED,  BG_GREEN, BG_BROWN, BG_BLUE,
        BG_MAGENTA,  BG_CYAN, BG_WHITE
    };

    static uint8 UnixColorFG[Color_count] =
    {
        FG_BLACK,                                           // BLACK
        FG_RED,                                             // RED
        FG_GREEN,                                           // GREEN
        FG_BROWN,                                           // BROWN
        FG_BLUE,                                            // BLUE
        FG_MAGENTA,                                         // MAGENTA
        FG_CYAN,                                            // CYAN
        FG_WHITE,                                           // WHITE
        FG_YELLOW,                                          // YELLOW
        FG_RED,                                             // LRED
        FG_GREEN,                                           // LGREEN
        FG_BLUE,                                            // LBLUE
        FG_MAGENTA,                                         // LMAGENTA
        FG_CYAN,                                            // LCYAN
        FG_WHITE                                            // LWHITE
    };

    fprintf((stdout_stream ? stdout : stderr), "\x1b[%d%sm", UnixColorFG[color], (color >= YELLOW && color < Color_count ? ";1" : ""));
#endif
}

void Logging::ResetColor(bool stdout_stream)
{
#ifdef WIN32
	HANDLE hConsole = GetStdHandle(stdout_stream ? STD_OUTPUT_HANDLE : STD_ERROR_HANDLE);
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
#else
	SetColor(true, WHITE); //hardcodded because fuck yea <3
#endif
}

std::string Logging::getCurrentTime()
{
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

void Logging::outString(const char* str, ...) // Do not go higher than 1024
{
    if (!str)
      return;

	SetColor(true, GREEN);
	va_list ap;
    char Buffer[1024];
    va_start(ap,str);
    vsprintf(Buffer,str,ap);
    va_end(ap);
	printf(Buffer);
    printf("\n");
    *m_file << getCurrentTime().data() << " : " <<  Buffer << std::endl; 
	m_file->flush();
	ResetColor(true);
}

void Logging::outError(const char* str, ...)
{
    if (!str)
      return;

	SetColor(true, RED);
	va_list ap;
    char Buffer[1024]; 
    va_start(ap,str);
    vsprintf(Buffer,str,ap);
    va_end(ap);
	printf(Buffer);
    printf("\n");
	*m_file << getCurrentTime().data() << " : " <<  Buffer << std::endl; 
	m_file->flush();
	ResetColor(true);
}

void Logging::outWarning(const char* str, ...)
{
    if (!str)
      return;

	SetColor(true, YELLOW);
	va_list ap;
    char Buffer[1024]; 
    va_start(ap,str);
    vsprintf(Buffer,str,ap);
    va_end(ap);
	printf(Buffer);
    printf("\n");
    *m_file << getCurrentTime().data() << " : " <<  Buffer << std::endl; 
	m_file->flush();
	ResetColor(true);
}
