

\# Pruebas del sistema SITRAC



\## Resultados de las pruebas



| Código | Descripción | Resultado | Evidencia |

|---|---|---|---|

| CP-01 | Registrar dos veces la ciudad Bogotá | Superada. El sistema rechazó el registro duplicado. | CP-01.png |

| CP-02 | Eliminar un pasajero inexistente con ID 9999 | Superada. El sistema informó que no existe el pasajero. | CP-02.png |

| CP-03 | Atender una cola vacía | Superada. El sistema informó que no hay solicitudes para atender. | CP-03.PNG |

| CP-04 | Crear una ruta con distancia negativa (-5) | Superada. El sistema rechazó la distancia negativa. | CP-04.PNG |

| CP-05 | Eliminar una ciudad conectada | Superada. Se eliminó Bogotá (ID 1) y Medellín (ID 2) quedó sin conexiones. | CP-05.PNG |

| CP-06 | Ingresar letras donde se solicita un número | Superada. El sistema detectó la entrada inválida y mostró un mensaje de error. | CP-06.png |



\## Evidencias



Las capturas de pantalla de las pruebas se encuentran en la carpeta `documentacion/evidencias/`.



\- CP-01.png

\- CP-02.png

\- CP-03.PNG

\- CP-04.PNG

\- CP-05.PNG

\- CP-06.png



\## Conclusión



Se ejecutaron seis pruebas para verificar el comportamiento de SITRAC ante registros duplicados, eliminaciones de elementos inexistentes, colas vacías, distancias negativas, eliminación de ciudades conectadas y entradas inválidas.



Los resultados observados fueron satisfactorios para los seis casos evaluados.



