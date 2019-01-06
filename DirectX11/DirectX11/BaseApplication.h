#pragma once

class BaseApplication
{
public:
	BaseApplication() {};

	virtual bool Initialize();

	virtual void Finalize();

	virtual void Tick();

	virtual void OnDraw() {};

	virtual void OnMouseMove(int x, int y);

	virtual void Quit();
	virtual bool IsQuit() { return m_bQuit; };

private:
	static bool m_bQuit;
};

extern BaseApplication *g_pApp;