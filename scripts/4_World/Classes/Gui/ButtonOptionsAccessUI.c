class ButtonOptionsAccessUI extends OptionsAccessUI
{
	void Update()
	{
		SwitchOptionsAccess switchOption = (SwitchOptionsAccess)m_optionAccess;
		ButtonWidget btn = (ButtonWidget)m_widget;

		if (switchOption && btn)
		{
			string text;
			switchOption.GetItemText(text);
			btn.SetText(text);
		}
	}

	void SetValue()
	{
		SwitchOptionsAccess switchOption = (SwitchOptionsAccess)m_optionAccess;

		if (switchOption)
		{
			switchOption.Switch();
			Update();
		}
	}

	bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		
		SetValue();
		return false;
	}
}
