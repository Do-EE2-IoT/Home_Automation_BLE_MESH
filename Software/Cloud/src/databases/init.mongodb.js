import mongoose from "mongoose";
import dotenv, { configDotenv } from "dotenv";
dotenv.config({ path: '././config.env' });

const mongoConnectingString = `mongodb+srv://${
  process.env.MONGODB_USERNAME
}:${encodeURIComponent(
  process.env.MONGODB_PASSWORD
)}@cluster0.mbcfkji.mongodb.net/AIThings`;

class Database {
  constructor() {
    this.connect();
  }

  connect(type = "mongodb") {
    mongoose
      .connect(mongoConnectingString, {})
      .then(() => console.log("Connected MongoDB"))
      .catch((err) =>
        console.log("Error connect to", mongoConnectingString, err)
      );
  }

  static getInstance() {
    if (!Database.instance) {
      Database.instance = new Database();
    }
    return Database.instance;
  }
}

const instanceMongoDB = Database.getInstance();

export default instanceMongoDB;
