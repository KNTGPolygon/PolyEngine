#pragma once

#include <qobject.h>

#include <ECS/World.hpp>

#include "Managers/EngineManager.hpp"
#include "Configs/ProjectConfig.hpp"

using namespace Poly;

class EditorApp;

class InspectorManager : public QObject
{
	Q_OBJECT

public:
	InspectorManager(EditorApp* app);

signals:
	void ProjectOpened(const ProjectConfig* config);
	void ProjectClosed();
	void EngineInitialized(World* world);
	void EngineDeinitialized();
	void StateChanged(eEngineState state);

private slots:
	void ProjectOpenedSlot(const ProjectConfig* config);
	void ProjectClosedSlot();
	void EngineInitializedSlot(World* world);
	void EngineDeinitializedSlot();
	void StateChangedSlot(eEngineState state);
};