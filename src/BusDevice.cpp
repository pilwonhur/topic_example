#include "BusDevice.h"

/**
 * Constructor for a generic bus device
 * @param bus the bus number
 * @param device the device number
 */
BusDevice::BusDevice(unsigned int bus, unsigned int device) {
	this->bus = bus;
	this->device = device;
	this->file=-1;
}
 
/**
 * Destructor is unused
 */
BusDevice::~BusDevice() {}

