# Tarea_1_EstructurasII
 memoria caché  implementación y simulación en un lenguaje de alto nivel.

Requisitos para ejecutar el programa: Ubuntu 14.04 o superior, g++ versión, (Ubuntu 4.8.4-2ubuntu1~14.04.1) 4.8.4.

Para compilar el programa: Acceda a una terminal y en el path (dirección) donde estan los archivos: TareaProg1Cache.cpp y Makefile, ejecute el comando make de la siguiente manera:

    $ cd directorio de los arvhivos
    $ make

Para ejecutar: ./cache <asociatividad> <tamaño del caché> <tamaño del bloque>

El parámetro <asociatividad>, es un número del 0 al 2 únicamente y se selecciona de la siguiente manera:

                  0: Directamente mapeado.
                  1: Asociativo de 4 vías.
                  2: Asociativo de 2 vías.
		  
El parámetro <tamaño del caché>, es un número entero.

El parámetro tamaño del bloque>, es un número entero.

En el directorio donde esta el ejecutable, debe existir el archivo aligned.trace. Disponible en: http://ie0521.jsdanielh.org/memory-trace-
gcc.trace.gz
 
