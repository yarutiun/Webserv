# Webserv

Webserv is a lightweight HTTP server implementation written in C++. It aims to provide a simple and efficient web server that can handle basic HTTP requests and serve static content.

## Features

- **HTTP/1.1 Support:** Webserv supports the HTTP/1.1 protocol, allowing it to handle various HTTP methods and headers.

- **Static Content Serving:** The server can serve static content, such as HTML, CSS, JavaScript, and image files.

- **Configurable:** Webserv is configurable through a simple configuration file, allowing users to specify server settings, document roots, and more.

## Getting Started

### Prerequisites

- C++98 compiler
- Make


## CGI Support

Webserv includes support for CGI (Common Gateway Interface), allowing you to dynamically generate content and execute scripts on the server side.

### What is CGI?

CGI (Common Gateway Interface) is a standard protocol for web servers to execute programs or scripts on behalf of a user, generating dynamic content. With CGI support in Webserv, you can leverage the power of server-side scripting languages to enhance the functionality of your web applications.

### Building

```git clone https://github.com/yarutiun/Webserv.git```

```cd Webserv```

```make```

```./webserv```

### Running

#### When executing our server there are two options possible:

- Calling the executable independently (Default config will be chosen).

- Calling the executable + config file of your choice.


After launching your server, you can access it in your web browser by searching for `http://localhost:8080`.

### Contributors

- [Pooneh Moghaddasi](https://github.com/Poonehmgh)

- [Yurii Arutiunov](https://github.com/yarutiun)

- [Santiago Tena](https://github.com/santiagotena)

Contributions are welcome! If you find any issues or have suggestions for improvements, please open an issue or submit a pull request.
