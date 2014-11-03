#include "Main.h"

BITMAP	*hBackBuffer;
CTimer	Timer;
StarShip *pPlayerShip;
StarShipAI *pEntity;
Obstacle *pObstacle;
CCamera *gCamera;
bool bSimulationPaused=false;
int SCREEN_WIDTH,SCREEN_HEIGHT;
GUI *gGUI=GUI::GetInstance();

structMouse CurrMouse, LastMouse;

void InitInput();
void InitGame();
void InitRender();
void NextFrame();
void GameRender();
void GatherInput();
void GameLogic(float fDelta);
void ProcessInput(float fDelta);

void InitGame()
{
	pPlayerShip=new StarShip(Vec2(1366/2,768/2),CColor(255,255,255),50,100.0f,360.0f,0.005f,Vec2(0,1));

	pEntity=new StarShipAI(new StarShip(Vec2(30,650),CColor(255,0,0)));
	pEntity->Chase(pPlayerShip);

	pEntity=new StarShipAI(new StarShip(Vec2(55,550),CColor(255,0,0)));
	pEntity->Chase(pPlayerShip);

	StarShip *IntercepterShip=new StarShip(Vec2(70,450),CColor(0,255,0));
	pEntity=new StarShipAI(IntercepterShip);
	pEntity->Intercept(pPlayerShip);

	pEntity=new StarShipAI(new StarShip(Vec2(70,350),CColor(0,255,255)));
	pEntity->Chase(IntercepterShip);
	pEntity->Evade(pPlayerShip);



	pEntity=new StarShipAI(new StarShip(Vec2(1366-30,650),CColor(255,0,0)));
	pEntity->Chase(pPlayerShip);

	pEntity=new StarShipAI(new StarShip(Vec2(1366-55,550),CColor(255,0,0)));
	pEntity->Chase(pPlayerShip);
	
	IntercepterShip=new StarShip(Vec2(1366-70,450),CColor(0,255,0));
	pEntity=new StarShipAI(IntercepterShip);
	pEntity->Intercept(pPlayerShip);

	pEntity=new StarShipAI(new StarShip(Vec2(1366-70,350),CColor(0,255,255)));
	pEntity->Chase(IntercepterShip);
	pEntity->Evade(pPlayerShip);

	new Obstacle(Vec2(500,500),70);

	gCamera=new CCamera();
	gCamera->SetZoom(1.0);
	gCamera->SetCamTarget(pPlayerShip);
	gCamera->EnableCamFallow();

	Obstacle::SetCamera(gCamera);
	StarShip::SetCamera(gCamera);

	gGUI->LoadSkin("Data\\skin.dat");

	//GUIHWND *pHWND=new GUIHWND(Vec2(50,50),std::string("Test GUIHWND"),300,300,CColor(255,0,0),false);
	//pHWND->mChilds.push_back((GUIObject*)(new GUIButton(Vec2(20,50),100,30,"HWND Button",false)));
	//gGUI->AddGUIHWND((GUIObject*)(pHWND));
	//gGUI->AddHUDObj((GUIObject*)(new GUIButton(Vec2(700,700),100,30,"HUD Button",false)));
}

void InitInput()
{
	install_keyboard();
    install_mouse();

	LastMouse.Pos.x=(int)((double)mouse_x*((double)VIRTUALSCREEN_WIDTH/(double)SCREEN_WIDTH));
	LastMouse.Pos.y=(int)((double)mouse_y*((double)VIRTUALSCREEN_HEIGHT/(double)SCREEN_HEIGHT));
	LastMouse.Button.Left=false;
	if(mouse_b&LEFTMBTN)
	{
		LastMouse.Button.Left=true;
	}
	LastMouse.Button.Right=false;
	if(mouse_b&RIGHTMBTN)
	{
		LastMouse.Button.Right=true;
	}
	LastMouse.Button.Middle=false;
	if(mouse_b&MIDDLEMBTN)
	{
		LastMouse.Button.Middle=true;
	}
}

void GatherInput()
{
	LastMouse=CurrMouse;

	CurrMouse.Pos.x=(int)((double)mouse_x*((double)VIRTUALSCREEN_WIDTH/(double)SCREEN_WIDTH));
	CurrMouse.Pos.y=(int)((double)mouse_y*((double)VIRTUALSCREEN_HEIGHT/(double)SCREEN_HEIGHT));
	CurrMouse.Wheel=(int)mouse_z;
	CurrMouse.Button.Left=false;
	if(mouse_b&LEFTMBTN)
	{
		CurrMouse.Button.Left=true;
	}
	CurrMouse.Button.Right=false;
	if(mouse_b&RIGHTMBTN)
	{
		CurrMouse.Button.Right=true;
	}
	CurrMouse.Button.Middle=false;
	if(mouse_b&MIDDLEMBTN)
	{
		CurrMouse.Button.Middle=true;
	}

	if(((int)CurrMouse.Pos.x!=(int)LastMouse.Pos.x) || ((int)CurrMouse.Pos.y!=(int)LastMouse.Pos.y))
	{
		gGUI->Post(IEvent(EVENT_MOUSEMOVE));
	}

	if(CurrMouse.Wheel!=LastMouse.Wheel)
	{
		gGUI->Post(IEvent(EVENT_MOUSEWHEEL));
	}

	if(CurrMouse.Button.Left!=LastMouse.Button.Left)
	{
		if(CurrMouse.Button.Left==true)
		{
			gGUI->Post(IEvent(EVENT_LBUTTONDOWN));
		}
		else
		{
			gGUI->Post(IEvent(EVENT_LBUTTONUP));
		}
	}

	if(CurrMouse.Button.Right!=LastMouse.Button.Right)
	{
		if(CurrMouse.Button.Right==true)
		{
			gGUI->Post(IEvent(EVENT_RBUTTONDOWN));
		}
		else
		{
			gGUI->Post(IEvent(EVENT_RBUTTONUP));
		}
	}

	if(CurrMouse.Button.Middle!=LastMouse.Button.Middle)
	{
		if(CurrMouse.Button.Middle==true)
		{
			gGUI->Post(IEvent(EVENT_MBUTTONDOWN));
		}
		else
		{
			gGUI->Post(IEvent(EVENT_MBUTTONUP));
		}
	}
}

void ProcessInput(float fDelta)
{
	GatherInput();

	bool bFocusOnGame=gGUI->Update(fDelta);
	if(bFocusOnGame)
	{
		static STimer T;
	
		if(key[KEY_Q] && T.Time()>KEYBOARDDELAY)
		{
			T.Start();
			bSimulationPaused=!bSimulationPaused;
		}

		bool playerLeftThruster=false;
		bool playerRightThruster=false;
		bool playerFrontThruster=false;
		bool playerBackThruster=false;

		if(key[KEY_A])
		{
			playerRightThruster=true;
		}
		if(key[KEY_D])
		{
			playerLeftThruster=true;
		}
		if(key[KEY_W])
		{
			playerBackThruster=true;
		}
		else if(key[KEY_S])
		{
			playerFrontThruster=true;
		}

		pPlayerShip->SetThrusters(playerFrontThruster,playerBackThruster,playerLeftThruster,playerRightThruster);

		gCamera->SetZoom(gCamera->GetZoom()+(double)(LastMouse.Wheel-CurrMouse.Wheel)*MOUSE_WHEEL_SPEED);
	}
}

void InitRender()
{
    if(allegro_init()!=0){
        throw FatalError("Allegro initialization failed!");
    }

    set_color_depth(32);

	/*
	SCREEN_WIDTH=800;
	SCREEN_HEIGHT=600;
	set_gfx_mode(GFX_AUTODETECT_WINDOWED,SCREEN_WIDTH,SCREEN_HEIGHT,0,0);
	/*/
	SCREEN_WIDTH=1366;
	SCREEN_HEIGHT=768;
	if(set_gfx_mode(GFX_DIRECTX_ACCEL,SCREEN_WIDTH,SCREEN_HEIGHT,0,0)!=0)
	{
		SCREEN_WIDTH=1024;
		SCREEN_HEIGHT=768;
		if(set_gfx_mode(GFX_DIRECTX_ACCEL,SCREEN_WIDTH,SCREEN_HEIGHT,0,0)!=0)
		{
			SCREEN_WIDTH=800;
			SCREEN_HEIGHT=600;
			if(set_gfx_mode(GFX_DIRECTX_ACCEL,SCREEN_WIDTH,SCREEN_HEIGHT,0,0)!=0)
			{
				SCREEN_WIDTH=640;
				SCREEN_HEIGHT=480;
				if(set_gfx_mode(GFX_DIRECTX_ACCEL,SCREEN_WIDTH,SCREEN_HEIGHT,0,0)!=0)
				{
					set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
					throw FatalError("Unable to set resolution!");
				}
			}
		}
	}//*/

	//set_trans_blender(255, 255, 255, 255);

    hBackBuffer=create_bitmap(VIRTUALSCREEN_WIDTH,VIRTUALSCREEN_HEIGHT);
}

void NextFrame()
{
	Timer.Tick();
	float fDelta = bSimulationPaused?0.0f:Timer.GetTimeElapsed();
	GameLogic(fDelta);
	GameRender();
}

void GameRender()
{
	Obstacle::DrawAll(hBackBuffer);
	StarShip::DrawAll(hBackBuffer);

	gGUI->Render(hBackBuffer);

	textprintf_ex(hBackBuffer,font,10,10,makecol(255,255,255),-1,"FPS = %lf",Timer.GetFrameRate());
	if(bSimulationPaused)
	{
		textprintf_ex(hBackBuffer,font,10,25,makecol(255,255,255),-1,"Simulation is paused");
	}
	else
	{
		textprintf_ex(hBackBuffer,font,10,25,makecol(255,255,255),-1,"Simulation is running");
	}
	textprintf_ex(hBackBuffer,font,10,40,makecol(255,255,255),-1,"X=%lf Y=%lf Z=%d %lf",CurrMouse.Pos.x,CurrMouse.Pos.y,CurrMouse.Wheel,pPlayerShip->mSafeDist);

	draw_sprite(hBackBuffer,mouse_sprite,(int)CurrMouse.Pos.x,(int)CurrMouse.Pos.y);

    acquire_screen();
	StretchBlit(hBackBuffer,screen,Vec2(0,0),Vec2(SCREEN_WIDTH,SCREEN_HEIGHT));
    release_screen();
    clear_bitmap(hBackBuffer);
}

void GameLogic(float fDelta)
{
	ProcessInput(fDelta);

	gCamera->Update();

	StarShipAI::UpdateAll();
	StarShip::UpdateAll(fDelta);
}