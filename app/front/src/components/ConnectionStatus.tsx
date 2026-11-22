import { useContext } from "react";

import BluetoothOn from "../assets/bluetooth-on.svg?react";
import BluetoothOff from "../assets/bluetooth-off.svg?react";
import { ConnectionContext } from "../utils/contexts";

const iconStyle: React.CSSProperties = {
    height: "1.3em",
    width: "1.3em",
};

function ConnectionStatus() {
    const connectionInfo = useContext(ConnectionContext);

    return (
        <span
            style={{
                display: "flex",
                alignItems: "center",
                gap: 8,
            }}
        >
            {connectionInfo.connected ? (
                <BluetoothOn style={iconStyle} />
            ) : (
                <BluetoothOff style={iconStyle} />
            )}
            <span>{connectionInfo.connected ? "Connecté" : "Déconnecté"}</span>
        </span>
    );
}

export default ConnectionStatus;
