#pragma once

#include "GPCS4Common.h"
#include "sce_types.h"
#include "sce_pad_types.h"

#include <memory>


class SceInputController
{
public:

	virtual ~SceInputController() = 0;

	virtual int read(ScePadData* data, int32_t num) = 0;
};


class SceGamepad : public SceInputController
{
public:
	SceGamepad();
	virtual ~SceGamepad();

	virtual int read(ScePadData* data, int32_t num) override;

private:

};


class SceKeyboard : public SceInputController
{
public:
	SceKeyboard();
	virtual ~SceKeyboard();

	virtual int read(ScePadData* data, int32_t num) override;
private:

};



class ScePad
{
public:
	ScePad(SceUserServiceUserId userId, int32_t type, int32_t index);
	~ScePad();

	SceUserServiceUserId userId() const;

	int32_t type() const;

	int read(ScePadData* data, int32_t num);

	int readState(ScePadData* data);
private:
	SceUserServiceUserId m_userId;
	int32_t m_type;
	int32_t m_index;
	std::unique_ptr<SceInputController> m_controller;
};

