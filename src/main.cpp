#include <iostream> // Permite mostrar y recibir información.
#include <string> // Permite trabajar con textos.
#include <sstream> // Ayuda a convertir datos ingresados.
#include <fstream> // Permite leer y guardar archivos.
#include <iomanip> // Organiza la presentación de los datos.
#include <limits> // Proporciona límites para los números.
using namespace std; // Evita escribir std:: en cada instrucción.

// =====================================================
// FUNCIONES PARA LEER DATOS SIN DEJAR EL PROGRAMA
// EN UN CICLO INFINITO CUANDO LA ENTRADA ES INCORRECTA
// =====================================================

bool leerEntero(string mensaje, int& numero) // Lee y valida un número entero.
{
    string entrada; // Guarda el dato escrito.
    char sobrante; // Detecta caracteres adicionales.

    while (true) // Repite hasta recibir un dato válido.
    {
        cout << mensaje; // Muestra el mensaje al usuario.

        if (!getline(cin, entrada)) // Intenta leer la entrada.
            return false; // Sale si no se pudo leer.

        stringstream conversion(entrada); // Prepara la conversión del texto.

        if (conversion >> numero && !(conversion >> sobrante)) // Comprueba que sea un entero válido.
            return true; // Acepta el número ingresado.

        cout << "Entrada invalida. Debe ingresar un numero entero.\n"; // Informa que el dato es incorrecto.
    }
}

bool leerDecimal(string mensaje, double& numero) // Lee y valida un número decimal.
{
    string entrada; // Guarda el dato escrito.
    char sobrante; // Detecta caracteres adicionales.

    while (true) // Repite hasta recibir un dato válido.
    {
        cout << mensaje; // Muestra el mensaje al usuario.

        if (!getline(cin, entrada)) // Intenta leer la entrada.
            return false; // Sale si no se pudo leer.

        stringstream conversion(entrada); // Prepara la conversión del texto.

        if (conversion >> numero && !(conversion >> sobrante) && // Comprueba que el dato sea numérico.
            numero == numero && // Evita aceptar valores NaN.
            numero != numeric_limits<double>::infinity() && // Evita el infinito positivo.
            numero != -numeric_limits<double>::infinity()) // Evita el infinito negativo.
            return true; // Acepta el decimal ingresado.

        cout << "Entrada invalida. Ingrese un numero decimal.\n"; // Informa que el dato es incorrecto.
    }
}

bool leerTexto(string mensaje, string& texto) // Lee un texto completo.
{
    cout << mensaje; // Muestra qué información se solicita.

    if (!getline(cin, texto)) // Lee el texto ingresado.
        return false; // Indica que no se pudo leer.

    return true; // Confirma que el texto fue recibido.
}

// =====================================================
// CLASE PRINCIPAL DEL SISTEMA SITRAC
// =====================================================

class SITRAC // Contiene las funciones principales del sistema.
{
private: // Protege los datos internos de la clase.

    // Estructura para las conexiones del grafo
    struct Conexion
    {
        int idCiudad; // Guarda el ID de la ciudad conectada.
        double distancia; // Guarda la distancia de la ruta.
        Conexion* siguiente; // Apunta a la siguiente conexión.
    };

    // Estructura de las ciudades
    struct Ciudad
    {
        int id; // Identifica cada ciudad.
        string nombre; // Guarda el nombre de la ciudad.
        Conexion* conexiones; // Guarda sus rutas conectadas.
        Ciudad* siguiente; // Apunta a la siguiente ciudad.
    };

    // Estructura de los pasajeros
    struct Pasajero
    {
        int id; // Identifica al pasajero.
        string nombre; // Guarda su nombre.
        string documento; // Guarda su documento.
        string telefono; // Guarda su teléfono.
        Pasajero* siguiente; // Apunta al siguiente pasajero.
    };

    // Estructura de las solicitudes de viaje
    struct Solicitud
    {
        int id; // Identifica la solicitud.
        int idPasajero; // Guarda el ID del pasajero.
        int origen; // Guarda la ciudad de salida.
        int destino; // Guarda la ciudad de llegada.
        Solicitud* siguiente; // Apunta a la siguiente solicitud.
    };

    // Estructura del historial
    struct Viaje
    {
        int idSolicitud; // Identifica la solicitud atendida.
        int idPasajero; // Identifica al pasajero.
        int origen; // Guarda la ciudad de origen.
        int destino; // Guarda la ciudad de destino.
        Viaje* siguiente; // Apunta al siguiente viaje.
    };

    // Punteros principales de las estructuras
    Ciudad* inicioCiudades; // Apunta a la primera ciudad.
    Pasajero* inicioPasajeros; // Apunta al primer pasajero.
    Solicitud* frente; // Señala la primera solicitud.
    Solicitud* final; // Señala la última solicitud.
    Viaje* inicioHistorial; // Apunta al primer viaje registrado.

    // Contadores de identificadores
    int siguienteCiudad; // Genera el ID de la próxima ciudad.
    int siguientePasajero; // Genera el ID del próximo pasajero.
    int siguienteSolicitud; // Genera el ID de la próxima solicitud.

    // =================================================
    // FUNCIONES INTERNAS DE CIUDADES Y GRAFO
    // =================================================

    Ciudad* buscarCiudad(int id) // Busca una ciudad por su ID.
    {
        Ciudad* actual = inicioCiudades; // Comienza desde la primera ciudad.

        while (actual != nullptr) // Recorre la lista de ciudades.
        {
            if (actual->id == id) // Comprueba si coincide el ID.
                return actual; // Devuelve la ciudad encontrada.

            actual = actual->siguiente; // Avanza a la siguiente ciudad.
        }

        return nullptr; // Indica que no encontró la ciudad.
    }

    Ciudad* buscarCiudadNombre(string nombre) // Busca una ciudad por su nombre.
    {
        Ciudad* actual = inicioCiudades; // Comienza desde la primera ciudad.

        while (actual != nullptr) // Recorre todas las ciudades.
        {
            if (actual->nombre == nombre) // Compara los nombres.
                return actual; // Devuelve la ciudad encontrada.

            actual = actual->siguiente; // Avanza en la lista.
        }

        return nullptr; // Indica que no encontró coincidencias.
    }

    bool existeConexion(int origen, int destino) // Verifica si existe una ruta.
    {
        Ciudad* ciudad = buscarCiudad(origen); // Busca la ciudad de origen.

        if (ciudad == nullptr) // Comprueba si existe el origen.
            return false; // Indica que no hay conexión.

        Conexion* actual = ciudad->conexiones; // Revisa sus conexiones.

        while (actual != nullptr) // Recorre las rutas disponibles.
        {
            if (actual->idCiudad == destino) // Comprueba si encuentra el destino.
                return true; // Confirma que existe la ruta.

            actual = actual->siguiente; // Avanza a la siguiente conexión.
        }

        return false; // Indica que no encontró la ruta.
    }

    void agregarConexion(int origen, int destino, double distancia) // Registra una conexión.
    {
        Ciudad* ciudad = buscarCiudad(origen); // Busca la ciudad de origen.

        Conexion* nueva = new Conexion; // Crea una nueva conexión.
        nueva->idCiudad = destino; // Guarda el ID del destino.
        nueva->distancia = distancia; // Guarda la distancia.
        nueva->siguiente = ciudad->conexiones; // Enlaza la conexión anterior.

        ciudad->conexiones = nueva; // Actualiza el inicio de las conexiones.
    }

    void quitarConexion(int origen, int destino) // Elimina una conexión específica.
    {
        Ciudad* ciudad = buscarCiudad(origen); // Busca la ciudad de origen.

        if (ciudad == nullptr) // Comprueba si existe la ciudad.
            return; // Termina si no existe.

        Conexion* actual = ciudad->conexiones; // Comienza a revisar las conexiones.
        Conexion* anterior = nullptr; // Guarda la conexión anterior.

        while (actual != nullptr) // Recorre las conexiones.
        {
            if (actual->idCiudad == destino) // Busca el destino indicado.
            {
                if (anterior == nullptr) // Comprueba si es la primera conexión.
                    ciudad->conexiones = actual->siguiente; // Actualiza el inicio.
                else
                    anterior->siguiente = actual->siguiente; // Desconecta el nodo.

                delete actual; // Libera la memoria de la conexión.
                return; // Termina después de eliminarla.
            }

            anterior = actual; // Guarda la conexión actual.
            actual = actual->siguiente; // Avanza a la siguiente conexión.
        }
    }

    // =================================================
    // FUNCIONES INTERNAS DE PASAJEROS
    // =================================================

    Pasajero* buscarPasajero(int id) // Busca un pasajero por su ID.
    {
        Pasajero* actual = inicioPasajeros; // Comienza desde el primer pasajero.

        while (actual != nullptr) // Recorre la lista de pasajeros.
        {
            if (actual->id == id) // Comprueba si coincide el ID.
                return actual; // Devuelve el pasajero encontrado.

            actual = actual->siguiente; // Avanza al siguiente pasajero.
        }

        return nullptr; // Indica que no encontró el pasajero.
    }

    // =================================================
    // FUNCIONES INTERNAS DE COLA E HISTORIAL
    // =================================================

    bool solicitudUsaCiudad(int idCiudad) // Verifica si una solicitud utiliza una ciudad.
    {
        Solicitud* actual = frente; // Comienza desde la primera solicitud.

        while (actual != nullptr) // Recorre la cola de solicitudes.
        {
            if (actual->origen == idCiudad ||
                actual->destino == idCiudad) // Comprueba origen y destino.
                return true; // Indica que la ciudad está siendo utilizada.

            actual = actual->siguiente; // Avanza a la siguiente solicitud.
        }

        return false; // Indica que ninguna solicitud la utiliza.
    }

    bool historialUsaCiudad(int idCiudad) // Verifica si la ciudad aparece en el historial.
    {
        Viaje* actual = inicioHistorial; // Comienza desde el primer viaje.

        while (actual != nullptr) // Recorre los viajes guardados.
        {
            if (actual->origen == idCiudad ||
                actual->destino == idCiudad) // Comprueba las ciudades del viaje.
                return true; // Indica que la ciudad aparece en el historial.

            actual = actual->siguiente; // Avanza al siguiente viaje.
        }

        return false; // Indica que no encontró esa ciudad.
    }

    void agregarHistorial(int idSolicitud, int idPasajero,
                          int origen, int destino) // Guarda un viaje atendido.
    {
        Viaje* nuevo = new Viaje; // Crea un nuevo registro.

        nuevo->idSolicitud = idSolicitud; // Guarda el ID de la solicitud.
        nuevo->idPasajero = idPasajero; // Guarda el ID del pasajero.
        nuevo->origen = origen; // Guarda la ciudad de salida.
        nuevo->destino = destino; // Guarda la ciudad de llegada.
        nuevo->siguiente = nullptr; // Indica que será el último registro.

        if (inicioHistorial == nullptr) // Comprueba si el historial está vacío.
        {
            inicioHistorial = nuevo; // Agrega el primer registro.
        }
        else
        {
            Viaje* actual = inicioHistorial; // Comienza desde el primer viaje.

            while (actual->siguiente != nullptr) // Busca el último registro.
                actual = actual->siguiente; // Avanza en el historial.

            actual->siguiente = nuevo; // Agrega el nuevo viaje al final.
        }
    }

    // =================================================
    // FUNCIONES PARA LIBERAR MEMORIA
    // =================================================

    void limpiarCiudades() // Libera la memoria de ciudades y conexiones.
    {
        Ciudad* actual = inicioCiudades; // Comienza desde la primera ciudad.

        while (actual != nullptr) // Recorre todas las ciudades.
        {
            Conexion* conexion = actual->conexiones; // Accede a sus conexiones.

            while (conexion != nullptr) // Recorre las conexiones de esa ciudad.
            {
                Conexion* temporal = conexion; // Guarda la conexión actual.
                conexion = conexion->siguiente; // Avanza a la siguiente conexión.
                delete temporal; // Libera la conexión anterior.
            }

            Ciudad* temporal = actual; // Guarda la ciudad actual.
            actual = actual->siguiente; // Avanza a la siguiente ciudad.
            delete temporal; // Libera la ciudad anterior.
        }

        inicioCiudades = nullptr; // Deja la lista vacía.
    }

    void limpiarPasajeros() // Libera la memoria de los pasajeros.
    {
        while (inicioPasajeros != nullptr) // Recorre mientras haya pasajeros.
        {
            Pasajero* temporal = inicioPasajeros; // Guarda el pasajero actual.
            inicioPasajeros = inicioPasajeros->siguiente; // Avanza en la lista.
            delete temporal; // Libera la memoria del pasajero.
        }
    }

    void limpiarCola() // Libera las solicitudes pendientes.
    {
        while (frente != nullptr) // Recorre mientras haya solicitudes.
        {
            Solicitud* temporal = frente; // Guarda la solicitud actual.
            frente = frente->siguiente; // Avanza a la siguiente solicitud.
            delete temporal; // Libera la solicitud anterior.
        }

        final = nullptr; // Deja vacía la cola.
    }

    void limpiarHistorial() // Libera los registros del historial.
    {
        while (inicioHistorial != nullptr) // Recorre todos los viajes.
        {
            Viaje* temporal = inicioHistorial; // Guarda el viaje actual.
            inicioHistorial = inicioHistorial->siguiente; // Avanza al siguiente viaje.
            delete temporal; // Libera el registro anterior.
        }
    }

    // =================================================
    // CONSTRUCTOR
    // =================================================

public: // Permite utilizar las funciones desde fuera de la clase.

    SITRAC() // Inicializa el sistema.
    {
        inicioCiudades = nullptr; // Inicia la lista de ciudades vacía.
        inicioPasajeros = nullptr; // Inicia la lista de pasajeros vacía.
        frente = nullptr; // Inicia vacía la cola.
        final = nullptr; // No hay última solicitud.
        inicioHistorial = nullptr; // Inicia vacío el historial.

        siguienteCiudad = 1; // El primer ID de ciudad será 1.
        siguientePasajero = 1; // El primer ID de pasajero será 1.
        siguienteSolicitud = 1; // El primer ID de solicitud será 1.
    }

    // =================================================
    // DESTRUCTOR
    // =================================================

    ~SITRAC() // Se ejecuta cuando termina de usarse el objeto.
    {
        limpiarCiudades(); // Libera las ciudades y sus rutas.
        limpiarPasajeros(); // Libera los pasajeros.
        limpiarCola(); // Libera las solicitudes pendientes.
        limpiarHistorial(); // Libera los viajes guardados.
    }

    // =================================================
    // MODULO 1: CIUDADES
    // =================================================

    void registrarCiudad(string nombre) // Registra una nueva ciudad.
    {
        if (nombre.empty()) // Comprueba que el nombre no esté vacío.
        {
            cout << "El nombre no puede estar vacio.\n"; // Informa el error.
            return; // Termina el registro.
        }

        if (buscarCiudadNombre(nombre) != nullptr) // Comprueba si ya existe.
        {
            cout << "La ciudad ya esta registrada.\n"; // Informa que está repetida.
            return; // Evita registrarla otra vez.
        }

        Ciudad* nueva = new Ciudad; // Crea una nueva ciudad.

        nueva->id = siguienteCiudad++; // Asigna un ID y aumenta el contador.
        nueva->nombre = nombre; // Guarda el nombre ingresado.
        nueva->conexiones = nullptr; // Inicia sin rutas.
        nueva->siguiente = nullptr; // Indica que será el último nodo.

        if (inicioCiudades == nullptr) // Comprueba si es la primera ciudad.
        {
            inicioCiudades = nueva; // La coloca al inicio de la lista.
        }
        else
        {
            Ciudad* actual = inicioCiudades; // Comienza desde la primera ciudad.

            while (actual->siguiente != nullptr) // Busca el final de la lista.
                actual = actual->siguiente; // Avanza a la siguiente ciudad.

            actual->siguiente = nueva; // Agrega la ciudad al final.
        }

        cout << "Ciudad registrada correctamente. ID: "
             << nueva->id << "\n"; // Muestra el ID asignado.
    }

    void mostrarCiudades() // Muestra todas las ciudades registradas.
    {
        if (inicioCiudades == nullptr) // Comprueba si no hay ciudades.
        {
            cout << "No hay ciudades registradas.\n"; // Informa que la lista está vacía.
            return; // Termina la consulta.
        }

        Ciudad* actual = inicioCiudades; // Comienza desde la primera ciudad.

        cout << "\n--- LISTA DE CIUDADES ---\n"; // Muestra el título de la lista.

        while (actual != nullptr) // Recorre todas las ciudades.
        {
            cout << "ID: " << actual->id
                 << " | Nombre: " << actual->nombre << "\n"; // Muestra sus datos.

            actual = actual->siguiente; // Avanza a la siguiente ciudad.
        }
    }

    void consultarCiudad(int id) // Consulta una ciudad por su ID.
    {
        Ciudad* ciudad = buscarCiudad(id); // Busca la ciudad solicitada.

        if (ciudad == nullptr) // Comprueba si fue encontrada.
        {
            cout << "La ciudad no existe.\n"; // Informa que no existe.
            return; // Termina la consulta.
        }

        cout << "ID: " << ciudad->id << "\n"; // Muestra el identificador.
        cout << "Nombre: " << ciudad->nombre << "\n"; // Muestra el nombre.
    }

    void eliminarCiudad(int id) // Elimina una ciudad del sistema.
    {
        Ciudad* actual = inicioCiudades; // Comienza desde la primera ciudad.
        Ciudad* anterior = nullptr; // Guarda el nodo anterior.

        while (actual != nullptr && actual->id != id) // Busca la ciudad indicada.
        {
            anterior = actual; // Guarda la ciudad anterior.
            actual = actual->siguiente; // Avanza en la lista.
        }

        if (actual == nullptr) // Comprueba si encontró la ciudad.
        {
            cout << "La ciudad no existe.\n"; // Informa que no existe.
            return; // Termina la eliminación.
        }

        // No se elimina si existen viajes pendientes
        // o viajes guardados en el historial que la utilizan.
        if (solicitudUsaCiudad(id) || historialUsaCiudad(id)) // Revisa si está en uso.
        {
            cout << "No se puede eliminar: la ciudad aparece "
                 << "en solicitudes o en el historial.\n"; // Explica por qué no se elimina.
            return; // Evita borrar una ciudad que está siendo utilizada.
        }

        // Quitar las conexiones que llegan a esta ciudad
        Ciudad* otra = inicioCiudades; // Comienza a revisar las demás ciudades.

        while (otra != nullptr) // Recorre la lista de ciudades.
        {
            quitarConexion(otra->id, id); // Elimina la conexión hacia esa ciudad.
            otra = otra->siguiente; // Avanza a la siguiente ciudad.
        }

        // Liberar las conexiones que salen de la ciudad
        Conexion* conexion = actual->conexiones; // Accede a sus rutas.

        while (conexion != nullptr) // Recorre todas sus conexiones.
        {
            Conexion* temporal = conexion; // Guarda la conexión actual.
            conexion = conexion->siguiente; // Avanza a la siguiente conexión.
            delete temporal; // Libera la conexión.
        }

        // Desconectar el nodo de la lista
        if (anterior == nullptr) // Comprueba si es la primera ciudad.
            inicioCiudades = actual->siguiente; // Actualiza el inicio de la lista.
        else
            anterior->siguiente = actual->siguiente; // Desconecta la ciudad.

        delete actual; // Libera la memoria de la ciudad.

        cout << "Ciudad eliminada correctamente.\n"; // Confirma la eliminación.
    }

    // =================================================
    // MODULO 2: RUTAS
    // =================================================

    void crearRuta(int origen, int destino, double distancia) // Crea una ruta entre dos ciudades.
    {
        if (distancia < 0) // Comprueba que la distancia no sea negativa.
        {
            cout << "Error: la distancia no puede ser negativa.\n"; // Informa el error.
            return; // Cancela la creación.
        }

        if (origen == destino) // Comprueba que no sea la misma ciudad.
        {
            cout << "No se puede conectar una ciudad consigo misma.\n"; // Informa el error.
            return; // Cancela la creación.
        }

        if (buscarCiudad(origen) == nullptr ||
            buscarCiudad(destino) == nullptr) // Comprueba que existan ambas ciudades.
        {
            cout << "Una o ambas ciudades no existen.\n"; // Informa el problema.
            return; // Cancela la creación.
        }

        if (existeConexion(origen, destino)) // Comprueba si la ruta ya existe.
        {
            cout << "La ruta ya se encuentra registrada.\n"; // Informa que está repetida.
            return; // Evita crearla otra vez.
        }

        // Se registra la conexion en los dos sentidos
        agregarConexion(origen, destino, distancia); // Conecta el origen con el destino.
        agregarConexion(destino, origen, distancia); // Conecta también en sentido contrario.

        cout << "Ruta creada correctamente.\n"; // Confirma que se creó la ruta.
    }

    void eliminarRuta(int origen, int destino) // Elimina una ruta entre dos ciudades.
    {
        if (buscarCiudad(origen) == nullptr ||
            buscarCiudad(destino) == nullptr) // Comprueba que existan ambas ciudades.
        {
            cout << "Una o ambas ciudades no existen.\n"; // Informa el problema.
            return; // Cancela la eliminación.
        }

        if (!existeConexion(origen, destino)) // Comprueba si la ruta está registrada.
        {
            cout << "La ruta no existe.\n"; // Informa que no existe.
            return; // Termina la operación.
        }

        quitarConexion(origen, destino); // Elimina la conexión de ida.
        quitarConexion(destino, origen); // Elimina la conexión de regreso.

        cout << "Ruta eliminada correctamente.\n"; // Confirma la eliminación.
    }

    void consultarVecinos(int id) // Muestra las ciudades conectadas a una ciudad.
    {
        Ciudad* ciudad = buscarCiudad(id); // Busca la ciudad indicada.

        if (ciudad == nullptr) // Comprueba si existe.
        {
            cout << "La ciudad no existe.\n"; // Informa que no fue encontrada.
            return; // Termina la consulta.
        }

        cout << "\nRutas desde " << ciudad->nombre << ":\n"; // Muestra el nombre de la ciudad.

        Conexion* actual = ciudad->conexiones; // Accede a sus conexiones.

        if (actual == nullptr) // Comprueba si tiene rutas.
        {
            cout << "La ciudad no tiene conexiones.\n"; // Informa que no tiene rutas.
            return; // Termina la consulta.
        }

        while (actual != nullptr) // Recorre todas las conexiones.
        {
            Ciudad* destino = buscarCiudad(actual->idCiudad); // Busca la ciudad conectada.

            if (destino != nullptr) // Comprueba que el destino exista.
            {
                cout << destino->nombre
                     << " | Distancia: "
                     << actual->distancia << "\n"; // Muestra el destino y su distancia.
            }

            actual = actual->siguiente; // Avanza a la siguiente conexión.
        }
    }

    // =================================================
    // MODULO 3: PASAJEROS
    // =================================================

    void registrarPasajero(string nombre, string documento,
                           string telefono) // Registra los datos de un pasajero.
    {
        if (nombre.empty() || documento.empty() || telefono.empty()) // Comprueba que no falten datos.
        {
            cout << "Todos los campos son obligatorios.\n"; // Informa que faltan datos.
            return; // Cancela el registro.
        }

        Pasajero* actual = inicioPasajeros; // Comienza desde el primer pasajero.

        while (actual != nullptr) // Recorre los pasajeros registrados.
        {
            if (actual->documento == documento) // Comprueba si el documento ya existe.
            {
                cout << "El documento ya esta registrado.\n"; // Informa que está repetido.
                return; // Evita duplicar el pasajero.
            }

            actual = actual->siguiente; // Avanza al siguiente pasajero.
        }

        Pasajero* nuevo = new Pasajero; // Crea un nuevo pasajero.

        nuevo->id = siguientePasajero++; // Asigna un ID y aumenta el contador.
        nuevo->nombre = nombre; // Guarda el nombre.
        nuevo->documento = documento; // Guarda el documento.
        nuevo->telefono = telefono; // Guarda el teléfono.
        nuevo->siguiente = nullptr; // Indica que será el último nodo.

        if (inicioPasajeros == nullptr) // Comprueba si es el primer pasajero.
        {
            inicioPasajeros = nuevo; // Lo coloca al inicio de la lista.
        }
        else
        {
            actual = inicioPasajeros; // Comienza desde el primer pasajero.

            while (actual->siguiente != nullptr) // Busca el final de la lista.
                actual = actual->siguiente; // Avanza al siguiente pasajero.

            actual->siguiente = nuevo; // Agrega el pasajero al final.
        }

        cout << "Pasajero registrado. ID: " << nuevo->id << "\n"; // Muestra el ID asignado.
    }

    void listarPasajeros() // Muestra todos los pasajeros registrados.
    {
        if (inicioPasajeros == nullptr) // Comprueba si la lista está vacía.
        {
            cout << "No hay pasajeros registrados.\n"; // Informa que no hay pasajeros.
            return; // Termina la consulta.
        }

        Pasajero* actual = inicioPasajeros; // Comienza desde el primer pasajero.

        cout << "\n--- PASAJEROS REGISTRADOS ---\n"; // Muestra el título de la lista.

        while (actual != nullptr) // Recorre todos los pasajeros.
        {
            cout << "ID: " << actual->id
                 << " | Nombre: " << actual->nombre
                 << " | Documento: " << actual->documento
                 << " | Telefono: " << actual->telefono << "\n"; // Muestra sus datos.

            actual = actual->siguiente; // Avanza al siguiente pasajero.
        }
    }

    void consultarPasajero(int id) // Consulta un pasajero por su ID.
    {
        Pasajero* pasajero = buscarPasajero(id); // Busca el pasajero solicitado.

        if (pasajero == nullptr) // Comprueba si fue encontrado.
        {
            cout << "El pasajero no existe.\n"; // Informa que no existe.
            return; // Termina la consulta.
        }

        cout << "ID: " << pasajero->id << "\n"; // Muestra el identificador.
        cout << "Nombre: " << pasajero->nombre << "\n"; // Muestra el nombre.
        cout << "Documento: " << pasajero->documento << "\n"; // Muestra el documento.
        cout << "Telefono: " << pasajero->telefono << "\n"; // Muestra el teléfono.
    }

    void actualizarPasajero(int id, string nombre,
                             string documento, string telefono) // Actualiza los datos de un pasajero.
    {
        Pasajero* pasajero = buscarPasajero(id); // Busca el pasajero por su ID.

        if (pasajero == nullptr) // Comprueba si existe.
        {
            cout << "El pasajero no existe.\n"; // Informa que no fue encontrado.
            return; // Cancela la actualización.
        }

        Pasajero* otro = inicioPasajeros; // Comienza a revisar los demás pasajeros.

        while (otro != nullptr) // Recorre la lista de pasajeros.
        {
            if (otro->id != id && otro->documento == documento) // Busca documentos repetidos.
            {
                cout << "Ese documento pertenece a otro pasajero.\n"; // Informa que ya está usado.
                return; // Evita guardar un documento duplicado.
            }

            otro = otro->siguiente; // Avanza al siguiente pasajero.
        }

        if (nombre.empty() || documento.empty() || telefono.empty()) // Comprueba que no falten datos.
        {
            cout << "Los campos no pueden estar vacios.\n"; // Informa que hay campos vacíos.
            return; // Cancela la actualización.
        }

        pasajero->nombre = nombre; // Actualiza el nombre.
        pasajero->documento = documento; // Actualiza el documento.
        pasajero->telefono = telefono; // Actualiza el teléfono.

        cout << "Pasajero actualizado correctamente.\n"; // Confirma la actualización.
    }

    void eliminarPasajero(int id) // Elimina un pasajero por su ID.
    {
        Pasajero* actual = inicioPasajeros; // Comienza desde el primer pasajero.
        Pasajero* anterior = nullptr; // Guarda el pasajero anterior.

        while (actual != nullptr && actual->id != id) // Busca el pasajero indicado.
        {
            anterior = actual; // Guarda el nodo anterior.
            actual = actual->siguiente; // Avanza al siguiente pasajero.
        }

        if (actual == nullptr) // Comprueba si encontró al pasajero.
        {
            cout << "No existe un pasajero con ese ID.\n"; // Informa que no existe.
            return; // Termina la eliminación.
        }

        // No eliminar pasajeros con solicitudes pendientes
        Solicitud* solicitud = frente; // Comienza a revisar la cola.

        while (solicitud != nullptr) // Recorre las solicitudes pendientes.
        {
            if (solicitud->idPasajero == id) // Comprueba si el pasajero tiene una solicitud.
            {
                cout << "No se puede eliminar: tiene viajes pendientes.\n"; // Informa por qué no se elimina.
                return; // Evita borrar un pasajero en uso.
            }

            solicitud = solicitud->siguiente; // Avanza a la siguiente solicitud.
        }

        if (anterior == nullptr) // Comprueba si es el primer pasajero.
            inicioPasajeros = actual->siguiente; // Actualiza el inicio de la lista.
        else
            anterior->siguiente = actual->siguiente; // Desconecta el pasajero de la lista.

        delete actual; // Libera la memoria del pasajero.

        cout << "Pasajero eliminado correctamente.\n"; // Confirma la eliminación.
    }

    // =================================================
    // MODULO 4: COLA DE SOLICITUDES
    // =================================================

    void encolarSolicitud(int idPasajero, int origen, int destino) // Agrega una solicitud a la cola.
    {
        if (buscarPasajero(idPasajero) == nullptr) // Comprueba que el pasajero exista.
        {
            cout << "El pasajero no existe.\n"; // Informa que no fue encontrado.
            return; // Cancela la solicitud.
        }

        if (buscarCiudad(origen) == nullptr ||
            buscarCiudad(destino) == nullptr) // Comprueba que existan las dos ciudades.
        {
            cout << "Una o ambas ciudades no existen.\n"; // Informa el problema.
            return; // Cancela la solicitud.
        }

        if (!existeConexion(origen, destino)) // Comprueba que haya una ruta directa.
        {
            cout << "No existe una ruta directa entre esas ciudades.\n"; // Informa que no hay conexión.
            return; // Cancela la solicitud.
        }

        Solicitud* nueva = new Solicitud; // Crea una nueva solicitud.

        nueva->id = siguienteSolicitud++; // Asigna un ID y aumenta el contador.
        nueva->idPasajero = idPasajero; // Guarda el pasajero.
        nueva->origen = origen; // Guarda la ciudad de salida.
        nueva->destino = destino; // Guarda la ciudad de llegada.
        nueva->siguiente = nullptr; // Indica que será la última solicitud.

        if (frente == nullptr) // Comprueba si la cola está vacía.
        {
            frente = nueva; // La nueva solicitud queda al frente.
            final = nueva; // También queda como la última.
        }
        else
        {
            final->siguiente = nueva; // Enlaza la nueva solicitud al final.
            final = nueva; // Actualiza el último elemento.
        }

        cout << "Solicitud agregada a la cola. Numero: "
             << nueva->id << "\n"; // Muestra el número de solicitud.
    }

    void mostrarFrente() // Muestra la primera solicitud pendiente.
    {
        if (frente == nullptr) // Comprueba si la cola está vacía.
        {
            cout << "La cola esta vacia.\n"; // Informa que no hay solicitudes.
            return; // Termina la consulta.
        }

        cout << "Primera solicitud: " << frente->id << "\n"; // Muestra el ID de la solicitud.
        cout << "ID pasajero: " << frente->idPasajero << "\n"; // Muestra el pasajero.
        cout << "Origen: " << frente->origen << "\n"; // Muestra la ciudad de salida.
        cout << "Destino: " << frente->destino << "\n"; // Muestra la ciudad de llegada.
    }

    void mostrarCola() // Muestra todas las solicitudes pendientes.
    {
        if (frente == nullptr) // Comprueba si la cola está vacía.
        {
            cout << "La cola esta vacia.\n"; // Informa que no hay solicitudes.
            return; // Termina la consulta.
        }

        Solicitud* actual = frente; // Comienza desde la primera solicitud.

        cout << "\n--- SOLICITUDES EN ESPERA ---\n"; // Muestra el título de la lista.

        while (actual != nullptr) // Recorre todas las solicitudes.
        {
            cout << "Solicitud: " << actual->id
                 << " | Pasajero: " << actual->idPasajero
                 << " | Origen: " << actual->origen
                 << " | Destino: " << actual->destino << "\n"; // Muestra los datos de la solicitud.

            actual = actual->siguiente; // Avanza a la siguiente solicitud.
        }
    }

    void atenderSolicitud() // Atiende la primera solicitud de la cola.
    {
        if (frente == nullptr) // Comprueba si hay solicitudes pendientes.
        {
            cout << "No hay solicitudes. No se puede atender la cola vacia.\n"; // Informa que está vacía.
            return; // Termina la atención.
        }

        Solicitud* atendida = frente; // Guarda la solicitud que será atendida.

        agregarHistorial(atendida->id,
                         atendida->idPasajero,
                         atendida->origen,
                         atendida->destino); // Registra el viaje en el historial.

        cout << "Solicitud atendida: " << atendida->id << "\n"; // Muestra la solicitud atendida.

        frente = frente->siguiente; // Pasa a la siguiente solicitud.

        if (frente == nullptr) // Comprueba si ya no quedan solicitudes.
            final = nullptr; // Deja vacío el final de la cola.

        delete atendida; // Libera la solicitud atendida.
    }

    // =================================================
    // MODULO 5: HISTORIAL
    // =================================================

    void mostrarHistorial() // Muestra los viajes que ya fueron atendidos.
    {
        if (inicioHistorial == nullptr) // Comprueba si el historial está vacío.
        {
            cout << "No hay viajes en el historial.\n"; // Informa que no hay registros.
            return; // Termina la consulta.
        }

        Viaje* actual = inicioHistorial; // Comienza desde el primer viaje.

        cout << "\n--- HISTORIAL DE VIAJES ATENDIDOS ---\n"; // Muestra el título del historial.

        while (actual != nullptr) // Recorre todos los viajes.
        {
            cout << "Solicitud: " << actual->idSolicitud
                 << " | Pasajero: " << actual->idPasajero
                 << " | Origen: " << actual->origen
                 << " | Destino: " << actual->destino << "\n"; // Muestra los datos del viaje.

            actual = actual->siguiente; // Avanza al siguiente registro.
        }
    }

    // =================================================
    // MODULO 6: BUSQUEDAS
    // =================================================

    void buscarPorDocumento(string documento) // Busca un pasajero por su documento.
    {
        Pasajero* actual = inicioPasajeros; // Comienza desde el primer pasajero.

        while (actual != nullptr) // Recorre la lista de pasajeros.
        {
            if (actual->documento == documento) // Compara el documento ingresado.
            {
                cout << "Pasajero encontrado:\n"; // Informa que encontró al pasajero.
                cout << "ID: " << actual->id << "\n"; // Muestra su ID.
                cout << "Nombre: " << actual->nombre << "\n"; // Muestra su nombre.
                cout << "Documento: " << actual->documento << "\n"; // Muestra su documento.
                cout << "Telefono: " << actual->telefono << "\n"; // Muestra su teléfono.
                return; // Termina después de encontrarlo.
            }

            actual = actual->siguiente; // Avanza al siguiente pasajero.
        }

        cout << "No se encontro un pasajero con ese documento.\n"; // Informa que no hubo coincidencias.
    }
        void buscarCiudadPorNombre(string nombre) // Busca una ciudad por su nombre.
    {
        Ciudad* ciudad = buscarCiudadNombre(nombre); // Busca la ciudad en la lista.

        if (ciudad == nullptr) // Comprueba si no fue encontrada.
        {
            cout << "No se encontro esa ciudad.\n"; // Informa que no existe.
            return; // Termina la búsqueda.
        }

        cout << "Ciudad encontrada: " << ciudad->nombre // Muestra el nombre encontrado.
             << " | ID: " << ciudad->id << "\n"; // Muestra también su identificador.
    }

    void buscarRuta(int origen, int destino) // Busca una ruta entre dos ciudades.
    {
        if (!existeConexion(origen, destino)) // Comprueba si existe la conexión.
        {
            cout << "No existe una ruta entre esas ciudades.\n"; // Informa que no hay ruta.
            return; // Termina la búsqueda.
        }

        Ciudad* ciudad = buscarCiudad(origen); // Busca la ciudad de salida.
        Conexion* actual = ciudad->conexiones; // Accede a sus conexiones.

        while (actual != nullptr) // Recorre las conexiones disponibles.
        {
            if (actual->idCiudad == destino) // Comprueba si llegó al destino.
            {
                cout << "Ruta encontrada.\n"; // Confirma que encontró la ruta.
                cout << "Distancia: " << actual->distancia << "\n"; // Muestra la distancia.
                return; // Termina la búsqueda.
            }

            actual = actual->siguiente; // Avanza a la siguiente conexión.
        }
    }

    // =================================================
    // MODULO 7: REPORTES
    // =================================================

    void generarReportes() // Genera un resumen de la información del sistema.
    {
        int totalCiudades = 0; // Cuenta las ciudades registradas.
        int totalRutas = 0; // Cuenta las rutas existentes.
        int totalPasajeros = 0; // Cuenta los pasajeros registrados.
        int totalSolicitudes = 0; // Cuenta las solicitudes pendientes.
        int totalViajes = 0; // Cuenta los viajes atendidos.

        Ciudad* ciudad = inicioCiudades; // Comienza desde la primera ciudad.

        while (ciudad != nullptr) // Recorre todas las ciudades.
        {
            totalCiudades++; // Suma una ciudad al total.

            Conexion* conexion = ciudad->conexiones; // Accede a sus conexiones.

            while (conexion != nullptr) // Recorre las rutas de esa ciudad.
            {
                totalRutas++; // Suma una conexión.
                conexion = conexion->siguiente; // Avanza a la siguiente conexión.
            }

            ciudad = ciudad->siguiente; // Avanza a la siguiente ciudad.
        }

        // Las conexiones se cuentan en ambos sentidos
        totalRutas = totalRutas / 2; // Evita contar dos veces cada ruta.

        Pasajero* pasajero = inicioPasajeros; // Comienza desde el primer pasajero.

        while (pasajero != nullptr) // Recorre los pasajeros.
        {
            totalPasajeros++; // Suma un pasajero.
            pasajero = pasajero->siguiente; // Avanza al siguiente pasajero.
        }

        Solicitud* solicitud = frente; // Comienza desde la primera solicitud.

        while (solicitud != nullptr) // Recorre la cola de solicitudes.
        {
            totalSolicitudes++; // Suma una solicitud pendiente.
            solicitud = solicitud->siguiente; // Avanza a la siguiente solicitud.
        }

        Viaje* viaje = inicioHistorial; // Comienza desde el primer viaje.

        while (viaje != nullptr) // Recorre el historial.
        {
            totalViajes++; // Suma un viaje atendido.
            viaje = viaje->siguiente; // Avanza al siguiente registro.
        }

        cout << "\n========== REPORTE GENERAL SITRAC ==========\n"; // Muestra el encabezado.
        cout << "Total de ciudades: " << totalCiudades << "\n"; // Muestra las ciudades.
        cout << "Total de rutas: " << totalRutas << "\n"; // Muestra las rutas.
        cout << "Total de pasajeros: " << totalPasajeros << "\n"; // Muestra los pasajeros.
        cout << "Solicitudes pendientes: " << totalSolicitudes << "\n"; // Muestra las solicitudes.
        cout << "Viajes atendidos: " << totalViajes << "\n"; // Muestra los viajes realizados.
        cout << "============================================\n"; // Cierra el reporte.
    }

    // =================================================
    // MODULO 8: ARCHIVOS
    // =================================================

    void guardarArchivo() // Guarda la información del sistema en un archivo.
    {
        ofstream archivo("sitrac.txt"); // Abre el archivo para guardar los datos.

        if (!archivo) // Comprueba si no se pudo abrir.
        {
            cout << "No se pudo crear el archivo.\n"; // Informa el problema.
            return; // Termina el proceso.
        }

        int cantidad = 0; // Cuenta los elementos que se guardarán.

        Ciudad* ciudad = inicioCiudades; // Comienza desde la primera ciudad.
        while (ciudad != nullptr) // Recorre todas las ciudades.
        {
            cantidad++; // Cuenta una ciudad.
            ciudad = ciudad->siguiente; // Avanza a la siguiente ciudad.
        }

        archivo << cantidad << "\n"; // Guarda la cantidad de ciudades.

        ciudad = inicioCiudades; // Vuelve al inicio de la lista.

        while (ciudad != nullptr) // Recorre las ciudades para guardarlas.
        {
            archivo << ciudad->id << " " // Guarda el ID de la ciudad.
                    << quoted(ciudad->nombre) << "\n"; // Guarda su nombre completo.

            ciudad = ciudad->siguiente; // Avanza a la siguiente ciudad.
        }

        // Guardar las rutas una sola vez
        cantidad = 0; // Reinicia el contador de rutas.
        ciudad = inicioCiudades; // Vuelve a la primera ciudad.

        while (ciudad != nullptr) // Recorre todas las ciudades.
        {
            Conexion* conexion = ciudad->conexiones; // Accede a sus conexiones.

            while (conexion != nullptr) // Recorre las conexiones.
            {
                if (ciudad->id < conexion->idCiudad) // Evita contar la ruta dos veces.
                    cantidad++; // Cuenta una ruta.

                conexion = conexion->siguiente; // Avanza a la siguiente conexión.
            }

            ciudad = ciudad->siguiente; // Avanza a la siguiente ciudad.
        }

        archivo << cantidad << "\n"; // Guarda la cantidad de rutas.

        ciudad = inicioCiudades; // Vuelve al inicio de las ciudades.

        while (ciudad != nullptr) // Recorre las ciudades.
        {
            Conexion* conexion = ciudad->conexiones; // Accede a sus conexiones.

            while (conexion != nullptr) // Recorre las rutas de la ciudad.
            {
                if (ciudad->id < conexion->idCiudad) // Guarda cada ruta una sola vez.
                {
                    archivo << ciudad->id << " " // Guarda el ID de origen.
                            << conexion->idCiudad << " " // Guarda el ID de destino.
                            << conexion->distancia << "\n"; // Guarda la distancia.
                }

                conexion = conexion->siguiente; // Avanza a la siguiente conexión.
            }

            ciudad = ciudad->siguiente; // Avanza a la siguiente ciudad.
        }

        cantidad = 0; // Reinicia el contador.
        Pasajero* pasajero = inicioPasajeros; // Comienza desde el primer pasajero.

        while (pasajero != nullptr) // Recorre los pasajeros.
        {
            cantidad++; // Cuenta un pasajero.
            pasajero = pasajero->siguiente; // Avanza al siguiente pasajero.
        }

        archivo << cantidad << "\n"; // Guarda la cantidad de pasajeros.

        pasajero = inicioPasajeros; // Vuelve al primer pasajero.

        while (pasajero != nullptr) // Recorre los pasajeros para guardarlos.
        {
            archivo << pasajero->id << " " // Guarda el ID.
                    << quoted(pasajero->nombre) << " " // Guarda el nombre.
                    << quoted(pasajero->documento) << " " // Guarda el documento.
                    << quoted(pasajero->telefono) << "\n"; // Guarda el teléfono.

            pasajero = pasajero->siguiente; // Avanza al siguiente pasajero.
        }

        cantidad = 0; // Reinicia el contador.
        Solicitud* solicitud = frente; // Comienza desde la primera solicitud.

        while (solicitud != nullptr) // Recorre la cola.
        {
            cantidad++; // Cuenta una solicitud.
            solicitud = solicitud->siguiente; // Avanza a la siguiente solicitud.
        }

        archivo << cantidad << "\n"; // Guarda la cantidad de solicitudes.

        solicitud = frente; // Vuelve al inicio de la cola.

        while (solicitud != nullptr) // Recorre las solicitudes para guardarlas.
        {
            archivo << solicitud->id << " " // Guarda el ID de la solicitud.
                    << solicitud->idPasajero << " " // Guarda el ID del pasajero.
                    << solicitud->origen << " " // Guarda el origen.
                    << solicitud->destino << "\n"; // Guarda el destino.

            solicitud = solicitud->siguiente; // Avanza a la siguiente solicitud.
        }

        cantidad = 0; // Reinicia el contador.
        Viaje* viaje = inicioHistorial; // Comienza desde el primer viaje.

        while (viaje != nullptr) // Recorre el historial.
        {
            cantidad++; // Cuenta un viaje.
            viaje = viaje->siguiente; // Avanza al siguiente registro.
        }

        archivo << cantidad << "\n"; // Guarda la cantidad de viajes.

        viaje = inicioHistorial; // Vuelve al inicio del historial.

        while (viaje != nullptr) // Recorre los viajes para guardarlos.
        {
            archivo << viaje->idSolicitud << " " // Guarda el ID de la solicitud.
                    << viaje->idPasajero << " " // Guarda el ID del pasajero.
                    << viaje->origen << " " // Guarda la ciudad de origen.
                    << viaje->destino << "\n"; // Guarda la ciudad de destino.

            viaje = viaje->siguiente; // Avanza al siguiente viaje.
        }

        archivo << siguienteCiudad << " " // Guarda el próximo ID de ciudad.
                << siguientePasajero << " " // Guarda el próximo ID de pasajero.
                << siguienteSolicitud << "\n"; // Guarda el próximo ID de solicitud.

        archivo.close(); // Cierra el archivo.

        cout << "Informacion guardada en sitrac.txt correctamente.\n"; // Confirma que se guardó.
    }

    void cargarArchivo() // Recupera la información guardada.
    {
        ifstream archivo("sitrac.txt"); // Abre el archivo para leerlo.

        if (!archivo) // Comprueba si no se pudo abrir.
        {
            cout << "No se encontro el archivo sitrac.txt.\n"; // Informa que no existe.
            return; // Termina la carga.
        }

        // Leer primero en una estructura temporal no es posible
        // si el archivo esta incompleto; se valida cada lectura.
        int cantidad; // Guarda la cantidad de ciudades del archivo.

        if (!(archivo >> cantidad) || cantidad < 0) // Comprueba que la cantidad sea válida.
        {
            cout << "El archivo tiene un formato incorrecto.\n"; // Informa el error.
            return; // Termina la carga.
        }

        limpiarCiudades(); // Limpia las ciudades actuales.
        limpiarPasajeros(); // Limpia los pasajeros actuales.
        limpiarCola(); // Limpia las solicitudes pendientes.
        limpiarHistorial(); // Limpia el historial actual.

        siguienteCiudad = 1; // Reinicia el contador de ciudades.
        siguientePasajero = 1; // Reinicia el contador de pasajeros.
        siguienteSolicitud = 1; // Reinicia el contador de solicitudes.

        bool correcto = true; // Indica si el archivo tiene datos válidos.

        for (int i = 0; i < cantidad && correcto; i++) // Lee todas las ciudades.
        {
            int id; // Guarda el ID de la ciudad.
            string nombre; // Guarda el nombre de la ciudad.

            if (!(archivo >> id >> quoted(nombre)) || // Lee el ID y el nombre.
                id <= 0 || nombre.empty() || // Comprueba que los datos sean válidos.
                buscarCiudad(id) != nullptr || // Evita IDs repetidos.
                buscarCiudadNombre(nombre) != nullptr) // Evita nombres repetidos.
            {
                correcto = false; // Marca que hay un error.
                break; // Detiene la lectura.
            }

            Ciudad* nueva = new Ciudad; // Crea la ciudad recuperada.
            nueva->id = id; // Asigna su identificador.
            nueva->nombre = nombre; // Guarda su nombre.
            nueva->conexiones = nullptr; // Inicia sin conexiones.
            nueva->siguiente = nullptr; // Indica que será el último nodo.

            if (inicioCiudades == nullptr) // Comprueba si es la primera ciudad.
                inicioCiudades = nueva; // La coloca al inicio.
            else
            {
                Ciudad* actual = inicioCiudades; // Comienza desde la primera ciudad.

                while (actual->siguiente != nullptr) // Busca el final de la lista.
                    actual = actual->siguiente; // Avanza a la siguiente ciudad.

                actual->siguiente = nueva; // Agrega la ciudad al final.
            }

            if (id >= siguienteCiudad) // Comprueba si el ID es el más alto.
                siguienteCiudad = id + 1; // Actualiza el próximo ID disponible.
        }

        int rutas; // Guarda la cantidad de rutas del archivo.

        if (correcto && (!(archivo >> rutas) || rutas < 0)) // Comprueba que la cantidad sea válida.
            correcto = false; // Marca que el archivo tiene un error.

        for (int i = 0; i < rutas && correcto; i++) // Lee todas las rutas.
        {
            int origen, destino; // Guarda las ciudades conectadas.
            double distancia; // Guarda la distancia de la ruta.

            if (!(archivo >> origen >> destino >> distancia) || // Lee los datos de la ruta.
                distancia < 0 || origen == destino || // Comprueba que la distancia y ciudades sean válidas.
                buscarCiudad(origen) == nullptr || // Comprueba que exista el origen.
                buscarCiudad(destino) == nullptr || // Comprueba que exista el destino.
                existeConexion(origen, destino)) // Evita rutas repetidas.
            {
                correcto = false; // Marca que hay un dato incorrecto.
                break; // Detiene la lectura.
            }

            agregarConexion(origen, destino, distancia); // Agrega la conexión de ida.
            agregarConexion(destino, origen, distancia); // Agrega la conexión de regreso.
        }

        int pasajeros; // Guarda la cantidad de pasajeros.

        if (correcto && (!(archivo >> pasajeros) || pasajeros < 0)) // Comprueba la cantidad.
            correcto = false; // Marca un error si no es válida.

        for (int i = 0; i < pasajeros && correcto; i++) // Lee los pasajeros guardados.
        {
            int id; // Guarda el identificador.
            string nombre, documento, telefono; // Guarda los datos personales.

            if (!(archivo >> id >> quoted(nombre) // Lee el ID y el nombre.
                         >> quoted(documento) >> quoted(telefono)) || // Lee documento y teléfono.
                id <= 0 || buscarPasajero(id) != nullptr) // Comprueba que el ID sea válido y único.
            {
                correcto = false; // Marca que hay un error.
                break; // Detiene la lectura.
            }

            Pasajero* nuevo = new Pasajero; // Crea el pasajero recuperado.
            nuevo->id = id; // Guarda su ID.
            nuevo->nombre = nombre; // Guarda su nombre.
            nuevo->documento = documento; // Guarda su documento.
            nuevo->telefono = telefono; // Guarda su teléfono.
            nuevo->siguiente = nullptr; // Indica que será el último nodo.

            if (inicioPasajeros == nullptr) // Comprueba si es el primer pasajero.
                inicioPasajeros = nuevo; // Lo coloca al inicio.
            else
            {
                Pasajero* actual = inicioPasajeros; // Comienza desde el primer pasajero.

                while (actual->siguiente != nullptr) // Busca el final de la lista.
                    actual = actual->siguiente; // Avanza al siguiente pasajero.

                actual->siguiente = nuevo; // Agrega el pasajero al final.
            }

            if (id >= siguientePasajero) // Comprueba si el ID es el más alto.
                siguientePasajero = id + 1; // Actualiza el próximo ID.
        }

        int solicitudes; // Guarda la cantidad de solicitudes.

        if (correcto && (!(archivo >> solicitudes) || solicitudes < 0)) // Comprueba la cantidad.
            correcto = false; // Marca un error si no es válida.

        for (int i = 0; i < solicitudes && correcto; i++) // Lee las solicitudes pendientes.
        {
            int id, idPasajero, origen, destino; // Guarda los datos de cada solicitud.

            if (!(archivo >> id >> idPasajero >> origen >> destino) || // Lee los datos.
                id <= 0 || // Comprueba que el ID sea positivo.
                buscarPasajero(idPasajero) == nullptr || // Comprueba que exista el pasajero.
                buscarCiudad(origen) == nullptr || // Comprueba que exista el origen.
                buscarCiudad(destino) == nullptr || // Comprueba que exista el destino.
                !existeConexion(origen, destino)) // Comprueba que exista la ruta.
            {
                correcto = false; // Marca que hay un error.
                break; // Detiene la lectura.
            }

            Solicitud* nueva = new Solicitud; // Crea la solicitud recuperada.
            nueva->id = id; // Guarda el ID de la solicitud.
            nueva->idPasajero = idPasajero; // Guarda el ID del pasajero.
            nueva->origen = origen; // Guarda la ciudad de salida.
            nueva->destino = destino; // Guarda la ciudad de llegada.
            nueva->siguiente = nullptr; // Indica que será la última solicitud.

            if (frente == nullptr) // Comprueba si la cola está vacía.
            {
                frente = nueva; // Coloca la solicitud al frente.
                final = nueva; // También la señala como última.
            }
            else
            {
                final->siguiente = nueva; // Enlaza la nueva solicitud al final.
                final = nueva; // Actualiza el último elemento.
            }

            if (id >= siguienteSolicitud) // Comprueba si el ID es el más alto.
                siguienteSolicitud = id + 1; // Actualiza el próximo ID.
        }

        int viajes; // Guarda la cantidad de viajes del historial.

        if (correcto && (!(archivo >> viajes) || viajes < 0)) // Comprueba la cantidad.
            correcto = false; // Marca un error si no es válida.

        for (int i = 0; i < viajes && correcto; i++) // Lee los viajes guardados.
        {
            int id, idPasajero, origen, destino; // Guarda los datos del viaje.

            if (!(archivo >> id >> idPasajero >> origen >> destino) || // Lee los datos.
                buscarPasajero(idPasajero) == nullptr || // Comprueba que exista el pasajero.
                buscarCiudad(origen) == nullptr || // Comprueba que exista el origen.
                buscarCiudad(destino) == nullptr) // Comprueba que exista el destino.
            {
                correcto = false; // Marca que hay un dato incorrecto.
                break; // Detiene la lectura.
            }

            agregarHistorial(id, idPasajero, origen, destino); // Agrega el viaje al historial.
        }

        int ciudadID, pasajeroID, solicitudID; // Guarda los próximos identificadores.

        if (correcto) // Continúa si los datos anteriores fueron válidos.
        {
            if (!(archivo >> ciudadID >> pasajeroID >> solicitudID) || // Lee los contadores.
                ciudadID < siguienteCiudad || // Comprueba el ID de ciudad.
                pasajeroID < siguientePasajero || // Comprueba el ID de pasajero.
                solicitudID < siguienteSolicitud) // Comprueba el ID de solicitud.
                correcto = false; // Marca que los contadores no son válidos.
            else
            {
                siguienteCiudad = ciudadID; // Actualiza el próximo ID de ciudad.
                siguientePasajero = pasajeroID; // Actualiza el próximo ID de pasajero.
                siguienteSolicitud = solicitudID; // Actualiza el próximo ID de solicitud.
            }
        }

        archivo.close(); // Cierra el archivo después de leerlo.

        if (!correcto) // Comprueba si se encontró algún error.
        {
            limpiarCiudades(); // Elimina las ciudades cargadas.
            limpiarPasajeros(); // Elimina los pasajeros cargados.
            limpiarCola(); // Limpia las solicitudes cargadas.
            limpiarHistorial(); // Limpia los viajes cargados.

            siguienteCiudad = 1; // Reinicia el contador de ciudades.
            siguientePasajero = 1; // Reinicia el contador de pasajeros.
            siguienteSolicitud = 1; // Reinicia el contador de solicitudes.

            cout << "El archivo esta incompleto o tiene datos invalidos. " // Explica el problema.
                 << "No se cargo la informacion.\n"; // Informa que no se completó la carga.
            return; // Termina el proceso.
        }

        cout << "Informacion cargada correctamente.\n"; // Confirma que se cargaron los datos.
    }

    // =================================================
    // SUBMENUS
    // =================================================

    bool menuCiudades() // Muestra las opciones para administrar ciudades.
    {
        int opcion, id; // Guarda la opción y el ID elegido.
        string nombre; // Guarda el nombre de la ciudad.

        while (true) // Mantiene abierto el menú.
        {
            cout << "\n===== CIUDADES =====\n"; // Muestra el título.
            cout << "1. Registrar ciudad\n"; // Opción para registrar.
            cout << "2. Mostrar ciudades\n"; // Opción para listar.
            cout << "3. Consultar ciudad\n"; // Opción para consultar.
            cout << "4. Eliminar ciudad\n"; // Opción para eliminar.
            cout << "0. Volver\n"; // Opción para regresar.

            if (!leerEntero("Seleccione: ", opcion)) // Lee la opción del usuario.
                return false; // Sale si no se pudo leer.

            switch (opcion) // Revisa qué opción seleccionó.
            {
                case 1: // Registrar una ciudad.
                    if (!leerTexto("Nombre: ", nombre)) return false; // Solicita el nombre.
                    registrarCiudad(nombre); // Registra la ciudad.
                    break; // Termina este caso.

                case 2: // Mostrar las ciudades.
                    mostrarCiudades(); // Muestra la lista.
                    break; // Termina este caso.

                case 3: // Consultar una ciudad.
                    if (!leerEntero("ID de ciudad: ", id)) return false; // Solicita el ID.
                    consultarCiudad(id); // Busca y muestra la ciudad.
                    break; // Termina este caso.

                case 4: // Eliminar una ciudad.
                    if (!leerEntero("ID para eliminar: ", id)) return false; // Solicita el ID.
                    eliminarCiudad(id); // Elimina la ciudad indicada.
                    break; // Termina este caso.

                case 0: // Volver al menú principal.
                    return true; // Sale del submenú.

                default: // Atiende opciones incorrectas.
                    cout << "Opcion no valida.\n"; // Informa que la opción no existe.
            }
        }
    }

    bool menuRutas() // Muestra las opciones para administrar rutas.
    {
        int opcion, origen, destino; // Guarda las opciones y ciudades.
        double distancia; // Guarda la distancia de la ruta.

        while (true) // Mantiene abierto el menú.
        {
            cout << "\n===== RUTAS =====\n"; // Muestra el título.
            cout << "1. Crear ruta\n"; // Opción para crear.
            cout << "2. Eliminar ruta\n"; // Opción para eliminar.
            cout << "3. Consultar vecinos\n"; // Opción para consultar conexiones.
            cout << "0. Volver\n"; // Opción para regresar.

            if (!leerEntero("Seleccione: ", opcion)) // Lee la opción.
                return false; // Sale si no se pudo leer.

            switch (opcion) // Revisa la opción seleccionada.
            {
                case 1: // Crear una ruta.
                    if (!leerEntero("ID origen: ", origen)) return false; // Solicita el origen.
                    if (!leerEntero("ID destino: ", destino)) return false; // Solicita el destino.
                    if (!leerDecimal("Distancia: ", distancia)) return false; // Solicita la distancia.
                    crearRuta(origen, destino, distancia); // Registra la ruta.
                    break; // Termina este caso.

                case 2: // Eliminar una ruta.
                    if (!leerEntero("ID origen: ", origen)) return false; // Solicita el origen.
                    if (!leerEntero("ID destino: ", destino)) return false; // Solicita el destino.
                    eliminarRuta(origen, destino); // Elimina la conexión.
                    break; // Termina este caso.

                case 3: // Consultar ciudades vecinas.
                    if (!leerEntero("ID ciudad: ", origen)) return false; // Solicita la ciudad.
                    consultarVecinos(origen); // Muestra sus conexiones.
                    break; // Termina este caso.

                case 0: // Volver al menú principal.
                    return true; // Sale del submenú.

                default: // Atiende opciones incorrectas.
                    cout << "Opcion no valida.\n"; // Informa que la opción no existe.
            }
        }
    }

    bool menuPasajeros() // Muestra las opciones para administrar pasajeros.
    {
        int opcion, id; // Guarda la opción y el ID.
        string nombre, documento, telefono; // Guarda los datos del pasajero.

        while (true) // Mantiene abierto el menú.
        {
            cout << "\n===== PASAJEROS =====\n"; // Muestra el título.
            cout << "1. Registrar pasajero\n"; // Opción para registrar.
            cout << "2. Listar pasajeros\n"; // Opción para listar.
            cout << "3. Buscar pasajero por ID\n"; // Opción para buscar.
            cout << "4. Actualizar pasajero\n"; // Opción para actualizar.
            cout << "5. Eliminar pasajero\n"; // Opción para eliminar.
            cout << "0. Volver\n"; // Opción para regresar.

            if (!leerEntero("Seleccione: ", opcion)) // Lee la opción.
                return false; // Sale si no se pudo leer.

            switch (opcion) // Revisa la opción elegida.
            {
                case 1: // Registrar un pasajero.
                    if (!leerTexto("Nombre: ", nombre)) return false; // Solicita el nombre.
                    if (!leerTexto("Documento: ", documento)) return false; // Solicita el documento.
                    if (!leerTexto("Telefono: ", telefono)) return false; // Solicita el teléfono.
                    registrarPasajero(nombre, documento, telefono); // Registra al pasajero.
                    break; // Termina este caso.

                case 2: // Listar pasajeros.
                    listarPasajeros(); // Muestra todos los pasajeros.
                    break; // Termina este caso.

                case 3: // Consultar un pasajero.
                    if (!leerEntero("ID del pasajero: ", id)) return false; // Solicita el ID.
                    consultarPasajero(id); // Muestra los datos del pasajero.
                    break; // Termina este caso.

                case 4: // Actualizar un pasajero.
                    if (!leerEntero("ID del pasajero: ", id)) return false; // Solicita el ID.
                    if (!leerTexto("Nuevo nombre: ", nombre)) return false; // Solicita el nuevo nombre.
                    if (!leerTexto("Nuevo documento: ", documento)) return false; // Solicita el nuevo documento.
                    if (!leerTexto("Nuevo telefono: ", telefono)) return false; // Solicita el nuevo teléfono.
                    actualizarPasajero(id, nombre, documento, telefono); // Guarda los cambios.
                    break; // Termina este caso.

                case 5: // Eliminar un pasajero.
                    if (!leerEntero("ID para eliminar: ", id)) return false; // Solicita el ID.
                    eliminarPasajero(id); // Elimina al pasajero.
                    break; // Termina este caso.

                case 0: // Volver al menú principal.
                    return true; // Sale del submenú.

                default: // Atiende opciones incorrectas.
                    cout << "Opcion no valida.\n"; // Informa que la opción no existe.
            }
        }
    }

    bool menuCola() // Muestra las opciones de solicitudes.
    {
        int opcion, pasajero, origen, destino; // Guarda los datos de la solicitud.

        while (true) // Mantiene abierto el menú.
        {
            cout << "\n===== COLA DE SOLICITUDES =====\n"; // Muestra el título.
            cout << "1. Registrar solicitud\n"; // Opción para agregar.
            cout << "2. Atender solicitud\n"; // Opción para atender.
            cout << "3. Mostrar frente\n"; // Opción para ver la primera.
            cout << "4. Mostrar toda la cola\n"; // Opción para listar.
            cout << "0. Volver\n"; // Opción para regresar.

            if (!leerEntero("Seleccione: ", opcion)) // Lee la opción.
                return false; // Sale si no se pudo leer.

            switch (opcion) // Revisa la opción seleccionada.
            {
                case 1: // Registrar una solicitud.
                    if (!leerEntero("ID pasajero: ", pasajero)) return false; // Solicita el pasajero.
                    if (!leerEntero("ID origen: ", origen)) return false; // Solicita el origen.
                    if (!leerEntero("ID destino: ", destino)) return false; // Solicita el destino.
                    encolarSolicitud(pasajero, origen, destino); // Agrega la solicitud a la cola.
                    break; // Termina este caso.

                case 2: // Atender una solicitud.
                    atenderSolicitud(); // Atiende la primera solicitud.
                    break; // Termina este caso.

                case 3: // Mostrar la primera solicitud.
                    mostrarFrente(); // Muestra el frente de la cola.
                    break; // Termina este caso.

                case 4: // Mostrar todas las solicitudes.
                    mostrarCola(); // Muestra la cola completa.
                    break; // Termina este caso.

                case 0: // Volver al menú principal.
                    return true; // Sale del submenú.

                default: // Atiende opciones incorrectas.
                    cout << "Opcion no valida.\n"; // Informa que la opción no existe.
            }
        }
    }

    bool menuHistorial() // Muestra las opciones del historial.
    {
        int opcion; // Guarda la opción seleccionada.

        while (true) // Mantiene abierto el menú.
        {
            cout << "\n===== HISTORIAL =====\n"; // Muestra el título.
            cout << "1. Mostrar viajes atendidos\n"; // Opción para consultar viajes.
            cout << "0. Volver\n"; // Opción para regresar.

            if (!leerEntero("Seleccione: ", opcion)) // Lee la opción.
                return false; // Sale si no se pudo leer.

            switch (opcion) // Revisa la opción elegida.
            {
                case 1: // Mostrar el historial.
                    mostrarHistorial(); // Muestra los viajes atendidos.
                    break; // Termina este caso.

                case 0: // Volver al menú principal.
                    return true; // Sale del submenú.

                default: // Atiende opciones incorrectas.
                    cout << "Opcion no valida.\n"; // Informa que la opción no existe.
            }
        }
    }

    bool menuBusquedas() // Muestra las opciones de búsqueda.
    {
        int opcion, origen, destino; // Guarda la opción y los IDs de ciudades.
        string texto; // Guarda el texto que se desea buscar.

        while (true) // Mantiene abierto el menú.
        {
            cout << "\n===== BUSQUEDAS =====\n"; // Muestra el título.
            cout << "1. Buscar ciudad por nombre\n"; // Opción para buscar ciudades.
            cout << "2. Buscar pasajero por documento\n"; // Opción para buscar pasajeros.
            cout << "3. Buscar ruta entre ciudades\n"; // Opción para buscar rutas.
            cout << "0. Volver\n"; // Opción para regresar.

            if (!leerEntero("Seleccione: ", opcion)) // Lee la opción.
                return false; // Sale si no se pudo leer.

            switch (opcion) // Revisa la opción seleccionada.
            {
                case 1: // Buscar una ciudad.
                    if (!leerTexto("Nombre de ciudad: ", texto)) return false; // Solicita el nombre.
                    buscarCiudadPorNombre(texto); // Busca la ciudad.
                    break; // Termina este caso.

                case 2: // Buscar un pasajero.
                    if (!leerTexto("Documento: ", texto)) return false; // Solicita el documento.
                    buscarPorDocumento(texto); // Busca al pasajero.
                    break; // Termina este caso.

                case 3: // Buscar una ruta.
                    if (!leerEntero("ID origen: ", origen)) return false; // Solicita el origen.
                    if (!leerEntero("ID destino: ", destino)) return false; // Solicita el destino.
                    buscarRuta(origen, destino); // Busca la conexión.
                    break; // Termina este caso.

                case 0: // Volver al menú principal.
                    return true; // Sale del submenú.

                default: // Atiende opciones incorrectas.
                    cout << "Opcion no valida.\n"; // Informa que la opción no existe.
            }
        }
    }

    bool menuReportes() // Muestra las opciones de reportes.
    {
        int opcion; // Guarda la opción seleccionada.

        while (true) // Mantiene abierto el menú.
        {
            cout << "\n===== REPORTES =====\n"; // Muestra el título.
            cout << "1. Reporte general\n"; // Opción para generar el reporte.
            cout << "2. Mostrar pasajeros\n"; // Opción para listar pasajeros.
            cout << "3. Mostrar solicitudes pendientes\n"; // Opción para ver solicitudes.
            cout << "4. Mostrar historial\n"; // Opción para consultar viajes.
            cout << "0. Volver\n"; // Opción para regresar.

            if (!leerEntero("Seleccione: ", opcion)) // Lee la opción.
                return false; // Sale si no se pudo leer.

            switch (opcion) // Revisa la opción elegida.
            {
                case 1: // Generar el reporte general.
                    generarReportes(); // Muestra los totales del sistema.
                    break; // Termina este caso.

                case 2: // Mostrar pasajeros.
                    listarPasajeros(); // Muestra los pasajeros registrados.
                    break; // Termina este caso.

                case 3: // Mostrar solicitudes pendientes.
                    mostrarCola(); // Muestra las solicitudes en espera.
                    break; // Termina este caso.

                case 4: // Mostrar el historial.
                    mostrarHistorial(); // Muestra los viajes atendidos.
                    break; // Termina este caso.

                case 0: // Volver al menú principal.
                    return true; // Sale del submenú.

                default: // Atiende opciones incorrectas.
                    cout << "Opcion no valida.\n"; // Informa que la opción no existe.
            }
        }
    }

    bool menuArchivos() // Muestra las opciones para manejar archivos.
    {
        int opcion; // Guarda la opción seleccionada.

        while (true) // Mantiene abierto el menú.
        {
            cout << "\n===== ARCHIVOS =====\n"; // Muestra el título.
            cout << "1. Guardar informacion\n"; // Opción para guardar.
            cout << "2. Cargar informacion\n"; // Opción para cargar.
            cout << "0. Volver\n"; // Opción para regresar.

            if (!leerEntero("Seleccione: ", opcion)) // Lee la opción.
                return false; // Sale si no se pudo leer.

            switch (opcion) // Revisa la opción elegida.
            {
                case 1: // Guardar información.
                    guardarArchivo(); // Guarda los datos en el archivo.
                    break; // Termina este caso.

                case 2: // Cargar información.
                    cargarArchivo(); // Recupera los datos guardados.
                    break; // Termina este caso.

                case 0: // Volver al menú principal.
                    return true; // Sale del submenú.

                default: // Atiende opciones incorrectas.
                    cout << "Opcion no valida.\n"; // Informa que la opción no existe.
            }
        }
    }

    // =================================================
    // MENU PRINCIPAL
    // =================================================

    void ejecutar() // Controla el funcionamiento general del sistema.
    {
        int opcion; // Guarda la opción elegida por el usuario.

        while (true) // Mantiene activo el menú principal.
        {
            cout << "\n========================================\n"; // Muestra una separación.
            cout << " SISTEMA INTELIGENTE DE TRANSPORTE\n"; // Muestra el nombre del sistema.
            cout << "                 SITRAC\n"; // Muestra sus siglas.
            cout << "========================================\n"; // Cierra el encabezado.
            cout << "1. Ciudades\n"; // Opción del módulo de ciudades.
            cout << "2. Rutas\n"; // Opción del módulo de rutas.
            cout << "3. Pasajeros\n"; // Opción del módulo de pasajeros.
            cout << "4. Cola de solicitudes\n"; // Opción de solicitudes.
            cout << "5. Historial\n"; // Opción del historial.
            cout << "6. Busquedas\n"; // Opción de búsquedas.
            cout << "7. Reportes\n"; // Opción de reportes.
            cout << "8. Archivos (guardar/cargar)\n"; // Opción para manejar archivos.
            cout << "0. Salir\n"; // Opción para cerrar el sistema.

            if (!leerEntero("Seleccione una opcion: ", opcion)) // Lee la opción del usuario.
            {
                cout << "\nFin de la entrada. Programa finalizado.\n"; // Informa que terminó la entrada.
                return; // Finaliza el programa.
            }

            bool continuar = true; // Controla si el menú puede continuar.

            switch (opcion) // Revisa qué módulo seleccionó el usuario.
            {
                case 1: // Abrir el módulo de ciudades.
                    continuar = menuCiudades(); // Ejecuta el menú de ciudades.
                    break; // Termina este caso.

                case 2: // Abrir el módulo de rutas.
                    continuar = menuRutas(); // Ejecuta el menú de rutas.
                    break; // Termina este caso.

                case 3: // Abrir el módulo de pasajeros.
                    continuar = menuPasajeros(); // Ejecuta el menú de pasajeros.
                    break; // Termina este caso.

                case 4: // Abrir el módulo de solicitudes.
                    continuar = menuCola(); // Ejecuta el menú de la cola.
                    break; // Termina este caso.

                case 5: // Abrir el historial.
                    continuar = menuHistorial(); // Ejecuta el menú del historial.
                    break; // Termina este caso.

                case 6: // Abrir las búsquedas.
                    continuar = menuBusquedas(); // Ejecuta el menú de búsquedas.
                    break; // Termina este caso.

                case 7: // Abrir los reportes.
                    continuar = menuReportes(); // Ejecuta el menú de reportes.
                    break; // Termina este caso.

                case 8: // Abrir las opciones de archivos.
                    continuar = menuArchivos(); // Ejecuta el menú de archivos.
                    break; // Termina este caso.

                case 0: // Salir del sistema.
                    cout << "Gracias por utilizar SITRAC.\n"; // Muestra el mensaje de despedida.
                    return; // Finaliza la ejecución.

                default: // Atiende una opción que no existe.
                    cout << "Opcion no valida. Intente nuevamente.\n"; // Pide elegir otra opción.
            }

            if (!continuar) // Comprueba si algún menú terminó por falta de entrada.
            {
                cout << "\nFin de la entrada. Programa finalizado.\n"; // Informa que terminó.
                return; // Finaliza el sistema.
            }
        }
    }
};

// =====================================================
// FUNCION PRINCIPAL
// =====================================================

int main() // Es el punto donde comienza el programa.
{
    SITRAC sistema; // Crea el objeto principal del sistema.
    sistema.ejecutar(); // Inicia el menú principal.

    return 0; // Finaliza el programa correctamente.
}