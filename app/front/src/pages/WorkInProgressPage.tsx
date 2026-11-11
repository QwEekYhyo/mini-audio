import TrafficCone from "../assets/traffic-cone.svg?react";

function WorkInProgressPage() {
    return (
        <div
            className="widget"
            style={{
                width: "100%",
                height: "100%",
                display: "flex",
                justifyContent: "center",
                alignItems: "center",
                flexDirection: "column",
                gap: "var(--gap-size)",
            }}
        >
            <TrafficCone
                style={{ height: 160, width: 160, color: "var(--pink-main)" }}
            />
            <h1>Page en construction</h1>
        </div>
    );
}

export default WorkInProgressPage;
