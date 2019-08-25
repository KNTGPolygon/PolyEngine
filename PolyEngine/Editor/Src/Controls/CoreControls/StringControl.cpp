#include "PolyEditorPCH.hpp"

#include "Managers/CommandsImpl.hpp"

ASSIGN_CONTROL(StringControl, RTTI::eCorePropertyType::STRING, STRING)

//------------------------------------------------------------------------------
StringControl::StringControl(QWidget* parent)
	: ControlBase(parent)
{
	Field = new QLineEdit();
	connect(Field, &QLineEdit::editingFinished, this, &StringControl::UpdateObject);

	Layout->addWidget(Field, 0, 1);
}

//------------------------------------------------------------------------------
void StringControl::Reset()
{
	ControlBase<::pe::core::storage::String>::Reset();

	Field->setText("");
}

//------------------------------------------------------------------------------
void StringControl::UpdateControl()
{
	if (Field->hasFocus())
		return;

	Field->setText(Object->GetCStr());
}

//------------------------------------------------------------------------------
void StringControl::UpdateObject()
{
	if (DisableEdit || *Object == Field->text().toLatin1().data())
		return;

	ControlCommand<::pe::core::storage::String>* cmd = new ControlCommand<::pe::core::storage::String>();
	cmd->Object = Object;
	cmd->Control = this;
	cmd->UndoValue = new core::storage::String(*Object);
	cmd->RedoValue = new core::storage::String(Field->text().toLatin1().data());

	*Object = Field->text().toLatin1().data();

	emit ObjectUpdated(cmd);
}
