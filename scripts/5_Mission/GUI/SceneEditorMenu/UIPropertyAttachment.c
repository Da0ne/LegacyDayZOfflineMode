class UIPropertyAttachment
{
	private Widget			m_WgtRoot;
	private Widget			m_WgtThis;
	private XComboBoxWidget	m_WgtComboBox;
	private TextWidget		m_WgtSlotName;
	private TStringArray	m_ComboItems;
	private int				m_PrevIndex;
	private EntityAI		m_Obj;
	private int				m_SlotID;
	
	
	void UIPropertyAttachment(Widget root)
	{
		m_WgtRoot		= root;
		
		m_ComboItems = new TStringArray;
		
		m_WgtThis = GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_scene_editor/day_z_scene_editor_attachment.layout", m_WgtRoot);
		m_WgtComboBox	= m_WgtThis.FindAnyWidget("combo_box");
		m_WgtSlotName	= m_WgtThis.FindAnyWidget("txt_slot_name");
	}
	
	void ~UIPropertyAttachment()
	{
		m_WgtRoot		= NULL;
		m_WgtComboBox	= NULL;
		
		m_WgtThis.Destroy();
	}
	
	bool OnClick(Widget w, int x, int y, int button)
	{
		if ( w == m_WgtComboBox )
		{
			if ( m_PrevIndex != 0 )
			{
				EntityAI attachment = m_Obj.FindAttachment(m_SlotID);
				m_Obj.RemoveAttachmentEx(attachment, m_SlotID);
			}
			
			int curr_index = m_WgtComboBox.GetCurrentItem();
			
			if ( curr_index != 0 )
			{				
				PluginDeveloper 	module_dev = GetPlugin(PluginDeveloper);				
				EntityAI e = module_dev.SpawnItem(PluginSceneManager.PLAYER, m_ComboItems.Get(curr_index), SPAWNTYPE_ATTACHMENT, 0, 0, 0, true, m_Obj.GetPosition(), m_Obj);
				
				if ( e != NULL ) 
				{		
					if ( e.IsInherited(ItemBase) )
					{
						ItemBase item = (ItemBase)e;
						item.SetQuantity(item.GetQuantityMax());
					}
					
					//m_Obj.TakeEntityAsAttachmentToSlot(e, m_SlotID);
				}
			}
				
			m_PrevIndex = curr_index;
			
			return true;
		}
		
		return false;
	}
	
	void SetPos(float x, float y)
	{
		m_WgtThis.SetPos(x, y);
	}
	
	void SetSize(float width, float height)
	{
		m_WgtThis.SetSize(width, height);
	}
	
	void Show(EntityAI e, string slot_name, TStringArray att_items)
	{
		m_WgtThis.Show(true);
		
		m_Obj = e;
		m_ComboItems.Clear();
		m_ComboItems.Insert("none");
		m_ComboItems.InsertAll(att_items);
		
		m_WgtSlotName.SetText(slot_name);
		
		m_WgtComboBox.ClearAll();
		
		m_SlotID = GetGame().GetInventorySlot(slot_name);
		EntityAI attachment = e.FindAttachment(m_SlotID);
		int selected_index = 0;
		
		for ( int i = 0; i < m_ComboItems.Count(); ++i )
		{
			string item_name = m_ComboItems.Get(i);
			
			m_WgtComboBox.AddItem(item_name);
			
			if ( attachment != NULL && attachment.GetType() == item_name )
			{
				selected_index = i;
			}
		}
		
		m_WgtComboBox.SetCurrentItem(selected_index);
		m_PrevIndex = selected_index;
	}
	
	void Hide()
	{
		m_WgtThis.Show(false);
		
		m_Obj = NULL;
		m_SlotID = -1;
	}
	
	bool IsVisible()
	{
		return m_WgtThis.IsVisible();
	}
}
