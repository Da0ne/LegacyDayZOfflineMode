class SliderOptionsAccessUI extends OptionsAccessUI
{
	void Update()
	{
		NumericOptionsAccess numeric = (NumericOptionsAccess)m_optionAccess;
		SliderWidget slider = (SliderWidget)m_widget;

		if (numeric && slider)
		{
			float min = numeric.GetMin();
			float max = numeric.GetMax();
			float value = numeric.ReadValue();

			float val = value - min;
			val = val / (max - min) * 100.0;
			slider.SetCurrent(val);

			if ( slider.GetName() == "FOVSliderWidget" )
			{
				TextWidget text_value_widget = (TextWidget)slider.FindWidget("FOVSliderLabel");
				float converted_value = Math.RAD2DEG * ( OPTIONS_FIELD_OF_VIEW_MIN + ( ( val / 100 ) * ( OPTIONS_FIELD_OF_VIEW_MAX - OPTIONS_FIELD_OF_VIEW_MIN ) ) );
				text_value_widget.SetText( converted_value.ToString() );
			}
		}
	}

	void SetValue()
	{
		NumericOptionsAccess numeric = (NumericOptionsAccess)m_optionAccess;
		SliderWidget slider = (SliderWidget)m_widget;

		if (numeric && slider)
		{
			float min = numeric.GetMin();
			float max = numeric.GetMax();
			float value = slider.GetCurrent() / 100.0;

			numeric.WriteValue(min + (value * (max - min)));
			Update();
		}
	}

	bool OnChange(Widget w, int x, int y, bool finished)
	{
		super.OnChange(w, x, y, finished);
		
		SetValue();
		return true;
	}
}
