class MapMenu extends UIScriptedMenu
{
	Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_map.layout");
		
		return layoutRoot;
	}

	bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		
		switch (w.GetUserID())
		{
			case IDC_CANCEL:
				Close();
				return true;
		}

		return false;
	}
}
