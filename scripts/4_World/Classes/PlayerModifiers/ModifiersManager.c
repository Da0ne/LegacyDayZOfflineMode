const int DEFAULT_TICK_TIME_ACTIVE = 3000;
const int DEFAULT_TICK_TIME_INACTIVE = 3000;

class ModifiersManager
{
	PlayerBase m_Player;
	autoptr map<int, ModifierBase> m_ModifierList;
	
	void ModifiersManager(PlayerBase player)
	{
		m_ModifierList = new map<int, ModifierBase>;
		m_Player = player;
		Init();
	}

	void ~ModifiersManager()
	{
		int c = m_ModifierList.Count();
		for (int i = 0; i < c; i++)
		{
			delete m_ModifierList.GetElement(i);
		}
		
		m_ModifierList.Clear();
	}

	void Init()
	{
		AddModifier(new Testing);
		AddModifier(new BloodRegen);
		AddModifier(new BoneRegen);
		AddModifier(new Health);
		AddModifier(new HealthRegen);
		AddModifier(new Hunger);
		AddModifier(new Shock);
		AddModifier(new ImmuneSystem);
		AddModifier(new Stomach);
		AddModifier(new Temperature);
		AddModifier(new Thirst);
		AddModifier(new Bleeding);
		//AddModifier(new Blinded);
		AddModifier(new BrokenArms);
		AddModifier(new BrokenLegs);
		AddModifier(new Burning);
		AddModifier(new Fever);
		AddModifier(new HeartAttack);
		AddModifier(new HemolyticReaction);
		AddModifier(new Hyperthermia);
		AddModifier(new Hypothermia);
		AddModifier(new Poisoning);
		AddModifier(new StuffedStomach);
		AddModifier(new Tremor);
		AddModifier(new CommonCold);
		AddModifier(new Cholera);
		AddModifier(new Influenza);
		AddModifier(new TestDisease);
	}

	void AddModifier(ModifierBase modifier)
	{
		modifier.InitBase(m_Player,this);
		int id = modifier.GetModifierID();
		//TODO: add a check for duplicity
		m_ModifierList.Insert(id, modifier);
	}
	
	void OnScheduledTick(float delta_time)
	{
		for(int i = 0; i < m_ModifierList.Count(); i++)
		{
			m_ModifierList.GetElement(i).Tick(delta_time);
		}

	}

	void ActivateModifier(int modifier_id, bool triggerEvent = true)
	{
		m_ModifierList.Get(modifier_id).Activate(triggerEvent);
	}

	void DeactivateModifier(int modifier_id, bool triggerEvent = true)
	{
		m_ModifierList.Get(modifier_id).Deactivate(triggerEvent);
	}
	
	void OnStoreSave( ParamsWriteContext ctx )
	{
		CashedObjectsArrays.ARRAY_INT.Clear();
		
		int modifier_count;
		for(int x = 0; x < m_ModifierList.Count(); x++)
		{
			ModifierBase mdfr = m_ModifierList.GetElement(x);
			if( mdfr.IsActive() ) 
			{
				modifier_count++;
				//save the modifier id
				CashedObjectsArrays.ARRAY_INT.Insert( mdfr.GetModifierID() );
				if( mdfr.IsTrackAttachedTime() )
				{
					//save the overall attached time
					CashedObjectsArrays.ARRAY_INT.Insert( mdfr.GetAttachedTime() );
				}
			}
		}

		//write the count		
		CashedObjectsParams.PARAM1_INT.param1 = modifier_count;
		ctx.Write(CashedObjectsParams.PARAM1_INT);

		//write the individual modifiers and respective attached times
		for(int i = 0; i < CashedObjectsArrays.ARRAY_INT.Count(); i++)
		{
			int item = CashedObjectsArrays.ARRAY_INT.Get(i);
			CashedObjectsParams.PARAM1_INT.param1 = item;
			ctx.Write(CashedObjectsParams.PARAM1_INT);
		}
	}
	
	void OnStoreLoad( ParamsReadContext ctx )
	{
		ctx.Read(CashedObjectsParams.PARAM1_INT);
		int num_of_mods = CashedObjectsParams.PARAM1_INT.param1;
		
		for(int i = 0; i < num_of_mods; i++)
		{
			ctx.Read(CashedObjectsParams.PARAM1_INT);
			int modifier_id = CashedObjectsParams.PARAM1_INT.param1;
			ModifierBase modifier = GetModifier(modifier_id);
			
			if( modifier.IsTrackAttachedTime() )
			{
				ctx.Read(CashedObjectsParams.PARAM1_INT);//get the attached time
				int time = CashedObjectsParams.PARAM1_INT.param1;
				modifier.SetAttachedTime( time );
			}
			
			ActivateModifier(modifier_id, false);
		}
	}

	ModifierBase GetModifier(int modifier_id)
	{
		return m_ModifierList.Get(modifier_id);
	}
		
	PlayerBase GetPlayer()
	{
		return m_Player;
	}
	
	void DbgGetActiveModifiers(array<string> modifiers)
	{
		for(int i = 0;i < m_ModifierList.Count(); i++)
		{
			ModifierBase modifier = m_ModifierList.GetElement(i);
			if( modifier.IsActive() ) modifiers.Insert( modifier.GetName() );
		}
	}

	void DbgGetInactiveModifiers(array<string> modifiers)
	{
		for(int i = 0;i < m_ModifierList.Count(); i++)
		{
			ModifierBase modifier = m_ModifierList.GetElement(i);
			if( !modifier.IsActive() ) 
			{
				modifiers.Insert( modifier.GetName() );
			}
		}
	}
}
