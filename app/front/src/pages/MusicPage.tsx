import { useState } from "react";

import { formatSeconds } from "../utils/time";
import Slider from "../components/Slider";

import MusicDisc from "../assets/music-disc.svg?react";
import NextIcon from "../assets/music-next.svg?react";
import PauseIcon from "../assets/music-pause.svg?react";
import ShuffleIcon from "../assets/music-shuffle.svg?react";
import VolumeIcon from "../assets/volume.svg?react";

import "../styles/MusicPage.css";

// TODO: put this somewhere else
type TrackMetadata = {
    title: string;
    artist: string;
    album: string;
    year?: number;
    img?: number; // I don't know how to do this yet
};

const defaultMetadata = (): TrackMetadata => ({
    title: "Aucune musique sélectionnée",
    artist: "Les informations concernant l'artiste apparaîtront ici.",
    album: "Album - Année",
});

function MusicPage() {
    // All track states are going to be store above in the components tree
    // (or using a context provider, haven't decided yet) to keep them alive
    // AND to have them available in other components such as the MiniPlayer

    // In seconds
    const [trackPosition, setTrackPosition] = useState(0);
    const [metadata, setMetadata] = useState(defaultMetadata());

    const trackDuration = 190;

    return (
        <div className="widget music-page-container">
            <div className="audio-visualizer"></div>

            <div className="track-info">
                <div className="track-metadata">
                    <div className="track-img">
                        <MusicDisc style={{ width: "60%", height: "60%" }} />
                    </div>

                    <div className="track-texts">
                        <span>{metadata.title}</span>
                        <span>{metadata.artist}</span>
                        <span>{metadata.album}</span>
                    </div>
                </div>

                <Slider
                    min={0}
                    max={trackDuration}
                    value={trackPosition}
                    onValueChange={(value) =>
                        setTrackPosition(Math.floor(value))
                    }
                    style={{ height: 8, width: "100%" }}
                />

                <div className="track-position">
                    <span>{formatSeconds(trackPosition)}</span>
                    <span>-{formatSeconds(trackDuration - trackPosition)}</span>
                </div>
            </div>

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
                    style={{ color: "var(--text-sub)" }}
                >
                    <VolumeIcon />
                </button>
            </div>
        </div>
    );
}

export default MusicPage;
