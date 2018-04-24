#include "NumberControl.hpp"

#include <QIntValidator>
#include <QDoubleValidator>

NumberControl::NumberControl(void* ptr, RTTI::eCorePropertyType type)
	: Object(ptr), Type(type)
{
}

NumberControl::NumberControl(void* ptr, const RTTI::Property& prop, RTTI::eCorePropertyType type)
	: Object(ptr), Type(type)
{
}

void NumberControl::UpdateObject()
{
}

void NumberControl::UpdateControl()
{
}

void NumberControl::InitializeControl()
{
	QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	sizePolicy.setVerticalStretch(2);
	sizePolicy.setHorizontalStretch(2);

	// TODO(squares): write validator for INT32 and INT63

	switch (Type)
	{
	case RTTI::eCorePropertyType::INT8:
		Validator = std::make_unique<QIntValidator>(-128, 127);
		Field->setText();
		break;
	case RTTI::eCorePropertyType::INT16:
		Validator = std::make_unique<QIntValidator>(-32768, 32767);
		Field->setText();
		break;
	case RTTI::eCorePropertyType::INT32:
		ASSERTE(false, "Not supported type");
		break;
	case RTTI::eCorePropertyType::INT64:
		ASSERTE(false, "Not supported type");
		break;
	case RTTI::eCorePropertyType::UINT8:
		Validator = std::make_unique<QIntValidator>(0, 255);
		Field->setText();
		break;
	case RTTI::eCorePropertyType::UINT16:
		Validator = std::make_unique<QIntValidator>(0, 65536);
		Field->setText();
		break;
	case RTTI::eCorePropertyType::UINT32:
		ASSERTE(false, "Not supported type");
		break;
	case RTTI::eCorePropertyType::UINT64:
		ASSERTE(false, "Not supported type");
		break;
	case RTTI::eCorePropertyType::FLOAT:
		Validator = std::make_unique<QDoubleValidator>(-3.4E+38, +3.4E+38, 7);
		Field->setText();
		break;
	case RTTI::eCorePropertyType::DOUBLE:
		Validator = std::make_unique<QDoubleValidator>(-1.7E+308, +1.7E+308, 16);
		Field->setText();
		break;
	default:
		ASSERTE(false, "Not supported type");
	}

	Field = std::make_unique<QLineEdit>(this);
	Field->setSizePolicy(sizePolicy);
	Field->setValidator(Validator.get());
}