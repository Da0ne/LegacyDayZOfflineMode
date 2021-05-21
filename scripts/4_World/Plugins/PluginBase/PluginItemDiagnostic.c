class PluginItemDiagnostic extends PluginBase
{
	autoptr Timer 						myTimer1;
	autoptr map<PlayerBase,ItemBase> 	m_ObserversMap = new map<PlayerBase,ItemBase>;
	autoptr array<string> 				m_Agents = new array<string>;
	autoptr map<string,float> 			m_Floats = new map<string,float>;
	autoptr map<string,string> 			m_Strings = new map<string,string>;
	autoptr map<string,float> 			m_VarsNumbersFinalServer = new map<string,float>;
	autoptr map<string,float> 			m_VarsFinalClient = new map<string,float>;
	array<Param> 						m_Properties = new array<Param>;
	bool 								m_IsActive	= false;
	string								m_NoteClient;
	
	Widget 				m_DebugRootWidget;
	TextListboxWidget 	m_DebugAgentListWidget;
	TextListboxWidget 	m_DebugFloatsListWidget;
	TextListboxWidget 	m_DebugStringsListWidget;
	TextListboxWidget 	m_DebugFloatsProperListWidget;
	TextListboxWidget 	m_DebugClientVarsWidget;
	TextWidget 			m_ClassNameWidget;
	ItemPreviewWidget 	m_ItemPreviewWidget;


	void PluginItemDiagnostic()
	{
		#ifndef NO_GUI
		InitializeWidgets();
		ShowWidgets(false);
		#endif
	}
	
	
	void ~PluginItemDiagnostic()
	{
		ClearProperties();
		delete m_Properties;
	}

	void RegisterDebugItem(ItemBase item, PlayerBase player)
	{
		if(!myTimer1)
		{
			myTimer1 = new Timer();
			myTimer1.Run(1, this, "Tick", NULL, true);
		}
		
		if(m_ObserversMap.Contains(player))
		{
			m_ObserversMap.Set(player,item);
		}
		else
		{
			m_ObserversMap.Insert(player,item);
		}
	}
	
	
	bool IsActive()
	{
		return m_IsActive;
	}

	void ShowWidgets(bool show)
	{
		if(m_DebugRootWidget) m_DebugRootWidget.Show(show);
	}

	void ToggleDebugWindowEvent()
	{
		if(m_IsActive)
		{		
			PlayerBase player = GetGame().GetPlayer();
			//GetGame().RPCSingleParam(player, RPC_ITEM_DIAG_CLOSE,NULL);
			ShowWidgets(false);
			ClearWidgets();
			m_IsActive = false;
		}
		else
		{
			ShowWidgets(true);
			m_IsActive = true;
		}

	}

	void ClearWidgets()
	{
		m_DebugAgentListWidget.ClearItems();
		m_DebugFloatsListWidget.ClearItems();
		m_DebugStringsListWidget.ClearItems();
		m_DebugFloatsProperListWidget.ClearItems();
		m_ItemPreviewWidget.SetItem(NULL);
		m_ClassNameWidget.SetText("");		
	}
	
	void Tick()
	{
		for(int i = 0; i < m_ObserversMap.Count();i++)
		{
			ItemBase item = m_ObserversMap.GetElement(i);
			PlayerBase player = m_ObserversMap.GetKey(i);

			if (item && player) 
			{
				GeneratePropertiesObject(item);
				SendRPC(item, player);
			}
			else
			{
				Debug.LogError("PluginItemDiagnostic: dead debug record, removing record");
				m_ObserversMap.RemoveElement(i);
			}
		}
		
	}
	
	void ClearProperties()
	{
		for(int i = 0; i < m_Properties.Count(); i++)
		{
			delete m_Properties.Get(i);
		}
		m_Properties.Clear();
	}


	void GeneratePropertiesObject(ItemBase item)
	{
		ClearProperties();
		GetLocalProperties(item, m_Properties);
		int count = m_Properties.Count();
		Debug.Log("items saved = "+count.ToString(),"fuckup");
	}
	
	void SendRPC(ItemBase item, PlayerBase player)
	{
		GetGame().RPC(item,RPC_ITEM_DIAG,m_Properties,player);
	}
	
	void StopWatchRequest(PlayerBase player)//called from player after an RPC call
	{
		if( m_ObserversMap.Contains(player) )
		{
			m_ObserversMap.Remove(player);
		}
	}

	void OnRPC(ItemBase item, ParamsReadContext ctx)
	{

		ctx.Read(CashedObjectsParams.PARAM1_INT);
		int agents_count 	= CashedObjectsParams.PARAM1_INT.param1;

		ctx.Read(CashedObjectsParams.PARAM1_INT);
		int string_count 	= CashedObjectsParams.PARAM1_INT.param1;

		ctx.Read(CashedObjectsParams.PARAM1_INT);
		int individual_items_count 	= CashedObjectsParams.PARAM1_INT.param1;
		
		FillAgentArray(ctx, agents_count);
		FillStringVars(ctx, string_count);
		
		FillServerFinalVars(individual_items_count, ctx);
		
		//Debug.Log("items read = "+ToString(items_read),"fuckup");
		FillClientVarsMap(item);
		FillClientString(item);
		DisplayAll(item);
	}
	
	void FillServerFinalVars(int count, ParamsReadContext ctx)
	{
		m_VarsNumbersFinalServer.Clear();
		
		for(int i = 0; i < count; i++)
		{
			ctx.Read(CashedObjectsParams.PARAM2_STRING_FLOAT);
			string key = CashedObjectsParams.PARAM2_STRING_FLOAT.param1;
			float value = CashedObjectsParams.PARAM2_STRING_FLOAT.param2;
			m_VarsNumbersFinalServer.Insert(key, value);
		}
			
	}
	
	void InitializeWidgets()
	{
		if(!m_DebugRootWidget) m_DebugRootWidget = GetGame().GetWorkspace().CreateWidgets("gui/layouts/debug_item.layout");
		m_DebugAgentListWidget = m_DebugRootWidget.FindAnyWidget("w_Agents");
		m_DebugFloatsListWidget = m_DebugRootWidget.FindAnyWidget("w_Floats");
		m_DebugStringsListWidget = m_DebugRootWidget.FindAnyWidget("w_Strings");
		m_DebugFloatsProperListWidget = m_DebugRootWidget.FindAnyWidget("w_FloatsProper");
		m_ItemPreviewWidget = m_DebugRootWidget.FindAnyWidget("w_ItemPreview");
		m_ClassNameWidget = m_DebugRootWidget.FindAnyWidget("w_ClassName");
	}

	void DisplayAll(ItemBase item)
	{
		m_ClassNameWidget.SetText( item.GetType() );
		m_ItemPreviewWidget.SetItem(item);
		UpdateAgentWidget();
		UpdateFloatWidget();
		UpdateStringWidget();
		UpdateVarsProperWidget();
	}	
	
	void FillClientVarsMap(ItemBase item)//this is done client side
	{
		m_VarsFinalClient.Clear();
		
		float quantity = item.GetQuantity2();
		float liquid_type = item.GetLiquidType2();
		float wetness = item.GetWet2();
		float temperature = item.GetTemperature2();

		m_VarsFinalClient.Insert("quantity",quantity);
		m_VarsFinalClient.Insert("liquid_type",liquid_type);
		m_VarsFinalClient.Insert("wetness",wetness);
		m_VarsFinalClient.Insert("temperature",temperature);
		
	}
	
	void FillClientString(ItemBase item)
	{
		m_NoteClient = item.GetItemVariableString("varNote");
	}
	
	void FillAgentArray(ParamsReadContext ctx, int agents_count)
	{
		m_Agents.Clear();
		for(int i = 0; i < agents_count; i++)
		{
			ctx.Read(CashedObjectsParams.PARAM1_STRING);
			string agent = CashedObjectsParams.PARAM1_STRING.param1;
			m_Agents.Insert(agent);
		}
	}

	void FillFloatVars(ParamsReadContext ctx, int item_count)
	{
		m_Floats.Clear();
		for(int i = 0; i < item_count; i++)
		{
			ctx.Read(CashedObjectsParams.PARAM2_STRING_FLOAT);
			string key = CashedObjectsParams.PARAM2_STRING_FLOAT.param1;
			float value = CashedObjectsParams.PARAM2_STRING_FLOAT.param2;
			m_Floats.Insert(key,value);
		}
	}

	void FillStringVars(ParamsReadContext ctx, int item_count)
	{
		m_Strings.Clear();
		for(int i = 0; i < item_count; i++)
		{
			ctx.Read(CashedObjectsParams.PARAM2_STRING_STRING);
			string key = CashedObjectsParams.PARAM2_STRING_STRING.param1;
			string value = CashedObjectsParams.PARAM2_STRING_STRING.param2;
			m_Strings.Insert(key,value);
		}
	}

	void UpdateAgentWidget()
	{
		m_DebugAgentListWidget.ClearItems();
		for(int i = 0; i < m_Agents.Count(); i++)
		{
			string agent = m_Agents.Get(i);
			m_DebugAgentListWidget.AddItem(agent,NULL,0);
		}
	}	
	
	void UpdateFloatWidget()
	{
		m_DebugFloatsListWidget.ClearItems();
		for(int i = 0; i < m_Floats.Count(); i++)
		{
			string key = m_Floats.GetKey(i);
			float value = m_Floats.GetElement(i);
			m_DebugFloatsListWidget.AddItem(key,NULL,0);
			m_DebugFloatsListWidget.SetItem(i,value.ToString(),NULL,1);
			
		}
	}
	
	void UpdateStringWidget()
	{
		m_DebugStringsListWidget.ClearItems();
		for(int i = 0; i < m_Strings.Count(); i++)
		{
			string key = m_Strings.GetKey(i);
			string value = m_Strings.GetElement(i);
			m_DebugStringsListWidget.AddItem(key,NULL,0);
			m_DebugStringsListWidget.SetItem(i,value,NULL,1);
		}
	}
	
	void UpdateVarsProperWidget()
	{
		m_DebugFloatsProperListWidget.ClearItems();
		int i = 0;
		for(; i < m_VarsNumbersFinalServer.Count(); i++)
		{
			string key = m_VarsNumbersFinalServer.GetKey(i);
			string value = m_VarsNumbersFinalServer.GetElement(i).ToString();
			
			string value_client;
			
			if( m_VarsFinalClient.Contains(key) )
			{
				value_client = m_VarsFinalClient.Get(key).ToString();
			}
			else
			{
				value_client = "NOT SYNCED";
			}
			
			m_DebugFloatsProperListWidget.AddItem(key,NULL,0);
			m_DebugFloatsProperListWidget.SetItem(i,value,NULL,1);
			m_DebugFloatsProperListWidget.SetItem(i,value_client,NULL,2);
		}
		m_DebugFloatsProperListWidget.AddItem("varNote",NULL,0);
		m_DebugFloatsProperListWidget.SetItem(i,"N/A",NULL,1);
		m_DebugFloatsProperListWidget.SetItem(i,m_NoteClient,NULL,2);
	}	
	
	void PrintOut()
	{
		Debug.LogArrayString(m_Agents,"ItemDebug");
		Debug.Log("----------------------","ItemDebug");
	}
}
	//-------------------------
	// QUERY FOR DEBUG PROPERTIES
	//-------------------------
	void GetLocalProperties(ItemBase item, array<Param> properties)
	{
		// -------- AGENTS ------------
		int num_of_agents = FillWithAgents(item, properties);
		Param1<int> agents_count = new Param1<int>(num_of_agents);
		properties.InsertAt(agents_count,0);
		// -------- STRINGS ------------
		int num_of_strings = FillWithStrings(item, properties);
		Param1<int> string_count = new Param1<int>(num_of_strings);
		properties.InsertAt(string_count,1);
		// ------ INDIVIDUAL VARS------
		int number_of_items = 0;
		Param2<string, float> quantity = new Param2<string, float>("quantity", item.GetQuantity2() );
		properties.Insert(quantity);
		number_of_items++;
		//-----------------------------
		Param2<string, float> liquid_type = new Param2<string, float>("liquid_type", item.GetLiquidType2() );
		properties.Insert(liquid_type);
		number_of_items++;
		//-----------------------------
		Param2<string, float> wetness = new Param2<string, float>("wetness", item.GetWet2() );
		properties.Insert(wetness);
		number_of_items++;
		//-----------------------------
		Param2<string, float> temperature = new Param2<string, float>("temperature", item.GetTemperature2() );
		properties.Insert(temperature);
		number_of_items++;
		//-----------------------------
		Param2<string, float> energy = new Param2<string, float>("energy", item.GetEnergy() );
		properties.Insert(energy);
		number_of_items++;
		//-----------------------------
		Param2<string, float> health = new Param2<string, float>("health", item.GetHealth("", "") );
		properties.Insert(health);
		number_of_items++;
		
		Param1<int> item_count = new Param1<int>(number_of_items);
		properties.InsertAt(item_count,2);
		
	}
	
	int FillWithStrings(ItemBase item, array<Param> properties)
	{
		int num_of_strings = 0;
		
		map<string,string> string_map = item.GetVariablesString();
		
		if(string_map)
		{
			num_of_strings = string_map.Count();
			
			for(int i = 0; i < num_of_strings; i++)
			{
				string value = string_map.GetElement(i);
				string key = string_map.GetKey(i);
				
				Param2<string,string> param2 = new Param2<string,string>(key,value);

				properties.Insert(param2);
			}
		
		}
		return num_of_strings;
		
	}
	
	int FillWithAgents(ItemBase item, array<Param> properties)
	{
		int agents = item.GetAgents();
		if(agents == 0) return 0;
		int num_of_agents = 0;
		autoptr array<int> agents_aray = new array<int>;
		PluginTransmissionAgents.BuildAgentArray(agents, agents_aray);
		int agents_count = agents_aray.Count();
	
		for(int i = 0; i < agents_count; i++)
		{
			PluginTransmissionAgents mta = GetPlugin(PluginTransmissionAgents);
			string agent_name = mta.GetNameByID(agents_aray.Get(i));
			Param1<string> param1 = new Param1<string>(agent_name);
			properties.Insert(param1);
			num_of_agents++;
		}

		return num_of_agents;
	}

