class Experience
{
	autoptr map<string, ExperienceLevel> m_Levels;

	void Experience()
	{
		m_Levels = new map<string, ExperienceLevel>;
	}

	void AddLevel(string level_name, ExperienceLevel exp_class)
	{
		m_Levels.Set(level_name, exp_class);
	}
}