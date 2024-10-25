#include "DeviceManager.h"
#include <string>
#include <constants/CoreConstants.h>
#include "services/supabase/SupabaseService.h"
#include "services/supabase/utils/SupabaseUtils.h"
#include "constants/SupabaseConstants.h"
#include "core/types/LedStripDevice.h"
#include "utils/TimeUtils.h"

using namespace SupabaseConstants::Tables::Device;

void DeviceManager::loop() {
    if (!isConfigured()) {
        configure();
        return;
    }
    if (!listenerActive) {
        registerChangeListener();
    }
    getDevice()->loop();
}

void DeviceManager::configure() {
    logger.info("Initial configuration of device");
    JsonDocument config = getDeviceConfig();

    handleConfigUpdate(config);
}

static unsigned long lastListenerRegisterAttemptMillis;

void DeviceManager::registerChangeListener() {
    if (!TimeUtils::isMillisElapsed(millis(), lastListenerRegisterAttemptMillis, 5000)) {
        return;
    }
    lastListenerRegisterAttemptMillis = millis();
    logger.info("Attempting to register change listener for device config");

    const std::string deviceIdFilter = SupabaseUtils::Filters::equals(COLUMN_ID, DEVICE_UUID);

    const bool listenerCreatedSuccessfully = SupabaseService::getInstance()
            .addRealtimeListener(TABLE_NAME, deviceIdFilter,
                                 "DeviceManager:device", [this](const JsonVariantConst& data) {
                        onConfigUpdate(data[SupabaseConstants::Realtime::UPDATE_RECORD_KEY]);
                    });

    listenerActive = listenerCreatedSuccessfully;
}

void DeviceManager::onConfigUpdate(const JsonVariantConst& config) {
    logger.info("Received device config update.");
    if (!isConfigured()) {
        logger.warn("Device not initialization. Ignoring");
        return;
    }

    handleConfigUpdate(config);
}

void DeviceManager::handleConfigUpdate(const JsonVariantConst& config) {
    logger.info("Handling device config update");
    JsonDocument settings;
    deserializeJson(settings, config[COLUMN_SETTINGS_JSON]);
    const std::string& deviceTypeString = config[COLUMN_TYPE].as<std::string>();

    CoreConstants::DeviceType::Value deviceType = CoreConstants::DeviceType::from(deviceTypeString);
    if (deviceType == CoreConstants::DeviceType::Value::UNKNOWN) {
        deviceType = CoreConstants::DeviceType::Value::LED_STRIP;
        logger.warn("Unknown device type: " + deviceTypeString + ". Defaulting to " + CoreConstants::DeviceType::toString(deviceType) + ".");
    }

    if (activeDeviceType == deviceType) {
        logger.info("Updating settings for active device type: " + CoreConstants::DeviceType::toString(deviceType));
        getDevice()->updateSettings(settings);
        return;
    }
    changeDevice(deviceType);
    getDevice()->initialize(settings);
}


void DeviceManager::changeDevice(CoreConstants::DeviceType::Value deviceType) {
    logger.info(
            "Changing device type from: " + CoreConstants::DeviceType::toString(activeDeviceType) + ", to: " + CoreConstants::DeviceType::toString(deviceType));
    if (!deviceTypes.contains(deviceType)) {
        deviceTypes[deviceType] = createDevice(deviceType);
    }
    activeDeviceType = deviceType;
}

std::shared_ptr<BaseDevice> DeviceManager::createDevice(CoreConstants::DeviceType::Value deviceType) {
    logger.info("Creating device pointer of type: " + CoreConstants::DeviceType::toString(deviceType));
    switch (deviceType) {
        case CoreConstants::DeviceType::Value::LED_STRIP:
            return std::make_shared<LedStripDevice>();
        default:
            return nullptr;
    }
}


JsonDocument DeviceManager::getDeviceConfig() {
    logger.info("Getting device config");
    std::optional<JsonDocument> deviceConfig =
            SupabaseService::getInstance().select(TABLE_NAME, COLUMN_ID, DEVICE_UUID);

    if (!deviceConfig) {
        logger.error("Failed to get device config");
        throw std::runtime_error("Failed to get device config");
    }

    const JsonArray jsonArray = deviceConfig->as<JsonArray>();
    if (jsonArray.size() != 1) {
        logger.error("Expected device config query to return exactly one element");
        throw std::runtime_error("Expected device config query to return exactly one element");
    }

    JsonDocument config(jsonArray[0]);
    return config;
}

bool DeviceManager::isConfigured() const {
    return activeDeviceType != CoreConstants::DeviceType::Value::UNKNOWN;
}

std::shared_ptr<BaseDevice> DeviceManager::getDevice() {
    return deviceTypes[activeDeviceType];
}


