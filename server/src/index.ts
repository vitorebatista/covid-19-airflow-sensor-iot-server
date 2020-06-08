import { Server } from "aedes";
import { createServer, Socket } from "net";
import mongoose from "mongoose";

const mqemitter = require("mqemitter-mongodb");
const mongoPersistence = require("aedes-persistence-mongodb");
const MONGO_URL = "mongodb://127.0.0.1:27017";

function startBroker(db: any) {
  const port = 1883;
  const broker = Server({
    heartbeatInterval: 60000,
    mq: mqemitter({
      db,
    }),
    persistence: mongoPersistence({
      db,
    }),
  });

  broker.on("publish", (packet, client) => {
    if (client) {
      console.log(
        "\n(publish) %s : topic %s : %s",
        client.id,
        packet.topic,
        packet.payload
      );
    }
  });

  broker.on("subscribe", function (subscriptions, client) {
    console.log(
      "\n(subscribe) MQTT client \x1b[32m" +
        (client ? client.id : client) +
        "\x1b[0m subscribed to topics: " +
        subscriptions.map((s) => s.topic).join("\n"),
      "from broker",
      broker.id
    );
  });

  broker.on("unsubscribe", function (subscriptions, client) {
    console.log(
      "\n(unsubscribe) MQTT client \x1b[32m" +
        (client ? client.id : client) +
        "\x1b[0m unsubscribed to topics: " +
        subscriptions.join("\n"),
      "from broker",
      broker.id
    );
  });

  // fired when a client connects
  broker.on("client", function (client) {
    console.log(
      "\n(client) Client Connected: \x1b[33m" +
        (client ? client.id : client) +
        "\x1b[0m",
      "to broker",
      broker.id
    );
  });

  // fired when a client disconnects
  broker.on("clientDisconnect", function (client) {
    console.log(
      "\n(clientDisconnect) Client Disconnected: \x1b[31m" +
        (client ? client.id : client) +
        "\x1b[0m",
      "to broker",
      broker.id
    );
  });

  // fired when a message is published
  broker.on("publish", async function (packet, client) {
    console.log(
      "\n(publish) Client \x1b[31m" +
        (client ? client.id : "BROKER_" + broker.id) +
        "\x1b[0m has published",
      packet.payload.toString(),
      "on",
      packet.topic,
      "to broker",
      broker.id
    );
  });

  // MQTT server
  const server = createServer(broker.handle);
  server.listen(port, function () {
    console.log("server started and listening on port ", port);
  });
}
function startDB() {
  mongoose.connect("mongodb://localhost/rmvs-server", {
    useNewUrlParser: true,
    useUnifiedTopology: true,
  });

  var mongo = mongoose.connection;
  mongo.on("error", console.error.bind(console, "connection error:"));
  mongo.once("open", function () {
    console.log("we're connected!");
    const db = mongoose.connection.useDb("rmvs-server").db;
    startBroker(db);
  });
}

startDB();
