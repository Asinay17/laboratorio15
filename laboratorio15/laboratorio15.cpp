#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <algorithm>  // para transform
#include <cctype>     // para tolower
#include 'pch.h'
using namespace std;

struct Nodo {
    string pregunta;
    string respuesta;
    Nodo* siguiente;
};

Nodo* crearNodo(const string& pregunta, const string& respuesta) {
    Nodo* nuevo = new Nodo;
    nuevo->pregunta = pregunta;
    nuevo->respuesta = respuesta;
    nuevo->siguiente = nullptr;
    return nuevo;
}

void insertarAlFinal(Nodo** cabeza, const string& pregunta, const string& respuesta) {
    Nodo* nuevo = crearNodo(pregunta, respuesta);
    if (*cabeza == nullptr) {
        *cabeza = nuevo;
    }
    else {
        Nodo* temp = *cabeza;
        while (temp->siguiente != nullptr) {
            temp = temp->siguiente;
        }
        temp->siguiente = nuevo;
    }
}

void liberarMemoria(Nodo* cabeza) {
    while (cabeza != nullptr) {
        Nodo* temp = cabeza;
        cabeza = cabeza->siguiente;
        delete temp;
    }
}

string aMinusculas(const string& str) {
    string resultado = str;
    transform(resultado.begin(), resultado.end(), resultado.begin(), [](unsigned char c) {
        return tolower(c);
        });
    return resultado;
}

Nodo* cargarBaseConocimiento(const string& archivo) {
    Nodo* cabeza = nullptr;
    ifstream file(archivo);
    if (!file.is_open()) {
        cerr << "Error: No se pudo abrir el archivo '" << archivo << "'." << endl;
        return nullptr;
    }

    string linea;
    while (getline(file, linea)) {
        size_t separador = linea.find('|');
        if (separador != string::npos) {
            string pregunta = linea.substr(0, separador);
            string respuesta = linea.substr(separador + 1);
            insertarAlFinal(&cabeza, aMinusculas(pregunta), respuesta);
        }
    }

    file.close();
    return cabeza;
}

string buscarRespuesta(Nodo* cabeza, const string& consulta) {
    string consultaNormalizada = aMinusculas(consulta);
    Nodo* temp = cabeza;
    while (temp != nullptr) {
        if (consultaNormalizada == temp->pregunta) {
            return temp->respuesta;
        }
        temp = temp->siguiente;
    }
    return "No tengo una respuesta para eso.";
}

int main() {
#ifdef _WIN32
    system("chcp 65001 > nul"); // Establece UTF-8 en Windows
#endif

    cout << "=== Chatbot simple ===" << endl;

    Nodo* base = cargarBaseConocimiento("conocimiento.txt");
    if (!base) {
        return 1;
    }

    string entrada;
    cout << "\nEscribe tu pregunta (o 'salir' para terminar):" << endl;

    while (true) {
        cout << "\nTú: ";
        getline(cin, entrada);
        if (aMinusculas(entrada) == "salir") break;
        cout << "Chatbot: " << buscarRespuesta(base, entrada) << endl;
    }

    liberarMemoria(base);
    cout << "\nHasta luego." << endl;
    return 0;
}
