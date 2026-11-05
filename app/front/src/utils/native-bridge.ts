type NativeMessage = {
    event: string;
    payload?: any;
};

type NativeBridge = {
    send(command: string, payload?: any): void;
};

declare global {
    interface Window {
        webkit?: {
            messageHandlers?: {
                native?: {
                    postMessage(message: any): void;
                };
            };
        };

        native: NativeBridge;
    }
}

window.native = {
    send: (command, payload = {}) => {
        window.webkit?.messageHandlers?.native?.postMessage({
            command,
            payload,
        });
    },
};

export function onNativeMessage(callback: (message: NativeMessage) => void) {
    const handler = (event: Event) => {
        const customEvent = event as CustomEvent<NativeMessage>;
        callback(customEvent.detail);
    };

    window.addEventListener("native-message", handler);

    return () => window.removeEventListener("native-message", handler);
}
