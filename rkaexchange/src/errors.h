typedef enum
{
    E_CMDREQ,
    E_UNKNOWNCMD,
    E_NOEXIST,
    E_CREATE,
    E_FMT,
	E_UNDEFERR	
} eErrLvl;

typedef struct
{
	eErrLvl errLvl;
	char* errMsg;
}tError;

void exitNicely(eErrLvl errorlevel);

