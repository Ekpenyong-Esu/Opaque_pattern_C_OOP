#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H

#include <stdbool.h>

// Forward declaration of DeviceManager for Opaque Pointer
typedef struct DeviceManager DeviceManager;

// Enum for device types
typedef enum {
    DEVICE_LIGHT,
    DEVICE_THERMOSTAT,
    DEVICE_CAMERA
} DeviceType;

// Create and destroy device manager
DeviceManager* device_manager_create(void);
void device_manager_destroy(DeviceManager* manager);

// Add and remove devices
bool device_manager_add_device(DeviceManager* manager, const char* name, DeviceType type, int ident);
bool device_manager_remove_device(DeviceManager* manager, int ident, const char* name);

// Control and query devices
bool device_manager_set_device_state(DeviceManager* manager, int ident, bool state);
bool device_manager_set_device_attribute(DeviceManager* manager, int ident, int value);
void device_manager_list_devices(DeviceManager* manager);
int device_manager_get_device_count(DeviceManager* manager);

// Save and load configuration
bool device_manager_save(DeviceManager* manager, const char* filename);
DeviceManager* device_manager_load(const char* filename);

// Additional functions to expose internal data for testing
const char* device_manager_get_device_name(DeviceManager* manager, int ident, const char* name);
DeviceType device_manager_get_device_type(DeviceManager* manager, int ident, const char* name);
bool device_manager_get_device_state(DeviceManager* manager, int ident, const char* name);
int device_manager_get_device_attribute(DeviceManager* manager, int ident);
DeviceManager* device_manager_get_device_head(DeviceManager* manager);
int get_device_id(DeviceManager* manager, const char* name);
#endif // DEVICE_MANAGER_H
