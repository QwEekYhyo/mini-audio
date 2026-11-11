const Rand = {
    randomUnsigned: (lower = 0, upper = Number.MAX_SAFE_INTEGER) => {
        return Math.floor(Math.random() * Math.abs(1 + upper - lower)) + lower;
    },
};

export default Rand;
