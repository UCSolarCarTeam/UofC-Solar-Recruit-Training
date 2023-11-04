import { useState } from "react";
import BatteryInput from "~/components/batteryInput";
import Header from "~/components/header";
import SpeedInput from "~/components/speedInput";
import WeatherInput from "~/components/weatherInput";

const App = () => {
  const [s, setSpeed]  = useState(0);
  const [b, setBattery] = useState(0);
  const[w, setWeather] = useState(0);
  const range = -(s * s * b / 2500) + (4 * b) + w;
  const [output, setOutput] = useState<string>("");

  const calculateRange = (range: any) => {
    if(!isNaN(s)){
      if(!isNaN(b)){
        setOutput(range);
      }else{
        setOutput("*Required Battery %*");
      }
    }else{
      setOutput("*Required Speed*");
    }
    
  }

  return (
    <div className="h-screen w-screen bg-[#212121]">
      <div className="flex h-full flex-col items-center pt-36 text-white">
        <Header />
        <form name="simulator" className="flex w-full flex-col items-center">
          <div className="mb-4 flex w-full flex-col items-center gap-y-4">
            <SpeedInput setSpeed={setSpeed}speed={s} />
            <BatteryInput setBattery = {setBattery}battery={b}/>
          </div>
          <div className="flex w-full flex-row justify-center gap-4">
            <WeatherInput setWeather={setWeather} />  
          </div>
        </form>
        <button className="bg-blue-500 text-white rounded-md py-2 px-4 text-center" onClick={()=>{calculateRange(range)}} >Calculate</button>
        <h1 className="py-2" id = "outputT">{output}</h1>
      </div>
    </div>
  );
};

export default App;
