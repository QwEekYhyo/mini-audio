import MusicNote from "../assets/music-note.svg?react";
import LightBulb from "../assets/light-bulb.svg?react";
import SettingsIcon from "../assets/settings.svg?react";

import "../styles/NavBar.css";

const PAGE_INDICES = [0, 1, 2] as const;

type PageIndex = (typeof PAGE_INDICES)[number];

type NavBarProps = {
    currentPage: PageIndex;
    onPageChange: (newPage: PageIndex) => void;
};

function NavBar({ currentPage, onPageChange }: NavBarProps) {
    return (
        <nav className="nav-container widget">
            {PAGE_INDICES.map((page) => (
                // TODO: change those divs to buttons
                <div
                    className={`nav-btn${page === currentPage ? " nav-btn-selected" : ""}`}
                    onClick={() => onPageChange(page)}
                >
                    {getPageNavIcon(page)}
                    <span>{getPageNavTitle(page)}</span>
                </div>
            ))}
        </nav>
    );
}

// Maybe put this in a separate file? Along with PageIndex type
const getPageTitle = (page: PageIndex): string => {
    switch (page) {
        case 0:
            return "MaBébéAudio";
        case 1:
            return "MaBébéLighting";
        case 2:
            return "MaBébéSettings";
    }
};

const getPageNavIcon = (page: PageIndex): React.JSX.Element => {
    switch (page) {
        case 0:
            return <MusicNote />;
        case 1:
            return <LightBulb />;
        case 2:
            return <SettingsIcon />;
    }
};

const getPageNavTitle = (page: PageIndex): string => {
    switch (page) {
        case 0:
            return "Musique";
        case 1:
            return "Lumière";
        case 2:
            return "Paramètres";
    }
};

export { NavBar as default, getPageTitle, type PageIndex };
