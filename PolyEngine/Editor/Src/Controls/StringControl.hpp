#pragma once

#include "ControlBase.hpp"

#include <QLineEdit.h>
#include <QGridLayout.h>

class StringControl : public ControlBase
{
public:
	StringControl() = default;

	void UpdateObject() override;
	void UpdateControl() override;

private:
	void InitializeControl() override;

	std::unique_ptr<QLineEdit> Field;
	std::unique_ptr<QGridLayout> Layout;
};