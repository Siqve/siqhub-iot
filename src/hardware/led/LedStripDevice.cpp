#include "LedStripDevice.h"

#include <set>
#include "constants/TableConstants.h"
#include "services/supabase/SupabaseQueryService.h"
#include "services/supabase/SupabaseRealtimeService.h"
#include "services/supabase/utils/SupabaseFilterUtils.h"

#include "utils/TimeUtils.h"
#include "utils/TextUtils.h"
#include "utils/ColorUtils.h"
#include "hardware/led/utils/LedUtils.h"
#include "constants/LedSettingsConstants.h"
#include "constants/CoreConstants.h"

const std::string REALTIME_TOPIC = "LedStripDevice";

using namespace LedSettingsConstants;
using namespace TableConstants;
using namespace ColorUtils;
using namespace LedUtils;

LedStripDevice::LedStripDevice() : BaseDevice(CoreConstants::DeviceType::toString(CoreConstants::DeviceType::Value::LED_STRIP)) {
    ledStrip.Begin();
}

void LedStripDevice::initialize(const JsonDocument &settings) {
    logger.info("Initializing LedStripDevice");
    // No specific initialization logic for this Device type
    updateSettings(settings);
}


void LedStripDevice::loop() {
    if (!listenerConnected && !colorProfileId.empty()) {
        createColorProfileListener();
    }
    if (isSingleColor) {
        singleColorLoop();
    } else {
        gradientLoop();
    }
}

void LedStripDevice::singleColorLoop() {
    logger.info("Single color loop");
    setSolidColor(ledStrip, colors[0]);
    ledStrip.Show();
}


void LedStripDevice::gradientLoop() {
//TODO
}

void LedStripDevice::updateSettings(const JsonDocument &settings) {
    logger.info("Updating settings for LedStripDevice");
    colorProfileId = settings[COLOR_PROFILE_ID_KEY].as<std::string>();
    reload();
}

JsonDocument LedStripDevice::getInitialColorProfile() {
    std::optional<JsonDocument> colorRowData = SupabaseQueryService::getInstance().select(ColorProfile::TABLE_NAME, ColorProfile::COLUMN_ID, colorProfileId);
    if (!colorRowData) {
        logger.error("Failed to get color data for color with id: " + colorProfileId);
        return JsonDocument();
    }
    JsonDocument colorData(colorRowData->as<JsonArray>()[0]);
    return colorData;
}


static unsigned long lastListenerRegisterAttemptMillis;

void LedStripDevice::createColorProfileListener() {
    if (!TimeUtils::isMillisElapsed(millis(), lastListenerRegisterAttemptMillis, 5000)) {
        return;
    }

    std::string filter = SupabaseFilterUtils::equals(ColorProfile::COLUMN_ID, colorProfileId);
    bool listenerCreatedSuccessfully =
            SupabaseRealtimeService::getInstance()
                    .addUpdateListener(REALTIME_TOPIC, ColorProfile::TABLE_NAME, filter,
                                       [this](const JsonVariantConst &data) {
                                           handleColorProfileUpdate(data);
                                       });
    listenerConnected = listenerCreatedSuccessfully;

    if (!listenerCreatedSuccessfully) {
        logger.error("Failed to create listener for LedStripDevice");
    }
}

void LedStripDevice::handleColorProfileUpdate(const JsonVariantConst &colorRow) {
    logger.info("Updating color profile for LedStripDevice");
    const std::string hexesString = colorRow[ColorProfile::COLUMN_HEXES];
    std::vector hexes = TextUtils::split(hexesString, ',');

    colors.clear();
    for (const std::string &hex: hexes) {
        colors.push_back(Gamma32(hexStringToColor(hex)));
    }

    if (colors.size() == 1) {
        isSingleColor = true;
    } else {
        isSingleColor = false;
    }
    loop();
}

int LedStripDevice::getFps() {
    // If single color, we don't really need to update the color that often
    if (isSingleColor) {
        return 1;
    }
    return fps;
}

void LedStripDevice::removeListener() {
    SupabaseRealtimeService::getInstance().removeListener(REALTIME_TOPIC);
    listenerConnected = false;
}

void LedStripDevice::reload() {
    removeListener();
    handleColorProfileUpdate(getInitialColorProfile());
}


