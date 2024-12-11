#include "device_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LEN 50

// Opaque structures
typedef struct Device {
    char name[NAME_LEN];
    DeviceType type;
    int id;
    bool state; // ON/OFF
    int attribute; // Generic attribute (e.g., brightness, temperature)
    struct Device* next;
} Device;


struct DeviceManager {
    Device* head;
};

// Create and destroy device manager
DeviceManager* device_manager_create(void) {
    DeviceManager* manager = (DeviceManager*)malloc(sizeof(DeviceManager));
    manager->head = NULL;
    return manager;
}

void device_manager_destroy(DeviceManager* manager) {
    Device* current = manager->head;
    while (current) {
        Device* next = current->next;
        free(current);
        current = next;
    }
    free(manager);
}

// Add a new device
bool device_manager_add_device(DeviceManager* manager, const char* name, DeviceType type, int ident) {
    Device* new_device = (Device*)malloc(sizeof(Device));
    if (!new_device || (name == NULL || name[0] == '\0')) {
        return false;
    }

    if (ident < 0) {
        free(new_device);
        return false;
    }
    strncpy(new_device->name, name , sizeof(new_device->name) - 1);
    new_device->name[sizeof(new_device->name) - 1] = '\0'; // Ensure null-termination
    new_device->type = type;
    new_device->id = ident;
    new_device->state = false; // Default OFF
    new_device->attribute = 0; // Default attribute
    new_device->next = manager->head;
    manager->head = new_device;

    return true;
}

const char* device_manager_get_device_name(DeviceManager* manager, int ident, const char* name) {
    Device* current = manager->head;
    while (current) {
        if (current->id == ident && strcmp(current->name, name) == 0) {
            return current->name;
        }
        current = current->next;
    }
    return NULL;
}

DeviceType device_manager_get_device_type(DeviceManager* manager, int ident, const char* name) {
    Device* current = manager->head;
    while (current) {
        if (current->id == ident && strcmp(current->name, name) == 0) {
            return current->type;
        }
        current = current->next;
    }
    return DEVICE_LIGHT;
}

bool device_manager_get_device_state(DeviceManager* manager, int ident, const char* name) {
    Device* current = manager->head;
    while (current) {
        if (current->id == ident && strcmp(current->name, name) == 0) {
            return current->state;
        }
        current = current->next;
    }
    return false;
}

int device_manager_get_device_attribute(DeviceManager* manager, int ident) {
    Device* current = manager->head;
    while (current) {
        if (current->id == ident) {
            return current->attribute;
        }
        current = current->next;
    }
    return 0;
}


int device_manager_get_device_count(DeviceManager* manager) {
    int count = 0;
    Device* current = manager->head;
    while (current) {
        count++;
        current = current->next;
    }
    return count;
}

int get_device_id(DeviceManager* manager, const char* name) {
    Device* current = manager->head;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            return current->id;
        }
        current = current->next;
    }
    return -1;
}

bool device_manager_remove_device(DeviceManager* manager, int ident, const char* name) {
    Device* current = manager->head;
    Device* previous = NULL;

    while (current) {
        if (current->id == ident && strcmp(current->name, name) == 0) {
            if (previous) {
                previous->next = current->next;
            } else {
                manager->head = current->next;
            }
            free(current);
            return true;
        }
        previous = current;
        current = current->next;
    }
    return false;
}

// Set device state
bool device_manager_set_device_state(DeviceManager* manager, int ident, bool state) {
    Device* current = manager->head;
    while (current) {
        if (current->id == ident) {
            current->state = state;
            return true;
        }
        current = current->next;
    }
    return false;
}

// Set device attribute
bool device_manager_set_device_attribute(DeviceManager* manager, int ident, int value) {
    Device* current = manager->head;
    while (current) {
        if (current->id == ident) {
            current->attribute = value;
            return true;
        }
        current = current->next;
    }
    return false;
}

// List all devices
void device_manager_list_devices(DeviceManager* manager) {
    Device* current = manager->head;
    while (current) {
        printf("Device ID: %d, Name: %s, Type: %d, State: %s, Attribute: %d\n",
               current->id, current->name, current->type, current->state ? "ON" : "OFF", current->attribute);
        current = current->next;
    }
}

// Save and load functions omitted for brevity// Save the device manager state to a file
bool device_manager_save(DeviceManager* manager, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file || !manager) {
        return false;
    }

    Device* current = manager->head;
    while (current) {
        fprintf(file, "%d %s %d %d %d\n", current->id, current->name, current->type, current->state, current->attribute);
        current = current->next;
    }

    fclose(file);
    return true;
}

// Load the device manager state from a file
DeviceManager* device_manager_load(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        return NULL;
    }

    DeviceManager* manager = device_manager_create();
    int ident = 0;
    int type = 0;
    int state = 0;
    int attribute = 0;

    char name[NAME_LEN];

    while (fscanf(file, "%d %49s %d %d %d", &ident, name, &type, &state, &attribute) == 5) {
        device_manager_add_device(manager, name, (DeviceType)type, ident);
        device_manager_set_device_state(manager, ident, state);
        device_manager_set_device_attribute(manager, ident,  attribute);
    }

    fclose(file);
    return manager;
}
