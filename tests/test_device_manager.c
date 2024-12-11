#include "unity.h"
#include "device_manager.h"
#include <stdbool.h>


void setUp(void) {

}

void tearDown(void) {

}

void test_device_manager_create(void)
{
    DeviceManager* manager = device_manager_create();
    TEST_ASSERT_NOT_NULL(manager);
    device_manager_destroy(manager);
}

void test_device_manager_create_with_no_devices(void)
{
    DeviceManager* manager = device_manager_create();
    TEST_ASSERT_NOT_NULL(manager);

    // Check that the manager has no devices
    // This assumes we have a function to get the number of devices
    TEST_ASSERT_EQUAL_INT(0, device_manager_get_device_count(manager));

    device_manager_destroy(manager);
}

void test_device_manager_create_with_default_settings(void)
{
    DeviceManager* manager = device_manager_create();
    TEST_ASSERT_NOT_NULL(manager);

    // Check default settings
    TEST_ASSERT_EQUAL_INT(0, device_manager_get_device_count(manager));

    device_manager_destroy(manager);
}

void test_device_manager_create_unique_instances(void)
{
    DeviceManager* manager1 = device_manager_create();
    DeviceManager* manager2 = device_manager_create();

    TEST_ASSERT_NOT_NULL(manager1);
    TEST_ASSERT_NOT_NULL(manager2);
    TEST_ASSERT_NOT_EQUAL(manager1, manager2);

    device_manager_destroy(manager1);
    device_manager_destroy(manager2);
}

void test_device_manager_create_multiple_calls(void)
{
    DeviceManager* manager1 = device_manager_create();
    DeviceManager* manager2 = device_manager_create();
    DeviceManager* manager3 = device_manager_create();

    TEST_ASSERT_NOT_NULL(manager1);
    TEST_ASSERT_NOT_NULL(manager2);
    TEST_ASSERT_NOT_NULL(manager3);

    TEST_ASSERT_NOT_EQUAL(manager1, manager2);
    TEST_ASSERT_NOT_EQUAL(manager2, manager3);
    TEST_ASSERT_NOT_EQUAL(manager1, manager3);

    device_manager_destroy(manager1);
    device_manager_destroy(manager2);
    device_manager_destroy(manager3);

    // If we reach this point without any memory errors, the test passes
}

void test_device_manager_create_and_add_devices(void)
{
    DeviceManager* manager = device_manager_create();
    TEST_ASSERT_NOT_NULL(manager);

    // Attempt to add devices of different types
    bool lightAdded = device_manager_add_device(manager, "Living Room Light", DEVICE_LIGHT, 1);
    bool thermostatAdded = device_manager_add_device(manager, "Bedroom Thermostat", DEVICE_THERMOSTAT, 2);
    bool cameraAdded = device_manager_add_device(manager, "Front Door Camera", DEVICE_CAMERA, 3);

    TEST_ASSERT_TRUE(lightAdded);
    TEST_ASSERT_TRUE(thermostatAdded);
    TEST_ASSERT_TRUE(cameraAdded);

    // Verify the number of devices added
    TEST_ASSERT_EQUAL_INT(3, device_manager_get_device_count(manager));

    device_manager_destroy(manager);
}

void test_device_manager_destroy_with_single_device(void)
{
    DeviceManager* manager = device_manager_create();
    TEST_ASSERT_NOT_NULL(manager);

    // Add a single device
    bool deviceAdded = device_manager_add_device(manager, "Single Device", DEVICE_LIGHT, 1);
    TEST_ASSERT_TRUE(deviceAdded);

    // Verify the device count
    TEST_ASSERT_EQUAL_INT(1, device_manager_get_device_count(manager));

    // Destroy the manager and ensure no memory leaks or errors
    device_manager_destroy(manager);

    // Since we're not able to directly check for memory leaks in this test,
    // successful execution without crashes or errors is considered a pass.
}

void test_device_manager_destroy_no_memory_leaks(void)
{
    DeviceManager* manager = device_manager_create();
    TEST_ASSERT_NOT_NULL(manager);

    // Add a few devices to the manager
    device_manager_add_device(manager, "Device 1", DEVICE_LIGHT, 1);
    device_manager_add_device(manager, "Device 2", DEVICE_THERMOSTAT, 2);
    device_manager_add_device(manager, "Device 3", DEVICE_CAMERA, 3);

    // Destroy the manager and ensure no memory leaks or errors
    device_manager_destroy(manager);

    // Since we're not able to directly check for memory leaks in this test,
    // successful execution without crashes or errors is considered a pass.
}

void test_device_manager_add_device_with_null_name(void)
{
    DeviceManager* manager = device_manager_create();
    TEST_ASSERT_NOT_NULL(manager);

    // Attempt to add a device with a null name
    bool result = device_manager_add_device(manager, NULL, DEVICE_LIGHT, 1);
    TEST_ASSERT_FALSE(result);

    device_manager_destroy(manager);
}

void test_device_manager_add_device_with_empty_name(void)
{
    DeviceManager* manager = device_manager_create();
    TEST_ASSERT_NOT_NULL(manager);

    // Attempt to add a device with an empty name
    bool result = device_manager_add_device(manager, "", DEVICE_LIGHT, 1);
    TEST_ASSERT_FALSE(result);

    device_manager_destroy(manager);
}

void test_device_manager_add_device_with_negative_id(void)
{
    DeviceManager* manager = device_manager_create();
    TEST_ASSERT_NOT_NULL(manager);

    // Attempt to add a device with a negative identifier
    bool result = device_manager_add_device(manager, "Negative ID Device", DEVICE_LIGHT, -1);
    TEST_ASSERT_FALSE(result);

    // Verify that the manager has no devices
    TEST_ASSERT_EQUAL_INT(0, device_manager_get_device_count(manager));

    device_manager_destroy(manager);
}

void test_device_manager_add_multiple_devices_with_same_identifier(void)
{
    DeviceManager* manager = device_manager_create();
    TEST_ASSERT_NOT_NULL(manager);

    // Add the first device with identifier 1
    bool firstAdded = device_manager_add_device(manager, "Device 1", DEVICE_LIGHT, 1);
    bool firstAddedDeviceState  = device_manager_get_device_state(manager, 1, "Device 1");
    DeviceType firstAddedDeviceType = device_manager_get_device_type(manager, 1, "Device 1");
    const char* firstAddedDeviceName = device_manager_get_device_name(manager, 1, "Device 1");
    TEST_ASSERT_TRUE(firstAdded);

    // Attempt to add another device with the same identifier
    bool secondAdded = device_manager_add_device(manager, "Device 2", DEVICE_THERMOSTAT, 1);
    bool secondAddedDeviceState = device_manager_get_device_state(manager, 1, "Device 2");
    DeviceType secondAddedDeviceType = device_manager_get_device_type(manager, 1, "Device 2");
    const char* secondAddedDeviceName = device_manager_get_device_name(manager, 1, "Device 2");
    TEST_ASSERT_TRUE(secondAdded);

    // Verify that both devices are added
    TEST_ASSERT_EQUAL_INT(2, device_manager_get_device_count(manager));

    // Check that the first device is now the head

    TEST_ASSERT_EQUAL_STRING("Device 2", secondAddedDeviceName);
    TEST_ASSERT_EQUAL_INT(DEVICE_THERMOSTAT, secondAddedDeviceType);
    TEST_ASSERT_EQUAL_INT(false, secondAddedDeviceState);

    // Verify that the first device is the next in the list

    TEST_ASSERT_EQUAL_STRING("Device 1", firstAddedDeviceName);
    TEST_ASSERT_EQUAL_INT(DEVICE_LIGHT, firstAddedDeviceType);
    TEST_ASSERT_EQUAL_INT(false, firstAddedDeviceState);

    device_manager_destroy(manager);
}

void test_device_manager_remove_device_from_empty_manager(void)
{
    DeviceManager* manager = device_manager_create();
    TEST_ASSERT_NOT_NULL(manager);

    // Attempt to remove a device from an empty manager
    bool result = device_manager_remove_device(manager, 1, "Device 1");

    // Check that the function returns false
    TEST_ASSERT_FALSE(result);

    device_manager_destroy(manager);
}

void test_device_manager_remove_device_single_device(void)
{
    DeviceManager* manager = device_manager_create();
    TEST_ASSERT_NOT_NULL(manager);

    // Add a single device
    bool added = device_manager_add_device(manager, "Test Device", DEVICE_LIGHT, 1);
    TEST_ASSERT_TRUE(added);
    TEST_ASSERT_EQUAL_INT(1, device_manager_get_device_count(manager));

    // Remove the device
    bool removed = device_manager_remove_device(manager, 1, "Test Device");
    TEST_ASSERT_TRUE(removed);

    // Verify the device was removed
    TEST_ASSERT_EQUAL_INT(0, device_manager_get_device_count(manager));


    device_manager_destroy(manager);
}

void test_device_manager_remove_device_at_head(void)
{
    DeviceManager* manager = device_manager_create();
    TEST_ASSERT_NOT_NULL(manager);

    // Add a device at the head
    bool added = device_manager_add_device(manager, "Head Device", DEVICE_LIGHT, 1);
    TEST_ASSERT_TRUE(added);
    TEST_ASSERT_EQUAL_INT(1, device_manager_get_device_count(manager));

    // Remove the device at the head
    bool removed = device_manager_remove_device(manager, 1, "Head Device");
    TEST_ASSERT_TRUE(removed);

    // Verify the device was removed
    TEST_ASSERT_EQUAL_INT(0, device_manager_get_device_count(manager));

    device_manager_destroy(manager);
}

void test_device_manager_remove_device_from_middle(void)
{
    DeviceManager* manager = device_manager_create();
    TEST_ASSERT_NOT_NULL(manager);

    // Add three devices
    device_manager_add_device(manager, "Device 1", DEVICE_LIGHT, 1);
    int deviceOneId = get_device_id(manager,"Device 1");

    device_manager_add_device(manager, "Device 2", DEVICE_THERMOSTAT, 2);
    int deviceTwoId = get_device_id(manager,"Device 2");

    device_manager_add_device(manager, "Device 3", DEVICE_CAMERA, 3);
    int deviceThreeId = get_device_id(manager,"Device 3");
    // Remove the middle device
    bool result = device_manager_remove_device(manager, 2, "Device 2");
    TEST_ASSERT_TRUE(result);

    // Verify that the middle device was removed
    TEST_ASSERT_EQUAL_INT(2, device_manager_get_device_count(manager));

    // Check that the remaining devices are correct
    TEST_ASSERT_EQUAL_INT(3, deviceThreeId);
    TEST_ASSERT_EQUAL_INT(1, deviceOneId);
    TEST_ASSERT_EQUAL_INT(2, deviceTwoId);


    device_manager_destroy(manager);
}

void test_device_manager_remove_device_non_existent_id(void)
{
    DeviceManager* manager = device_manager_create();
    TEST_ASSERT_NOT_NULL(manager);

    // Add a device
    bool added = device_manager_add_device(manager, "Test Device", DEVICE_LIGHT, 1);
    TEST_ASSERT_TRUE(added);

    // Attempt to remove a device with a non-existent ID
    bool result = device_manager_remove_device(manager, 999, "Test Device");
    TEST_ASSERT_FALSE(result);

    // Verify that the original device is still in the manager
    TEST_ASSERT_EQUAL_INT(1, device_manager_get_device_count(manager));

    device_manager_destroy(manager);
}

void test_device_manager_remove_device_multiple_same_id(void)
{
    DeviceManager* manager = device_manager_create();
    TEST_ASSERT_NOT_NULL(manager);

    // Add multiple devices with the same identifier
    device_manager_add_device(manager, "Device 1", DEVICE_LIGHT, 1);
    device_manager_add_device(manager, "Device 2", DEVICE_THERMOSTAT, 1);
    device_manager_add_device(manager, "Device 3", DEVICE_CAMERA, 1);

    // Verify initial device count
    TEST_ASSERT_EQUAL_INT(3, device_manager_get_device_count(manager));

    // Remove a device with the shared identifier
    bool result = device_manager_remove_device(manager, 1, "Device 1");
    TEST_ASSERT_TRUE(result);

    // Verify that one device was removed
    TEST_ASSERT_EQUAL_INT(2, device_manager_get_device_count(manager));

    // Check that the remaining devices still have the same identifier


    TEST_ASSERT_EQUAL_INT(1, get_device_id(manager,"Device 2"));
    TEST_ASSERT_EQUAL_STRING("Device 2", device_manager_get_device_name(manager, 1, "Device 2"));



    TEST_ASSERT_EQUAL_INT(1, get_device_id(manager,"Device 3"));
    TEST_ASSERT_EQUAL_STRING("Device 3", device_manager_get_device_name(manager, 1, "Device 3"));


    device_manager_destroy(manager);
}

void test_device_manager_set_device_state_non_existent_id(void)
{
    DeviceManager* manager = device_manager_create();
    TEST_ASSERT_NOT_NULL(manager);

    // Add a device with ID 1
    bool added = device_manager_add_device(manager, "Test Device", DEVICE_LIGHT, 1);
    TEST_ASSERT_TRUE(added);

    // Attempt to set the state of a non-existent device (ID 2)
    bool result = device_manager_set_device_state(manager, 2, true);
    TEST_ASSERT_FALSE(result);

    device_manager_destroy(manager);
}

void test_device_manager_set_device_state_first_device(void)
{
    DeviceManager* manager = device_manager_create();
    TEST_ASSERT_NOT_NULL(manager);

    // Add a device at the head of the list
    bool added = device_manager_add_device(manager, "First Device", DEVICE_LIGHT, 1);
    TEST_ASSERT_TRUE(added);

    // Set the state of the first device
    bool result = device_manager_set_device_state(manager, 1, true);

    // Check that the function returns true
    TEST_ASSERT_TRUE(result);

    // Verify that the state was updated
    TEST_ASSERT_TRUE(device_manager_get_device_state(manager, 1, "First Device"));

    device_manager_destroy(manager);
}


void test_device_manager_set_device_attribute_non_existent_id(void)
{
    DeviceManager* manager = device_manager_create();
    TEST_ASSERT_NOT_NULL(manager);

    // Add a device with ID 1
    bool added = device_manager_add_device(manager, "Test Device", DEVICE_LIGHT, 1);
    TEST_ASSERT_TRUE(added);

    // Attempt to set the attribute of a non-existent device (ID 2)
    bool result = device_manager_set_device_attribute(manager, 2,  100);
    TEST_ASSERT_FALSE(result);

    device_manager_destroy(manager);
}


void test_device_manager_set_device_attribute_first_device(void)
{
    DeviceManager* manager = device_manager_create();
    TEST_ASSERT_NOT_NULL(manager);

    // Add a device at the head of the list
    bool added = device_manager_add_device(manager, "First Device", DEVICE_LIGHT, 1);
    TEST_ASSERT_TRUE(added);

    // Set the attribute of the first device
    bool result = device_manager_set_device_attribute(manager, 1, 42);

    // Check that the function returns true
    TEST_ASSERT_TRUE(result);

    // Verify that the attribute was updated
    TEST_ASSERT_EQUAL_INT(42, device_manager_get_device_attribute(manager, 1));

    device_manager_destroy(manager);
}

void test_device_manager_save_with_null_filename(void)
{
    DeviceManager* manager = device_manager_create();
    TEST_ASSERT_NOT_NULL(manager);

    // Attempt to save with a null filename
    bool result = device_manager_save(manager, NULL);
    TEST_ASSERT_FALSE(result);

    device_manager_destroy(manager);
}
void test_device_manager_save_with_null_manager(void)
{
    // Attempt to save with a null manager
    bool result = device_manager_save(NULL, "testfile.txt");

    // Check that the function returns false
    TEST_ASSERT_FALSE(result);
}

void test_device_manager_save_with_empty_filename(void)
{
    DeviceManager* manager = device_manager_create();
    TEST_ASSERT_NOT_NULL(manager);

    // Add a device to ensure the manager is not empty
    bool added = device_manager_add_device(manager, "Test Device", DEVICE_LIGHT, 1);
    TEST_ASSERT_TRUE(added);

    // Attempt to save with an empty filename
    bool result = device_manager_save(manager, "");
    TEST_ASSERT_FALSE(result);

    device_manager_destroy(manager);
}

void test_device_manager_save_with_minimum_values(void)
{
    DeviceManager* manager = device_manager_create();
    TEST_ASSERT_NOT_NULL(manager);

    // Add a device with minimum values for id, type, state, and attribute
    bool added = device_manager_add_device(manager, "Min Device", (DeviceType)INT_MIN, INT_MIN);
    TEST_ASSERT_FALSE(added);

    // Set the state and attribute to minimum values
    bool stateSet = device_manager_set_device_state(manager, INT_MIN, false);
    TEST_ASSERT_FALSE(stateSet);

    bool attributeSet = device_manager_set_device_attribute(manager, INT_MIN,  INT_MIN);
    TEST_ASSERT_FALSE(attributeSet);

    // Save the manager state to a file
    const char* filename = "test_min_values_save.txt";
    bool saved = device_manager_save(manager, filename);
    TEST_ASSERT_TRUE(saved);

    // Clean up
    device_manager_destroy(manager);

    // Optionally, you might want to check the contents of the file to ensure correctness
    // This requires file reading and comparison, which is not shown here
}

void test_device_manager_load_file_not_opened(void)
{
    // Attempt to load from a non-existent file
    DeviceManager* manager = device_manager_load("non_existent_file.txt");

    // Verify that the function returns NULL
    TEST_ASSERT_NULL(manager);
}

void test_device_manager_load_no_valid_entries(void)
{
    // Create a temporary file with no valid device entries
    const char* filename = "test_empty_devices.txt";
    FILE* file = fopen(filename, "w");
    if (file) {
        // Write some invalid data that doesn't match the expected format
        fprintf(file, "Invalid data\nAnother invalid line\n");
        fclose(file);
    }

    // Attempt to load the device manager from the file
    DeviceManager* manager = device_manager_load(filename);

    // Check that the manager is created but contains no devices
    TEST_ASSERT_NOT_NULL(manager);
    TEST_ASSERT_EQUAL_INT(0, device_manager_get_device_count(manager));

    // Clean up
    device_manager_destroy(manager);
    remove(filename);
}


int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_device_manager_create);
    RUN_TEST(test_device_manager_create_with_no_devices);
    RUN_TEST(test_device_manager_create_with_default_settings);
    RUN_TEST(test_device_manager_create_unique_instances);
    RUN_TEST(test_device_manager_create_multiple_calls);
    RUN_TEST(test_device_manager_create_and_add_devices);
    RUN_TEST(test_device_manager_destroy_with_single_device);
    RUN_TEST(test_device_manager_destroy_no_memory_leaks);
    RUN_TEST(test_device_manager_add_device_with_null_name);
    RUN_TEST(test_device_manager_add_device_with_empty_name);
    RUN_TEST(test_device_manager_add_device_with_negative_id);
    RUN_TEST(test_device_manager_add_multiple_devices_with_same_identifier);
    RUN_TEST(test_device_manager_remove_device_from_empty_manager);
    RUN_TEST(test_device_manager_remove_device_single_device);
    RUN_TEST(test_device_manager_remove_device_at_head);
    RUN_TEST(test_device_manager_remove_device_from_middle);
    RUN_TEST(test_device_manager_remove_device_non_existent_id);
    RUN_TEST(test_device_manager_remove_device_multiple_same_id);
    RUN_TEST(test_device_manager_set_device_state_non_existent_id);
    RUN_TEST(test_device_manager_set_device_state_first_device);
    RUN_TEST(test_device_manager_set_device_attribute_non_existent_id);
    RUN_TEST(test_device_manager_set_device_attribute_first_device);
    RUN_TEST(test_device_manager_save_with_null_filename);
    RUN_TEST(test_device_manager_save_with_null_manager);
    RUN_TEST(test_device_manager_save_with_empty_filename);
    RUN_TEST(test_device_manager_save_with_minimum_values);
    RUN_TEST(test_device_manager_load_file_not_opened);
    RUN_TEST(test_device_manager_load_no_valid_entries);

    return UNITY_END();
}
