# Telematica_Mqtt

## Introducción

El Protocolo de Telemetría de Mensajería en Cola (MQTT por sus siglas en inglés) es un protocolo de mensajería ligero diseñado para dispositivos con restricciones de ancho de banda y recursos de procesamiento, como dispositivos IoT (Internet de las cosas). MQTT facilita la comunicación entre estos dispositivos y servidores o entre dispositivos mismos, permitiendo la transferencia eficiente de datos en redes con ancho de banda limitado.

El objetivo de este proyecto es desarrollar una implementación propia del protocolo MQTT para comprender en profundidad su funcionamiento y explorar sus características clave. La replicación del protocolo MQTT implicará la creación de un sistema que pueda manejar la publicación y suscripción de mensajes entre dispositivos, así como la gestión de los diversos aspectos del protocolo, como QoS (Calidad de Servicio), retención de mensajes y temas.

### Objetivos del Proyecto

Los principales objetivos de este proyecto son:

- Diseñar e implementar un sistema de comunicación basado en el protocolo MQTT.
- Desarrollar un servidor MQTT que pueda gestionar la publicación y suscripción de mensajes.
- Explorar y comprender las diferentes características y niveles de calidad de servicio (QoS) que ofrece MQTT.
- Probar el sistema replicado en diferentes escenarios y condiciones de red para evaluar su rendimiento y fiabilidad.

### Alcance del Proyecto

El proyecto se centrará en replicar las funcionalidades básicas del protocolo MQTT, incluyendo:

- Establecimiento de conexión entre clientes y servidor.
- Publicación y suscripción de mensajes en temas específicos.
- Implementación del nivel QoS (0).
- Gestión de la retención de mensajes para permitir a los clientes recibir mensajes que se publicaron mientras estaban desconectados.

## Desarrollo

### Tecnologías Utilizadas

Para llevar a cabo este proyecto, se utilizarán las siguientes tecnologías:

- Lenguaje de programación: C
- Amazon Web Services

Durante el proceso de desarrollo del proyecto, se siguieron los siguientes hitos y actividades detalladas:

**Primera Semana: Lectura de Especificaciones y Diseño del Modelo**
  - Se dedicó esta semana inicial a comprender en profundidad las especificaciones del protocolo MQTT.
  - Se realizó un diseño detallado del modelo, definiendo la estructura de los diferentes tipos de paquetes que conforman el protocolo, como connect, disconnect, subscribe y publish, prestando especial atención a la calidad de servicio (QoS) 0.

![Mqtt_diagrams drawio](https://github.com/santig005/Mqtt_Protocol/assets/130608466/d08680c7-0ce6-4077-91be-f68b3f717782)

**Segunda Semana: Implementación Inicial y Resolución de Problemas de Concurrencia**
  - Comenzamos la implementación del código, priorizando la creación del paquete connect y su estructura.
  - Se abordaron problemas relacionados con la concurrencia para garantizar la estabilidad y eficiencia del sistema.
  - Se trabajó en la gestión de clientes a través de listas enlazadas, estableciendo una base sólida para la comunicación cliente-servidor.

**Tercera Semana: Refinamiento de la Estructura del Connect y Desarrollo de Suscripciones**
  - Se dedicó tiempo a detallar y refinar la estructura del paquete connect, asegurando su coherencia y funcionalidad.
  - Se implementó una estructura para gestionar las suscripciones de los clientes, permitiendo un manejo eficiente de los temas y mensajes.

**Cuarta Semana: Finalización del Paquete Disconnect y Optimización**
  - Se centró en completar la implementación del paquete disconnect, asegurando que los clientes pudieran desconectarse del servidor de manera adecuada y segura.
  - Se realizaron optimizaciones adicionales en el código para mejorar el rendimiento y la eficiencia del sistema en general.

**Quinta Semana: Culminación Exitosa de las Funcionalidades Principales**
  - Se finalizó la implementación del proceso de subscripción, permitiendo a los clientes enviar suscribirse en tópicos especificos.


## Aspectos logrados y no logrados

### Logros
- Como grupo se pudo culminar el CONNECT y su respectivo DISCONNECT.
- Respecto al SUBSCRIBE, el broker procesa solicitudes con calidad de servicio iguales a cero(0).
- El PUBLISH que es un proceso en via full duplex, el cliente es capaz de publicar tópicos especificos, y el broker capaz de enviarlos a los respectivos clientes suscritos en dicho topico.

### No logramos
- El manejo de una calidad de servicio mayor a cero(0) en los paquetes que disponen de esta caracteristica.
- Uso de las wildcards para el proceso de subscripción.
- Liberación de recursos.
- Estructura para la gestión de datos no son las más optimas.
- No hay Clean Session
- No se tomaron en cuenta muchos condicionales para mantener la conexión de un cliente o para la gestión de los tipos de paquetes.

## Conclusiones

El proyecto proporcionó una valiosa experiencia técnica y un profundo entendimiento del funcionamiento interno del protocolo MQTT. Se destacaron habilidades en el diseño e implementación de sistemas de red de baja latencia y en la resolución de desafíos relacionados con la escalabilidad, rendimiento y confiabilidad.

En resumen, la replicación del protocolo MQTT fue un proyecto que permitió explorar y comprender en detalle uno de los protocolos de mensajería más utilizados en el ámbito del IoT y las comunicaciones en red.

## Referencias

[MQTT ESSENTIALS(YOUTUBE)](https://www.youtube.com/watch?v=jTeJxQFD8Ak&list=PLRkdoPznE1EMXLW6XoYLGd4uUaB6wB0wd)

[Sockets concurentes](https://www.geeksforgeeks.org/socket-programming-cc/)


### Video

[Video presentación(drive)](https://eafit-my.sharepoint.com/:f:/g/personal/vjvilladia_eafit_edu_co/EpuXgBy5suFEuxEHIP9V95YBmIzYlwhGfVqbxrfuGP2qZg?e=kvacRB)
