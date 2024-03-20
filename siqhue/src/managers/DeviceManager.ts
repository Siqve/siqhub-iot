import { getCookie, hasCookie } from "@/managers/CookieManager";

export type Device = {
    ip: string;
    name: string;
    gradientClass: string;
    textClass: string;
};

const DEVICES_COOKIE = "devices";
const DEFAULT_DEVICES: Device[] = [
    {
        ip: "192.168.0.200",
        name: "Desk",
        gradientClass:
            "bg-gradient-to-br from-c_pink-500 to-90% to-c_purple-700",
        textClass: "text-c_pink-100",
    },
    {
        ip: "192.168.0.201",
        name: "Black",
        gradientClass: "bg-gradient-to-br from-c_blue-300 to-c_purple-500",
        textClass: "text-c_blue-100",
    },
];

export const getDeviceList = (): Device[] => {
    if (!hasCookie(DEVICES_COOKIE)) return DEFAULT_DEVICES;
    return JSON.parse(getCookie(DEVICES_COOKIE)!.value);
};
