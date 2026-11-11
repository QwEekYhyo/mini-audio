import "../styles/MusicPage.css";

function MusicPage() {
    return (
        <div className="widget music-page-container">
            <div className="audio-visualizer"></div>
            <div className="track-info"></div>
            <div className="music-controls">
                <button className="round-btn main-btn">||</button>
            </div>
        </div>
    );
}

export default MusicPage;
