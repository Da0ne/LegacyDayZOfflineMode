Mission CreateMission(string path)
{
	SetDispatcher(new DispatcherCaller);
	g_Game.SetMissionPath(path);
	Print("CreateMission() " + path);
	g_Game.m_OfflineMissions += 1;
	Print(g_Game.m_OfflineMissions);
	return new MissionGameplay;
}

void DestroyMission(Mission mission)
{
	delete mission;
}