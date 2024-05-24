import serial

# Configura el puerto serie
puerto_serie = serial.Serial('/dev/cu.usbmodem1101', 9600)  # Cambia '/dev/cu.usbmodem1301' por el puerto serie que estés utilizando y 9600 por la velocidad de baudios adecuada
archivo_salida = "datos_serial01.csv"  # Nombre del archivo de salida

try:
    with open(archivo_salida, 'w') as file:
        while True:
            # Lee los datos del puerto serie
            datos = puerto_serie.readline().decode().strip()  # Lee una línea de datos y la decodifica a cadena
            print(datos)  # Muestra los datos por pantalla
            
            # Escribe los datos en el archivo
            file.write(datos.replace(" ", ";") + "\n")  # Reemplaza los espacios por ";" y añade un retorno de carro al final de cada muestra

except KeyboardInterrupt:
    print("Se interrumpió la lectura del puerto serie.")

finally:
    puerto_serie.close()  # Cierra el puerto serie al finalizar
