#pragma once

#include "Windows/PolyMainWindow.hpp"
#include "Widgets/ViewportWidget.hpp"
#include "Widgets/LoggerWidget.hpp"
#include "Widgets/EntityManagerWidget.hpp"
#include "Widgets/WorldExplorerWidget.hpp"

class EditorUi : public QObject
{
	friend class DockManager;
	friend class PolyWindow;

public:
	EditorUi() = default;

	void Init();
	void AddWindow() { PolyWindow* wnd = new PolyWindow(); Windows.PushBack(wnd); wnd->show(); }

	ViewportWidget* MainViewport;
	LoggerWidget* MainLogger;
	LoggerWidget* CmdLogger;
	WorldExplorerWidget* WorldExplorer;
	EntityManagerWidget* EntityMgr;

private:
	PolyMainWindow* MainWindow;
	Poly::Dynarray<PolyWindow*> Windows;
	Poly::Dynarray<PolyDockWindow*> DockWindows;
};