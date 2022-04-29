#include "ScePad.h"
#include "sce_errors.h"
#include "sce_pad_error.h"
#include "Platform/PlatProcess.h"

#include <stdexcept>

using namespace plat;

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

SceKeyboard::SceKeyboard():
	m_device(plat::createInputeDevice())
{
	
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
	if (m_device->getKeyState(KeyCode::KEY_T) == KeyState::Press)
	{
		buttons |= SCE_PAD_BUTTON_UP;
	}

	if (m_device->getKeyState(KeyCode::KEY_G) == KeyState::Press)
	{
		buttons |= SCE_PAD_BUTTON_DOWN;
	}

	if (m_device->getKeyState(KeyCode::KEY_F) == KeyState::Press)
	{
		buttons |= SCE_PAD_BUTTON_LEFT;
	}

	if (m_device->getKeyState(KeyCode::KEY_H) == KeyState::Press)
	{
		buttons |= SCE_PAD_BUTTON_RIGHT;
	}

	if (m_device->getKeyState(KeyCode::KEY_I) == KeyState::Press)
	{
		buttons |= SCE_PAD_BUTTON_TRIANGLE;
	}

	if (m_device->getKeyState(KeyCode::KEY_K) == KeyState::Press)
	{
		buttons |= SCE_PAD_BUTTON_CROSS;
	}

	if (m_device->getKeyState(KeyCode::KEY_J) == KeyState::Press)
	{
		buttons |= SCE_PAD_BUTTON_SQUARE;
	}

	if (m_device->getKeyState(KeyCode::KEY_L) == KeyState::Press)
	{
		buttons |= SCE_PAD_BUTTON_CIRCLE;
	}

	if (m_device->getKeyState(KeyCode::KEY_SPACE) == KeyState::Press)
	{
		buttons |= SCE_PAD_BUTTON_TOUCH_PAD;
	}

	if (m_device->getKeyState(KeyCode::KEY_LCONTROL) == KeyState::Press)
	{
		buttons |= SCE_PAD_BUTTON_OPTIONS;
	}

	ScePadAnalogButtons analogButtons = { 0, 0, { 2, 2 } };

	if (m_device->getKeyState(KeyCode::KEY_Q) == KeyState::Press)
	{
		buttons |= SCE_PAD_BUTTON_L1;
	}

	if (m_device->getKeyState(KeyCode::KEY_E) == KeyState::Press)
	{
		buttons |= SCE_PAD_BUTTON_L2;
		analogButtons.l2 = 255;
	}

	if (m_device->getKeyState(KeyCode::KEY_X) == KeyState::Press)
	{
		buttons |= SCE_PAD_BUTTON_L3;
	}

	if (m_device->getKeyState(KeyCode::KEY_U) == KeyState::Press)
	{
		buttons |= SCE_PAD_BUTTON_R1;
	}

	if (m_device->getKeyState(KeyCode::KEY_O) == KeyState::Press)
	{
		buttons |= SCE_PAD_BUTTON_R2;
		analogButtons.r2 = 255;
	}

	if (m_device->getKeyState(KeyCode::KEY_M) == KeyState::Press)
	{
		buttons |= SCE_PAD_BUTTON_R3;
	}

	LOG_DEBUG_IF(buttons != 0, "pad buttons %08X", buttons);

	ScePadAnalogStick leftStick = { 0x80, 0x80 };

	if (m_device->getKeyState(KeyCode::KEY_A) == KeyState::Press)
	{
		leftStick.x += -127;
	}

	if (m_device->getKeyState(KeyCode::KEY_D) == KeyState::Press)
	{
		leftStick.x += 127;
	}

	if (m_device->getKeyState(KeyCode::KEY_W) == KeyState::Press)
	{
		leftStick.y += -127;
	}

	if (m_device->getKeyState(KeyCode::KEY_S) == KeyState::Press)
	{
		leftStick.y += 127;
	}

	ScePadAnalogStick rightStick = { 0x80, 0x80 };

	if (m_device->getKeyState(KeyCode::KEY_LEFT) == KeyState::Press)
	{
		rightStick.x += -127;
	}

	if (m_device->getKeyState(KeyCode::KEY_RIGHT) == KeyState::Press)
	{
		rightStick.x += 127;
	}

	if (m_device->getKeyState(KeyCode::KEY_UP) == KeyState::Press)
	{
		rightStick.y += -127;
	}

	if (m_device->getKeyState(KeyCode::KEY_DOWN) == KeyState::Press)
	{
		rightStick.y += 127;
	}

	memset(data, 0, sizeof(ScePadData));
	data->connected      = true;
	data->connectedCount = 1;
	data->timestamp      = plat::GetProcessTimeCounter();
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

