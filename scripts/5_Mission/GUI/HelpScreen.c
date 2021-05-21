class HelpScreen extends UIScriptedMenu
{
	TextListboxWidget m_KeyBindingsTextListboxWidget;
	TextListboxWidget m_MouseBindingsTextListboxWidget;

	ButtonWidget m_CloseConsoleButton;

	void HelpScreen()
	{
		
	}

	void ~HelpScreen()
	{	
	}

	Widget Init()
	{
		PluginKeyBinding module_keybinding = (PluginKeyBinding)GetPlugin(PluginKeyBinding);
		
		Widget layoutRoot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/help_screen.layout");
		m_KeyBindingsTextListboxWidget = layoutRoot.FindAnyWidget("KeyBindingsTextListboxWidget");
		m_MouseBindingsTextListboxWidget = layoutRoot.FindAnyWidget("MouseBindingsTextListboxWidget");
		
		array<KeyBinding> keybindings = module_keybinding.GetKeyBindings();
		for ( int i = 0; i < keybindings.Count(); i++ )
		{
			m_KeyBindingsTextListboxWidget.AddItem( keybindings.Get(i).GetInfoBind(), NULL, 0 );
			m_KeyBindingsTextListboxWidget.SetItem( i, keybindings.Get(i).GetInfoDescription(), NULL, 1 );
		}
		
		array<MouseBinding> mousebindings = module_keybinding.GetMouseBindings();
		for ( i = 0; i < mousebindings.Count(); i++ )
		{
			m_MouseBindingsTextListboxWidget.AddItem( mousebindings.Get(i).GetInfoBind(), NULL, 0 );
			m_MouseBindingsTextListboxWidget.SetItem( i, mousebindings.Get(i).GetInfoDescription(), NULL, 1 );
		}
		
		m_CloseConsoleButton = layoutRoot.FindAnyWidget("CloseButtonWidget");
		
		return layoutRoot;
	}

	bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		
		if ( w == m_CloseConsoleButton )
		{
			Close();
			return true;
		}
		
		return false;
	}
}
