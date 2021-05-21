class PlayerNameHandler extends ScriptedWidgetEventHandler
{
	static const string s_InvalidChars = " ~!@#$%^&*()+-=<>/?.:[]{}|\\0123456789abcdefghijlkmnoprqstuvwxyzABCDEFGHIJLKMNOPRQSTUVWXYZ";
	
	bool OnFocusLost(Widget w, int x, int y)
	{
		super.OnFocusLost(w, x, y);
		
		EditBoxWidget name_widget = (EditBoxWidget)w;

		string player_name = name_widget.GetText().Trim();

		g_Game.SetPlayerName(player_name);
		g_Game.GetPlayerName(player_name);

		name_widget.SetText(player_name);

		return false;
	}

	bool OnChange(Widget w, int x, int y, bool finished)
	{
		super.OnChange(w, x, y, finished);
		
		if (!finished) return false;
		
		EditBoxWidget name_widget = (EditBoxWidget)w;

		string player_name = name_widget.GetText().Trim();

		g_Game.SetPlayerName(player_name);
		g_Game.GetPlayerName(player_name);

		name_widget.SetText(player_name);

		return false;
	}

	bool OnKeyPress(Widget  w, int  x, int  y, int  key)
	{
		super.OnKeyPress(w, x, y, key);
		
		EditBoxWidget name_widget = (EditBoxWidget)w;
		
		if (name_widget.GetText().Length() > 14) return true; // player name is too long, block another key
		if (key > 128) return true; // no extended characters(diacritics) allowed
		if (s_InvalidChars.IndexOf(key.AsciiToString()) == -1) return true; // not allowed character -> blocked
		
		return false;
	}
}