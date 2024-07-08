import dotenv from "dotenv";
import app from "./src/app.js";
import FollowingService from "./src/following.service.app.js";

dotenv.config({ path: "./config.env" });

const port1 = process.env.PORT || 3001;
const port2 = process.env.FOLLOWINGSERVICEPORT || 3002;

const server_1 = app.listen(port1, () => {
  console.log("Listen on port ", port1);
});

const server_2 = FollowingService.listen(port2, () => {
  console.log("Listen on port ", port2);
});

process.on("SIGNT", () => {
  server_1.close(() => {
    console.log("Server 1 exit");
  });

  server_2.close(() => {
    console.log("Server 2 exit");
  });
});


