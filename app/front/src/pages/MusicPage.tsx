import PauseIcon from "../assets/music-pause.svg?react";
import NextIcon from "../assets/music-next.svg?react";
import ShuffleIcon from "../assets/music-shuffle.svg?react";
import VolumeIcon from "../assets/volume.svg?react";

import "../styles/MusicPage.css";

function MusicPage() {
    return (
        <div className="widget music-page-container">
            <div className="audio-visualizer"></div>
            <div className="track-info"></div>
            <div className="music-controls">
                <button
                    className="round-btn secondary-btn"
                    style={{ color: "var(--subtle)" }}
                >
                    <ShuffleIcon />
                </button>
                <button className="round-btn secondary-btn">
                    <NextIcon style={{ transform: "rotate(180deg)" }} />
                </button>
                <button className="round-btn main-btn">
                    <PauseIcon />
                </button>
                <button className="round-btn secondary-btn">
                    <NextIcon />
                </button>
                <button
                    className="round-btn secondary-btn"
                    style={{ color: "#aaa" }}
                >
                    <VolumeIcon />
                </button>
            </div>
        </div>
    );
}

export default MusicPage;
