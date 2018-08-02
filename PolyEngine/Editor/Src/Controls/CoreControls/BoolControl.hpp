#pragma once

#include "Controls/IControlBase.hpp"

#include <QtWidgets/qpushbutton.h>
#include <QtCore/qstatemachine.h>

class Command;

class BoolControl : public IControlBase
{
public:
	BoolControl(QWidget* parent);
	~BoolControl();

	void Reset() override;
	void UpdateObject() override;
	void UpdateControl() override;
	void Confirm() override;

private:
	QPushButton* Button;
	QStateMachine* Machine;
	QGridLayout* Layout;

	QState* False;
	QState* True;

	bool InitializationConfirm = false;
};