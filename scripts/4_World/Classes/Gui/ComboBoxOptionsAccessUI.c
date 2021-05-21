class ComboBoxOptionsAccessUI extends OptionsAccessUI
{
	void Init()
	{
		ListOptionsAccess list = (ListOptionsAccess)m_optionAccess;
		XComboBoxWidget combo = (XComboBoxWidget)m_widget;

		if (list && combo)
		{
			combo.ClearAll();

			string text;
			int c = list.GetItemsCount();
			for (int i = 0; i < c; i++)
			{
				list.GetItemText(i, text);
				combo.AddItem(text);
			}
		}
	}

	void Update()
	{
		ListOptionsAccess list = (ListOptionsAccess)m_optionAccess;
		XComboBoxWidget combo = (XComboBoxWidget)m_widget;

		if (list && combo)
		{
			combo.SetCurrentItem(list.GetIndex());
		}
	}

	void SetValue()
	{
		ListOptionsAccess list = (ListOptionsAccess)m_optionAccess;
		XComboBoxWidget combo = (XComboBoxWidget)m_widget;

		if (list && combo)
		{
			list.SetIndex(combo.GetCurrentItem());
			Update();
		}
	}

	bool OnChange(Widget w, int x, int y, bool finished)
	{
		super.OnChange(w, x, y, finished);
		
		SetValue();
		return false;
	}
}
