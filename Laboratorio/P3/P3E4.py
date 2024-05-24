import serial
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# Configura el puerto serie
puerto_serie = serial.Serial('/dev/cu.usbmodem11101', 9600)
tiempo_acumulacion = 5  # Tiempo de acumulación en segundos
muestras_por_segundo = 10  # Número de muestras por segundo
num_muestras = tiempo_acumulacion * muestras_por_segundo  # Número total de muestras

# Inicializa listas para almacenar los datos
tiempo = np.linspace(0, tiempo_acumulacion, num_muestras)
datos = []

# Crea la figura y el gráfico
fig, ax = plt.subplots()
line, = ax.plot([], [], label='Datos en tiempo real')
ax.set_xlabel('Tiempo (s)')
ax.set_ylabel('Datos')
ax.legend(loc='upper left')

# Función de inicialización del gráfico
def init():
    line.set_data([], [])
    return line,

# Función para actualizar el gráfico en cada iteración
def update(frame):
    # Lee los datos del puerto serie
    try:
        datos_str = puerto_serie.readline().decode().strip()
        print("Datos leídos:", datos_str)
        
        # Divide la cadena en valores individuales (suponiendo que están separados por comas)
        valores = datos_str.split(',')
        for valor in valores:
            datos.append(float(valor))
    except serial.SerialException as e:
        print("Error al leer del puerto serie:", e)
        return

    # Actualiza los datos en el gráfico
    line.set_data(tiempo[:len(datos)], datos)
    ax.relim()
    ax.autoscale_view()
    return line,

# Crea la animación
ani = FuncAnimation(fig, update, frames=None, init_func=init, blit=True, interval=10000/muestras_por_segundo)

# Muestra el gráfico
plt.tight_layout()
plt.show()


