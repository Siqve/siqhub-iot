#include "DeviceManager.h"
#include <string>
#include <constants/CoreConstants.h>
#include "services/supabase/SupabaseRealtimeService.h"
#include "services/supabase/SupabaseQueryService.h"
#include "constants/TableConstants.h"
#include "hardware/led/LedStripDevice.h"
#include "utils/TimeUtils.h"
#include "services/supabase/utils/SupabaseFilterUtils.h"

using namespace TableConstants::Device;

static uint32_t lastLoopInMicros;

void DeviceManager::loop() {
    if (!isConfigured()) {
        configure();
        return;
    }
    if (!listenerConnected) {
        registerChangeListener();
    }

    if (!TimeUtils::isFrameRipe(micros(), lastLoopInMicros, getDevice()->getFps())) {
        return;
    }
    lastLoopInMicros = micros();
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

    const std::string deviceIdFilter = SupabaseFilterUtils::equals(COLUMN_ID, DEVICE_UUID);

    const bool listenerCreatedSuccessfully = SupabaseRealtimeService::getInstance()
            .addUpdateListener("DeviceManager:device", TABLE_NAME, deviceIdFilter,
                               [this](const JsonVariantConst &data) {
                                   onConfigUpdate(data);
                               });

    listenerConnected = listenerCreatedSuccessfully;
}

void DeviceManager::onConfigUpdate(const JsonVariantConst &config) {
    logger.info("Received device config update.");
    if (!isConfigured()) {
        logger.warn("Device not initialization. Ignoring");
        return;
    }

    handleConfigUpdate(config);
}

void DeviceManager::handleConfigUpdate(const JsonVariantConst &config) {
    logger.info("Handling device config update");
    JsonDocument settings;
    deserializeJson(settings, config[COLUMN_SETTINGS_JSON]);
    const std::string &deviceTypeString = config[COLUMN_TYPE];

    CoreConstants::DeviceType::Value deviceType = CoreConstants::DeviceType::from(deviceTypeString);
    if (deviceType == CoreConstants::DeviceType::Value::UNKNOWN) {
        deviceType = CoreConstants::DeviceType::Value::LED_STRIP;
        logger.warn("Unknown device type: " + deviceTypeString + ". Defaulting to " + CoreConstants::DeviceType::toString(deviceType) + ".");
    }

    if (activeDeviceType == deviceType) {
        logger.info("Updating settings for active device type: " + CoreConstants::DeviceType::toString(deviceType));
        getDevice()->handleSettingsUpdate(settings);
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
    std::optional<JsonDocument> deviceRows =
            SupabaseQueryService::getInstance().select(TABLE_NAME, COLUMN_ID, DEVICE_UUID);

    if (!deviceRows) {
        logger.error("Failed to get device config");
        throw std::runtime_error("Failed to get device config");
    }

    const JsonVariantConst deviceRow = deviceRows->as<JsonArray>()[0];
    JsonDocument config(deviceRow);
    return config;
}

bool DeviceManager::isConfigured() const {
    return activeDeviceType != CoreConstants::DeviceType::Value::UNKNOWN;
}

std::shared_ptr<BaseDevice> DeviceManager::getDevice() {
    return deviceTypes[activeDeviceType];
}


