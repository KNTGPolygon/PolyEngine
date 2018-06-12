#pragma once

#include <ECS/Entity.hpp>
#include <Core.hpp>

#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <qscrollarea.h>

#include "Widgets/Inspectors/InspectorWidgetBase.hpp"

class SectionContainer;
class RTTIInspectorWidget;
class TransformControl;

using namespace Poly;

// This widget allows user change entity and its components properties.
// User also can add and remove any component.
// Updates object ASAP.
class EntityInspectorWidget : public InspectorWidgetBase
{
public:
	EntityInspectorWidget(QWidget* parent);

	// Set entity to display; updates automatically.
	void SetSelectedEntities(Dynarray<Entity*> entities);

	// Initializes object connections with other inspectors and inspector manager.
	void InitializeConnections() override;

	// Sets default empty view.
	void Reset() override;

	// Updates every field within widget from currently set entity.
	// If there was any component added it will not show up after update.
	// If thre aren't any new component use this one rather than ReloadInspector.
	void UpdateInspector();

	// Releases all controls and creates everything again.
	// If any component was added you should use this one rather than UpdateInspector.
	void ReloadInspector();

private:
	Dynarray<Entity*> Entities;

	QGridLayout* MainLayout;
	QMenu* ContextMenu;
		QAction* AddComponentAction;
		QAction* RemoveComponentAction;

	QLabel* NameText;			QLineEdit* NameField;
	QLabel* UniqueIdText;		QLineEdit* UniqueIdField;
	QLabel* ParentIdNameText;	QLineEdit* ParentIdNameField;	QPushButton* ParentIdNameButton;
	QLabel* ChildrenIdNameText;	QComboBox* ChildrenIdNameField;	QPushButton* ChildrenIdNameButton;

	SectionContainer* TransformSection;
	TransformControl* Transform;

	// Component sections
	Dynarray<SectionContainer*> ComponentSections;
	Dynarray<RTTIInspectorWidget*> ComponentInspectors;

public slots:
	void SpawnContextMenu(QPoint pos); 
		void AddComponent(); 
		void RemoveComponent();
};