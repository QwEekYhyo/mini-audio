// import { useEffect, useState } from "react";

import Header from "./components/Header";
import NavBar from "./components/NavBar";

import "./styles/App.css";

function App() {
    return (
        <div className="app-container">
            <Header title={"MonBébéAudio"} />
            <main style={{ width: "100%", flexGrow: 1 }}></main>
            <NavBar />
        </div>
    );
}

export default App;
