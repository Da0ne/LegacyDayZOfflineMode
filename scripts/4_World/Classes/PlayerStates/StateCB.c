class StateCB extends HumanCommandActionCallback
{
	StateBase m_State;
	bool m_DestroyStateOnFinish;
	
	void OnFinish(bool pCanceled)
	{
		if(m_State) m_State.OnAnimationFinish(pCanceled, m_DestroyStateOnFinish);
	}
	
	void Init(StateBase state, bool destroy)
	{
		EnableCancelCondition(true);
		m_State = state;
		m_DestroyStateOnFinish = destroy;

	}
	
	bool CancelCondition()
	{
		return false;
	}

	bool IsSymptomCallback()
	{
		return true;
	}
	
};
