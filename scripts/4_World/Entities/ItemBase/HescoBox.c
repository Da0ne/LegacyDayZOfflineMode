class EN5C_HescoBox extends EN5C_Inventory_Base
{
	static const int FOLDED 		= 0;
	static const int UNFOLDED 		= 1;
	static const int FILLED 		= 2;
	
	private int m_State;
	
	void EN5C_HescoBox()
	{
		m_State = FOLDED;
	}

	bool ConditionIntoInventory(EntityAI player)
	{
		return false;
	}

	bool ConditionIntoHands(EntityAI player)
	{
		if ( m_State == FOLDED )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool ConditionOutOfHands(EntityAI player)
	{
		return true;
	}

	void OnPlacementFinished( PlayerBase player )
	{
		Unfold();
	}

	int GetState()
	{
		return m_State;
	}

	void Fold()
	{
		this.ShowSelection( "inventory" );
		this.HideSelection( "placing" );
		this.HideSelection( "filled" );
		
		m_State = FOLDED;
		
		AddDamage( 0.05 );
	}

	void Unfold()
	{
		this.HideSelection( "inventory" );
		this.ShowSelection( "placing" );
		this.HideSelection( "filled" );
		
		m_State = UNFOLDED;
		
		AddDamage( 0.05 );
	}

	void Fill()
	{
		this.HideSelection( "inventory" );
		this.HideSelection( "placing" );
		this.ShowSelection( "filled" );
		
		m_State = FILLED;
		
		AddDamage( 0.05 );
	}
}
