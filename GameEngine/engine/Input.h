#ifndef INPUT__H
#define INPUT__H

#ifdef _WIN32
#include<Windows.h>
#endif

#include <list>
#include <vector>
#include <string>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

enum KeyBoard_key {
	KEY_Q = 0, KEY_A, KEY_Z,
	KEY_W, KEY_S, KEY_X,
	KEY_E, KEY_D, KEY_C,
	KEY_R, KEY_F, KEY_V,
	KEY_T, KEY_G, KEY_B,
	KEY_Y, KEY_H, KEY_N,
	KEY_U, KEY_J, KEY_M,
	KEY_I, KEY_K,
	KEY_O, KEY_L,
	KEY_P,

	KEY_1, KEY_6,
	KEY_2, KEY_7,
	KEY_3, KEY_8,
	KEY_4, KEY_9,
	KEY_5, KEY_0,

	KEY_ARR_DOWN, KEY_ARR_UP,
	KEY_ARR_LEFT, KEY_ARR_RIGHT,

	KEY_L_MAYUS,
	KEY_R_MAYUS,
	KEY_CNTRL,
	KEY_SPACE,
	KEY_ENTER,
	KEY_L_ALT,
	KEY_BACKSPACE,

	MAX_KEYS
};

enum MouseButton {
	MOUSE_LEFT = 0,
	MOUSE_RIGHT,
	MOUSE_MIDDLE,

	MAX_MOUSE_BUTTONS,
};

enum KeyState {
	KEY_PRESSED,
	KEY_DOWN,
	KEY_RELEASED
};

class Input 
{
private:
	KeyState Keyboard_keys[MAX_KEYS];
	KeyState Mouse_keys[MAX_MOUSE_BUTTONS];

public:

	Input();
	
	bool Initialize(GLFWwindow* window);

	static void GLFW_KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void GLFW_MouseMoveCallback(GLFWwindow* window,double xpos, double ypos);
	static void GLFW_MouseButtonCallback(GLFWwindow* window, int key, int action, int mods);
	static void GLFW_FocusWindowCallback(GLFWwindow* window, int focused);

	bool ShutDown();

	float mouse_x = 0;
	float mouse_y = 0;

	float mouse_delta_x = 0;
	float mouse_delta_y = 0;

	KeyState GetKey(KeyBoard_key);
	KeyState GetMouseKey(MouseButton);
	void GetMousePosition(float&x,float&y);
	void GetMouseDeltaPosition(float& x, float& y);
};

Input* GetInput(GLFWwindow* window);

#endif // !ENGINE__H
