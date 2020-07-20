import mongoose from 'mongoose';

export function dataBase(): mongoose.Connection {
  const MONGO_URL = 'mongodb://127.0.0.1/rmvs-server';

  mongoose.connect(MONGO_URL, {
    useNewUrlParser: true,
    useUnifiedTopology: true,
  });

  return mongoose.connection;
}

// module.exports = {
//   dataBase,
// };
