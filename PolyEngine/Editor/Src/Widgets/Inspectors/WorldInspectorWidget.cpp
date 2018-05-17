#include "PolyEditorPCH.hpp"

#include <sstream>

WorldExplorerWidget::WorldExplorerWidget()
{
	Tree = std::make_unique<QTreeWidget>(this);
	Tree->setHeaderLabels(QStringList() << "Entity ID");

	setLayout(new QGridLayout(this));
	layout()->addWidget(Tree.get());

	connect(Tree.get(), &QTreeWidget::itemDoubleClicked, this, &WorldExplorerWidget::SelectionChanged);
}

void WorldExplorerWidget::SetWorld(::World* world)
{
	World = world;

	UpdateViewer();
}

void WorldExplorerWidget::UpdateViewer()
{
	Tree->clear();
	std::stringstream ss;
	ss << World->GetRoot()->GetID();

	QTreeWidgetItem* entityTree = new QTreeWidgetItem(Tree.get());
	entityTree->setText(0, (&ss.str()[0]));
	EntityFromID.insert(std::pair<QTreeWidgetItem*, Entity*>(entityTree, World->GetRoot()));

	for (auto child : World->GetRoot()->GetChildren())
		AddEntityToTree(entityTree, child);
}

void WorldExplorerWidget::AddEntityToTree(QTreeWidgetItem* parent, Entity* entity)
{
	std::stringstream ss;
	ss << entity->GetID();

	QTreeWidgetItem* entityTree = new QTreeWidgetItem(parent);
	entityTree->setText(0, (&ss.str()[0]));
	EntityFromID.insert(std::pair<QTreeWidgetItem*, Entity*>(entityTree, entity));

	for (auto child : entity->GetChildren())
		AddEntityToTree(entityTree, child);
}

void WorldExplorerWidget::SelectionChanged(QTreeWidgetItem* item, int column)
{
	emit EntitySelected(EntityFromID[item]);
}
