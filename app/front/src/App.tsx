import { useState } from "react";

import Header from "./components/Header";
import NavBar, { getPageTitle, type PageIndex } from "./components/NavBar";
import WorkInProgressPage from "./pages/WorkInProgressPage";

import "./styles/App.css";

function App() {
    const [pageIndex, setPageIndex] = useState<PageIndex>(0);

    return (
        <div className="app-container">
            <Header title={getPageTitle(pageIndex)} />
            <main style={{ flexGrow: 1 }}>
                <WorkInProgressPage />
            </main>
            <NavBar currentPage={pageIndex} onPageChange={setPageIndex} />
        </div>
    );
}

export default App;
