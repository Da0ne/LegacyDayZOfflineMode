class VPPFreeCam extends Camera
{
	//---------
	protected float CAM_SPEED = 5.0;
	protected float MAX_CAM_SPEED = 75.0;
	protected float CAM_BOOST = 10.0;
	protected float CAM_MOVE_DRAG = 0.7;
	protected float CAM_MOUSE_SENSE = 0.4;
	protected float CAM_SMOOTHNESS = 0.8;
	protected float CAM_FOV = 1.0;
	//---------
	
	protected vector localVelocity;
	protected float  localyawVelocity;
	protected float  localfovVelocity = 1.0;
	protected float  localpitchVelocity;
	protected float  local_camspeed;
	
	void VPPFreeCam()
	{
		SetEventMask( EntityEvent.FRAME );
	}
	
	void ~VPPFreeCam()
	{

	}
	
	override void EOnFrame( IEntity other, float timeSlice )
	{
		FreeFly(timeSlice);
	}
	
	void FreeFly(float timeSlice)
	{
		vector OldOrientation = GetOrientation();
		vector oldPos    = GetPosition();
		vector oldDir    = GetDirection();

		Input input = GetGame().GetInput();
		
		float forward  = input.GetActionDown(UAMoveForward, false) - input.GetActionDown(UAMoveBack, false);
		float strafe   = input.GetActionDown(UAMoveRight, false ) - input.GetActionDown(UAMoveLeft, false);
		float altitude = input.GetActionDown(UALeanLeft, false ) - input.GetActionDown(UALeanRight, false );
		float yawDiff = input.GetActionDown(UAAimLeft, false ) - input.GetActionDown(UAAimRight, false);
		float pitchDiff = input.GetActionDown(UAAimDown, false ) - input.GetActionDown(UAAimUp, false);
		float speedInc = 0.0;//input.LocalValue("UACamSpeedAdd") - input.LocalValue("UACamSpeedDeduct");
		bool increaseSpeeds = input.GetActionDown(UATurbo, false);

		//Keyboard Control
		local_camspeed = CAM_SPEED;
		if ( CAM_BOOST > 0 )
		{
			CAM_SPEED = CAM_SPEED + ( CAM_SPEED * speedInc / CAM_BOOST );
			
			if ( CAM_SPEED < 0.01 ) {
				CAM_SPEED = 0.01;
			}
			
			local_camspeed = CAM_SPEED;

			if( increaseSpeeds )
				local_camspeed = local_camspeed * CAM_BOOST;
		}
		
		if (local_camspeed >= MAX_CAM_SPEED && !increaseSpeeds)
			CAM_SPEED = MAX_CAM_SPEED;
		
		vector up = vector.Up;
		vector direction = GetDirection();
		vector directionAside = vector.Up * direction;
		
		up = up * altitude * local_camspeed;
		direction = direction * forward * local_camspeed;
		directionAside = directionAside * strafe * local_camspeed;

		localVelocity = localVelocity * CAM_MOVE_DRAG;
		localVelocity = localVelocity + (direction + directionAside + up) * timeSlice;

		vector newPos = oldPos + localVelocity;

		float surfaceY = GetGame().SurfaceY( newPos[0], newPos[2] ) + 0.15; //0.25
		if ( newPos[1] < surfaceY )
			newPos[1] = surfaceY;

		//FOV
		/*if (input.LocalValue("UACamFOV") > 0)
		{
			CAM_MOUSE_SENSE = 0.01;
			float camFov = input.LocalValue("UACamSpeedAdd") - input.LocalValue("UACamSpeedDeduct");
			float fovVelocity =  (fovVelocity + camFov * 0.01);

			CAM_FOV += fovVelocity;
			if (CAM_FOV > 8.0)
				CAM_FOV = 0.005;
			else if (CAM_FOV < 0.005)
				CAM_FOV = 1.0;

			SetFOV(CAM_FOV);
		}else{
			CAM_MOUSE_SENSE = 0.4;
		}*/

		//Mouse Control
		localyawVelocity =  (localyawVelocity + yawDiff * CAM_MOUSE_SENSE);
		localpitchVelocity = (localpitchVelocity + pitchDiff * CAM_MOUSE_SENSE);

		vector OrientNew = OldOrientation;
		OrientNew[0] = OldOrientation[0] - Math.RAD2DEG * localyawVelocity * timeSlice;
		OrientNew[1] = OldOrientation[1] - Math.RAD2DEG * localpitchVelocity * timeSlice;

		localyawVelocity   *= CAM_SMOOTHNESS;
		localpitchVelocity *= CAM_SMOOTHNESS;

		if( OrientNew[1] < -89 )
			OrientNew[1] = -89;
			
		if( OrientNew[1] > 89 )
			OrientNew[1] = 89;
			
		//Update Position & rotation
		SetPosition(newPos);
		SetOrientation(OrientNew);
	}
	
	void SnapToAngle(float angle)
	{
		vector OldOrientation = GetOrientation();
		SetOrientation(Vector(OldOrientation[0],angle,OldOrientation[2]));
	}
	
	void SetYaw(float yaw)
	{
		vector OldOrientation = GetOrientation();
		SetOrientation(Vector(yaw,OldOrientation[1],OldOrientation[2]));
	}
	
	void SetPitch(float pitch)
	{
		vector OldOrientation = GetOrientation();
		SetOrientation(Vector(OldOrientation[0],pitch,OldOrientation[2]));
	}
	
	void SetRoll(float roll)
	{
		vector OldOrientation = GetOrientation();
		SetOrientation(Vector(OldOrientation[0],OldOrientation[1],roll));
	}
	
	void SetFov(float fov)
	{
		CAM_FOV = fov;
		SetFOV(CAM_FOV);
	}
	
	float GetFov()
	{
		return CAM_FOV;
	}
};