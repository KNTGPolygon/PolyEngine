#pragma once

#include <ECS/World.hpp>
#include <ECS/Entity.hpp>
#include <UniqueID.hpp>

#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qtreewidget.h>
#include <QtWidgets/qlayout.h>
#include <QtWidgets/qlabel.h>

#include "Widgets/Inspectors/InspectorWidgetBase.hpp"

using namespace Poly;

class CustomTree : public QTreeWidget
{
	Q_OBJECT

public:
	CustomTree(QWidget* parent) : QTreeWidget(parent) {}

signals:
	void Dropped(QDropEvent* e);

private:
	void dropEvent(QDropEvent* e) override { emit Dropped(e);  QTreeWidget::dropEvent(e); }
};

// Displays tree of entities in the world. Enables adding, removing and reparenting entities.
class WorldInspectorWidget : public InspectorWidgetBase
{
	Q_OBJECT

public:
	WorldInspectorWidget(QWidget* parent);

	// Initializes object connections with other inspectors and inspector manager.
	void InitializeConnections() override;

	// Removes all entities from list.
	void Reset() override;


public slots:
	// Sets currently viewed world to given object and updates inspector.
	void WorldChanged(World* world);

	// Adds this new entity to tree.
	void EntitiesSpawned(Dynarray<Entity*> entities);

	void EntitiesDestroyed();

	// Reparents all currently selected entities to new parent.
	void EntitiesReparented(Entity* parent);

	// Sets selected entities.
	void EntitiesSelectionChanged(Dynarray<Entity*> entities);

	// Updates fields within tree items.
	void SoftUpdate();


private:
	// Called recursively to add entity and all its chiildren to tree
	void AddEntityToTree(Entity* entity, QTreeWidgetItem* parent);


	World* WorldObj;
	Dynarray<Entity*> SelectedEntities;

	QMenu* ContextMenu;
		QAction* AddEntityAction;
		QAction* RemoveEntityAction;
		QAction* ReparentEntityAction;

	CustomTree* Tree;
	std::map<QTreeWidgetItem*, Entity*> EntityFromItem;

private slots:
	void SelectionChanged(QTreeWidgetItem* item, int column);
	void Drop(QDropEvent* e);

	void SpawnContextMenu(QPoint pos);
		void SpawnEntities();
		void DestroyEntities();
		void ReparentEntities();
};