const mongoose = require('mongoose');
const publicWorker = require('worker_threads');
// const { dataBase } = require('../database');

const { workerData } = publicWorker;

function checkAirFlow() {
  const MONGO_URL = 'mongodb://localhost/rmvs-server';

  mongoose.connect(MONGO_URL, {
    useNewUrlParser: true,
    useUnifiedTopology: true,
  });

  const mongo = mongoose.connection;
  mongo.on('error', console.error.bind(console, 'connection error:'));
  mongo.once('open', function () {
    console.log('Job connected to the database!');
    const db = mongoose.connection.useDb('rmvs-server').db;

    mongo.close();
  });
  console.log('teste', workerData);
}

checkAirFlow();
