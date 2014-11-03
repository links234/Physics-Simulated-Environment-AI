#include "GUI.h"



//************************************	IEvent
IEvent::IEvent(EventId eventId)
	: mEventId(eventId)
{
}

//************************************	GUIObject
void GUIObject::Render(BITMAP *hBackBuffer)
{
}

void GUIObject::SetWH(int W, int H)
{
	destroy_bitmap(hClientArea);
	mWidth=W;
	mHeight=H;
	hClientArea=create_bitmap(mWidth,mHeight);
}

//************************************	GUIButton
GUIButton::GUIButton(Vec2 position, int width, int height, std::string text, bool hide)
	: mPosition(position), mText(text), mHide(hide), mState(BUTTON_NORMAL)
{
	mWidth=width;
	mHeight=height;
	hClientArea=create_bitmap(mWidth,mHeight);
}

void GUIButton::Render(BITMAP *hBackBuffer)
{
	if(!mHide)
	{
		StretchBlit(GUI::GetInstance()->BTNSkin[mState],hClientArea,Vec2(0,0),Vec2(mWidth-1,mHeight-1));

		int centreX=mWidth>>1;
		int centreY=(mHeight-text_height(font))>>1;
		textprintf_centre_ex(hClientArea,font,centreX,centreY,makecol(0,0,0),-1,mText.c_str());

		StretchBlit(hClientArea,hBackBuffer,mPosition,mPosition+Vec2(mWidth-1,mHeight-1));
	}
}

//************************************	GUIHWND
GUIHWND::GUIHWND(Vec2 position, std::string caption, int width, int height, CColor captionColor, bool hide)
	: mPosition(position), mCaption(caption), mCaptionColor(captionColor), mHide(hide)
{
	mWidth=width;
	mHeight=height;
	hClientArea=create_bitmap(mWidth,mHeight);
}

void GUIHWND::Render(BITMAP *hBackBuffer)
{
	if(!mHide)
	{
		clear_bitmap(hClientArea);

		CColor background=GUI::GetInstance()->SKINBackground;
		rectfill(hClientArea,0,0,mWidth-1,mHeight-1,makecol(background.mRed,background.mGreen,background.mBlue));

		for(std::vector<GUIObject*>::iterator it=mChilds.begin();it!=mChilds.end();++it)
		{
			(*it)->Render(hClientArea);
		}

		int left=GUI::GetInstance()->GUIHWND_OffsetLeft;
		int right=GUI::GetInstance()->GUIHWND_OffsetRight;
		int top=GUI::GetInstance()->GUIHWND_OffsetTop;
		int bottom=GUI::GetInstance()->GUIHWND_OffsetBottom;
		int mBorder=GUI::GetInstance()->GUIHWND_Border;

		P[1]=mPosition;
		P[2]=P[1]+Vec2(mBorder,0);

		P_ClientAreaTL=P[1]+Vec2(left,top);
		P_ClientAreaBR=P_ClientAreaTL+Vec2(mWidth,mHeight);

		P[4].y=P[1].y;
		P[4].x=P_ClientAreaBR.x+right;

		P[3].y=P[1].y;
		P[3].x=P[4].x-mBorder;

		for(register int i=5;i<=8;++i)
		{
			P[i]=P[i-4]+Vec2(0,mBorder);
		}

		for(register int i=13;i<=16;++i)
		{
			P[i].y=P_ClientAreaBR.y+bottom;
			P[i].x=P[i-8].x;
		}

		for(register int i=9;i<=12;++i)
		{
			P[i]=P[i+4]-Vec2(0,mBorder);
		}
		
		StretchBlit(GUI::GetInstance()->HWNDSkin[0],hBackBuffer,P[1],P[6]);
		StretchBlit(GUI::GetInstance()->HWNDSkin[1],hBackBuffer,P[2],P[7]);
		StretchBlit(GUI::GetInstance()->HWNDSkin[2],hBackBuffer,P[3],P[8]);
		StretchBlit(GUI::GetInstance()->HWNDSkin[3],hBackBuffer,P[5],P[10]);
		StretchBlit(GUI::GetInstance()->HWNDSkin[4],hBackBuffer,P[7],P[12]);
		StretchBlit(GUI::GetInstance()->HWNDSkin[5],hBackBuffer,P[9],P[14]);
		StretchBlit(GUI::GetInstance()->HWNDSkin[6],hBackBuffer,P[10],P[15]);
		StretchBlit(GUI::GetInstance()->HWNDSkin[7],hBackBuffer,P[11],P[16]);
		StretchBlit(hClientArea,hBackBuffer,P_ClientAreaTL,P_ClientAreaBR);
		int centreX=(P[1].x+P[4].x)/2;
		int centreY=P[1].y+(top-text_height(font))/2;
		textprintf_centre_ex(hBackBuffer,font,centreX,centreY,makecol(mCaptionColor.mRed,mCaptionColor.mGreen,mCaptionColor.mBlue),-1,mCaption.c_str());
	}
}

//************************************	GUI
GUI GUI::sGUIInstance;

GUI::GUI()
	: GUIHWND_Border(40)
{
	for(register int i=0;i<8;++i)
	{
		HWNDSkin[i]=NULL;
	}
	for(register int i=0;i<3;++i)
	{
		BTNSkin[i]=NULL;
	}
}

GUI* GUI::GetInstance()
{
	return &sGUIInstance;
}

void GUI::Post(IEvent iEvent)
{
	mEventQueue.push(iEvent);
}

void GUI::Render(BITMAP *hBackBuffer)
{
	for(std::vector<GUIObject*>::iterator it=mHWNDs.begin();it!=mHWNDs.end();++it)
	{
		(*it)->Render(hBackBuffer);
	}

	for(std::vector<GUIObject*>::iterator it=mHUDObjs.begin();it!=mHUDObjs.end();++it)
	{
		(*it)->Render(hBackBuffer);
	}
}

GUI::~GUI()
{
	//UnloadSkin();
}

void GUI::UnloadSkin()
{
	for(register int i=0;i<8;++i)
	{
		if(HWNDSkin[i]!=NULL)
		{
			destroy_bitmap(HWNDSkin[i]);
		}
	}
	for(register int i=0;i<3;++i)
	{
		if(BTNSkin[i]!=NULL)
		{
			destroy_bitmap(BTNSkin[i]);
		}
	}
}

void GUI::LoadSkin(std::string skinfile)
{
	UnloadSkin();
	PALLETE pallete;
	std::ifstream fin(skinfile.c_str());
	std::string str;
	fin>>str;
	while(!fin.eof())
	{
		if(str=="[GUIHWND]")
		{
			for(register int i=0;i<8;++i)
			{
				fin>>str;
				HWNDSkin[i]=load_bitmap(str.c_str(),pallete);
			}
		}
		else if(str=="[GUIButton]")
		{
			for(register int i=0;i<3;++i)
			{
				fin>>str;
				BTNSkin[i]=load_bitmap(str.c_str(),pallete);
			}
		}
		else if(str=="[Background-Color]")
		{
			fin>>str>>str>>SKINBackground.mRed;
			fin>>str>>str>>SKINBackground.mGreen;
			fin>>str>>str>>SKINBackground.mBlue;
		}
		else if(str=="[GUIHWND-Border]")
		{
			fin>>str>>str>>GUIHWND_Border;
		}
		else if(str=="[GUIHWND-Client-Area-Offset]")
		{
			fin>>str>>str>>GUIHWND_OffsetLeft;
			fin>>str>>str>>GUIHWND_OffsetRight;
			fin>>str>>str>>GUIHWND_OffsetTop;
			fin>>str>>str>>GUIHWND_OffsetBottom;
		}
		fin>>str;
	}
	fin.close();
}

void GUI::AddGUIHWND(GUIObject *pObj)
{
	mHWNDs.push_back(pObj);
}

void GUI::AddHUDObj(GUIObject *pObj)
{
	mHUDObjs.push_back(pObj);
}

bool GUI::Update(float fDelta)
{
	while(!mEventQueue.empty())
	{
		mEventQueue.pop();
	}
	return true;
}
