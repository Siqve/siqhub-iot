#include "LedStripDevice.h"

#include <set>
#include <constants/TableConstants.h>
#include <services/supabase/SupabaseQueryService.h>
#include <services/supabase/SupabaseRealtimeService.h>
#include <services/supabase/utils/SupabaseFilterUtils.h>

#include "utils/TimeUtils.h"
#include "hardware/led/modes/StaticLedMode.h"
#include "hardware/led/modes/FadeLedMode.h"
#include "constants/LedConstants.h"

const std::string REALTIME_TOPIC = "LedStripDevice";

using namespace LedConstants;
using namespace TableConstants::Color;

LedStripDevice::LedStripDevice() : BaseDevice("LED_STRIP") {
    initEffects();
    ledStrip.Begin();
}


void LedStripDevice::initialize(const JsonDocument &settings) {
    logger.info("Initializing LedStripDevice");
    // No specific initialization logic for this Device type
    updateSettings(settings);
}


static uint32_t lastLoopTimeMillis;

void LedStripDevice::loop() {
    if (!TimeUtils::isMillisElapsed(millis(), lastLoopTimeMillis, 5000)) {
        return;
    }
    lastLoopTimeMillis = millis();
    logger.info("LedStripDevice loop");
    getMode()->loop();
}

void LedStripDevice::updateSettings(const JsonDocument &settings) {
    logger.info("Updating settings for LedStripDevice");
    const std::string &modeSetting = settings[Settings::MODE_KEY].as<std::string>(); //TODO: Prøv uten casting til string
    LedModeType::Value ledModeType = LedModeType::from(modeSetting);

    SupabaseRealtimeService::getInstance().removeListener(REALTIME_TOPIC);

    if (ledModeType == LedModeType::Value::UNKNOWN) {
        ledModeType = LedModeType::Value::STATIC;
        logger.error("Unknown LED mode set: " + modeSetting + ". Defaulting to STATIC.");
    }
    activeModeType = ledModeType;

    JsonDocument initialSettings;
    if (activeModeType == LedModeType::Value::STATIC) {
        createStaticListener(settings);
        initialSettings = getInitialStaticSettings(settings);
        //TODO: Tror jeg er ferdig med denne delen nå. Commit dette. Bytt opp på den setFps logikk (ha fps i LedMode isteden).
        // deretter tror jeg jeg burde teste at det fungerer. Så kan jeg se på FadeLedMode og se om jeg kan få til det samme der.
    } else {
    }

    getMode()->handleUpdate(initialSettings);
}


void LedStripDevice::initEffects() {
    ledModes[LedModeType::Value::STATIC] = std::make_shared<StaticLedMode>(ledStrip, [this](int newFps) { setFps(newFps); });
    ledModes[LedModeType::Value::FADE] = std::make_shared<FadeLedMode>(ledStrip, [this](int newFps) { setFps(newFps); });
}

std::shared_ptr<LedMode> LedStripDevice::getMode() {
    return ledModes[activeModeType];
}

JsonDocument LedStripDevice::prepareStaticSettings(const JsonVariantConst &data) {
    JsonDocument settings;
    settings[Settings::Static::Mode::COLOR_KEY] = data[COLUMN_HEX];
    return settings;
}

void LedStripDevice::setFps(int newFps) {
    activeFps = newFps; //TODO: Move this to LedMode instead and use getMode()->getFps() instead
}

void LedStripDevice::createStaticListener(const JsonDocument &settings) {
    std::string filter = SupabaseFilterUtils::equals(COLUMN_ID, settings[Settings::Static::ACTIVE_COLOR_ID_KEY].as<std::string>()); //TODO: Prøv uten casting til string
    bool listenerCreatedSuccessfully = SupabaseRealtimeService::getInstance().addUpdateListener(REALTIME_TOPIC, TABLE_NAME, filter, [this](const JsonVariantConst &data) {
        getMode()->handleUpdate(prepareStaticSettings(data));
    });
    if (!listenerCreatedSuccessfully) {
        logger.error("Failed to create listener for LedStripDevice");
    }
}

JsonDocument LedStripDevice::getInitialStaticSettings(const JsonDocument &settings) {
    std::string colorId = settings[Settings::Static::ACTIVE_COLOR_ID_KEY].as<std::string>(); //TODO: Prøv uten casting til string
    std::optional<JsonDocument> colorRowData = SupabaseQueryService::getInstance().select(TABLE_NAME, COLUMN_ID, colorId);

    if (!colorRowData) {
        logger.error("Failed to get color data for color with id: " + colorId);
        return JsonDocument();
    }

    return prepareStaticSettings(colorRowData->as<JsonArray>()[0]);
}
