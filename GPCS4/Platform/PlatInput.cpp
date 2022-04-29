#include "PlatInput.h"

namespace plat
{
	InputDevice::~InputDevice()
	{
	}



#ifdef GPCS4_WINDOWS

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef WIN32_LEAN_AND_MEAN
#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

	class DirectInputDevice : public InputDevice
	{
		constexpr static uint32_t KeyPressMask = 0x80;
	public:
		DirectInputDevice()
		{
			initialize();
		}

		virtual ~DirectInputDevice()
		{
			shutdown();
		}

		KeyState getKeyState(KeyCode key) override
		{
			readKeyboard();

			uint32_t code = static_cast<uint32_t>(key);
			return m_keyboardState[code] & KeyPressMask ? 
				KeyState::Press : KeyState::Release;
		}

	private:
		void initialize()
		{
			HRESULT result = -1;

			do 
			{
				// Initialize the main direct input interface.
				result = DirectInput8Create(GetModuleHandleA(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
				if (FAILED(result))
				{
					break;
				}

				// Initialize the direct input interface for the keyboard.
				result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
				if (FAILED(result))
				{
					break;
				}

				// Set the data format.  In this case since it is a keyboard we can use the predefined data format.
				result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
				if (FAILED(result))
				{
					break;
				}

				// We don't need to set cooperative level

				// Set the cooperative level of the keyboard to not share with other programs.
				// result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
				// if (FAILED(result))
				//{
				//    break;
				//}

				// Now acquire the keyboard.
				result = m_keyboard->Acquire();
				if (FAILED(result))
				{
					break;
				}
			} while (false);
		}

		void shutdown()
		{
			// Release the keyboard.
			if (m_keyboard)
			{
				m_keyboard->Unacquire();
				m_keyboard->Release();
				m_keyboard = nullptr;
			}

			// Release the main interface to direct input.
			if (m_directInput)
			{
				m_directInput->Release();
				m_directInput = nullptr;
			}
		}

		void readKeyboard()
		{
			HRESULT result = -1;
			// Read the keyboard device.
			result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
			if (FAILED(result))
			{
				// If the keyboard lost focus or was not acquired then try to get control back.
				if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
				{
					m_keyboard->Acquire();
				}
			}
		}

	private:
		IDirectInput8*       m_directInput = nullptr;
		IDirectInputDevice8* m_keyboard    = nullptr;

		unsigned char m_keyboardState[256];
	};



	//////////////////////////////////////////////////

	std::unique_ptr<InputDevice> createInputeDevice()
	{
		return std::make_unique<DirectInputDevice>();
	}


#else

	InputDevice* createInputeDevice()
	{
	}

	void destoryInputDevice(InputDevice* device)
	{
	}

#endif



}  // namespace plat