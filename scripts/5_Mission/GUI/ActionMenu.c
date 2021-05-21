class ActionMenu
{
	private const float FADE_IN_TIME = 0.3;
	private const float FADE_OUT_TIME = 0.3;
	private const float HIDE_MENU_TIME = 5;

	private autoptr WidgetFadeTimer m_fade_timer;
	private autoptr TActionRefArray m_actions;
	private autoptr array<TextWidget> m_actionItems;
	private TextWidget m_defaultActionWidget;
	private Widget m_actionsPanelWidget;
	private autoptr Timer m_hide_timer;
	private int m_selectedActionIndex;
	private bool m_enabled;

	void ActionMenu()
	{
		m_actionItems = new array<TextWidget>;
		m_actions = new TActionRefArray;
		m_fade_timer = new WidgetFadeTimer;
		m_hide_timer = new Timer(CALL_CATEGORY_GUI);
		m_selectedActionIndex = -1;
		m_enabled = true;
	}

	void Init(Widget actions_panel_widget, TextWidget default_action_widget)
	{
		m_defaultActionWidget = default_action_widget;
		m_actionsPanelWidget = actions_panel_widget;
		m_actionItems.Clear();
		int i = 0;
		TextWidget w = m_actionsPanelWidget.FindAnyWidget("ActionListItem" + i.ToString());

		while (w)
		{
			m_actionItems.Insert(w);
			w.Show(false);
			i++;
			w = m_actionsPanelWidget.FindAnyWidget("ActionListItem" + i.ToString());
		}

		m_defaultActionWidget.Show(false);
		m_actionsPanelWidget.Show(true);
		m_actionsPanelWidget.SetAlpha(0);
	}

	void UpdateActions()
	{
		int prevActionsCount = m_actions.Count();
		
		m_actions.Release();
		GetGame().GetActionMenuActions(m_actions);
		
		Man player = GetGame().GetPlayer();
		// remove actions with empty text
		for (int i = m_actions.Count() - 1; i >= 0; i--)
		{
			string tmp;
			m_actions.Get(i).GetText(player, tmp);
			if (tmp == "") m_actions.Remove(i);
		}

		if (m_actions.Count())
		{
			if (m_selectedActionIndex < 0 || m_selectedActionIndex >= m_actions.Count() || prevActionsCount != m_actions.Count())
			{
				m_selectedActionIndex = 0;
			}
		}
		else
		{
			m_selectedActionIndex = -1;
		}

		UpdateWidgets();
	}
	
	void UpdateWidgets()
	{
		Man player = GetGame().GetPlayer();
		string action_name;
		Action selectedAction = GetSelectedAction();

		if (!m_enabled)
		{
			m_hide_timer.Stop();
			m_fade_timer.Stop();
			m_actionsPanelWidget.Show(false);
			m_defaultActionWidget.Show(false);
			return;
		}
		
		// default action
		if (selectedAction)
		{
			selectedAction.GetTextRich(player, action_name);
			if (action_name.Length() == 0)
			{
				selectedAction.GetText(player, action_name);
			}
			m_defaultActionWidget.SetText(action_name);
			m_defaultActionWidget.Show(true);
			
		}
		else
		{
			m_defaultActionWidget.Show(false);
		}


		// action list
		if (IsVisible())
		{
			m_fade_timer.FadeIn(m_actionsPanelWidget, FADE_IN_TIME, true);
			
			for (int i = 0; i < m_actionItems.Count(); i++)
			{
				TextWidget w = m_actionItems.Get(i);

				if (i < m_actions.Count())
				{
					m_actions.Get(i).GetText(player, action_name);
					w.SetText(action_name);

					if (i == m_selectedActionIndex)
					{
						w.SetColor(0xFFFFEC44);
					}
					else
					{
						w.SetColor(0xFFFFFFFF);
					}

					w.Show(true);
				}
				else
				{
					w.Show(false);
				}
			}

			float sx;
			float sy;
			m_actionsPanelWidget.GetSize(sx, sy);
			m_actionsPanelWidget.SetSize(sx, 8 + (m_actions.Count() * 32));
		}
		else
		{
			m_fade_timer.FadeOut(m_actionsPanelWidget, FADE_OUT_TIME, true);
		}

		m_hide_timer.Run(HIDE_MENU_TIME, this, "Hide");
	}

	void NextAction()
	{
		if (m_actions.Count() == 0) return;
		m_selectedActionIndex++;
		if (m_selectedActionIndex >= m_actions.Count()) m_selectedActionIndex = 0;
		UpdateWidgets();
	}

	void PrevAction()
	{
		if (m_actions.Count() == 0) return;
		m_selectedActionIndex--;
		if (m_selectedActionIndex < 0) m_selectedActionIndex = m_actions.Count() - 1;
		UpdateWidgets();
	}

	void PerformAction()
	{
		Action selectedAction = GetSelectedAction();

		if (selectedAction)
		{
			if (m_defaultActionWidget.IsVisible())
			{
				Man player = GetGame().GetPlayer();
				GetGame().PerformAction(player, selectedAction);
			}
			else
			{
				UpdateWidgets();
			}

			// refresh ingame hud quickbar
			Mission mission = GetGame().GetMission();
			if ( mission )
			{
				IngameHud hud = mission.GetHud();
				if ( hud )
				{
					hud.RefreshQuickbar();
				}
			}
		}
	}

	void Hide()
	{
		m_defaultActionWidget.Show(false);
		if (m_actionsPanelWidget.IsVisible()) m_fade_timer.FadeOut(m_actionsPanelWidget, FADE_OUT_TIME, true);
	}

	void SetEnabled(bool enabled)
	{
		if (m_enabled == enabled) return;
		
		m_enabled = enabled;

		UpdateWidgets();
	}

	bool IsVisible()
	{
		if (m_actions.Count() > 1)
		{
			return true;
		}

		return false;
	}
	
	bool HasDefaultAction()
	{
		if (GetSelectedAction())
		{
			return true;
		}

		return false;
	}

	Action GetSelectedAction()
	{
		if (m_selectedActionIndex < 0) return NULL;
		
		return m_actions.Get(m_selectedActionIndex);
	}
}
