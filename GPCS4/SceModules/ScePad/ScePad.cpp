#include "ScePad.h"
#include "sce_errors.h"

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
}

SceKeyboard::~SceKeyboard()
{
}

int SceKeyboard::read(ScePadData* data, int32_t num)
{
	
}


int SceKeyboard::readState(ScePadData* data)
{
	LOG_SCE_DUMMY_IMPL();
	memset(data, 0, sizeof(ScePadData));
	data->connected = false;
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

