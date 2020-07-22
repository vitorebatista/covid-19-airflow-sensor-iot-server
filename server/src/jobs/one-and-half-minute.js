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
    const schema = new mongoose.Schema({
      type: String,
      time: Number,
      rate: Number,
      total: Number,
    });
    const PubSub = mongoose.model('PubSub', schema);
    const now = new Date().getTime() - 1.5 * 60000;
    const oneMinuteAndHalfLater = new Date().getTime() - 1.5 * 60000;
    PubSub.find()
      .where('type')
      .equals('flow')
      .where('time')
      .gt(oneMinuteAndHalfLater)
      .lt(now)
      // .limit(10)
      .select('rate total')
      .exec((error, results) => calcAirFlow(error, results, mongo));
  });
}

function calcAirFlow(error, results, mongo) {
  let flow = 0;

  if (results) {
    results.map((item) => (flow += item.rate));
  }

  mongo.close();
  return flow > 400;
}

checkAirFlow();
