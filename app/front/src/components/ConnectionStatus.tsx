import { useState } from "react";

import BluetoothOn from "../assets/bluetooth-on.svg?react";
import BluetoothOff from "../assets/bluetooth-off.svg?react";

const iconStyle: React.CSSProperties = {
    height: "1.3em",
    width: "1.3em",
};

function ConnectionStatus() {
    const [connected, setConnected] = useState(false);

    return (
        <span
            style={{
                display: "flex",
                alignItems: "center",
                gap: 8,
            }}
        >
            {connected ? (
                <BluetoothOn style={iconStyle} />
            ) : (
                <BluetoothOff style={iconStyle} />
            )}
            <span>{connected ? "Connecté" : "Déconnecté"}</span>
        </span>
    );
}

export default ConnectionStatus;
