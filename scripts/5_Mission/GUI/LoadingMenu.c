class LoadingMenu extends UIScriptedMenu
{
	void LoadingMenu()
	{
	}

	void ~LoadingMenu()
	{
	}
	
	Widget Init()
	{
		Widget layoutRoot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/loading.layout");
		
		m_label = (TextWidget)layoutRoot.FindAnyWidget("TextWidget");
		m_progressBar = (ProgressBarWidget)layoutRoot.FindAnyWidget("ProgressBarWidget");
		m_image = (ImageWidget)layoutRoot.FindAnyWidget("ImageBackground");
		
		m_image.LoadImageFile( 0, GetRandomLoadingBackground() );
		
		return layoutRoot;
	}

	TextWidget m_label;
	ProgressBarWidget m_progressBar;
	ImageWidget m_image;
}
