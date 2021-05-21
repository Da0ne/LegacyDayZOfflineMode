class ManBase extends DayZPlayerImplement // PSOVIS originaly extends Man
{
	private bool m_IsSkinned;
	
	bool	m_IsInteractable;
	bool	m_SynchHideBody;
	
	autoptr Timer m_Timer;

	void ManBase()
	{
		//Print("PSOVIS: this is ManBase");

		AllowInteractions(true);
		m_IsSkinned = false;
	}

	// -------------------------------------------------------------------------
	bool IsPlayer()
	{
		if( GetGame().GetPlayer() == this )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	// -------------------------------------------------------------------------
	void AllowInteractions(bool state)
	{
		m_IsInteractable = state;
	}

	// -------------------------------------------------------------------------
	bool IsInteractable()
	{
		return m_IsInteractable;
	}

	// -------------------------------------------------------------------------
	void OnSynchronize(ParamsWriteContext ctx)
	{
		super.OnSynchronize(ctx);
		autoptr Param1<bool> param1_hide_body = new Param1<bool>(m_SynchHideBody);
		ctx.Write(param1_hide_body);
	}

	// -------------------------------------------------------------------------
	void OnSynchronized(ParamsReadContext ctx)
	{
		super.OnSynchronized(ctx);
		autoptr Param1<bool> param1_hide_body = new Param1<bool>(false);
			
		if ( ctx.Read(param1_hide_body) )
		{
			if ( param1_hide_body.param1 == true )
			{
				HideBodyStart();
			}
		}
	}

	// -------------------------------------------------------------------------

	Object GetCursorObject()
	{
		if ( !GetGame().IsClient() )
		{
			return GetGame().GetPlayerCursorObject(this);
		}
		return NULL;
	}

	// -------------------------------------------------------------------------
	void HideBodyStart()
	{
		if ( m_SynchHideBody == false )
		{
			m_SynchHideBody = true;
			SetAnimVariableBool("hideBody", true); // Start the hiding animation
			AllowInteractions(false); // Disable skinning or other actions
			
			if ( GetGame().IsServer() )
			{
				m_Timer = new Timer( CALL_CATEGORY_GAMEPLAY );
				m_Timer.Run(9, this, "HideBodyEnd"); // TO DO: Use an animation event for deleting the body instead of a timer
				SetSynchDirty(); // Calls OnSynchronized and OnSynchronize on server and client
			}
		}
	}

	// -------------------------------------------------------------------------
	void HideBodyEnd()
	{
		if (this) {
			g_Game.ObjectDelete(this); 
			this.Release();
		}
	}

	// Skinning
	bool IsSkinned()
	{
		return m_IsSkinned;
	}

	// Skinning
	void MarkAsSkinned()
	{
		m_IsSkinned = true;
	}
	// Skinning
	bool CanBeSkinned(PlayerBase user, ItemBase tool)
	{
		if ( !IsSkinned()  &&  user  &&  tool )
		{
			if ( !IsAlive()  &&  tool.ConfigGetBool("canSkinBodies") )
			{
				return true;
			}
		}
		
		return false;
	}
}