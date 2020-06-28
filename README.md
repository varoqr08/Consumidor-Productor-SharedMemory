# Instituto Tenológico de Costa Rica
# Principios de Sistemas Operativos
# Consumidor-Productor-SharedMemory



La idea fundamental de este proyecto es crear un buffer que sirva de buzón circular y tamaño finito. Este buffer debe inicializarse y crearse en un proceso independiente del consumidor y productor, además existirá un agente externo (Proceso) encargado de liberar todos los recursos de manera elegante y cerrar de manera idónea cada proceso.

## Compilación

* En la raíz del proyecto, ejecutar el siguiente comando
```
make
```

## Ejecución
  * Posicionarse en la carpeta bin mediante el comando 
  ```
  cd  bin
  ```

  * Para generar el inicializador, se pasan los parámetros del nombre y tamaño (número entero) de la memoria mediante el comando 
  ```
  ./inicialzador  nombre  5.  
  ```

  * Para los productores, se pasan los parámetros del nombre y la media en segundos de producción de mensajes mediante el comando 
  ```
  ./productor  nombre  2.
  ```
  
  * Para consumidores, se pasan los parámetros del nombre, la media en segundos de consumo de mensajes y el modo de operación (1 Automatico, 0 Manual) mediante el comando
  ```
  ./consumidor  nombre  3  1.
  ```
  
  * Para el finalizador es necesario pasar el parámetro del nombre mediante el comando 
  ```
  ./finalizador  nombre.
  ```


## Contribuyendo
* [Mauricio Álvarez Varela.](https://github.com/mau18alvarez)
* [José Núñez Valverde.](https://github.com/JoseK21)
* [Álvaro Quesada Ramirez.](https://github.com/varoqr08)