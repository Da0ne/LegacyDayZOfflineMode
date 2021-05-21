// -----------------------------------------------------------
class HorizontalSpacer : SpacerBase
{
	reference int border;
	reference int gap;
	
	protected void UpdateChild(Widget child, float w, float h, int index)
	{
		float itemWidth = (w - (border * 2) - ((m_count - 1) * gap)) / m_count;
		float itemHeight = h - (2 * border);
	
		child.SetPos(border + ((itemWidth + gap) * index), border);
		child.SetSize(itemWidth, itemHeight);
	}
};