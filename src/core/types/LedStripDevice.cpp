#include "LedStripDevice.h"

#include <set>
#include <constants/TableConstants.h>
#include <services/supabase/SupabaseQueryService.h>
#include <services/supabase/SupabaseRealtimeService.h>
#include <services/supabase/utils/SupabaseFilterUtils.h>

#include "utils/TimeUtils.h"
#include "constants/LedConstants.h"
#include "utils/TextUtils.h"
#include "utils/ColorUtils.h"
#include "hardware/led/utils/LedUtils.h"

const std::string REALTIME_TOPIC = "LedStripDevice";

using namespace LedConstants;
using namespace TableConstants::ColorProfile;
using namespace ColorUtils;
using namespace LedUtils;

LedStripDevice::LedStripDevice() : BaseDevice("LED_STRIP") {
    ledStrip.Begin();
}

void LedStripDevice::initialize(const JsonDocument& settings) {
    logger.info("Initializing LedStripDevice");
    // No specific initialization logic for this Device type
    updateSettings(settings);
}

static uint32_t lastLoopTimeMillis;

void LedStripDevice::loop() {
    //TODO: Move this to DeviceManager
    if (!TimeUtils::isMillisElapsed(millis(), lastLoopTimeMillis, 5000)) {
        //TODO: Fps logic
        return;
    }
    lastLoopTimeMillis = millis();
    logger.info("LedStripDevice loop");

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

void LedStripDevice::updateSettings(const JsonDocument& settings) {
    logger.info("Updating settings for LedStripDevice");
    SupabaseRealtimeService::getInstance().removeListener(REALTIME_TOPIC);
    createStaticListener(settings);
    handleColorProfileUpdate(getInitialStaticSettings(settings));
}

void LedStripDevice::createStaticListener(const JsonDocument& settings) {
    std::string filter = SupabaseFilterUtils::equals(COLUMN_ID, settings[Settings::COLOR_PROFILE_ID_KEY]);
    bool listenerCreatedSuccessfully =
            SupabaseRealtimeService::getInstance()
                    .addUpdateListener(REALTIME_TOPIC, TABLE_NAME, filter,
                                       [this](const JsonVariantConst& data) {
                                           handleColorProfileUpdate(data);
                                       });
    if (!listenerCreatedSuccessfully) {
        logger.error("Failed to create listener for LedStripDevice");
    }
}

JsonDocument LedStripDevice::getInitialStaticSettings(const JsonDocument& settings) {
    std::string colorId = settings[Settings::COLOR_PROFILE_ID_KEY];
    std::optional<JsonDocument> colorRowData = SupabaseQueryService::getInstance().select(TABLE_NAME, COLUMN_ID, colorId);

    if (!colorRowData) {
        logger.error("Failed to get color data for color with id: " + colorId);
        return JsonDocument();
    }
    JsonDocument colorData(colorRowData->as<JsonArray>()[0]);
    return colorData;
}

void LedStripDevice::handleColorProfileUpdate(const JsonVariantConst& colorRow) {
    logger.info("Updating color profile for LedStripDevice");
    const std::string hexesString = colorRow[COLUMN_HEXES];
    std::vector hexes = TextUtils::split(hexesString, ',');
    colors.clear();
    for (const std::string& hex : hexes) {
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


