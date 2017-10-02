#include "Input.h"

std::bitset<Key::KeyCount> Input::m_currentKeyStates;
std::bitset<Key::KeyCount> Input::m_lastKeyStates;

std::bitset<MouseButton::ButtonCount> Input::m_currentMouseStates;
std::bitset<MouseButton::ButtonCount> Input::m_lastMouseStates;

vec2 Input::m_mousePosition = vec2(0.0f, 0.0f);

void Input::reset()
{
	m_lastKeyStates = m_currentKeyStates;
	m_lastMouseStates = m_currentMouseStates;

	m_currentKeyStates.reset();
	m_currentMouseStates.reset();
}

bool Input::getKey(Key keyCode)
{
	if (keyCode < Key::KeyCount) {
		return m_currentKeyStates.test(static_cast<size_t>(keyCode));
	}
	else {
		return false;
	}
}

bool Input::getKeyDown(Key keyCode)
{
	if (keyCode < Key::KeyCount) {
		return !m_lastKeyStates.test(static_cast<size_t>(keyCode)) && 
				m_currentKeyStates.test(static_cast<size_t>(keyCode));
	}
	else {
		return false;
	}
}

bool Input::getKeyUp(Key keyCode)
{
	if (keyCode < Key::KeyCount) {
		return m_lastKeyStates.test(static_cast<size_t>(keyCode)) && 
				!m_currentKeyStates.test(static_cast<size_t>(keyCode));
	}
	else {
		return false;
	}
}

bool Input::getMouse(MouseButton mouseCode)
{
	if (mouseCode < MouseButton::ButtonCount) {
		return m_currentMouseStates.test(static_cast<size_t>(mouseCode));
	}
	else {
		return false;
	}
}

bool Input::getMouseDown(MouseButton mouseCode)
{
	if (mouseCode < MouseButton::ButtonCount) {
		return !m_lastMouseStates.test(static_cast<size_t>(mouseCode)) && 
				m_currentMouseStates.test(static_cast<size_t>(mouseCode));
	}
	else {
		return false;
	}
}

bool Input::getMouseUp(MouseButton mouseCode)
{
	if (mouseCode < MouseButton::ButtonCount) {
		return m_lastMouseStates.test(static_cast<size_t>(mouseCode)) && 
				!m_currentMouseStates.test(static_cast<size_t>(mouseCode));
	}
	else {
		return false;
	}
}

void Input::setMousePosition(const vec2 & pos)
{
	
}

vec2 Input::getMousePosition()
{
	return m_mousePosition;
}

void Input::setCursorVisible(bool visible)
{
	
}
