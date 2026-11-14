import { useEffect, useState } from "react";

import "../styles/Slider.css";

type SliderProps = {
    min: number;
    max: number;
    value: number;
    onValueChange: (value: number) => void;
    style?: React.CSSProperties;
};

const clamp = (value: number, min: number, max: number): number => {
    return value > max ? max : value < min ? min : value;
};

const valueToFraction = (value: number, min: number, max: number): number => {
    value -= min;
    max -= min;

    return clamp(value / max, 0, 1);
};

const fractionToValue = (value: number, min: number, max: number): number => {
    const delta = value * (max - min);

    return clamp(min + delta, min, max);
};

function Slider({ min, max, value, onValueChange, style }: SliderProps) {
    const [currentWidth, setCurrentWidth] = useState<string | number>(0);

    useEffect(() => {
        setCurrentWidth(`${valueToFraction(value, min, max) * 100}%`);
    }, [value]);

    const handleSliderClick: React.MouseEventHandler<HTMLDivElement> = (
        event,
    ) => {
        event.preventDefault();

        const widthClicked = event.clientX - event.currentTarget.offsetLeft;

        setCurrentWidth(widthClicked);

        onValueChange(
            fractionToValue(
                widthClicked / event.currentTarget.clientWidth,
                min,
                max,
            ),
        );
    };

    return (
        <div
            className="slider-container"
            style={style}
            tabIndex={0}
            role="button"
            onClick={handleSliderClick}
        >
            <div
                className="slider-inside"
                style={{ width: currentWidth }}
            ></div>
        </div>
    );
}

export default Slider;
