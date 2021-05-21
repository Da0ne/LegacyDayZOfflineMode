// -----------------------------------------------------------
class CollapsibleContainer extends ScriptedWidgetEventHandler
{
	Widget ButtonsPanel;
	// -----------------------------------------------------------
	bool OnClick(Widget w, int x, int y, int button)
	{
		// Print("CollapsibleContainer::OnClick");
		/*
		if (w && w.GetParent() == ButtonsPanel)
		{
			SelectedTab = 0;
			Widget iw = ButtonsPanel.GetChildren();
			while(iw)
			{
				if (iw == w) break;
				SelectedTab++;
				iw = iw.GetSibling();
			}
	
			SelectTab(SelectedTab);
		}
		*/
	}
	
	void OnWidgetScriptInit(Widget w)
	{
		// Print("CollapsibleContainer::OnWidgetScriptInit");
	/*	m_root = w;
		m_count = 0;
	
		Widget child = m_root.GetChildren();
		while (child)
		{
			m_count++;
			child.SetFlags(WidgetFlags.EXACTPOS | WidgetFlags.EXACTSIZE, false);
			child = child.GetSibling();
		}
	
		m_root.SetHandler(this);*/
	}	
};