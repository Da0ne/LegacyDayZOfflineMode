class ScriptConsoleRenamePresetDialog extends UIScriptedMenu
{
	void ScriptConsoleRenamePresetDialog()
	{

	}

	void ~ScriptConsoleRenamePresetDialog()
	{
	}

	Widget Init()
	{
		Widget layoutRoot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/script_console_dialog_preset.layout");
		m_Editbox = layoutRoot.FindAnyWidget("PrimaryEditBox");
		m_Label = layoutRoot.FindAnyWidget("WindowLabel");
		m_Message = layoutRoot.FindAnyWidget("MessageText");

		MissionBase mission = GetGame().GetMission();

		UIScriptedMenu ui_menu = GetGame().GetUIManager().FindMenu(MENU_SCRIPTCONSOLE);
		if ( ui_menu != NULL )
		{
			ScriptConsole scripted_console = ui_menu;

			m_Editbox.SetText( scripted_console.GetCurrentPresetName() );
			m_Label.SetText("RENAME PRESET");
			m_Message.SetText( scripted_console.GetCurrentPresetName() );
		}

		return layoutRoot;
	}

	bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);

		if (w.GetUserID() == IDC_OK)
		{
			UIScriptedMenu ui_menu = GetGame().GetUIManager().FindMenu(MENU_SCRIPTCONSOLE);
			if ( ui_menu != NULL )
			{
				ScriptConsole scripted_console = ui_menu;
				scripted_console.RenamePreset( m_Editbox.GetText() );
				Close();
				return true;
			}
		}
		else if (w.GetUserID() == IDC_CANCEL)
		{
			Close();
			return true;
		}

		return false;
	}
	
	EditBoxWidget m_Editbox;
	TextWidget m_Label;
	TextWidget m_Message;
}
