class ParticleClient extends ScriptedEntity
{
	protected string	m_SyncSourcePath;
	protected bool 		m_SyncIsPlaying;	
	
	private Object m_ParticleEffect;
	
	//====================================
	// Client: Constructor
	//====================================
	void ParticleClient()
	{
		SetFlags(EntityFlags.VISIBLE, true);
	}	
	
	//====================================
	// Client: UpdateState
	//====================================
	protected void UpdateState()
	{
		if ( m_SyncIsPlaying == false && m_ParticleEffect != NULL )
		{
			DestroyParticleEffect();
		}
		else if ( m_SyncIsPlaying == true && m_ParticleEffect == NULL )
		{
			CreateParticleEffect();
		}
	}
	
	//====================================
	// Client: CreateParticleEffect
	//====================================
	private void CreateParticleEffect()
	{
		m_ParticleEffect = GetGame().CreateObject("#particlesourceEnf", Vector(0,0,0), true);
		
		this.AddChild(m_ParticleEffect, -1);
		vobject vobj = GetObject(m_SyncSourcePath);
		m_ParticleEffect.SetObject(vobj, "");
		ReleaseObject(vobj);		
	}
	
	//====================================
	// Client: DestroyParticleEffect
	//====================================	
	private void DestroyParticleEffect()
	{
PrintString("StopOnClient - Stop");
		if ( m_ParticleEffect && GetGame() )
		{
PrintString("StopOnClient - Stop Execute");
			GetGame().ObjectDelete( m_ParticleEffect );
			m_ParticleEffect = NULL;
		}
	}
	
	// -------------------------------------------------------------------------
	void OnSynchronized(ParamsReadContext ctx)
	{
		super.OnSynchronized(ctx);

		autoptr Param1<string> source_path = new Param1<string>( "" );
		if ( ctx.Read(source_path) )
		{
			m_SyncSourcePath = source_path.param1;
		}
		
		autoptr Param1<bool> is_playing = new Param1<bool>( false );
		if ( ctx.Read(is_playing) )
		{
			m_SyncIsPlaying = is_playing.param1;
		}
		
		UpdateState();
	}
}