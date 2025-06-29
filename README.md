# MoniAuxServer

**MoniAuxServer** es una aplicación en C con GUI para Linux (GTK), que recibe comandos desde un microcontrolador por puerto serial (este caso para el dispositivo [MoniAux](https://github.com/JGomezC145/MoniAux)) y simula teclas del teclado del sistema usando `uinput`. 

### Sobre [MoniAux](https://github.com/JGomezC145/MoniAux) (Dispositivo HID)
<!-- ![MoniAux](https://raw.githubusercontent.com/JGomezC145/MoniAux/main/resources/MoniAux.png) -->

Esta aplicación depende del dispositivo MoniAux, el cual contiene 12 botones y un encoder rotativo con pulsador, permitiendo controlar funciones del sistema como accesos directos, control de volumen, reproducción multimedia, etc. También contiene una pantalla TFT qu permite mostrar información tal como la hora, fecha, etc.



## Propósito

Permitir que botones físicos o un encoder rotativo (conectados al dispositivo MoniAux) puedan controlar funciones del sistema como un acceso directo. Además de permitir la visualización de información importante.


## Requisitos

- Ubuntu / Debian Linux
- GTK+3 (`libgtk-3-dev`)
- Acceso a `/dev/uinput` (se requiere root o permisos adecuados)
- Compilador C (`gcc`)
- CMake
- Dispositivo MoniAux conectado por USB

---

## Instalación


### 1. Instala dependencias:

```bash
sudo apt update
sudo apt install build-essential cmake libgtk-3-dev libuinput-dev
````

### 2. Compila el proyecto:
> [!IMPORTANT]
> Este proyecto fue compilado y programado en CLion 2025.1.1 en Windows, usando un puente con WSL para compilar para Ubuntu. En caso de de no querer usar CLion, puede compilarlo directamente en Ubuntu con Cmake o Make (usando el Makefile descargable.). 
> 
```bash
cmake -B build
cmake --build build
```

El ejecutable aparecerá en `build/MoniAuxServer`.



## Ejecución

Ejecuta como root (necesario para simular teclas):

```bash
sudo ./MoniAuxServer [-p /dev/ttyACM0]
```

Si no se especifica `-p`, usará el puerto por defecto `/dev/ttyACM1`.



## Funcionalidad

* **Lectura serial** de comandos como `Btn1`, `EncoderR`, etc.
* **Simulación de teclas** usando `uinput`
* **GUI en GTK** con:

    * Etiqueta de estado
    * Botón para cerrar
    * Botón para recargar configuración
* **Ícono embebido en el binario** (no se requieren archivos externos)



## Archivo de configuración: `keymap.cfg`

Al iniciarse, la app intentará cargar `keymap.cfg`. Si no existe, lo creará con valores por defecto.

Ejemplo de contenido:

```ini
# Botones físicos
Btn1=KEY_F13
Btn2=KEY_F14
Btn3=KEY_F15
...
Btn9=KEY_MEDIA_PREV
BtnA=KEY_MEDIA_PLAYPAUSE
BtnB=KEY_MEDIA_NEXT

# Encoder
EncoderR=KEY_VOLUMEUP
EncoderL=KEY_VOLUMEDOWN
EncoderSW=KEY_MUTE
```

> [!NOTE]
> Al crearse el archivo `keymap.cfg`, se generará bajo el nombre de root, puede cambiar los permisos o abrir usando sudo, si desea editarlo.


Puedes cambiar los valores por cualquier nombre de tecla compatible con `input-event-codes.h`, como:

* `KEY_A`, `KEY_B`, `KEY_ENTER`
* `KEY_VOLUMEUP`, `KEY_F14`, `KEY_ESC`, etc.

Véase el documento [KeyTable LookUp](KeyTableLookUp.md)

## Permisos

Para poder acceder a `/dev/uinput`, necesitas:

* Ejecutar como root **O**
* Crear una regla udev:

```bash
echo 'KERNEL=="uinput", MODE="0666"' | sudo tee /etc/udev/rules.d/99-uinput.rules
sudo udevadm control --reload-rules
```



## Estructura del proyecto

```
MoniAuxServer/
├── src/             # Archivos fuente .c
├── include/         # Archivos de cabecera .h
├── resources/       # (opcional) Archivos como el ícono original
├── keymap.cfg       # Archivo de mapeo (se genera si no existe)
├── CMakeLists.txt
```



## Ejemplo de flujo

1. El dispositivo HID envía `Btn1\n` por serial.
2. La app lo recibe y consulta `keymap.cfg`.
3. Si `Btn1=KEY_F13`, se simula la tecla F13.
4. El estado se actualiza visualmente en la ventana (puede que no ocurra). 




## Autor

Desarrollado por: *JGomezC145*\
Fecha: 2025


