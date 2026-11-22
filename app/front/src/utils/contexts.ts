import { createContext } from "react";

type ConnectionInfo = {
    connected: boolean;
    deviceName?: string;
};

const defaultConnectionInfo = (): ConnectionInfo => ({
    connected: false,
});

const ConnectionContext = createContext<ConnectionInfo>({ connected: false });

export { type ConnectionInfo, defaultConnectionInfo, ConnectionContext };
