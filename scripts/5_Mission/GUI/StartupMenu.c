class StartupMenu extends UIScriptedMenu
{
	void StartupMenu()
	{
	}

	void ~StartupMenu()
	{
	}

	Widget Init()
	{
		Widget layoutRoot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/startup.layout");
		
		m_label = (TextWidget)layoutRoot.FindAnyWidget("TextWidget");

		return layoutRoot;
	}

	TextWidget m_label;
}
