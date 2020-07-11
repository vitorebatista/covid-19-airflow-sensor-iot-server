import { createServer } from 'net';
import mongoose from 'mongoose';
import { startBroker } from './broker';

async function main(): Promise<void> {
  const MONGO_URL = 'mongodb://localhost/rmvs-server';
  const port = 1883;
  mongoose.connect(MONGO_URL, {
    useNewUrlParser: true,
    useUnifiedTopology: true,
  });

  const mongo = mongoose.connection;
  mongo.on('error', console.error.bind(console, 'connection error:'));
  mongo.once('open', function () {
    console.log("We're connected!");
    const db = mongoose.connection.useDb('rmvs-server').db;
    const broker = startBroker(db);

    // MQTT Server
    const server = createServer(broker.handle);
    server.listen(port, function () {
      console.log('Server started and listening on port ', port);
    });
  });
}

main();
