import { getCookie, hasCookie } from "@/managers/CookieManager";

export type Device = {
    name: string;
    ip: string;
    gradientClass: string;
    textClass: string;
};

const DEVICES_COOKIE = "devices";
const DEFAULT_DEVICES: Device[] = [
    {
        name: "Desk",
        ip: "192.168.0.200",
        gradientClass:
            "bg-gradient-to-br from-c_pink-500 to-90% to-c_purple-700",
        textClass: "text-c_pink-100",
    },
    {
        name: "Black",
        ip: "192.168.0.201",
        gradientClass: "bg-gradient-to-br from-c_blue-300 to-c_purple-500",
        textClass: "text-c_blue-100",
    },
];

export const getDeviceList = (): Device[] => {
    if (!hasCookie(DEVICES_COOKIE)) return DEFAULT_DEVICES;
    return JSON.parse(getCookie(DEVICES_COOKIE)!.value);
};


export const getDevice = (deviceId: string): Device | undefined => {
    return getDeviceList().find(device => device.name === deviceId);
};
