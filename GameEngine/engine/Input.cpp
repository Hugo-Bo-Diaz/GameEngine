#include "../Globals.h"

//#include <glad/glad.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/type_ptr.hpp>
//
//#include <glm/gtx/matrix_decompose.hpp>

#include <GLFW/glfw3.h>

#include "../MessageManager.h"

#include "Engine.h"
#include "Input.h"

Input::Input()
{
	
}

bool Input::Initialize(GLFWwindow* window)
{
	glfwSetWindowUserPointer(window, Engine::Get().in);

	glfwSetCursorPosCallback(window, GLFW_MouseMoveCallback);
	glfwSetMouseButtonCallback(window, GLFW_MouseButtonCallback);
	glfwSetKeyCallback(window, GLFW_KeyboardCallback);
	glfwSetWindowFocusCallback(window, GLFW_FocusWindowCallback);

	return true;
}

void Input::GLFW_KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Input* in = GetInput(window);

	KeyBoard_key my_key = MAX_KEYS;

	switch (key)
	{
	case GLFW_KEY_Q: my_key = KEY_Q; break;	case GLFW_KEY_A: my_key = KEY_A; break;	case GLFW_KEY_Z: my_key = KEY_Z; break;
	case GLFW_KEY_W: my_key = KEY_W; break;	case GLFW_KEY_S: my_key = KEY_S; break;	case GLFW_KEY_X: my_key = KEY_X; break;
	case GLFW_KEY_E: my_key = KEY_E; break;	case GLFW_KEY_D: my_key = KEY_D; break;	case GLFW_KEY_C: my_key = KEY_C; break;
	case GLFW_KEY_R: my_key = KEY_R; break;	case GLFW_KEY_F: my_key = KEY_F; break;	case GLFW_KEY_V: my_key = KEY_V; break;
	case GLFW_KEY_T: my_key = KEY_T; break;	case GLFW_KEY_G: my_key = KEY_G; break;	case GLFW_KEY_B: my_key = KEY_B; break;
	case GLFW_KEY_Y: my_key = KEY_Y; break;	case GLFW_KEY_H: my_key = KEY_H; break;	case GLFW_KEY_N: my_key = KEY_N; break;
	case GLFW_KEY_U: my_key = KEY_U; break;	case GLFW_KEY_J: my_key = KEY_J; break;	case GLFW_KEY_M: my_key = KEY_M; break;
	case GLFW_KEY_I: my_key = KEY_I; break;	case GLFW_KEY_K: my_key = KEY_K; break;
	case GLFW_KEY_O: my_key = KEY_O; break;	case GLFW_KEY_L: my_key = KEY_L; break;
	case GLFW_KEY_P: my_key = KEY_P; break;

	case GLFW_KEY_1: my_key = KEY_1; break;	case GLFW_KEY_2: my_key = KEY_2; break;
	case GLFW_KEY_3: my_key = KEY_3; break;	case GLFW_KEY_4: my_key = KEY_4; break;
	case GLFW_KEY_5: my_key = KEY_5; break;	case GLFW_KEY_6: my_key = KEY_6; break;
	case GLFW_KEY_7: my_key = KEY_7; break;	case GLFW_KEY_8: my_key = KEY_8; break;
	case GLFW_KEY_9: my_key = KEY_9; break;	case GLFW_KEY_0: my_key = KEY_0; break;

	case GLFW_KEY_DOWN: my_key = KEY_ARR_DOWN; break;	case GLFW_KEY_UP: my_key = KEY_ARR_UP; break;
	case GLFW_KEY_LEFT: my_key = KEY_ARR_LEFT; break;	case GLFW_KEY_RIGHT: my_key = KEY_ARR_RIGHT; break;

	case GLFW_KEY_LEFT_SHIFT: my_key = KEY_L_MAYUS; break;	case GLFW_KEY_RIGHT_SHIFT: my_key = KEY_R_MAYUS; break;
	case GLFW_KEY_LEFT_CONTROL: my_key = KEY_CNTRL; break;
	case GLFW_KEY_SPACE: my_key = KEY_SPACE; break;
	case GLFW_KEY_LEFT_ALT: my_key = KEY_L_ALT; break;

	case GLFW_KEY_ENTER: my_key = KEY_ENTER; break;
	case GLFW_KEY_BACKSPACE: my_key = KEY_BACKSPACE; break;

	default:
		break;
	}

	if (my_key < MAX_KEYS)
	{
		
		switch (action)
		{
		case GLFW_PRESS:
			in->Keyboard_keys[my_key] = KEY_PRESSED;
			break;
		case GLFW_REPEAT:
			in->Keyboard_keys[my_key] = KEY_DOWN;
			break;
		case GLFW_RELEASE:
			in->Keyboard_keys[my_key] = KEY_RELEASED;
			break;
		default:
			break;
		}
	}

}

void Input::GLFW_MouseMoveCallback(GLFWwindow* window, double xpos, double ypos)
{
	Input* in = GetInput(window);

	in->mouse_delta_x = xpos - in->mouse_x;
	in->mouse_delta_y = ypos - in->mouse_y;

	in->mouse_x = xpos;
	in->mouse_y = ypos;

}

void Input::GLFW_MouseButtonCallback(GLFWwindow* window, int key, int action, int mods)
{
	Input* in = GetInput(window);

	MouseButton my_button = MAX_MOUSE_BUTTONS;

	switch (key)
	{
	case GLFW_MOUSE_BUTTON_LEFT: my_button = MOUSE_LEFT; break;
	case GLFW_MOUSE_BUTTON_MIDDLE: my_button = MOUSE_MIDDLE; break;
	case GLFW_MOUSE_BUTTON_RIGHT: my_button = MOUSE_RIGHT; break;
	default:
		break;
	}

	if (my_button < MAX_MOUSE_BUTTONS)
	{

		switch (action)
		{
		case GLFW_PRESS:
			in->Mouse_keys[my_button] = KEY_PRESSED;
			break;
		case GLFW_REPEAT:
			in->Mouse_keys[my_button] = KEY_DOWN;
			break;
		case GLFW_RELEASE:
			in->Mouse_keys[my_button] = KEY_RELEASED;
			break;
		default:
			break;
		}
	}
}

void Input::GLFW_FocusWindowCallback(GLFWwindow* window, int focused)
{
	if (focused == 1)
	{
		msg* m = new msg();

		m->dest = EDITOR;
		m->code = GOT_FOCUS;

		MessageManager::Get().AddMessage(m);
	}
}

bool Input::ShutDown()
{
	return true;
}

KeyState Input::GetKey(KeyBoard_key key)
{

	return Keyboard_keys[key];
}

KeyState Input::GetMouseKey(MouseButton button)
{
	return Mouse_keys[button];
}

void Input::GetMousePosition(float& x, float& y)
{
	x = mouse_x;
	y = mouse_y;
}

void Input::GetMouseDeltaPosition(float& x, float& y)
{
	x = mouse_delta_x;
	y = mouse_delta_y;
}

Input* GetInput(GLFWwindow* window)
{
	return (Input*)glfwGetWindowUserPointer(window);
}
