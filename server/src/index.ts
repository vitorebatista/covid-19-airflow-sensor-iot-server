import { createServer } from 'net';
import mongoose from 'mongoose';
import { dataBase } from './database';
import { startBroker } from './broker';
import { scheduleJob } from './bree';

async function main(): Promise<void> {
  const port = 1883;
  const mongo = dataBase();

  mongo.on('error', console.error.bind(console, 'connection error:'));
  mongo.once('open', function () {
    console.log("We're connected!");
    const db = mongoose.connection.useDb('rmvs-server').db;
    const broker = startBroker(db);

    // MQTT Server
    const server = createServer(broker.handle);
    server.listen(port, function () {
      console.log('Server started and listening on port ', port);

      scheduleJob();
    });
  });
}

main();
