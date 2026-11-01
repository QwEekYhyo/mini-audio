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

export {};
