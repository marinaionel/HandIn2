#pragma once
extern "C" {
#include "FreeRTOS.h"
}
#include "gmock.h"

namespace freertos {
	class FreeRTOSInterface {
	public:
		virtual ~FreeRTOSInterface() {}
		virtual void* pvPortMalloc(int size) = 0;
	};

	class FreeRTOSMock : public FreeRTOSInterface {
	public:
		virtual ~FreeRTOSMock() {}
		MOCK_METHOD1(pvPortMalloc, void* (int size));
	};
}