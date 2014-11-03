#ifndef GUI_H
#define GUI_H

#include "Main.h"

enum EventId {NOEVENT, EVENT_MOUSEMOVE, EVENT_MOUSEWHEEL, EVENT_LBUTTONDOWN, EVENT_LBUTTONUP, EVENT_RBUTTONDOWN, EVENT_RBUTTONUP, EVENT_MBUTTONDOWN, EVENT_MBUTTONUP};
enum ButtonState{BUTTON_NORMAL, BUTTON_HOVER, BUTTON_};

class IEvent
{
public:
	IEvent(EventId eventId);

	EventId mEventId;
};

class GUIObject
{
public:
	virtual void Render(BITMAP *hBackBuffer);
	virtual void SetWH(int W, int H);

protected:
	BITMAP *hClientArea;
	int mWidth,mHeight;
};

class GUIButton : public GUIObject 
{
public:
	GUIButton(Vec2 position=Vec2(0,0), int width=50, int height=50, std::string text="NULL", bool hide=false);

	virtual void Render(BITMAP *hBackBuffer);

	Vec2 mPosition;
	ButtonState mState;
	std::string mText;
	bool mHide;
};

class GUIHWND : public GUIObject
{
public:
	GUIHWND(Vec2 position=Vec2(0,0), std::string caption="NULL", int width=200, int height=200, CColor captionColor=CColor(255,0,0), bool hide=false);

	virtual void Render(BITMAP *hBackBuffer);

	std::vector<GUIObject*> mChilds;
	std::string mCaption;
	Vec2 mPosition;
	CColor mCaptionColor;
	bool mHide;

private:
	Vec2 P[17],P_ClientAreaTL,P_ClientAreaBR;
};

class GUI
{
public:

	void Post(IEvent iEvent);
	void Render(BITMAP *hBackBuffer);
	void BindScreenWH(int *W, int *H);
	void SetVirtualWH(int VW,int VH);
	void LoadSkin(std::string skinfile);
	void AddGUIHWND(GUIObject *pObj);
	void AddHUDObj(GUIObject *pObj);
	void UnloadSkin();
	
	bool Update(float fDelta);

	static GUI* GetInstance();

	int GUIHWND_Border;
	int GUIHWND_OffsetLeft;
	int GUIHWND_OffsetRight;
	int GUIHWND_OffsetTop;
	int GUIHWND_OffsetBottom;
	CColor SKINBackground;
	BITMAP *HWNDSkin[8];
	BITMAP *BTNSkin[3];

private:
	GUI();
	~GUI();

	std::vector<GUIObject*> mHWNDs;
	std::vector<GUIObject*> mHUDObjs;
	std::queue<IEvent> mEventQueue;

	static GUI sGUIInstance;
};

#endif

