# Pokemon Mini Game for PIC
PG-7233 C for Embedded Systems - Proyecto Final 

Este proyecto es un minijuego de Pokemon, desarrollado para el Microcontrolador PIC24FJ128GA010 utilizando [MPLABX](https://www.microchip.com/en-us/development-tools-tools-and-software/mplab-x-ide) y simulado utilizando [Proteus](https://www.labcenter.com/simulation/).
Utilice a Charmander o Squirtle para derrotar a Pikachu (computador)

<p align="center">
  <img width="460" height="300" src="https://github.com/DiegoNegrette/PG7233-Proyecto-Final/blob/main/Imagenes/animation.gif">
</p>

<p align="center">
  <small>Ejemplo Gameplay</small>  
</p>


## Tabla de contenidos

1. [Inicio](#getting-started) 
    1. [Source](#source)
    2. [Dependencias](#dependencies)
2. [Detalles tecnicos](#technical-details) 
3. [Simulacion](#simulation)
4. [Mejoras Futuras](#upcoming-enhancements)
5. [Autor](#authors)
6. [Licencia](#license)

# Getting Started

## Source

Este proyecto se encuentra alojado en [GitHub](https://github.com/DiegoNegrette/PG7233-Proyecto-Final). Puedes clonarlo directamente utilizando el comando:

```
git clone https://github.com/DiegoNegrette/PG7233-Proyecto-Final.git
```

**[Volver al Inicio](#table-of-contents)**

## Dependencias

Este proyecto requiere de los siguientes elementos para su uso:

* [MPLABX](https://www.microchip.com/en-us/development-tools-tools-and-software/mplab-x-ide)
* [Proteus](https://www.labcenter.com/simulation/)

**[Volver al Inicio](#table-of-contents)**

# Detalles técnicos

## PIC
Este proyecto fue desarrollado para el [PIC24FJ128GA010](https://ww1.microchip.com/downloads/en/DeviceDoc/39747F.pdf). 

## Pantalla
El controlador LCD utilizado es el T6963C conectado a una pantalla de 240 x 128 pixels.
Se modificó la libreria [T6963C_PIC](https://github.com/camilstaps/T6963C_PIC.git) de camilstaps para que funcionara de acuerdo a los requerimientos del proyecto.

## Botones
Para la interacción del usuario con el juego se dispusieron 5 botones (A,B,C,D y START) conectados en configuración pull down y manejados a través de interrupciones, cuyo funcionamiento depende de la etapa del juego en la que se encuentre.

El boton de START es utilizado para dar inicio al juego, asi como también para seleccionar al pokemon a utilizar una vez que esta en el mapa.
Los botones A,B,C y D funcionan como flechas de movimiento mientras navega por el mapa y son utilizados para seleccionar las habilidades durante la batalla.

Las interrupciones ligadas a cada boton son deshabilitadas y habilitadas a conveniencia para evitar comportamientos indeseados durante la carga de los sprites en la pantalla.

<p align="center">
  <img src="https://github.com/DiegoNegrette/PG7233-Proyecto-Final/blob/main/Imagenes/botones.png">
</p>

## Movimiento en el mapa
Durante su estancia en el mapa, debe mover al jugador a la parte baja del mapa para seleccionar alguno de los dos Pokemones disponibles.
Mediante una rutina de detección de colisiones se evita que el jugador se desplace sobre puntos del mapa que contienen otros objetos.
Las Pokebolas y el jugador son agregados al mapa original mediante una superposición de ambos sprites.

<p align="center">
  <img width="460" height="300" src="https://github.com/DiegoNegrette/PG7233-Proyecto-Final/blob/main/Imagenes/animation.gif">
</p>

Durante el desarrollo del juego, podrá visualizar los mensajes generados en la consola enviados mediante comunicación serial desde el PIC

<p align="center">
  <img width="460" height="300" src="https://github.com/DiegoNegrette/PG7233-Proyecto-Final/blob/main/Imagenes/Screenshot_1.png">
</p>

## Sprites
Los sprites utilizados para el mapa, los pokemones, la pantalla de inicio, las pokebolas y el personaje fueron conseguidos en internet y fueron editadas para que sus tamaños estuvieran acorde a las dimenciones de las pantalla.

Para la conversión de las imagenes a matrices se utilizó la herramienta [LCD Assistant](http://en.radzio.dxp.pl/bitmap_converter/)

<p align="center">
  <img src="https://github.com/DiegoNegrette/PG7233-Proyecto-Final/blob/main/Imagenes/map_original.png">
</p>
<p align="center">
  <small> Mapa original </small>  
</p>
<p align="center">
  <img src="https://github.com/DiegoNegrette/PG7233-Proyecto-Final/blob/main/Imagenes/map_resize.png">
</p>
<p align="center">
  <small> Mapa 240x188 </small>  
</p>
<p align="center">
  <img src="https://github.com/DiegoNegrette/PG7233-Proyecto-Final/blob/main/Imagenes/map_bitmap.bmp">
</p>
<p align="center">
  <small> Mapa bitmap </small>  
</p>


## Logica de Juego
Para la implementación de la batalla se utilizó este [artículo](https://towardsdatascience.com/gotta-simulate-em-all-pokemon-390553008e2d) que describe los atributos de los Pokemones y la lógica para el cálculo del daño de ataque utilizando los datos de cada Pokemon y sus habilidades.

El código fuente que describe el artículo fue escrito por tylerwmarrs para python y se puede consultar [aqui](https://github.com/tylerwmarrs/pokemon-battle-simulation)

Los datos utilizados para la vida, ataque, defensa y habilidades de cada pokemon son los mismos utilizados para el popular juego de gameboy y se pueden consultar a continuación:
[Pikachu](https://pokemondb.net/pokedex/pikachu)
[Squirtle](https://pokemondb.net/pokedex/squirtle)
[Charmander](https://pokemondb.net/pokedex/charmander)

Para la selección del orden de ataque, los movimientos utilizados por la computadora y el boost de ataque crítico, se utilizó el Timer2 como contador cuyo valor permite generar una número aleatorio.

El cálculo del daño de ataque se realiza como se describe en el siguiente segmento de código para python.

    """
    def calculate_damage(a, b, c, d, x, y, crit):
    Computes the damage to apply given some parameters.
    
    Parameters
    ----------
    a : int
        The level of the attacker.
    b : int
        The attacker's special attack or normal attack statistic.
    c : int
        The attacker's attack power statistic.
    d : int
        The defender's special defense or normal defense statistic.
    x : float
        Same type attack bonus to apply (1 or 1.5).
    y : float
        The type modifier statistic to apply.
    crit : boolean
        A flag indicating if a critical strike should be computed.
    
    Returns
    -------
    float :
        The computed damage.
    """
    z = np.random.choice(np.arange(217, 256))
    crit_mult = 2
    if crit:
        crit_mult = 4
    
    damage = np.floor(((((crit_mult * a) / 5) + 2) * b * c) / d)
    damage = np.floor(damage / 50) + 2
    damage = np.floor(damage * x)    
    damage = np.floor(damage * y)
    
    return np.floor((damage * z) / 255)

Los cálculos intermedios pueden ser verificados en proteus enciendo la bandera flag_debug localizada [aqui](https://github.com/DiegoNegrette/PG7233-Proyecto-Final/Mplab/Proyecto.X/pokemon.h).
Dicha bandera se encuentra apagada en el código de este repositorio para mejorar la fluidez del juego

<p align="center">
  <img width="460" height="300" src="https://github.com/DiegoNegrette/PG7233-Proyecto-Final/blob/main/Imagenes/Screenshot_2.png">
</p>

<p align="center">
  <img width="460" height="300" src="https://github.com/DiegoNegrette/PG7233-Proyecto-Final/blob/main/Imagenes/Screenshot_3.png">
</p>

## Simulacion

En el siguiente video se puede visualizar una partida completa de este minijuego utilizando Proteus para simular el dispositivo

https://github.com/DiegoNegrette/PG7233-Proyecto-Final/blob/main/VideoGameplay/Gameplay.mp4


**[Back to top](#table-of-contents)**

# Próximas mejoras
Para próximas ediciones se desea agregar animaciones a la pantalla de pelea, asi como también tomar en cuenta otros factores utilizados para el cálculo del daño como el nivel, la probabilidad de hits adicionales y cambiar el cálculo de la probabilidad de crítico en función de la velocidad de ataque del personaje.

Adicionalmente, el tipo de ataque puede generar multiplicaciones al daño adicional dependiendo del tipo de Pokemon al que se este atacando.

**[Back to top](#table-of-contents)**

# Autor

* **[Diego Negrette](https://github.com/DiegoNegrette)**

**[Back to top](#table-of-contents)**

## Licencia
The MIT License [(MIT)](https://github.com/DiegoNegrette/PG7233-Proyecto-Final/blob/main/LICENSE)
**[Back to top](#table-of-contents)**
