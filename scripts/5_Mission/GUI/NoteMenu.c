class NoteMenu extends UIScriptedMenu
{
	protected MultilineEditBoxWidget m_edit;
	protected HtmlWidget m_html;
	protected autoptr Link<EntityAI> m_paper;
	protected autoptr Link<EntityAI> m_pen;

	void NoteMenu()
	{
	}

	void ~NoteMenu()
	{
	}

	void InitRead(string text)
	{
		if (m_edit)
		{
			m_edit.Show(false);
		}
		
		if (m_html)
		{
			m_html.Show(true);
			m_html.SetText("<html><body><p>" + text + "</p></body></html>");
		}
	}

	void InitWrite(EntityAI paper, EntityAI pen)
	{
		if (m_edit)
		{
			string text;
			m_edit.Show(true);
			m_edit.SetText("");
			
			delete m_paper;
			delete m_pen;
			m_paper = new Link<EntityAI>(paper);
			m_pen = new Link<EntityAI>(pen);
		}
		
		if (m_html)
		{
			m_html.Show(false);
		}
	}

	Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_inventory_note.layout");
		m_edit = layoutRoot.FindAnyWidget("EditWidget");
		m_html = layoutRoot.FindAnyWidget("HtmlWidget");
		
		return layoutRoot;
	}

	bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		
		switch (w.GetUserID())
		{
			case IDC_CANCEL:
				Close();
				return true;
			case IDC_OK:
				if (m_paper && m_paper.IsNull() == false && m_pen && m_pen.IsNull() == false)
				{
					autoptr WritePaperParams params = new WritePaperParams("", m_pen.Ptr());
					m_edit.GetText(params.param1);
					m_paper.Ptr().RPCSingleParam(RPC_WRITE_NOTE, params);
				}
				Close();
			return true;
		}

		return false;
	}
}
