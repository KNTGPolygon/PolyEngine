#pragma once

#include <QFormLayout>
#include <QTreeView>
#include <QFileSystemModel>

#include "Configs/ProjectConfig.hpp"
#include "Widgets/Inspectors/InspectorWidgetBase.hpp"

// File explorer with additional functionality, updates content ASAP.
class ResourceInspectorWidget : public InspectorWidgetBase
{
public:
	ResourceInspectorWidget(QWidget* parent);

	// Initializes object connections with other inspectors and inspector manager.
	void InitializeConnections() override;

	// Removes all items from viewer.
	void Reset() override;

	void UpdateInspector() override {}
	void ReloadInspector() override {}

	// Fills layout with list of found resources
	void SetObject(const ProjectConfig* config);

private:
	const ProjectConfig* Config;

	QGridLayout* Layout;

	QFileSystemModel* Model;
	QTreeView* Tree;
};