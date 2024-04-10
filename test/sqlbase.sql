
 CREATE TABLE usuarios (
    id INTEGER PRIMARY KEY,
    username TEXT,
    password TEXT
);
 CREATE TABLE roles (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    rol TEXT
);
 CREATE TABLE usuario_roles (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    id_usuario INTEGER,
    id_rol INTEGER,
    FOREIGN KEY (id_usuario) REFERENCES usuarios (id),
    FOREIGN KEY (id_rol) REFERENCES roles (id)
);
