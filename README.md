# RMVS-IOT-SERVER

This is a proof of concept of Rapidly Manufactured Ventilation System integrated with flow sensor and MQTT broker. The basic idea of this project is to integrate a flow sensor with an existing RMVS to collect the flow of oxygen and send to an MQTT broker. This broker will persist the data and it will evaluate the flow based on the patient's profile to determine inconsistencies and thus generate alerts.

# Configurations

The project is separated in IOT and Server, so its important to you read the specific README files

- [IOT](./iot/README.md)
- [Server](./server/README.md)

# License

RMVS-IOT-SERVER is [MIT licensed](./LICENSE).
