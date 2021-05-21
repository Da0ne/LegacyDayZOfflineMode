class EarlyAccessMenu extends UIScriptedMenu
{
	void EarlyAccessMenu()
	{
	}

	void ~EarlyAccessMenu()
	{
	}
	
	Widget Init()
	{
		Widget layoutRoot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_early_access.layout");
		
		return layoutRoot;
	}

	bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		
		if (w.GetUserID() == IDC_OK)
		{
			Close();
			return true;
		}

		return false;
	}
}
