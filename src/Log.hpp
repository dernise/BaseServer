#define sLog Log::getInstance()

class Log{
public:
	static Log& getInstance()
    {
        static Log instance;
        return instance;
    }
    void outString(const char* string);
private:
	Log();
};
