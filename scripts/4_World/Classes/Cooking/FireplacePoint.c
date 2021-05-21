class FireplacePoint
{
	string m_ActionSelection;		//selection name in p3d which will trigger action (place)
	string m_FirePoint;				//memory point name in p3d where the fireplace will be placed
	string m_SmokePoint;			//memory point name in p3d where smoke particles will be emitted (e.g. chimney)
	ItemBase m_Object;				//reference of placed object into fire point, if NULL fire point is available
	
	void FireplacePoint( string action_selection, string fire_point, string smoke_point, ItemBase object )
	{
		m_ActionSelection = action_selection;
		m_FirePoint = fire_point;
		m_SmokePoint = smoke_point;
		m_Object = object;
	}
	
	//action selection
	string GetActionSelection()
	{
		return m_ActionSelection;
	}
	
	//fire point
	string GetFirePoint()
	{
		return m_FirePoint;
	}
	
	//smoke point
	string GetSmokePoint()
	{
		return m_SmokePoint;
	}
	
	//object
	ItemBase GetObject()
	{
		return m_Object;
	}
	
	void SetObject( ItemBase object )
	{
		m_Object = object;
	}
	
	void ClearObject()
	{
		m_Object = NULL;
	}
}