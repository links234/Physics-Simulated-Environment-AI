#include "Main.h"
#include "MainEngine.h"

int main()
{
	try
	{
		InitRandom();
		InitRender();
		InitInput();
		InitGame();

		Timer.Tick();
		while(!key[KEY_ESC]){
			NextFrame();
		}
	}
	catch (FatalError *eMsg)
	{
		allegro_message(eMsg->getMsg().c_str(), allegro_error);
	}

    return 0;
}
END_OF_MAIN()
