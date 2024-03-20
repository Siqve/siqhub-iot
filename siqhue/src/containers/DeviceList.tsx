import { getDeviceList } from "@/managers/DeviceManager";
import { DeviceIcon } from "@components/home/DeviceIcon";

export const DeviceList = () => {
    const devices = getDeviceList();

    return (
        <div className="flex flex-col gap-4 rounded-3xl p-4 m-5 shadow-[0_0_10px_1px_rgba(0,0,0,0.5)]">
            <h2>Devices</h2>
            <div className="flex gap-3">
                {devices.map((device) => (
                    <DeviceIcon key={device.name} device={device} />
                ))}
            </div>
        </div>
    );
};
