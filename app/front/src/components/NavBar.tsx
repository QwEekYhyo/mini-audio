import TrafficCone from "../assets/traffic-cone.svg?react";

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
                <div
                    className={`nav-btn${page === currentPage ? " nav-btn-selected" : ""}`}
                    onClick={() => onPageChange(page)}
                >
                    <TrafficCone />
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
            return "MonBébéAudio";
        case 1:
            return "MonBébéLighting";
        case 2:
            return "MonBébéSettings";
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
