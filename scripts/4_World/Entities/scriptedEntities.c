#ifdef DEVELOPER
const bool DebugTrigger = true;
#else
const bool DebugTrigger = false;
#endif

class TriggerInsider
{
	autoptr OLinkT insider;
	int timeStamp;
	void TriggerInsider(Object obj)
	{
		insider = new OLinkT(obj);
	}
};

class Trigger extends ScriptedEntity
{
	const int TIMEOUT = 1000;
	autoptr array<TriggerInsider> m_insiders;
	Shape shape;

	protected void Cleanup()
	{
		for(int n = 0; n < m_insiders.Count(); n++)
		{
			TriggerInsider ins = m_insiders.Get(n);
			delete ins;
		}
		
		if(shape)
		{
			shape.Destroy();
			shape = NULL;
		}			
	}

	void OnEnter(Object obj)
	{
		autoptr Param1<Object> param = new Param1<Object>(obj);
		OnEvent(EEOnEnter, param);
		if(DebugTrigger)
		{		
			Print("entered");
			Print(obj);
		}
	}

	void OnLeave(Object obj)
	{
		autoptr Param1<Object> param = new Param1<Object>(obj);
		OnEvent(EEOnLeave, param);
		if(DebugTrigger)
		{		
			Print("left");
			Print(obj);
		}
	}
	
	protected void UpdateInsiders()
	{
		int timeStamp = g_Game.GetTime();

		for(int n = 0; n < m_insiders.Count(); )
		{
			TriggerInsider ins = m_insiders.Get(n);
			if(!ins.insider.Ptr())
			{
				//object has been deleted. Remove it
				m_insiders.Remove(n);
				delete ins;
				continue;
			}
			
			int timeDiff = timeStamp - ins.timeStamp;
			if(timeDiff > TIMEOUT)
			{
				//object left. Remove it
				m_insiders.Remove(n);
				OnLeave(ins.insider.Ptr());
				delete ins;
				continue;
			}
			 
			n++;			
		}			
	}

	protected void AddInsider(Object obj)
	{
		TriggerInsider ins;
		for(int n = 0; n < m_insiders.Count(); n++)
		{
			ins = m_insiders.Get(n);
			//already in?
			if(ins.insider.Ptr() == obj)
			{
				//just update timestamp
				ins.timeStamp = g_Game.GetTime();
				return;
			}
		}
		ins = new TriggerInsider(obj);
		ins.timeStamp = g_Game.GetTime();
		m_insiders.Insert(ins);
		OnEnter(obj);  				
	}
	
	void EOnFrame(IEntity other, float timeSlice)
	{
		UpdateInsiders();
	}
	
	void EOnTouch(IEntity other, int extra)
	{
		AddInsider(other);			
	}

	private float GetRadius(vector min, vector max)
	{
		vector v;
		for(int n = 0; n < 3; n++)
		{
			float m1 = Math.AbsFloat(min[n]);
			float m2 = Math.AbsFloat(max[n]);

			if(m1 > m2)
				v[n] = m1;
			else
				v[n] = m2;			
		}
		
		return v.Length();			
	}
	
	void SetExtents(vector mins, vector maxs)
	{
		SetCollisionBox(mins, maxs);

		if(DebugTrigger)
		{
			vector mat[4];
			GetTransform(mat);
			if(shape)
			{
				shape.Destroy();
				shape = NULL;
			}
			
			shape = Shape.Create(ShapeType.BBOX, 0x1fff7f7f, ShapeFlags.TRANSP|ShapeFlags.NOZWRITE, mins, maxs);
			
			if (shape) // Hackfix! Function AddDShape must return Shape! That's why this condition is probably never passed!
			{
				shape.SetMatrix(mat);
			}
		}	
	}


	void EOnInit(IEntity other, int extra)
	{
		const vector mins = "-2 -4 -2";
		const vector maxs = "2 4 2";
		SetExtents(mins, maxs);				
	}

	private void ~Trigger()
	{
		Cleanup();
	}
			
	private void Trigger()
	{
		m_insiders = new array<TriggerInsider>;
		SetEventMask(EntityEvent.FRAME|EntityEvent.INIT|EntityEvent.TOUCH);
		SetFlags(EntityFlags.TRIGGER, false);		
	}
};

class V3S_Cargo extends Car
{

/*
	void EOnInit(IEntity other, int extra)
	{
		vector pos = GetOrigin()
		pos[1] = g_Game.SurfaceY(pos[0], pos[2]);
		vector off = GetTransformAxis(0);
		Trigger obj = g_Game.CreateObject( "Trigger", off * 5 + pos);
		obj.SetExtents("-4 -2 -4", "4 8 4");
	}

	void EOnFrame(IEntity other, float timeSlice) //EntityEvent.FRAME
	{
	}
		
	void V3S_Cargo()
	{

		//debug
//		Print("vytvarim V3S_Cargo");
		SetEventMask(EntityEvent.FRAME|EntityEvent.INIT);
		
	}
	*/
};


