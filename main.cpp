#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

// Vertex shader źródło
const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 aPos; // Pozycja wierzchołka

void main()
{
    gl_Position = vec4(aPos, 1.0); // Ustaw pozycję wierzchołka bez transformacji
}
)";

// Fragment shader źródło
const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor; // Kolor wyjściowy piksela

void main()
{
    FragColor = vec4(0.0, 0.0, 1.0, 1.0); // R G B A
}
)";

// Funkcja wywoływana, gdy rozmiar okna się zmieni
// Część kodu odpowiadająca za dynamiczna zmiane wielkości okna
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height); // Dostosuj obszar rysowania
}

int main()
{
    // 1. Inicjalizacja GLFW (zarządzanie oknem)
    // Inicjalizacja biblioteki GLFW i konfigurację kontekstu
    // Chce kupic samochod
    glfwInit();
    // Chce samochód z roku 20.8
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // i chce model Sport
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // W wersji core - czyli bez zbednych i starych funkcji
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 2. Tworzenie okna
    GLFWwindow* window = glfwCreateWindow(800, 600, "trojkat", nullptr, nullptr);
    // wszystkie kolejne polocenia rysowania i manipulacji odnosza sie do biezacego kontekstu - czyli ponizszego
    glfwMakeContextCurrent(window);
    // nasluchuje czy rozmiar okna sie nie zmienil
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 3. Ładowanie funkcji OpenGL (GLAD)
    // kluczowa funkcja biblioteki glad - jej zadaniem jest zaladowanie wszystkich wskaznikow
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // 4. Kompilacja i linkowanie shaderów
    // shader wierzcholkow
    // Vertex Shader
    // tworzy pusty obiekt shader na gpu
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //przypisuje kod zrodlowy GLSL do obiektu shader
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    // kompiluje kod zrodlowy na glsl
    glCompileShader(vertexShader);

    // Fragment Shader
    // prawie identyczny jak wczesniej
    // tylko tym razem to shader fragmentow
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // Program Shaderów
    // tworzy pusty obiekt programu shaderowego
    unsigned int shaderProgram = glCreateProgram();
    // ta funkcja dolacza (attach) skompilownay shader do obiektu programu shaderowego
    glAttachShader(shaderProgram, vertexShader);
    //Ta funkcja dołącza kolejny skompilowany shader do tego samego programu shaderowego.
    glAttachShader(shaderProgram, fragmentShader);
    // Ta funkcja łączy (linkuje) wszystkie dołączone shadery w jeden wykonywalny program shaderowy.
    glLinkProgram(shaderProgram);

    // Usuwanie shaderów po zlinkowaniu
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //.8. Dane wierzchołków trójkąta (X, Y, Z)
    // surowa definicja danych wierzcholkow dla trojkata
    float vertices[] = {
        0.0f,  0.5f, 0.0f,  // góra
        -0.5f, -0.5f, 0.0f,  // dół lewy
        0.5f, -0.5f, 0.0f   // dół prawy
    };

    // 6. Konfiguracja VAO (Vertex Array Object) i VBO (Vertex Buffer Object)
    // dwie zmienne ktore beda "uchwytami" do obiektow ktore zostaly utworzone
    unsigned int VAO, VBO;
    // generuje jeden lub wiecej ID dla obiektow
    glGenVertexArrays(1, &VAO);
    // to smao - generuje i zwraca
    glGenBuffers(1, &VBO);

    // wiaze dany obiekt VAO czyli staje sie aktywnym obiektem VAO
    glBindVertexArray(VAO); // Aktywuj VAO
    // wiąże (binduje) dany obiekt bufora (VBO) do określonego typu celu bufora
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Aktywuj VBO
    //kopiuje dane z pamięci CPU (RAM) do pamięci GPU (VRAM), do aktywowanego bufora.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Skopiuj dane do VBO

    // Powiedz OpenGL, jak interpretować dane w VBO dla atrybutu o location 0
    // opisuje uklad danych dla konkretnego atrybutu wierzcholka
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // wlacza kolejny atrybut wierzcholka
    glEnableVertexAttribArray(0); // Włącz atrybut

    // Odwiąż VBO i VAO (dobra praktyka)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // 7. Pętla renderująca (rysuj co klatkę)
    while (!glfwWindowShouldClose(window))
    {
        // Ustaw kolor tła na czarny i wyczyść ekran
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Użyj naszego programu shaderów
        glUseProgram(shaderProgram);

        // Aktywuj VAO trójkąta
        glBindVertexArray(VAO);
        // Narysuj 3 wierzchołki jako trójkąt
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Pokaż narysowaną klatkę i sprawdzaj zdarzenia
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 8. Sprzątanie zasobów
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}