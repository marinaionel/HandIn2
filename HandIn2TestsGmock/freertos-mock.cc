extern "C" {
#include "freertos-mock.h"
#include "FreeRTOS.h"
}
extern freertos::FreeRTOSMock freertosmock;

void* pvPortMalloc(int size)
{
	return freertosmock.pvPortMalloc(size);
}