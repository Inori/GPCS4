#include "ScePad.h"
#include "sce_errors.h"
#include "sce_pad_error.h"

#include "Platform/UtilProcess.h"

#include "../../Graphic/GraphicShared.h"
#include "../../Graphic/Sce/SceVideoOut.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>

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

int SceGamepad::getInformation(ScePadControllerInformation* info)
{
	throw std::logic_error("The method or operation is not implemented.");
}

int SceGamepad::setLightBar(int32_t handle, const ScePadLightBarParam* pParam)
{
	throw std::logic_error("The method or operation is not implemented.");
}

int SceGamepad::resetLightBar(int32_t handle)
{
	throw std::logic_error("The method or operation is not implemented.");
}

int SceGamepad::setVibration(int32_t handle, const ScePadVibrationParam* pParam)
{
	throw std::logic_error("The method or operation is not implemented.");
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
	// 2020/2/14 13:03 UTC  DummyCatz: Retro City RamPage calls this
	// function, but it is not implemented and pops illegal instruction.
	// I just make a dummy implemention for it in order to continue running.
	LOG_SCE_DUMMY_IMPL();
	*data = {};
	data->connected = true;

	return SCE_OK;
}


int SceKeyboard::readState(ScePadData* data)
{
	// TODO:
	// Just quick and dirty implement currently. :)

	uint32_t buttons = 0;
	if (glfwGetKey(m_window, GLFW_KEY_T) == GLFW_PRESS)
	{
		buttons |= SCE_PAD_BUTTON_UP;
	}

	if (glfwGetKey(m_window, GLFW_KEY_G) == GLFW_PRESS)
	{
		buttons |= SCE_PAD_BUTTON_DOWN;
	}

	if (glfwGetKey(m_window, GLFW_KEY_F) == GLFW_PRESS)
	{
		buttons |= SCE_PAD_BUTTON_LEFT;
	}

	if (glfwGetKey(m_window, GLFW_KEY_H) == GLFW_PRESS)
	{
		buttons |= SCE_PAD_BUTTON_RIGHT;
	}

	if (glfwGetKey(m_window, GLFW_KEY_I) == GLFW_PRESS)
	{
		buttons |= SCE_PAD_BUTTON_TRIANGLE;
	}

	if (glfwGetKey(m_window, GLFW_KEY_K) == GLFW_PRESS)
	{
		buttons |= SCE_PAD_BUTTON_CROSS;
	}

	if (glfwGetKey(m_window, GLFW_KEY_J) == GLFW_PRESS)
	{
		buttons |= SCE_PAD_BUTTON_SQUARE;
	}

	if (glfwGetKey(m_window, GLFW_KEY_L) == GLFW_PRESS)
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


	ScePadAnalogButtons analogButtons = {0, 0, { 2, 2 } };

	if (glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		buttons |= SCE_PAD_BUTTON_L1;
	}

	if (glfwGetKey(m_window, GLFW_KEY_E) == GLFW_PRESS)
	{
		buttons |= SCE_PAD_BUTTON_L2;
		analogButtons.l2 = 255;
	}

	if (glfwGetKey(m_window, GLFW_KEY_X) == GLFW_PRESS)
	{
		buttons |= SCE_PAD_BUTTON_L3;
	}


	if (glfwGetKey(m_window, GLFW_KEY_U) == GLFW_PRESS)
	{
		buttons |= SCE_PAD_BUTTON_R1;
	}

	if (glfwGetKey(m_window, GLFW_KEY_O) == GLFW_PRESS)
	{
		buttons |= SCE_PAD_BUTTON_R2;
		analogButtons.r2 = 255;
	}

	if (glfwGetKey(m_window, GLFW_KEY_M) == GLFW_PRESS)
	{
		buttons |= SCE_PAD_BUTTON_R3;
	}

	LOG_DEBUG_IF(buttons != 0, "pad buttons %08X", buttons);


	ScePadAnalogStick leftStick = {0x80,0x80};

	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) 
	{
		leftStick.x += -127;
	}

	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) 
	{
		leftStick.x += 127;
	}

	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) 
	{
		leftStick.y += -127;
	}

	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) 
	{
		leftStick.y += 127;
	}


	ScePadAnalogStick rightStick = {0x80,0x80};

	if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS) 
	{
		rightStick.x += -127;
	}

	if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS) 
	{
		rightStick.x += 127;
	}

	if (glfwGetKey(m_window, GLFW_KEY_UP) == GLFW_PRESS) 
	{
		rightStick.y += -127;
	}

	if (glfwGetKey(m_window, GLFW_KEY_DOWN) == GLFW_PRESS) 
	{
		rightStick.y += 127;
	}


	memset(data, 0, sizeof(ScePadData));
	data->connected      = true;
	data->connectedCount = 1;
	data->timestamp      = UtilProcess::GetProcessTimeCounter();
	data->buttons        = buttons;
	data->leftStick      = leftStick;
	data->rightStick     = rightStick;
	data->analogButtons  = analogButtons;

	return SCE_OK;
}

int SceKeyboard::getInformation(ScePadControllerInformation* info)
{
	int ret = SCE_PAD_ERROR_INVALID_ARG;
	do 
	{
		if (!info)
		{
			break;
		}

		memset(info, 0, sizeof(ScePadControllerInformation));
		info->touchPadInfo.pixelDensity = 1;
		info->touchPadInfo.resolution.x = 256;
		info->touchPadInfo.resolution.y = 256;
		info->stickInfo.deadZoneLeft    = 2;
		info->stickInfo.deadZoneRight   = 2;

		info->connectionType = SCE_PAD_CONNECTION_TYPE_LOCAL;
		info->connectedCount = 1;
		info->connected      = true;
		info->deviceClass    = SCE_PAD_DEVICE_CLASS_STANDARD;

		ret = SCE_OK;
	} while (false);
	return ret;
}

int SceKeyboard::setLightBar(int32_t handle, const ScePadLightBarParam* pParam)
{
	return SCE_OK;
}

int SceKeyboard::resetLightBar(int32_t handle)
{
	return SCE_OK;
}

int SceKeyboard::setVibration(int32_t handle, const ScePadVibrationParam* pParam)
{
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

int ScePad::getInformation(ScePadControllerInformation* info)
{
	return m_controller->getInformation(info);
}

int ScePad::setLightBar(int32_t handle, const ScePadLightBarParam* pParam)
{
	return m_controller->setLightBar(handle, pParam);
}

int ScePad::resetLightBar(int32_t handle)
{
	return m_controller->resetLightBar(handle);
}

int ScePad::setVibration(int32_t handle, const ScePadVibrationParam* pParam)
{
	return m_controller->setVibration(handle, pParam);
}

