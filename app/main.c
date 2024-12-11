#include <stdio.h>
#include "device_manager.h"


int main(void) {
    DeviceManager* manager = device_manager_create();

    device_manager_add_device(manager, "LivingRoomLight", DEVICE_LIGHT, 1);
    device_manager_add_device(manager, "Thermostat", DEVICE_THERMOSTAT, 2);
    device_manager_add_device(manager, "FrontDoorCamera", DEVICE_CAMERA, 3);

    printf("Initial Device List:\n");
    device_manager_list_devices(manager);

    device_manager_set_device_state(manager, 1, true);
    device_manager_set_device_attribute(manager, 2,  72);

    printf("\nUpdated Device List:\n");
    device_manager_list_devices(manager);

    device_manager_save(manager, "devices.txt");
    device_manager_destroy(manager);

    printf("\nReloading Devices:\n");
    manager = device_manager_load("devices.txt");
    device_manager_list_devices(manager);

    device_manager_destroy(manager);
    return 0;
}
