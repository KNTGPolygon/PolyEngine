#pragma once

#include "Controls/ControlBase.hpp"

#include <QtWidgets/qpushbutton.h>
#include <QtCore/qstatemachine.h>

class BoolControl : public ControlBase
{
public:
	BoolControl(QWidget* parent);
	~BoolControl();

	void Reset() override;

	void UpdateObject() override;
	void UpdateControl() override;

private:
	QPushButton* Button;
	QStateMachine* Machine;
	QGridLayout* Layout;

	QState* False;
	QState* True;
};