#include "ScePad.h"


ScePad::ScePad(SceUserServiceUserId userId, int32_t type, int32_t index):
	m_userId(userId),
	m_type(type),
	m_index(index)
{

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

}

int ScePad::readState(ScePadData* data)
{

}
