import { Device } from "@/managers/DeviceManager";
import clsx from "clsx";
import Link from "next/link";

export const DeviceIcon = ({ device }: { device: Device }) => {

    const redirectPage = `/device/${device.name}`;

    return (
        <div className="flex w-fit flex-col items-center gap-1">
            <Link href={redirectPage}>
                <div
                    className={clsx(
                        "h-28 w-28 rounded-full ",
                        device.gradientClass,
                    )}
                />
            </Link>
            <Link href={redirectPage}>
                <p className={device.textClass}>{device.name}</p>
            </Link>
        </div>
    );
};
