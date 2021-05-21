class WidgetObjectFollower : ScriptedWidgetEventHandler
{
	reference float speed;

	protected autoptr TimerUpdate m_TimerUpdate;
	protected Widget	m_Root;
	protected Object	m_Target;

	//=========================================
	// Constructor
	//=========================================
	void WidgetObjectFollower()
	{
		m_TimerUpdate = new TimerUpdate(this, "Update", CALL_CATEGORY_GUI);
	}
	
	//=========================================
	// OnWidgetScriptInit
	//=========================================
	void OnWidgetScriptInit(Widget w)
	{
		m_Root = w;
	}
	
	//=========================================
	// SetTarget
	//=========================================
	void SetTarget(Object target)
	{
		m_Target = target;
	}	
	
	//-----------------------------------------
	// Update
	//-----------------------------------------
	protected void Update()
	{
		if ( m_Target )
		{
			vector pos_obj_screen_relative = GetGame().GetScreenPosRelative( m_Target.GetPosition() );
			
			Print("= x:"+ pos_obj_screen_relative[0] +" y: "+ pos_obj_screen_relative[1] +" z: "+ pos_obj_screen_relative[2]);
			
			float x = pos_obj_screen_relative[0] - 0.5;
			float y = pos_obj_screen_relative[1] - 0.5;
			float z = pos_obj_screen_relative[2];
			
			if ( z <= 0 )
			{
				x = -x;
				
				if ( y > 0 )
				{
					y = -0.5;
				}
				else
				{
					y = 0.5;
				}
			}
			
			
			m_Root.SetPos(x, y, true);	
			
			
			
		}
	}
};