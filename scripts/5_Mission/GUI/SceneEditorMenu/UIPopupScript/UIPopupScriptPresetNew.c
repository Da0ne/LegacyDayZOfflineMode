class UIPopupScriptPresetNew extends UIPopupScript
{
	private ButtonWidget m_BtnOk;
	private ButtonWidget m_BtnCancel;
	
	//================================================
	// UIPopupScriptSceneNew
	//================================================	
	void UIPopupScriptPresetNew( Widget wgt )
	{
		m_BtnOk		= wgt.FindAnyWidget("btn_ppp_pn_ok");
		m_BtnCancel	= wgt.FindAnyWidget("btn_ppp_pn_cancel");
	}

	//================================================
	// OnClick
	//================================================	
	bool OnClick( Widget w, int x, int y, int button )
	{
		super.OnClick( w, x, y, button );
		
		if ( w == m_BtnOk )
		{
			EditBoxWidget wgt_text = m_WgtRoot.FindAnyWidget("fld_ppp_pn_new_name");
			PluginConfigDebugProfile m_ConfigDebugProfile		= (PluginConfigDebugProfile)GetPlugin(PluginConfigDebugProfile);
			m_ConfigDebugProfile.PresetAdd( wgt_text.GetText() );
			
			SceneEditorMenu menu = GetGame().GetUIManager().GetMenu();
			menu.RefreshLists();
			PopupBack();
			
			return true;
		}
		else if ( w == m_BtnCancel )
		{
			PopupBack();
			
			return true;
		}
		
		return false;
	}
}
