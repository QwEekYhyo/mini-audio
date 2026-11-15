function formatSeconds(sec: number): string {
    const minutes = Math.floor(sec / 60);
    sec -= minutes * 60;

    return `${minutes}:${String(sec).padStart(2, "0")}`;
}

export { formatSeconds };
