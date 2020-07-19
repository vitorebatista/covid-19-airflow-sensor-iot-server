## Server (WORK IN PROGRESS)

### Dependencies

- Node v12 or higher
- Yarn
- Docker
- Docker Compose

### Installation

```bash
$ yarn
```

### Local environment

```
$ yarn dev:up
```

### Running the app

```bash
# watch mode
$ yarn start:dev

# production mode
$ yarn start
```

### Manual MQTT calls

Play and Stop the buzzer

```
$ mosquitto_pub -h localhost -t "alert/buzzer" -m "ON" -p 1883
$ mosquitto_pub -h localhost -t "alert/buzzer" -m "OFF" -p 1883
```

Change semaphore color

```
$ mosquitto_pub -h localhost -t "alert/semaphore" -m "RED" -p 1883
$ mosquitto_pub -h localhost -t "alert/semaphore" -m "YELLOW" -p 1883
$ mosquitto_pub -h localhost -t "alert/semaphore" -m "GREEN" -p 1883
```
