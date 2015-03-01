State bluetoothReceiveState(){
  byte pkt[10];                                // allocate memory for the bytes in the packet
  int sz;                                      // holds the size of the message (in bytes)
  byte type;                                   // hold the message type id
  byte data1[10];

  if (btmaster.readPacket(pkt)) {
    if (pcol.getData(pkt, data1, type)) {      // see if we can extract the type and data
      if (pkt[4] != 0x00 && pkt[4] != BLUETOOTH_ADDRESS) return;           // if we have received a message

      switch (type) {                          // process the message based on the type
      case STORAGE_TUBE_AVAILABILITY:                               // received a storage tube message
        bluetoothSpentAvailability = data1[0];                // extract and save the storage-related data (the byte bitmask)
        break;
      case SUPPLY_TUBE_AVAILABILITY:                               // received a supply tube message
        bluetoothNewAvailability = data1[0];                 // extract and save the supply-related data (the byte bitmask)
        break;
      case STOP_MOVEMENT:
        bluetoothStopFlag = true;
        break;
      case RESUME_MOVEMENT:
        bluetoothResumeFlag = true;
        break;
      default:
        severe(F("Unexp BT type recvd"));
        break;
      }
    }
  }
}


State bluetoothSendHBState(){
  byte pkt[10];                                // allocate memory for the bytes in the packet
  int sz;                                      // holds the size of the message (in bytes)
  byte type;                                   // hold the message type id
  byte data1[3];
  pcol.setDst(0x00);			       // this will be a broadcast message
  sz = pcol.createPkt(HEART_BEAT, data1, pkt);     // create a packet using the heartbeat type ID (there is no data)
  btmaster.sendPkt(pkt, sz);
  bluetoothSendSM.Set(BluetoothSendWaitState1);
}

State BluetoothSendWaitState1(){
  if (bluetoothSendSM.Timeout(20)){
    bluetoothSendSM.Set(bluetoothSendRadiationLevelState);
  }

}

State bluetoothSendRadiationLevelState(){
  byte pkt[10];                                // allocate memory for the bytes in the packet
  int sz;                                      // holds the size of the message (in bytes)
  byte type;                                   // hold the message type id
  byte data1[3];
  pcol.setDst(0x00);			       // this will be a broadcast message
  data1[0] = radiationLevel;                           // indicate a new fuel rod
  sz = pcol.createPkt(RADIATION_ALERT, data1, pkt);     // create a packet using the radiation alert type ID (1 byte of data used this time)
  btmaster.sendPkt(pkt, sz);                 // send to the field computer
  bluetoothSendSM.Set(BluetoothSendWaitState2);
}

State BluetoothSendWaitState2(){
  if (bluetoothSendSM.Timeout(1500)){
    bluetoothSendSM.Set(bluetoothSendHBState);
  }
}
