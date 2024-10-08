import { useCallback, useReducer, useState } from "react";

import BatteryInput from "~/components/batteryInput";
import Header from "~/components/header";
import SpeedInput from "~/components/speedInput";
import WeatherInput from "~/components/weatherInput";

interface inputState {
  speed: number;
  battery: number;
  weather: number;
}

interface inputAction {
  type: "changed_speed" | "changed_battery" | "changed_weather";
  payload: number;
}

const App = () => {
  const [showResult, setShowResult] = useState(false);
  const [errors, setErrors] = useState({ speedError: "", batteryError: "" });

  //the reducer function, updates the state when input values (speed,battery,weather) are changed
  const reducer = (state: inputState, action: inputAction) => {
    setShowResult(false); // Reset showResult when inputs change
    switch (action.type) {
      case "changed_speed":
        return { ...state, speed: action.payload };
      case "changed_battery":
        return { ...state, battery: action.payload };
      case "changed_weather":
        return { ...state, weather: action.payload };
      default:
        return state;
    }
  };

  const [state, dispatch] = useReducer(reducer, {
    //useReducer to manage more complex states
    speed: 0, //the initial state of speed, battery, weather values
    battery: 0,
    weather: 50,
  });

  //validate speed and battery inputs
  const validateInputs = useCallback(() => {
    const newErrors = { speedError: "", batteryError: "" };
    let isValid = true;

    if (!state.speed) {
      newErrors.speedError = "Speed is required";
      isValid = false;
    } else if (state.speed < 0 || state.speed > 90) {
      newErrors.speedError = "The speed should be with the range of 0 to 90";
      isValid = false;
    }

    if (!state.battery) {
      newErrors.batteryError = "Battery percentage is required";
      isValid = false;
    } else if (state.battery < 0 || state.battery > 100) {
      newErrors.batteryError =
        "The battery percentage should be with the range of 0 to 100";
      isValid = false;
    }

    setErrors(newErrors);
    return isValid;
  }, [state.speed, state.battery]);

  const calculateRange = (e: React.FormEvent<HTMLFormElement>) => {
    e.preventDefault(); //clicking this button would previously trigger the form to submit and refresh the page, deleting the input. This line prevents that default submission
    if (validateInputs()) {
      setShowResult(true);
    }
  };

  //use the given formula to estimate range
  const range =
    -((state.speed * state.speed * state.battery) / 2500) +
    4 * state.battery +
    state.weather;

  return (
    <div className="h-screen w-screen bg-[#212121]">
      <div className="flex h-full flex-col items-center pt-36 text-white">
        <Header />
        <form
          onSubmit={calculateRange}
          className="flex w-full flex-col items-center"
        >
          <div className="mb-4 flex w-full flex-col items-center gap-y-4">
            <SpeedInput //pass a stateChanger prop to each component, causing changes to inputs to call the dispatch function of useReducer
              stateChanger={(value) =>
                dispatch({ type: "changed_speed", payload: value })
              }
            />
            {errors.speedError && (
              <p className="text-red-500">{errors.speedError}</p>
            )}
            <BatteryInput
              stateChanger={(value) =>
                dispatch({ type: "changed_battery", payload: value })
              }
            />
            {errors.batteryError && (
              <p className="text-red-500">{errors.batteryError}</p>
            )}
          </div>
          <div className="flex w-full flex-row justify-center gap-4">
            <WeatherInput
              stateChanger={(value) =>
                dispatch({ type: "changed_weather", payload: value })
              }
            />
          </div>
          <button type="submit" className="rounded bg-blue-700 text-center">
            Calculate
          </button>
        </form>
        {showResult && (
          <p className="mt-4 text-green-500">
            The predicted range of the Elysia is {range.toFixed(2)} km.
          </p>
        )}
      </div>
    </div>
  );
};

export default App;
