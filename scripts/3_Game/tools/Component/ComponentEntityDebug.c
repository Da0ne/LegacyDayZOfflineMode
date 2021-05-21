class ComponentEntityDebug extends Component
{
	
	protected Shape				m_DebugShapeBBox;
	protected Shape				m_DebugShapeDirection;
	protected float				m_DebugShapeDirectionDist;
	protected autoptr TimerUpdate m_TimerUpdate;

	void ComponentEntityDebug()
	{
		m_TimerUpdate = new TimerUpdate(this, "OnDrawing", CALL_CATEGORY_SYSTEM);
		m_TimerUpdate.Pause();
	}

	void ~ComponentEntityDebug()
	{
		m_TimerUpdate.Stop();
	}

	// -------------------------------------------------------------------------
	Shape DebugBBoxDraw()
	{
		if ( !GetGame().IsDebug() )
			return NULL;
		
		if ( m_DebugShapeBBox != NULL )
			m_DebugShapeBBox.Destroy();
		
		
		vector min_max[2];
		
		if(!m_ThisEntityAI.GetCollisionBox(min_max))
			return NULL;
		
		m_DebugShapeBBox = Debug.DrawBox(min_max[0], min_max[1]);
		
		m_TimerUpdate.Continue();
		
		m_ThisEntityAI.SetEventMask(EntityEvent.FRAME);
	}

	// -------------------------------------------------------------------------
	void DebugBBoxSetColor(int color)
	{
		if ( m_DebugShapeBBox )
		{
			m_DebugShapeBBox.SetColor(color);
		}
	}

	// -------------------------------------------------------------------------
	void DebugBBoxDelete()
	{
		if ( !GetGame().IsDebug() )
		{
			return;
		}
		
		if ( m_DebugShapeBBox != NULL )
		{
			m_DebugShapeBBox.Destroy();
			m_DebugShapeBBox = NULL;
		}
		
		if ( m_DebugShapeDirection == NULL && m_DebugShapeBBox == NULL )
		{
			m_TimerUpdate.Pause();
		}
	}

	// -------------------------------------------------------------------------
	Shape DebugDirectionDraw(float distance = 1)
	{	
		if ( !GetGame().IsDebug() )
		{
			return NULL;
		}
		
		if ( m_DebugShapeDirection != NULL )
		{
			m_DebugShapeDirection.Destroy();
		}
		
		vector p1 = "0 0 0";
		vector p2 = m_ThisEntityAI.GetDirection() * m_DebugShapeDirectionDist;
		m_DebugShapeDirectionDist = distance;
		m_DebugShapeDirection = Debug.DrawArrow(p1, p2);
		
		m_TimerUpdate.Continue();
		
		m_ThisEntityAI.SetEventMask(EntityEvent.FRAME);
	}

	// -------------------------------------------------------------------------
	void DebugDirectionDelete()
	{
		if ( !GetGame().IsDebug() )
		{
			return;
		}
		
		if ( m_DebugShapeDirection != NULL )
		{
			m_DebugShapeDirection.Destroy();
			m_DebugShapeDirection = NULL;
		}
		
		if ( m_DebugShapeDirection == NULL && m_DebugShapeBBox == NULL )
		{
			m_TimerUpdate.Pause();
		}
	}

	void OnDrawing()
	{
		if ( !GetGame().IsDebug() )
		{
			return;
		}
		
		if ( m_DebugShapeBBox != NULL || m_DebugShapeDirection != NULL )
		{		
			vector mat[4];
			m_ThisEntityAI.GetTransform(mat);
		
			if ( m_DebugShapeBBox != NULL )
			{
				m_DebugShapeBBox.SetMatrix(mat);
			}
			
			if ( m_DebugShapeDirection != NULL )
			{
				m_DebugShapeDirection.SetMatrix(mat);			
			}
		}
	}
}