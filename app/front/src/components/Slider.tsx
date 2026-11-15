import { useEffect, useState } from "react";

import "../styles/Slider.css";

type SliderProps = {
    min: number;
    max: number;
    value: number;
    // TODO: add a "disabled" field
    onValueChange: (value: number) => void;
    // TODO: add a "onSliderRelease" or something
    style?: React.CSSProperties;
};

const clamp = (value: number, min: number, max: number): number => {
    return value > max ? max : value < min ? min : value;
};

// The two linear interpolation sort of functions below are horrendously named
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
    const [isBeingDragged, setIsBeingDragged] = useState(false);

    useEffect(() => {
        // If the slider is being dragged we don't want a value change to conflict with the drag
        // User drag has precedence over computational value changes
        if (isBeingDragged) return;

        setCurrentWidth(`${valueToFraction(value, min, max) * 100}%`);
    }, [value]);

    // Maybe change all the generics below to something more generic that HTMLDivElement?

    /**
     * Update the width of the slider's inside and change the underlying value
     * from a PointerEvent fired as the slider is being dragged.
     *
     * @param event - PointerEvent used to get where the slider is being clicked / dragged
     */
    const updateWidthAndValueFromEvent = (
        event: React.PointerEvent<HTMLDivElement>,
    ) => {
        // Clamp width to avoid slider going out of it's container on the right
        // and trying to set a negative CSS width (which doesn't work)
        let widthClicked = clamp(
            event.clientX - event.currentTarget.offsetLeft,
            0,
            event.currentTarget.clientWidth,
        );

        setCurrentWidth(widthClicked);

        // Notify a value change
        onValueChange(
            fractionToValue(
                widthClicked / event.currentTarget.clientWidth,
                min,
                max,
            ),
        );
    };

    const handlePointerDown: React.PointerEventHandler<HTMLDivElement> = (
        event,
    ) => {
        // TODO: test if this works with touches or not
        if (event.button !== 0) return;

        // Make it so that the currentTarget continues to receive pointer events with the matching pointerId
        // even when they occur out of it's bounding box
        event.currentTarget.setPointerCapture(event.pointerId);

        setIsBeingDragged(true);

        updateWidthAndValueFromEvent(event);
    };

    const handlePointerMove: React.PointerEventHandler<HTMLDivElement> = (
        event,
    ) => {
        if (!isBeingDragged) return;

        updateWidthAndValueFromEvent(event);
    };

    const handlePointerUp: React.PointerEventHandler<HTMLDivElement> = (
        event,
    ) => {
        if (!isBeingDragged) return;

        setIsBeingDragged(false);

        updateWidthAndValueFromEvent(event);
    };

    return (
        <div
            className={`slider-container${isBeingDragged ? " dragged" : ""}`}
            style={style}
            tabIndex={0}
            role="button"
            onPointerDown={handlePointerDown}
            onPointerUp={handlePointerUp}
            onPointerCancel={handlePointerUp}
            onPointerMove={handlePointerMove}
        >
            <div
                className="slider-inside"
                style={{ width: currentWidth }}
            ></div>
        </div>
    );
}

export default Slider;
