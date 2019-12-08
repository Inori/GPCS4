#include "ScePad.h"


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


SceKeyboard::SceKeyboard()
{
}

SceKeyboard::~SceKeyboard()
{
}

int SceKeyboard::read(ScePadData* data, int32_t num)
{
	
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

}

