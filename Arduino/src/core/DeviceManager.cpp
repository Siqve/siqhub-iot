#include "DeviceManager.h"
#include <string>
#include "services/SupabaseService.h"
#include "utils/SupabaseUtils.h"
#include "constants/SupabaseConstants.h"
#include "core/types/LedStripDevice.h"


void DeviceManager::loop() {
    if (!initialized) {
        init();
        return;
    }
    if (!listenerActive) {
        registerChangeListener();
    }
    device->loop();
}

void DeviceManager::init() {
    logger.info("Initializing Device Manager");

    JsonDocument initialConfig = getDeviceConfig();
    configure(initialConfig);
    initialized = true;
}

JsonDocument DeviceManager::getDeviceConfig() {
    logger.info("Getting device config");
    std::optional<JsonDocument> deviceConfig = SupabaseService::getInstance().select(SupabaseConstants::Tables::Device::TABLE_NAME,
                                                                                     SupabaseConstants::Tables::Device::COLUMN_ID, DEVICE_UUID);

    // Check if the deviceConfig has value
    if (!deviceConfig.has_value()) {
        logger.error("Failed to get device config");
        throw std::runtime_error("Failed to get device config");
    }

    // Ensure the document contains a JsonArray
    auto jsonArray = deviceConfig->as<JsonArray>();

    // Check if the array has at least one element
    if (jsonArray.size() != 1) {
        logger.error("Expected device config query to return exactly one element");
        throw std::runtime_error("Expected device config query to return exactly one element");
    }

    // Clone the first element of the array and return it
    JsonDocument firstElement;
    firstElement.set(jsonArray[0]);
    return firstElement;
}

void DeviceManager::registerChangeListener() {
    logger.info("Registering change listener for device config");
    std::string deviceIdFilter = SupabaseUtils::Filters::equals(SupabaseConstants::Tables::Device::COLUMN_ID, DEVICE_UUID);

    listenerActive = SupabaseService::getInstance().createRealtimeChannel(SupabaseConstants::Tables::Device::TABLE_NAME, deviceIdFilter,
                                                                          "DeviceManager:device", [this](const JsonVariantConst& data) {
                logger.info("Received device config change");
                configure(data[SupabaseConstants::Realtime::UPDATE_RECORD_KEY]);
            });
}


void DeviceManager::configure(const JsonVariantConst& config) {
    logger.info("Configuring device with new config");
    logger.debug("Config: " + config.as<std::string>());
    auto deviceType = config[SupabaseConstants::Tables::Device::COLUMN_TYPE].as<std::string>();
    if (!device || deviceType != device->getType()) {
        if (!device) {
            logger.info("Creating new device. Device type: " + deviceType);
        } else {
            logger.info("Changing device type from: " + device->getType() + ", to: " + deviceType);
        }
        device = createDevice(deviceType);
    }

    auto settingsRef = config[SupabaseConstants::Tables::Device::COLUMN_SETTINGS_JSON];
    JsonDocument settings;
    settings.set(settingsRef);
    device->applySettings(settings);
}


std::unique_ptr<BaseDevice> DeviceManager::createDevice(const std::string& deviceType) {
    if (deviceType == "LED_STRIP") {
        return std::make_unique<LedStripDevice>();
    }

    logger.error("Unknown device type: " + deviceType);
    throw std::runtime_error("Unknown device type: " + deviceType);
}
