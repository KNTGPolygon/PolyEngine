#include "PolyEditorPCH.hpp"

#include "AI/PathfindingComponent.hpp"
#include "Audio/SoundEmitterComponent.hpp"
#include "Audio/SoundListenerComponent.hpp"
#include "Movement/FreeFloatMovementComponent.hpp"
#include "Physics2D/Physics2DColliders.hpp"
#include "Physics2D/Rigidbody2DComponent.hpp"
#include "Physics3D/Collider3DComponent.hpp"
#include "Physics3D/Rigidbody3DComponent.hpp"
#include "Rendering/Camera/CameraComponent.hpp"
#include "Rendering/Lighting/LightSourceComponent.hpp"
#include "Rendering/Particles/ParticleComponent.hpp"
#include "Rendering/MeshRenderingComponent.hpp"
#include "Rendering/PostprocessSettingsComponent.hpp"
#include "Rendering/SpritesheetComponent.hpp"
#include "UI/ScreenSpaceTextComponent.hpp"

#include "Audio/SoundWorldComponent.hpp"
#include "Debugging/DebugWorldComponent.hpp"
#include "ECS/DeferredTaskWorldComponent.hpp"
#include "Input/InputWorldComponent.hpp"
#include "Physics2D/Physics2DWorldComponent.hpp"
#include "Physics3D/Physics3DWorldComponent.hpp"
#include "Rendering/SkyboxWorldComponent.hpp"
#include "Rendering/ViewportWorldComponent.hpp"
#include "Time/TimeWorldComponent.hpp"
#include <ECS/DeferredTaskSystem.hpp>

#include <sstream>

#define ADD_COMPONENT(COMPONENT) \
	if (!entity->HasComponent<COMPONENT>()) \
	{ \
		QTreeWidgetItem* cmp = new QTreeWidgetItem(ComponentsTree); \
		cmp->setText(1, #COMPONENT); \
		cmp->setCheckState(0, Qt::Unchecked); \
		ComponentCreators.insert(std::pair<QString, ComponentCreator>(#COMPONENT, [](::Entity* e) \
		{ Poly::DeferredTaskSystem::AddComponentImmediate<COMPONENT>(e->GetWorld(), e); })); \
	}

#define ADD_COMPONENT_DESTROYER(COMPONENT) \
	if (entity->HasComponent<COMPONENT>()) \
	{ \
		QTreeWidgetItem* cmp = new QTreeWidgetItem(ComponentsTree); \
		cmp->setText(1, #COMPONENT); \
		cmp->setCheckState(0, Qt::Unchecked); \
		ComponentDestroyers.insert(std::pair<QString, ComponentDestroyer>(#COMPONENT, [](::Entity* e) \
		{ Poly::DeferredTaskSystem::RemoveComponent<COMPONENT>(e->GetWorld(), e); })); \
	}

#define ADD_WORLD_COMPONENT(COMPONENT) \
	if (world->HasWorldComponent<COMPONENT>()) \
	{ \
		QTreeWidgetItem* cmp = new QTreeWidgetItem(ComponentsTree); \
		cmp->setText(1, #COMPONENT); \
		cmp->setCheckState(0, Qt::Unchecked); \
		WorldComponentCreators.insert(std::pair<QString, WorldComponentCreator>(#COMPONENT, [](::World* w) \
		{ Poly::DeferredTaskSystem::RemoveWorldComponentImmediate<COMPONENT>(w); })); \
	}

#define ADD_WORLD_COMPONENT_DESTROYER(COMPONENT) \
	if (world->HasWorldComponent<COMPONENT>()) \
	{ \
		QTreeWidgetItem* cmp = new QTreeWidgetItem(ComponentsTree); \
		cmp->setText(1, #COMPONENT); \
		cmp->setCheckState(0, Qt::Unchecked); \
		WorldComponentDestroyers.insert(std::pair<QString, WorldComponentDestroyer>(#COMPONENT, [](::World* w) \
		{ Poly::DeferredTaskSystem::RemoveWorldComponentImmediate<COMPONENT>(w); })); \
	}

//------------------------------------------------------------------------------
void ComponentDialog::AddComponent(Entity* entity)
{
	InitUi(eMode::ADD);

	// entity info
	QPalette disabledEditPalette;
	disabledEditPalette.setColor(QPalette::Base, QColor(218, 218, 218));
	disabledEditPalette.setColor(QPalette::Text, Qt::black);

	EntityIdNameText = new QLabel(this);
	EntityIdNameText->setText("Name / Entity ID");
	MainLayout->addWidget(EntityIdNameText, 1, 0);

	EntityIdNameField = new QLineEdit(this);
	EntityIdNameField->setReadOnly(true);
	std::stringstream ss;
	ss << entity->GetID();
	EntityIdNameField->setText(QString(entity->Name.GetCStr()) + QString(&ss.str()[0]));
	EntityIdNameField->setPalette(disabledEditPalette);
	MainLayout->addWidget(EntityIdNameField, 1, 1, 1, 2);

	//ADD_COMPONENT(PathfindingComponent)
	//ADD_COMPONENT(SoundEmitterComponent)
	//ADD_COMPONENT(SoundListenerComponent)
	//ADD_COMPONENT(FreeFloatMovementComponent)
	//ADD_COMPONENT(Box2DColliderComponent)
	//ADD_COMPONENT(Circle2DColliderComponent)
	//ADD_COMPONENT(RigidBody2DComponent)
	//ADD_COMPONENT(Collider3DComponent)
	//ADD_COMPONENT(Rigidbody3DComponent)
	//ADD_COMPONENT(CameraComponent)
	//ADD_COMPONENT(AmbientLightWorldComponent)
	//ADD_COMPONENT(DirectionalLightComponent)
	//ADD_COMPONENT(PointLightComponent)
	//ADD_COMPONENT(SpotLightComponent)
	//ADD_COMPONENT(MeshRenderingComponent)
	//ADD_COMPONENT(PostprocessSettingsComponent)
	//ADD_COMPONENT(SpritesheetComponent)
	//ADD_COMPONENT(ScreenSpaceTextComponent)

	exec();
}

//------------------------------------------------------------------------------
void ComponentDialog::RemoveComponent(Entity* entity)
{
	InitUi(eMode::REMOVE);

	// entity info
	QPalette disabledEditPalette;
	disabledEditPalette.setColor(QPalette::Base, QColor(218, 218, 218));
	disabledEditPalette.setColor(QPalette::Text, Qt::black);

	EntityIdNameText = new QLabel(this);
	EntityIdNameText->setText("Name / Entity ID");
	MainLayout->addWidget(EntityIdNameText, 1, 0);

	EntityIdNameField = new QLineEdit(this);
	EntityIdNameField->setReadOnly(true);
	std::stringstream ss;
	ss << entity->GetID();
	EntityIdNameField->setText(QString(entity->Name.GetCStr()) + QString(&ss.str()[0]));
	EntityIdNameField->setPalette(disabledEditPalette);
	MainLayout->addWidget(EntityIdNameField, 1, 1, 1, 2);

	ADD_COMPONENT_DESTROYER(PathfindingComponent)
	ADD_COMPONENT_DESTROYER(SoundEmitterComponent)
	ADD_COMPONENT_DESTROYER(SoundListenerComponent)
	ADD_COMPONENT_DESTROYER(FreeFloatMovementComponent)
	ADD_COMPONENT_DESTROYER(Box2DColliderComponent)
	ADD_COMPONENT_DESTROYER(Circle2DColliderComponent)
	ADD_COMPONENT_DESTROYER(RigidBody2DComponent)
	ADD_COMPONENT_DESTROYER(Collider3DComponent)
	ADD_COMPONENT_DESTROYER(Rigidbody3DComponent)
	ADD_COMPONENT_DESTROYER(CameraComponent)
	ADD_COMPONENT_DESTROYER(AmbientLightWorldComponent)
	ADD_COMPONENT_DESTROYER(DirectionalLightComponent)
	ADD_COMPONENT_DESTROYER(PointLightComponent)
	ADD_COMPONENT_DESTROYER(SpotLightComponent)
	ADD_COMPONENT_DESTROYER(MeshRenderingComponent)
	ADD_COMPONENT_DESTROYER(PostprocessSettingsComponent)
	ADD_COMPONENT_DESTROYER(SpritesheetComponent)
	ADD_COMPONENT_DESTROYER(ScreenSpaceTextComponent)

	exec();
}

//------------------------------------------------------------------------------
void ComponentDialog::AddWorldComponent(World* world)
{
	InitUi(eMode::ADD);

	//ADD_WORLD_COMPONENT(SoundWorldComponent)
	//ADD_WORLD_COMPONENT(DebugWorldComponent)
	//ADD_WORLD_COMPONENT(DeferredTaskWorldComponent)
	//ADD_WORLD_COMPONENT(InputWorldComponent)
	//ADD_WORLD_COMPONENT(Physics2DWorldComponent)
	//ADD_WORLD_COMPONENT(Physics3DWorldComponent)
	//ADD_WORLD_COMPONENT(SkyboxWorldComponent)
	//ADD_WORLD_COMPONENT(ViewportWorldComponent)
	//ADD_WORLD_COMPONENT(TimeWorldComponent)

	exec();
}

//------------------------------------------------------------------------------
void ComponentDialog::RemoveWorldComponent(World* world)
{
	InitUi(eMode::REMOVE);

	ADD_WORLD_COMPONENT_DESTROYER(SoundWorldComponent)
	ADD_WORLD_COMPONENT_DESTROYER(DebugWorldComponent)
	ADD_WORLD_COMPONENT_DESTROYER(DeferredTaskWorldComponent)
	ADD_WORLD_COMPONENT_DESTROYER(InputWorldComponent)
	ADD_WORLD_COMPONENT_DESTROYER(Physics2DWorldComponent)
	ADD_WORLD_COMPONENT_DESTROYER(Physics3DWorldComponent)
	ADD_WORLD_COMPONENT_DESTROYER(SkyboxWorldComponent)
	ADD_WORLD_COMPONENT_DESTROYER(ViewportWorldComponent)
	ADD_WORLD_COMPONENT_DESTROYER(TimeWorldComponent)

	exec();
}

//------------------------------------------------------------------------------
void ComponentDialog::InitUi(eMode mode)
{
	setModal(true);

	// create main layout
	MainLayout = new QGridLayout(this);
	MainLayout->setColumnStretch(0, 1);
	MainLayout->setColumnStretch(1, 1);
	MainLayout->setColumnStretch(2, 1);

	// create components tree
	ComponentsTree = new QTreeWidget(this);
	switch (mode)
	{
	case ComponentDialog::eMode::ADD:
		ComponentsTree->setHeaderLabels(QStringList() << "Add" << "Component Name");
		break;

	case ComponentDialog::eMode::REMOVE:
		ComponentsTree->setHeaderLabels(QStringList() << "Remove" << "Component Name");
		break;
	default:
		throw new std::exception();
	}
	MainLayout->addWidget(ComponentsTree, 0, 0, 1, 3);

	// entity info
	QPalette disabledEditPalette;
	disabledEditPalette.setColor(QPalette::Base, QColor(218, 218, 218));
	disabledEditPalette.setColor(QPalette::Text, Qt::black);

	// buttons
	CancelButton = new QPushButton(this);
	CancelButton->setText("Cancel");
	connect(CancelButton, &QPushButton::clicked, this, &ComponentDialog::Cancel);
	MainLayout->addWidget(CancelButton, 2, 0);

	OkButton = new QPushButton(this);
	switch (mode)
	{
	case ComponentDialog::eMode::ADD:
		OkButton->setText("Add");
		break;

	case ComponentDialog::eMode::REMOVE:
		OkButton->setText("Remove");
		break;
	default:
		throw new std::exception();
	}
	connect(OkButton, &QPushButton::clicked, this, &ComponentDialog::Ok);
	MainLayout->addWidget(OkButton, 2, 2);
}

//------------------------------------------------------------------------------
void ComponentDialog::Ok()
{
	Canceled = false;
	close();
}

//------------------------------------------------------------------------------
void ComponentDialog::Cancel()
{
	close();
}
