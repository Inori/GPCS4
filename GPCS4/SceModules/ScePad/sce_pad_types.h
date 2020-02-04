#pragma once

#include "GPCS4Common.h"

enum ScePadDeviceClass
{
	SCE_PAD_DEVICE_CLASS_INVALID        = -1,
	SCE_PAD_DEVICE_CLASS_STANDARD       = 0,
	SCE_PAD_DEVICE_CLASS_GUITAR         = 1,
	SCE_PAD_DEVICE_CLASS_DRUM           = 2,
	SCE_PAD_DEVICE_CLASS_DJ_TURNTABLE   = 3,
	SCE_PAD_DEVICE_CLASS_DANCEMAT       = 4,
	SCE_PAD_DEVICE_CLASS_NAVIGATION     = 5,
	SCE_PAD_DEVICE_CLASS_STEERING_WHEEL = 6,
	SCE_PAD_DEVICE_CLASS_STICK          = 7,
	SCE_PAD_DEVICE_CLASS_FLIGHT_STICK   = 8,
	SCE_PAD_DEVICE_CLASS_GUN            = 9,
};

#define SCE_PAD_BUTTON_START SCE_PAD_BUTTON_OPTIONS


enum ScePadButtonDataOffset
{
	SCE_PAD_BUTTON_L3          = 0x00000002,
	SCE_PAD_BUTTON_R3          = 0x00000004,
	SCE_PAD_BUTTON_OPTIONS     = 0x00000008,
	SCE_PAD_BUTTON_UP          = 0x00000010,
	SCE_PAD_BUTTON_RIGHT       = 0x00000020,
	SCE_PAD_BUTTON_DOWN        = 0x00000040,
	SCE_PAD_BUTTON_LEFT        = 0x00000080,
	SCE_PAD_BUTTON_L2          = 0x00000100,
	SCE_PAD_BUTTON_R2          = 0x00000200,
	SCE_PAD_BUTTON_L1          = 0x00000400,
	SCE_PAD_BUTTON_R1          = 0x00000800,
	SCE_PAD_BUTTON_TRIANGLE    = 0x00001000,
	SCE_PAD_BUTTON_CIRCLE      = 0x00002000,
	SCE_PAD_BUTTON_CROSS       = 0x00004000,
	SCE_PAD_BUTTON_SQUARE      = 0x00008000,
	SCE_PAD_BUTTON_TOUCH_PAD   = 0x00100000,
	SCE_PAD_BUTTON_INTERCEPTED = 0x80000000,
};


struct SceFQuaternion
{
	float x, y, z, w;
};



struct SceFVector3
{
	float x, y, z;
};



struct ScePadAnalogButtons
{
	uint8_t l2;
	uint8_t r2;
	uint8_t padding[2];
};



struct ScePadAnalogStick
{
	uint8_t x;
	uint8_t y;
};


struct ScePadStickInformation
{
	uint8_t deadZoneLeft;
	uint8_t deadZoneRight;
};



struct ScePadTouch
{
	uint16_t x;
	uint16_t y;
	uint8_t id;
	uint8_t reserve[3];
};



#define SCE_PAD_MAX_TOUCH_NUM	2

struct ScePadTouchData
{
	uint8_t touchNum;
	uint8_t reserve[3];
	uint32_t reserve1;
	ScePadTouch touch[SCE_PAD_MAX_TOUCH_NUM];
};



struct ScePadTouchPadInformation
{
	float pixelDensity;

	struct 
	{
		uint16_t x;
		uint16_t y;
	} resolution;
};



struct ScePadVibrationParam
{
	uint8_t largeMotor;
	uint8_t smallMotor;
};



struct ScePadExtensionUnitData
{
	uint32_t extensionUnitId;
	uint8_t reserve[1];
	uint8_t dataLength;
	uint8_t data[10];
};




struct ScePadControllerInformation
{
	ScePadTouchPadInformation touchPadInfo;
	ScePadStickInformation stickInfo;
	uint8_t connectionType;
	uint8_t connectedCount;
	bool connected;
	ScePadDeviceClass deviceClass;
	uint8_t reserve[8];
};

#define SCE_PAD_CONNECTION_TYPE_LOCAL              0
#define SCE_PAD_CONNECTION_TYPE_REMOTE_VITA        1
#define SCE_PAD_CONNECTION_TYPE_REMOTE_DUALSHOCK4  2



#define SCE_PAD_MAX_DEVICE_UNIQUE_DATA_SIZE		12

struct ScePadData
{
	uint32_t buttons;
	ScePadAnalogStick leftStick;
	ScePadAnalogStick rightStick;
	ScePadAnalogButtons analogButtons;
	SceFQuaternion orientation;
	SceFVector3 acceleration;
	SceFVector3 angularVelocity;
	ScePadTouchData touchData;
	bool connected;
	uint64_t timestamp;
	ScePadExtensionUnitData extensionUnitData;
	uint8_t connectedCount;
	uint8_t reserve[2];
	uint8_t deviceUniqueDataLen;
	uint8_t deviceUniqueData[SCE_PAD_MAX_DEVICE_UNIQUE_DATA_SIZE];
};



struct ScePadDeviceClassData
{
	ScePadDeviceClass deviceClass;
	bool bDataValid;
	union
	{
		struct
		{
			float steeringWheelAngle;
			uint16_t steeringWheel;
			uint16_t acceleratorPedal;
			uint16_t brakePedal;
			uint16_t clutchPedal;
			uint16_t handBrake;
			uint8_t gear;
			uint8_t reserved[1];
		} steeringWheel;

		struct
		{
			uint8_t toneNumber;
			uint8_t whammyBar;
			uint8_t tilt;
			uint8_t fret;
			uint8_t fretSolo;
			uint8_t reserved[11];
		} guitar;

		struct
		{
			uint8_t snare;
			uint8_t tom1;
			uint8_t tom2;
			uint8_t floorTom;
			uint8_t hihatCymbal;
			uint8_t rideCymbal;
			uint8_t crashCymbal;
			uint8_t reserved[9];
		} drum;

		struct
		{
			uint16_t stickAxisX;
			uint16_t stickAxisY;
			uint8_t stickTwist;
			uint8_t throttle;
			uint8_t trigger;
			uint8_t rudderPedal;
			uint8_t brakePedalLeft;
			uint8_t brakePedalRight;
			uint8_t antennaKnob;
			uint8_t rangeKnob;
			uint8_t reserved[4];
		} flightStick;

		struct
		{
			uint8_t dataLen;
			uint8_t reserved[3];
			uint8_t data[SCE_PAD_MAX_DEVICE_UNIQUE_DATA_SIZE];
		} others;
	} classData;
};



struct ScePadDeviceClassExtendedInformation
{
	ScePadDeviceClass deviceClass;
	uint8_t reserved[4];
	union
	{
		struct
		{
			uint8_t capability;
			uint8_t reserved1[1];
			uint16_t maxPhysicalWheelAngle;
			uint8_t reserved2[8];
		} steeringWheel;

		struct
		{
			uint8_t capability;
			uint8_t quantityOfSelectorSwitch;
			uint8_t reserved[10];
		} guitar;

		struct
		{
			uint8_t capability;
			uint8_t reserved[11];
		} drum;

		struct
		{
			uint8_t capability;
			uint8_t reserved[11];
		} flightStick;

		uint8_t data[12];
	} classData;
};



struct ScePadColor
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t reserve[1];
};

typedef ScePadColor ScePadLightBarParam;


struct ScePadOpenParam
{
	uint8_t reserve[8];
};

#define SCE_PAD_PORT_TYPE_STANDARD			0 	
#define SCE_PAD_PORT_TYPE_SPECIAL			2	
#define SCE_PAD_PORT_TYPE_REMOTE_CONTROL	16
