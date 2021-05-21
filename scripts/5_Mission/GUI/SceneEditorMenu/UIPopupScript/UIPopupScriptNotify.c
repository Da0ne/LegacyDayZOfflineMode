class UIPopupScriptNotify extends UIPopupScript
{
	private ButtonWidget	m_BtnOk;
	private TextWidget		m_TxtLabel;
	
	//================================================
	// UIPopupScriptNotify
	//================================================	
	void UIPopupScriptNotify(Widget wgt)
	{
		m_BtnOk		= wgt.FindAnyWidget("btn_ppp_notify_ok");
		m_TxtLabel	= wgt.FindAnyWidget("txt_ppp_notify_label");
	}

	//================================================
	// OnClick
	//================================================	
	bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		
		if ( w == m_BtnOk )
		{
			PopupBack();
			
			return true;
		}
		
		return false;
	}

	//================================================
	// OnClick
	//================================================	
	void SetLabelText(string text)
	{
		m_TxtLabel.SetText(text);
	}
}
