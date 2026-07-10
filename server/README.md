# Server

Node.js application that runs an [Aedes](https://github.com/moscajs/aedes) MQTT broker on port `1883`, persists every message to MongoDB, evaluates the flow readings published by the IoT device, and publishes alerts back (semaphore color and buzzer). A [Bree](https://github.com/breejs/bree) job scheduler periodically analyzes the stored readings.

## Prerequisites

- [Node.js](https://nodejs.org/) v12 or higher
- [Yarn](https://classic.yarnpkg.com/en/docs/install)
- [Docker](https://docs.docker.com/get-docker/) and [Docker Compose](https://docs.docker.com/compose/install/) (for MongoDB)
- [mosquitto clients](https://mosquitto.org/download/) (optional — only for the manual MQTT calls below)

## Setup and run

1. **Install dependencies**

   ```bash
   yarn
   ```

2. **Start MongoDB** (and mongo-express, a web UI available at http://localhost:8081, user/password: `root`/`root`)

   ```bash
   yarn dev:up
   ```

   To stop the containers later: `yarn dev:down`. Data is persisted in `./data/db`.

3. **Start the broker**

   ```bash
   # development (watch mode, restarts on file change)
   yarn start:dev

   # production (compiles TypeScript to ./build and runs it)
   yarn start
   ```

   You should see:

   ```
   We're connected!
   Server started and listening on port  1883
   ```

The broker is now accepting MQTT connections on `localhost:1883`.

## Other scripts

| Command         | Description                       |
| --------------- | --------------------------------- |
| `yarn build`    | Compile TypeScript to `./build`   |
| `yarn lint`     | Run ESLint on `src/`              |
| `yarn lint:fix` | Run ESLint and auto-fix           |
| `yarn dev:down` | Stop the MongoDB/mongo-express containers |

## Manual MQTT calls

With the broker running (and the IoT device connected), you can trigger the device manually using `mosquitto_pub`.

Play and stop the buzzer:

```bash
mosquitto_pub -h localhost -p 1883 -t "alert/buzzer" -m "ON"
mosquitto_pub -h localhost -p 1883 -t "alert/buzzer" -m "OFF"
```

Change the semaphore color:

```bash
mosquitto_pub -h localhost -p 1883 -t "alert/semaphore" -m "RED"
mosquitto_pub -h localhost -p 1883 -t "alert/semaphore" -m "YELLOW"
mosquitto_pub -h localhost -p 1883 -t "alert/semaphore" -m "GREEN"
```

Simulate a flow sensor reading (what the device publishes every second):

```bash
mosquitto_pub -h localhost -p 1883 -t "sensor/flow" -m '{"rate": 20, "total": 1500, "time": 123456}'
```

You can also watch all alerts with:

```bash
mosquitto_sub -h localhost -p 1883 -t "alert/#" -v
```

## Source layout

```
src/
├── index.ts    # entry point: connects to MongoDB, starts the broker and job scheduler
├── broker.ts   # Aedes MQTT broker: persistence + flow evaluation + alert publishing
├── database.ts # MongoDB connection
├── bree.ts     # Bree job scheduler setup
└── jobs/
    └── one-and-half-minute.js  # job that analyzes recent flow readings
```
