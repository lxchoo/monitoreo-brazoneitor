# Sistema de Monitoreo y Control para Brazo Robótico (BRAZONEITOR)

Proyecto para el "Taller de aplicaciones informáticas" que consiste en un sistema distribuido para la ingesta, procesamiento y visualización de datos en tiempo real, controlando un brazo robótico 4-DOF.

El sistema integra múltiples tecnologías (Arduino, Python, Node.js) que se comunican a través de una arquitectura de red (TCP, UDP, WebSockets y XML-RPC) para controlar el hardware y monitorear sus sensores desde un dashboard web.

## Demo

`[Video de demostración o captura de pantalla del Dashboard en vivo]`

## Características Principales
* **Control Autónomo:** El firmware en C/C++ (Arduino) implementa una máquina de estados para la detección y agarre de objetos usando sensores de ultrasonido.
* **Dashboard en Tiempo Real:** Interfaz web que se actualiza instantáneamente con la telemetría de los sensores usando **WebSockets (Socket.IO)**.
* **Middleware Central (Hub):** Un servidor en **Node.js** consolida datos de múltiples fuentes heterogéneas (TCP, UDP, Serial).
* **Arquitectura de Microservicios:** Un servicio externo en Python es consumido vía **XML-RPC** para el procesamiento de datos.
* **Integración Hardware-Software:** El sistema crea un puente entre la programación embebida de bajo nivel (Arduino) y un sistema de control de alto nivel (Node.js, Python).

## Arquitectura del Sistema
El flujo de datos del sistema está altamente desacoplado:

1.  **Hardware (Firmware):** El **Arduino** (C/C++) controla los servos del brazo 4-DOF y lee los sensores. Envía los datos de telemetría a través del puerto **Serial**.
2.  **Puente Serial (Python):** Un script de Python (`Sensor_1 Arduino.py`) utiliza `pyserial` para leer los datos del puerto serial del Arduino y los retransmite a través de sockets **UDP** al Middleware.
3.  **Middleware (Hub):** Es el cerebro del sistema.
    * Un servidor **Node.js** con **Express.js** escucha en puertos TCP y UDP.
    * Recibe los datos del puente de Python (UDP) y de otros sensores simulados (TCP/UDP).
    * Llama a un microservicio externo (Python) vía **XML-RPC** para procesar los datos.
    * Sirve el `index.html` (el dashboard) a los clientes.
    * Envía todos los datos consolidados en tiempo real al dashboard usando **Socket.IO**.
4.  **Frontend (Dashboard):** Un archivo **HTML/JavaScript** se conecta al Middleware vía **Socket.IO** y renderiza la información en vivo, mostrando las alertas y estados de los sensores.
5.  **Microservicio (Python):** Un servidor **XML-RPC** (`#servidor xmlrpc.py`) escucha peticiones, procesa los datos que recibe (ej. `dato * 100`) y los retorna al Middleware.

## Tecnologías Utilizadas

| Componente | Tecnologías |
| :--- | :--- |
| **Firmware** | Arduino (C/C++) |
| **Middleware (Hub)** | Node.js, Express.js, Socket.IO |
| **Frontend** | HTML, JavaScript, Socket.IO (Cliente) |
| **Scripts y Servicios**| Python, XML-RPC, Sockets (TCP/UDP), Pyserial |

## Instalación y Uso
Para ejecutar este proyecto, deberás iniciar los 5 componentes en el orden correcto.

1.  **Clonar el repositorio:**
    ```bash
    git clone https://github.com/lxchoo/monitoreo-brazoneitor.git
    cd monitoreo-brazoneitor/
    ```

2.  **Hardware:**
    * Cargar el archivo `codigoFinalBrazo.ino` a la placa Arduino.
    * Asegurarse de que el Arduino esté conectado al PC y verificar el puerto COM (ej. `COM5`).

3.  **Instalar dependencias del Middleware (Node.js):**
    ```bash
    # (Asegúrate de estar en la carpeta que contiene middleware.js y package.json)
    npm install
    ```

4.  **Ejecutar los servicios (en terminales separadas):**

    * **Terminal 1: Iniciar Microservicio XML-RPC (Python)**
        ```bash
        python "#servidor xmlrpc.py"
        ```

    * **Terminal 2: Iniciar Puente Serial (Python)**
        *(Recuerda modificar el puerto `COM5` en el script si es diferente)*
        ```bash
        python "Sensor_1 Arduino.py"
        ```

    * **Terminal 3: Iniciar Middleware (Node.js)**
        ```bash
        node middleware.js
        ```

5.  **Abrir el Dashboard:**
    * Abre tu navegador web y ve a la dirección:
        ```
        http://localhost:1100
        ```

6.  **(Opcional) Iniciar Sensores Simulados:**
    * Para ver más datos en el dashboard, ejecuta los scripts de simulación en terminales adicionales:
        ```bash
        # Terminal 4
        python "Random python udp.py"

        # Terminal 5
        node "productorTCPNode datorandom.js"
        ```

## Autores
Este proyecto fue desarrollado por:
* Francisco Muñoz
* Luis Salinas
* Ignacio Valdés
