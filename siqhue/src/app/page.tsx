import { HomeHeader } from "@components/home/HomeHeader";
import { DeviceList } from "@/containers/DeviceList";

const Home = () => {
    return (
        <div className="flex flex-col gap-10">
            <HomeHeader />
            <DeviceList/>
        </div>
    );
};

export default Home;
