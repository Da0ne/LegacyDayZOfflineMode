class CAContinuousMineWood : CABase
{
	protected float 				m_TimeElpased;
	protected float 				m_TimeBetweenEachMaterialDrop;
	protected float					m_DamageToMiningItemEachDrop;
	protected int 					m_AmountOfDrops;
	protected string				m_Material;
	protected float 				m_AmountOfMaterialPerDrop;
	protected autoptr Param1<float>	m_SpentUnits;
	protected bool					m_DataLoaded = false;
	
	void CAContinuousMineWood(float time_between_drops)
	{
		m_ContinuousAction = true;
		m_TimeBetweenEachMaterialDrop = time_between_drops;
	}
	
	void Setup( PlayerBase player, Object target, ItemBase item )
	{
		m_TimeElpased = 0;
		if ( !m_SpentUnits )
		{ 
			m_SpentUnits = new Param1<float>(0);
		}
		else
		{	
			m_SpentUnits.param1 = 0;
		}
		m_DataLoaded = GetMiningData(player,target,item);
	}
	
	int Execute( PlayerBase player, Object target, ItemBase item )
	{
		if ( !player || !item || !target || !m_DataLoaded )
		{
			return UA_ERROR;
		}
		
		if ( item.GetDamage() == 1 )
		{
			return UA_FAILED;
		}
		else
		{
			if ( m_TimeElpased < m_TimeBetweenEachMaterialDrop )
			{
				m_TimeElpased += GetDeltaT();
			}
			else
			{
				ItemBase drop = GetGame().CreateObject(m_Material,player.GetPosition(), false);
				drop.SetQuantity2(m_AmountOfMaterialPerDrop,false);
				target.SetDamage( (target.GetDamage() + ( 1 / m_AmountOfDrops)) );
				if ( target.GetDamage() == 1 )
				{
					if ( m_SpentUnits )
					{
						m_SpentUnits.param1 = m_TimeElpased;
						SetACData(m_SpentUnits);	
					}
					return UA_FINISHED;
				}				
				item.SetDamage( (item.GetDamage() + m_DamageToMiningItemEachDrop) );
				m_TimeElpased = 0;
			}
			return UA_PROCESSING;
		}	
	}
	
	int Interupt( PlayerBase player, Object target, ItemBase item )
	{
		return UA_CANCEL;
		
	}

	//---------------------------------------------------------------------------
	
	bool GetMiningData(PlayerBase player, Object target, ItemBase item )
	{
		if ( target && target.IsWoodBase() )
		{
			WoodBase ntarget = target;
			m_AmountOfDrops = Math.Max(1,ntarget.GetAmountOfDrops(item));
			m_Material = ntarget.GetMaterial(item);
			m_AmountOfMaterialPerDrop = Math.Max(1,ntarget.GetAmountOfMaterialPerDrop(item));
			m_DamageToMiningItemEachDrop = ntarget.GetDamageToMiningItemEachDrop(item);
			return true;
		}
		return false;
	}
	
};