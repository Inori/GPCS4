#pragma once

#include "GPCS4Common.h"
#include "sce_types.h"
#include "sce_pad_types.h"

#include <memory>

class GLFWwindow;

class SceInputController
{
public:

	virtual ~SceInputController() = 0;

	virtual int read(ScePadData* data, int32_t num) = 0;

	virtual int readState(ScePadData* data) = 0;

	virtual int getInformation(ScePadControllerInformation* info) = 0;

	virtual int setLightBar(int32_t handle, const ScePadLightBarParam* pParam) = 0;

	virtual int resetLightBar(int32_t handle) = 0;

	virtual int setVibration(int32_t handle, const ScePadVibrationParam* pParam) = 0;
};


class SceGamepad : public SceInputController
{
public:
	SceGamepad();
	virtual ~SceGamepad();

	virtual int read(ScePadData* data, int32_t num) override;

	virtual int readState(ScePadData* data) override;

	virtual int getInformation(ScePadControllerInformation* info) override;

	virtual int setLightBar(int32_t handle, const ScePadLightBarParam* pParam) override;

	virtual int resetLightBar(int32_t handle) override;

	virtual int setVibration(int32_t handle, const ScePadVibrationParam* pParam) override;

 private:

};


class SceKeyboard : public SceInputController
{
public:
	SceKeyboard();
	virtual ~SceKeyboard();

	virtual int read(ScePadData* data, int32_t num) override;

	virtual int readState(ScePadData* data) override;

	virtual int getInformation(ScePadControllerInformation* info) override;

 	virtual int setLightBar(int32_t handle, const ScePadLightBarParam* pParam) override;

 	virtual int resetLightBar(int32_t handle) override;

 	virtual int setVibration(int32_t handle, const ScePadVibrationParam* pParam) override;

 private:
	GLFWwindow* m_window = nullptr;
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

	int getInformation(ScePadControllerInformation* info);

	int setLightBar(int32_t handle, const ScePadLightBarParam* pParam);

	int resetLightBar(int32_t handle);

	int setVibration(int32_t handle, const ScePadVibrationParam* pParam);

private:
	SceUserServiceUserId m_userId;
	int32_t m_type;
	int32_t m_index;
	std::unique_ptr<SceInputController> m_controller;
};

