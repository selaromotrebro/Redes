import serial

# Configura el puerto serie
puerto_serie = serial.Serial('/dev/cu.usbmodem1101', 9600)  # Cambia '/dev/ttyUSB0' por el puerto serie que estés utilizando y 9600 por la velocidad de baudios adecuada

try:
    while True:
        # Lee los datos del puerto serie
        datos = puerto_serie.readline().decode().strip()  # Lee una línea de datos y la decodifica a cadena
        print(datos)  # Muestra los datos por pantalla

except KeyboardInterrupt:
    print("Se interrumpió la lectura del puerto serie.")

finally:
    puerto_serie.close()  # Cierra el puerto serie al finalizar
