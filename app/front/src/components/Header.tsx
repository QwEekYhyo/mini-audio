import "../styles/Header.css";

type HeaderProps = {
    title: string;
};

function Header({ title }: HeaderProps) {
    return (
        <div className="header-container">
            <h1>{title}</h1>
            <span>Placeholder</span>
        </div>
    );
}

export { Header as default, type HeaderProps };
