class UIPopupScriptPositionManager extends UIPopupScript
{
	private TextListboxWidget	m_LstPositionList;
	private EditBoxWidget m_TxtSelectedX;
	private EditBoxWidget m_TxtSelectedY;
	private EditBoxWidget m_TxtCurrentX;
	private EditBoxWidget m_TxtCurrentY;
	private PluginConfigDebugProfileFixed m_ConfigDebugProfileFixed;
	private ButtonWidget m_TeleportButton;
	private ButtonWidget m_CancelButton;
			
	//================================================
	// UIPopupScriptPositionManager
	//================================================	
	void UIPopupScriptPositionManager( Widget wgt )
	{
		m_ConfigDebugProfileFixed	= (PluginConfigDebugProfileFixed)GetPlugin( PluginConfigDebugProfileFixed );
		m_LstPositionList	= wgt.FindAnyWidget("tls_ppp_pm_positions_list");
		m_TxtSelectedX = wgt.FindAnyWidget("pnl_ppp_pm_selected_x_value");
		m_TxtSelectedY = wgt.FindAnyWidget("pnl_ppp_pm_selected_y_value");
		m_TxtCurrentX = wgt.FindAnyWidget("pnl_ppp_pm_current_x_value");
		m_TxtCurrentY = wgt.FindAnyWidget("pnl_ppp_pm_current_y_value");
		m_TeleportButton = wgt.FindAnyWidget("btn_ppp_pm_teleport");
		m_CancelButton = wgt.FindAnyWidget("btn_ppp_pm_cancel");
		
		TStringArray positions_array = new TStringArray;
		m_ConfigDebugProfileFixed.GetAllPositionsNames( positions_array );
		
		for ( int i = 0; i < positions_array.Count(); i++ )
		{
			m_LstPositionList.AddItem( positions_array.Get(i), new PresetParams ( positions_array.Get(i), true, false), 0);
		}
		
		delete positions_array;
	}
	
	//================================================
	// OnOpen
	//================================================	
	void OnOpen( Param param )
	{
		if (!PluginSceneManager.GetInstance().GetSelectedSceneObject())
			return;
		
		vector player_pos = PluginSceneManager.GetInstance().GetSelectedSceneObject().GetPosition();

		m_TxtCurrentX.SetText( player_pos[0].ToString() );
		m_TxtCurrentY.SetText( player_pos[2].ToString() );
	}
	
	//================================================
	// OnClick
	//================================================
	bool OnClick( Widget w, int x, int y, int button )
	{
		if ( w == m_TeleportButton )
		{
			float pos_x = m_TxtSelectedX.GetText().ToFloat();
			float pos_z = m_TxtSelectedY.GetText().ToFloat();
			
			PluginSceneManager.GetInstance().SelectedObjectSetPosX( pos_x );
			PluginSceneManager.GetInstance().SelectedObjectSetPosZ( pos_z );
			PopupBack();
			return true;
		}
		else if ( w == m_CancelButton )
		{
			PopupBack();
			return true;
		}
	}
	
	bool OnItemSelected( Widget w, int x, int y, int row, int column, int oldRow, int oldColumn )
	{	
		vector position = m_ConfigDebugProfileFixed.GetPositionByName( GetCurrentPositionName() );
		m_TxtSelectedX.SetText( position[0].ToString() );
		m_TxtSelectedY.SetText( position[2].ToString() );
	}
	
	string GetCurrentPositionName()
	{
		if ( m_LstPositionList.GetSelectedRow() != -1 )
		{
			string position_name =  m_LstPositionList.GetItem( m_LstPositionList.GetSelectedRow(), 0 );
			return position_name;
		}
		return "";
	}
}
