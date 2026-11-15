import MusicNote from "../assets/music-note.svg?react";

import "../styles/AudioVisualizer.css";

function Nothing() {
    return (
        <>
            <div className="audio-viz-nothing-img">
                <MusicNote />
            </div>
            <h1>Aucune musique n'est jouée</h1>
            <span>Ma Bébé n'attend qu'une chose pour s'ambiancer</span>
        </>
    );
}

function AudioVisualizer() {
    return (
        <div className="audio-viz-container">
            <Nothing />
        </div>
    );
}

export default AudioVisualizer;
