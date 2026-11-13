

\# Estructuras de datos utilizadas en SITRAC



\## 1. Lista enlazada de ciudades



| Elemento | Descripción |

|---|---|

| Estructura | Lista enlazada simple |

| Nodo | Ciudad |

| Datos | ID, nombre y conexiones |

| Operaciones | Registrar, buscar, consultar y eliminar ciudades |

| Justificación | Permite agregar y eliminar ciudades dinámicamente. |



\## 2. Grafo de rutas



| Elemento | Descripción |

|---|---|

| Estructura | Grafo mediante listas de adyacencia |

| Nodo | Ciudad y sus conexiones |

| Datos | ID de ciudad destino y distancia |

| Operaciones | Crear, eliminar y consultar conexiones |

| Justificación | Permite representar las conexiones entre ciudades y sus distancias. |



\## 3. Lista enlazada de pasajeros



| Elemento | Descripción |

|---|---|

| Estructura | Lista enlazada simple |

| Nodo | Pasajero |

| Datos | ID, nombre, documento y teléfono |

| Operaciones | Registrar, buscar, actualizar, listar y eliminar |

| Justificación | Facilita administrar una cantidad variable de pasajeros. |



\## 4. Cola de solicitudes



| Elemento | Descripción |

|---|---|

| Estructura | Cola enlazada |

| Nodo | Solicitud |

| Datos | ID, pasajero, origen y destino |

| Operaciones | Encolar, consultar frente, mostrar y atender |

| Justificación | Respeta el orden de llegada de las solicitudes (FIFO). |



\## 5. Historial de viajes



| Elemento | Descripción |

|---|---|

| Estructura | Lista enlazada simple |

| Nodo | Viaje |

| Datos | Solicitud, pasajero, origen y destino |

| Operaciones | Registrar y consultar viajes realizados |

| Justificación | Permite conservar los viajes atendidos y consultarlos posteriormente. |



\## 6. Búsquedas



| Elemento | Descripción |

|---|---|

| Estructura | Recorrido de listas enlazadas y grafo |

| Operaciones | Buscar pasajero por documento, ciudad por nombre y ruta |

| Justificación | Permite localizar información almacenada en las estructuras del sistema. |



\## 7. Reportes



| Elemento | Descripción |

|---|---|

| Estructura | Recorrido de las estructuras existentes |

| Operaciones | Contar ciudades, rutas, pasajeros, solicitudes y viajes |

| Justificación | Proporciona información general sobre el estado del sistema. |



\## 8. Archivos



| Elemento | Descripción |

|---|---|

| Estructura | Archivos de texto |

| Operaciones | Guardar y cargar información |

| Justificación | Permite conservar la información del sistema mediante archivos. |



\## Conclusión



SITRAC utiliza estructuras de datos dinámicas implementadas mediante nodos y punteros. Se emplean listas enlazadas, un grafo con listas de adyacencia y una cola enlazada, evitando reemplazar las estructuras principales por contenedores STL.



