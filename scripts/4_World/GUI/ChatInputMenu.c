class ChatInputMenu extends UIScriptedMenu
{
	private EditBoxWidget m_edit_box;
	private TextWidget m_channel_text;
	private autoptr Timer m_close_timer;

	void ChatInputMenu()
	{
		m_close_timer = new Timer();
	}

	void ~ChatInputMenu()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).RemoveCalls(this);
	}
	
	Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_chat_input.layout");
		m_edit_box = layoutRoot.FindAnyWidget("InputEditBoxWidget");
		m_channel_text = layoutRoot.FindAnyWidget("ChannelText");
		
		UpdateChannel();
		return layoutRoot;
	}
	
	bool UseKeyboard() 
	{ 
		return true; 
	}
	
	bool OnChange(Widget w, int x, int y, bool finished)
	{
		super.OnChange(w, x, y, finished);
		
		if (!finished) return false;
		
		string text = m_edit_box.GetText();

		if (text != "")
		{
			GetGame().ChatPlayer(GetGame().ChatGetChannel(), text);
		}

		m_close_timer.Run(0.1, this, "Close");
		return true;
	}

	
	void Refresh()
	{	
	}
	
	void Update(float timeslice)
	{
		GetGame().GetInput().DisableKey(KeyCode.KC_RETURN);
	}
	
	void UpdateChannel()
	{
		m_channel_text.SetText(GetChannelName(GetGame().ChatGetChannel()));	
	}
	
	static string GetChannelName(ChatChannel channel)
	{
		switch(channel)
		{
			case CCNone:
				return "None";
			case CCGlobal:
				return "Global";
			case CCVehicle:
				return "Vehicle";
			case CCItemTransmitter:
				return "Radio";
			case CCPublicAddressSystem:
				return "PublicAddressSystem";
			case CCItemMegaphone:
				return "ItemMegaphone";   
			case CCDirect:
				return "Direct";   
			case CCCustom1:
				return "Custom1";   
			case CCCustom2:
				return "Custom2";   
			case CCCustom3:
				return "Custom3";   
			case CCCustom4:
				return "Custom4";   
			case CCCustom5:
				return "Custom5";   
			case CCCustom6:
				return "Custom6";   
			case CCCustom7:
				return "Custom7";   
			case CCCustom8:
				return "Custom8";   
			case CCCustom9:
				return "Custom9";   
			case CCCustom10:
				return "Custom10";   
			case CCCustomLast:
				return "CustomLast";   
			case CCStatus:
				return "Status";   
			case CCSystem:
				return "System";   
		}	
		
		return "";
	}
}
