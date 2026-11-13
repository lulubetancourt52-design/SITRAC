

\# Diagrama modular de SITRAC



\## Sistema Inteligente de Transporte y Gestión de Rutas



El sistema SITRAC está organizado en ocho módulos principales que permiten administrar la información del transporte.



```mermaid

flowchart TD

&#x20;   A\[MENU PRINCIPAL SITRAC]



&#x20;   A --> B\[1. Ciudades]

&#x20;   A --> C\[2. Rutas]

&#x20;   A --> D\[3. Pasajeros]

&#x20;   A --> E\[4. Cola de solicitudes]

&#x20;   A --> F\[5. Historial]

&#x20;   A --> G\[6. Búsquedas]

&#x20;   A --> H\[7. Reportes]

&#x20;   A --> I\[8. Archivos]



&#x20;   B --> B1\[Registrar, consultar y eliminar ciudades]

&#x20;   C --> C1\[Crear, eliminar y consultar rutas]

&#x20;   D --> D1\[Registrar, consultar, actualizar y eliminar pasajeros]

&#x20;   E --> E1\[Agregar y atender solicitudes]

&#x20;   F --> F1\[Consultar viajes realizados]

&#x20;   G --> G1\[Buscar pasajeros, ciudades y rutas]

&#x20;   H --> H1\[Generar estadísticas]

&#x20;   I --> I1\[Guardar y cargar información]

```



\## Descripción



El menú principal permite acceder a los ocho módulos del sistema. Cada módulo realiza operaciones específicas para administrar ciudades, conexiones, pasajeros y solicitudes de transporte.



Los módulos trabajan sobre las estructuras de datos implementadas en C++ dentro de la clase SITRAC.



