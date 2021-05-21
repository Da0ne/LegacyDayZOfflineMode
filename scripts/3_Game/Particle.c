class Particle extends ParticleClient
{
	Object m_ParentObject;
	
	void ~Particle()
	{
		if ( g_Game )
		{
			Stop();
		}
	}
	
	/** \name Create a particle
		You can create a particle either at some position, or create it as an attachment on some object.
	*/
	
	/**
	\brief [server] Creates a particle emitter and attaches it on the given object
		\param source \p string File path to the .ptc file. List of particles can be found in Scripts\3_Game\ParticleList.c
		\param parent_obj \p Object Instance on which this particle will be attached
		\param local_pos \p vector Attachment position local to the parent (optional)
		\param local_ori \p vector Orientation local to the parent (Pitch, Yawn, Roll in degrees) (Optional)
		\return \p Particle Created particle instance
	*/
	static Particle Create( string source, Object parent_obj, vector local_pos = "0 0 0", vector local_ori = "0 0 0" )
	{
		vector global_pos = parent_obj.GetPosition();
		Particle new_particle = Create(source, global_pos);
		new_particle.AddAsChild(parent_obj, local_pos, local_ori);
		
		return new_particle;
	}
	
	/**
	\brief [server] Creates a particle emitter on the given position
		\param source \p string File path to the .ptc file. List of particles can be found in Scripts\3_Game\ParticleList.c
		\param global_pos \p Vector Position where the particel will be created
		\return \p Particle Created particle instance
	*/
	static Particle Create( string source, vector global_pos )
	{
		Particle p = GetGame().CreateObject("Particle", global_pos, false);
		p.SetSource(source);
		
		return p;
	}
	
	
	
	/** \name Play/Stop particle
		You can use the following Play(...) functions to (create and) activate a particle in 1 line of your script.
	*/
	
	
	/**
	\brief [server] Creates a particle emitter, attaches it on the given object and activates it
		\param source \p string File path to the .ptc file. List of particles can be found in Scripts\3_Game\ParticleList.c
		\param parent_obj \p Object Instance on which this particle will be attached
		\param local_pos \p vector Attachment position local to the parent (optional)
		\param local_ori \p vector Orientation local to the parent (Pitch, Yawn, Roll in degrees) (Optional)
		\return \p Particle Created particle instance
	*/
	static Particle Play( string source, Object parent_obj, vector local_pos = "0 0 0", vector local_ori = "0 0 0" )
	{
		Particle p = Create(source, parent_obj, local_pos, local_ori);
		p.Play();
		
		return p;
	}
	
	/**
	\brief [server] Creates a particle emitter on the given position and activates it
		\param source \p string File path to the .ptc file. List of particles can be found in Scripts\3_Game\ParticleList.c
		\param global_pos \p Vector Position where the particel will be created
		\return \p Particle Created particle instance
	*/
	static Particle Play( string source, vector global_pos)
	{
		Particle p = Create(source, global_pos);
		p.Play();
		
		return p;
	}
	
	//! [server] Plays the current particle. The optional parameter changes this particle for the new one.
	void Play(string source="")
	{
		if ( source != "" )
		{
			m_SyncSourcePath = source;
		}
		
		m_SyncIsPlaying = true;

		Synch();
	}
	
	//! [server] Stops this particle.
	void Stop() 
	{
		m_SyncIsPlaying = false;
		Synch();
	}
	
	/**
	\brief [server] Attaches this particle onto some object. If NULL value is provided then the particle will be detached from the current parent.
		\param parent \p Object Parent onto which this particle will be attached
		\param local_pos \p vector Attachment position local to the parent (optional)
		\param local_ori \p vector Orientation local to the parent (Pitch, Yawn, Roll in degrees) (Optional)
	*/
	void AddAsChild(Object parent, vector local_pos = "0 0 0", vector local_ori = "0 0 0")
	{
		if (parent)
		{
			SetPosition(local_pos);
			SetOrientation(local_ori);
			m_ParentObject = parent;
			parent.AddChild(this, -1); // Pivot does not work in DayZ so we use -1.
		}
		else
		{
			if (m_ParentObject)
			{
				m_ParentObject.RemoveChild(this);
				SetPosition( m_ParentObject.ModelToWorld( GetPosition() ) ); // Move the detached particle to its last position on the parent. Otherwise it will be around [0,0,0].
				m_ParentObject = NULL;
			}
		}
	}
	
	//! [server] [TO DO!] Deactivates this emmiter and deletes it from memory when all of its particles disappear
	void Destroy()
	{
		// TO DO! Currently not possible to do. We need programmers support for this.
	}
	
	//! [server] Sets particle file (.ptc)
	void SetSource(string source)
	{
		m_SyncSourcePath = source;
	}
	
	// Synchronization
	void Synch()
	{
		if ( GetGame().IsMultiplayer() && GetGame().IsServer() )
		{
			SetSynchDirty();
		}
		else
		{
			UpdateState();
		}
	}
	
	// Synchronization
	void OnSynchronize(ParamsWriteContext ctx)
	{
		super.OnSynchronize(ctx);

		autoptr Param1<string> source_path = new Param1<string>( m_SyncSourcePath );
		ctx.Write( source_path );
		
		autoptr Param1<bool> is_playing = new Param1<bool>( m_SyncIsPlaying );
		ctx.Write( is_playing );
	}
}