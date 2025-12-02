
## 🔧 Getting Started

### Prerequisites

*   A C++ compiler containing support for C++20 (e.g., MSVC, GCC, Clang).
*   **CMake** (Version 3.20 or higher recommended).

### Build Instructions

1.  Clone the repository:
    ```bash
    git clone https://github.com/NicholasAli1/HTTPWebserver
    cd HTTPWebserver
    ```

2.  Create a build directory and run CMake:
    ```bash
    mkdir build
    cd build
    cmake ..
    ```

3.  Compile the project:
    ```bash
    cmake --build .
    ```

## 🏃‍♂️ Running the Server

After a successful build, execute the generated binary from the build directory.

1.  Run the executable:
    ```bash
    ./HTTPWebserver
    ```

2.  Open your web browser and navigate to:
    ```
    http://localhost:8080
    ```
    *(Note: Port depends on configuration in `main.cpp`, default is typically 8080 or 8000)*

## 🔌 API & Endpoints

The server includes a basic router that handles the following endpoints:

*   `GET /`: Serves the Home page (`templates/home.html`).
*   `GET /about`: Serves the About page (`templates/about.html`).
*   `GET /contact`: Serves the Contact page (`templates/contact.html`).
*   `GET /api`: Displays API information (`templates/api.html`).
*   `GET /style.css`: Serves the stylesheet from the public directory.

## 📝 Development Notes

*   **Modules**: This project uses `.ixx` files. Ensure your IDE and compiler are configured to handle C++ modules correctly.
*   **HTML Templates**: Modify files in the `templates/` folder to update the content of the web pages.
*   **Styles**: Update `public/style.css` to change the look and feel of the served pages.
