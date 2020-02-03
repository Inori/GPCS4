#include "ScePad.h"
#include "sce_errors.h"

#include "Platform/UtilProcess.h"

#include "../../Graphic/GraphicShared.h"
#include "../../Graphic/Sce/SceVideoOut.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

LOG_CHANNEL(SceModules.ScePad);

SceInputController::~SceInputController()
{
}



SceGamepad::SceGamepad()
{
}

SceGamepad::~SceGamepad()
{
}


int SceGamepad::read(ScePadData* data, int32_t num)
{
	
}


int SceGamepad::readState(ScePadData* data)
{
}

SceKeyboard::SceKeyboard()
{
	auto videoOut = getVideoOut(SCE_VIDEO_HANDLE_MAIN);
	m_window      = videoOut->getWindowHandle();
}

SceKeyboard::~SceKeyboard()
{
}

int SceKeyboard::read(ScePadData* data, int32_t num)
{
	
}


int SceKeyboard::readState(ScePadData* data)
{
	// TODO:
	// Just quick and dirty implement currently. :)

	uint32_t buttons = 0;
	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
	{
		buttons |= SCE_PAD_BUTTON_UP;
	}

	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
	{
		buttons |= SCE_PAD_BUTTON_DOWN;
	}

	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
	{
		buttons |= SCE_PAD_BUTTON_LEFT;
	}

	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
	{
		buttons |= SCE_PAD_BUTTON_RIGHT;
	}

	if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		buttons |= SCE_PAD_BUTTON_TRIANGLE;
	}

	if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		buttons |= SCE_PAD_BUTTON_CROSS;
	}

	if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		buttons |= SCE_PAD_BUTTON_SQUARE;
	}

	if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		buttons |= SCE_PAD_BUTTON_CIRCLE;
	}

	if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		buttons |= SCE_PAD_BUTTON_TOUCH_PAD;
	}

	if (glfwGetKey(m_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		buttons |= SCE_PAD_BUTTON_OPTIONS;
	}

	LOG_DEBUG_IF(buttons != 0, "pad buttons %08X", buttons);

	memset(data, 0, sizeof(ScePadData));
	data->connected      = true;
	data->connectedCount = 1;
	data->timestamp      = UtilProcess::GetProcessTimeCounter();
	data->buttons        = buttons;

	return SCE_OK;
}

ScePad::ScePad(SceUserServiceUserId userId, int32_t type, int32_t index):
	m_userId(userId),
	m_type(type),
	m_index(index)
{
	// TODO:
	// use somewhat config to decide which one to use
	m_controller = std::make_unique<SceKeyboard>();
	//m_controller = std::make_unique<SceGamepad>();
}

ScePad::~ScePad()
{
}

SceUserServiceUserId ScePad::userId() const
{
	return m_userId;
}

int32_t ScePad::type() const
{
	return m_type;
}

int ScePad::read(ScePadData* data, int32_t num)
{
	return m_controller->read(data, num);
}

int ScePad::readState(ScePadData* data)
{
	return m_controller->readState(data);
}

