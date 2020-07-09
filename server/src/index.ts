import mongoose from 'mongoose'
import { startBroker } from './broker'

const MONGO_URL = 'mongodb://localhost/rmvs-server'

function startDB () {
  mongoose.connect(MONGO_URL, {
    useNewUrlParser: true,
    useUnifiedTopology: true
  })

  var mongo = mongoose.connection
  mongo.on('error', console.error.bind(console, 'connection error:'))
  mongo.once('open', function () {
    console.log("we're connected!")
    const db = mongoose.connection.useDb('rmvs-server').db
    startBroker(db)
  })
}

startDB()
