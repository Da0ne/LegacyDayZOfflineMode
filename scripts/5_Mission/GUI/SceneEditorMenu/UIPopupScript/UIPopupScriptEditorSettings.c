class UIPopupScriptEditorSettings extends UIPopupScript
{
	private CheckBoxWidget		m_WgtTglSeleHighlight;
	private CheckBoxWidget		m_WgtTglSavePlayerPos;
	private EditBoxWidget		m_EdxRotationDelta;
	
	private ButtonWidget		m_BtnCancel;
	
	private PluginSceneManager	m_ModuleSceneManager;
	
	//================================================
	// UIPopupScriptEditorSettings
	//================================================	
	void UIPopupScriptEditorSettings(Widget wgt)
	{
		m_ModuleSceneManager = GetPlugin(PluginSceneManager);
		
		m_WgtTglSeleHighlight	= wgt.FindAnyWidget("cbx_ppp_est_flag_selection");
		m_WgtTglSavePlayerPos	= wgt.FindAnyWidget("cbx_ppp_est_flag_load_player_pos");
		m_EdxRotationDelta		= wgt.FindAnyWidget("ebx_ppp_est_rotation_delta_value");
		
		m_BtnCancel				= wgt.FindAnyWidget("btn_ppp_est_cancel");
	}

	//================================================
	// OnOpen
	//================================================	
	void OnOpen(Param param)
	{
		m_WgtTglSeleHighlight.SetChecked( m_ModuleSceneManager.GetDrawSelection() );
		m_WgtTglSavePlayerPos.SetChecked( m_ModuleSceneManager.GetLoadPlayerPos() );
		m_EdxRotationDelta.SetText( m_ModuleSceneManager.GetRotationAngle().ToString() );
	}

	//================================================
	// OnClick
	//================================================	
	bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		
		if ( w == m_WgtTglSeleHighlight )
		{
			m_ModuleSceneManager.SetDrawSelection(m_WgtTglSeleHighlight.IsChecked());
		}
		else if ( w == m_WgtTglSavePlayerPos )
		{
			m_ModuleSceneManager.SetLoadPlayerPos(m_WgtTglSavePlayerPos.IsChecked());
		}
		else if ( w == m_BtnCancel )
		{
			PopupBack();
			
			return true;
		}
		
		return false;
	}

	//============================================
	// OnChange
	//============================================
	bool OnChange(Widget w, int x, int y, bool finished)
	{	
		if ( w == m_EdxRotationDelta )
		{
			int angle = m_EdxRotationDelta.GetText().ToInt();
			
			if ( angle > 0 )
			{
				m_ModuleSceneManager.SetRotationAngle( angle );
			}
			
			return true;
		}
		
		return false;
	}
}
