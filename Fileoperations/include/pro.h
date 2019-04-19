#define KOMUTBUFFER 256
#define SATIRBUFFER 1024 
#define ERROR "\x1B[31m" "ERROR : " "\x1B[0m"
#define PROMPT "\x1B[33m" "> "

int IsCharIsNumber(char number[]);
int Add(char **komutlar);
int Search(char **komutlar);
int Write(char **komutlar);
int Print();
int Quit();
int ClearMemory();

