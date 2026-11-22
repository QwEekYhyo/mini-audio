import { useEffect, useState } from "react";

import Header from "./components/Header";
import NavBar, { getPageTitle, type PageIndex } from "./components/NavBar";
import MusicPage from "./pages/MusicPage";
import WorkInProgressPage from "./pages/WorkInProgressPage";
import { onNativeMessage } from "./utils/native-bridge";
import {
    ConnectionContext,
    defaultConnectionInfo,
    type ConnectionInfo,
} from "./utils/contexts";

import "./styles/App.css";

function App() {
    const [connectionInfo, setConnectionInfo] = useState<ConnectionInfo>(
        defaultConnectionInfo(),
    );
    const [pageIndex, setPageIndex] = useState<PageIndex>(0);

    // Put as lambda here so that the closure captures states or whatever needed.
    // Although this is called everytime the component re-renders so this might not be optimal?
    const getPageNode = (page: PageIndex): React.ReactNode => {
        switch (page) {
            case 0:
                return <MusicPage />;
            default:
                return <WorkInProgressPage />;
        }
    };

    useEffect(() => {
        const unsubscribe = onNativeMessage((message) => {
            if (message.event === "connection_status") {
                setConnectionInfo({
                    connected:
                        message.payload?.status === "connected" ? true : false,
                    deviceName: message.payload?.device.name,
                });
            }
        });

        return unsubscribe;
    }, []);

    return (
        <div className="app-container">
            <ConnectionContext value={connectionInfo}>
                <Header title={getPageTitle(pageIndex)} />
                <main style={{ flexGrow: 1 }}>{getPageNode(pageIndex)}</main>
                <NavBar currentPage={pageIndex} onPageChange={setPageIndex} />
            </ConnectionContext>
        </div>
    );
}

export default App;
