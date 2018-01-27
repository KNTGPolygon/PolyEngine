#pragma once

#include <Queue.hpp>
#include <Vector2i.hpp>
#include "KeyBindings.hpp"

namespace Poly
{
	enum class eInputEventType
	{
		KEYDOWN,
		KEYUP,
		MOUSEBUTTONDOWN,
		MOUSEBUTTONUP,
		MOUSEMOVE,
		MOUSEPOS,
		WHEELMOVE,
        CONTROLLER_ADDED,
        CONTROLLER_REMOVED,
        CONTROLLER_BUTTON_DOWN,
        CONTROLLER_BUTTON_UP,
        CONTROLLER_AXIS_MOTION,
		_COUNT
	};

	struct InputEvent : public BaseObject<>
	{
		InputEvent() = default;
		InputEvent(eInputEventType type, eKey key) : Type(type), Key(key) {}
		InputEvent(eInputEventType type, eMouseButton button) : Type(type), MouseButton(button) {}
		InputEvent(eInputEventType type, const Vector2i& pos) : Type(type), Pos(pos) {}
		InputEvent(eInputEventType type, size_t id, eControllerButton button) : Type(type), JoystickID(id), ControllerButton(button) {}
		InputEvent(eInputEventType type, size_t id, eControllerAxis axis, float value) : Type(type), JoystickID(id), ControllerAxis(axis), AxisValue(value) {}
		InputEvent(eInputEventType type, size_t id) : Type(type), JoystickID(id) {}

		eInputEventType Type = eInputEventType::_COUNT;
		eKey Key = eKey::_COUNT;
		eMouseButton MouseButton = eMouseButton::_COUNT;
		Vector2i Pos;
        size_t JoystickID;
        eControllerButton ControllerButton = eControllerButton::_COUNT;
        eControllerAxis ControllerAxis = eControllerAxis::_COUNT;
        float AxisValue;
    };

	using InputQueue = Queue<InputEvent>;
}
